/*----------------------------------------------------------------------------------------------*/
/*! \file correlator.cpp
//
// FILENAME: correlator.cpp
//
// DESCRIPTION: Implements member functions of Correlator class.
//
// DEVELOPERS: Gregory W. Heckler (2003-2009)
//
// LICENSE TERMS: Copyright (c) Gregory W. Heckler 2009
//
// This file is part of the GPS Software Defined Radio (GPS-SDR)
//
// The GPS-SDR is free software; you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version. The GPS-SDR is distributed in the hope that
// it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// Note:  Comments within this file follow a syntax that is compatible with
//        DOXYGEN and are utilized for automated document extraction
//
// Reference:
*/
/*----------------------------------------------------------------------------------------------*/

#include "correlator.h"

/*----------------------------------------------------------------------------------------------*/
void *Correlator_Thread(void *_arg)
{

	Correlator *aCorrelator = pCorrelator;

	while(grun)
	{
		aCorrelator->Import();
		aCorrelator->Correlate();
		aCorrelator->IncExecTic();
	}

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::Start()
{

	/* With new priority specified */
	Start_Thread(Correlator_Thread, NULL);

	if(gopt.verbose)
		printf("Correlator thread started\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Correlator::Correlator():Threaded_Object("CORTASK")
{

	int32 lcv;

	packet_count = 0;

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		//aChannel[lcv] = pChannels[lcv];
		states[lcv].active = 0;
	}

	/* Malloc memory for local code vector */
//	code_table = new MIX[(2*CODE_BINS+1)*2*SAMPS_MS];
//	code_rows = new MIX*[2*CODE_BINS+1];
//	for(lcv = 0; lcv < 2*CODE_BINS+1; lcv++)
//		code_rows[lcv] = &code_table[lcv*2*SAMPS_MS];

	/* Hold the pre computed tables */
	main_sine_table = new CPX[(2*CARRIER_BINS+1)*2*SAMPS_MS];
	main_sine_rows = new CPX*[2*CARRIER_BINS+1];
	main_code_table = new MIX[MAX_SV*(2*CODE_BINS+1)*2*SAMPS_MS];
	main_code_rows = new MIX*[MAX_SV*(2*CODE_BINS+1)];

	/* Get the pointers */
	for(lcv = 0; lcv < 2*CARRIER_BINS+1; lcv++)
		main_sine_rows[lcv] = &main_sine_table[lcv*2*SAMPS_MS];

	/* Create the wipeoff */
	for(lcv = -CARRIER_BINS; lcv <= CARRIER_BINS; lcv++)
		sine_gen(main_sine_rows[lcv+CARRIER_BINS], -IF_FREQUENCY-(float)lcv*CARRIER_SPACING, SAMPLE_FREQUENCY, 2*SAMPS_MS);

	for(lcv = 0; lcv < (2*CODE_BINS+1)*MAX_SV; lcv++)
		main_code_rows[lcv] = &main_code_table[lcv*2*SAMPS_MS];

	SamplePRN();

	if(gopt.verbose)
		printf("Creating Correlator\n");

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Correlator::~Correlator()
{

	delete [] main_sine_table;
	delete [] main_sine_rows;
	delete [] main_code_table;
	delete [] main_code_rows;

	if(gopt.verbose)
		printf("Destructing Correlator\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::Import()
{
	int32 chan;
	int32 bread;
	int32 lcv;
	Acq_Command_S temp;

	/* Wait for a command to start a new channel */
	bread = read(SVS_2_COR_P[READ], &result, sizeof(Acq_Command_S));
	if(bread == sizeof(Acq_Command_S))
	{
		chan = result.chan;
		InitCorrelator(&states[chan]);

		aChannel[chan]->Lock();

		switch(result.type)
		{
			case ACQ_STRONG:
				aChannel[chan]->Start(result.sv, result, 1);
				break;
			case ACQ_MEDIUM:
				aChannel[chan]->Start(result.sv, result, 10);
				break;
			case ACQ_WEAK:
				aChannel[chan]->Start(result.sv, result, 10);
				break;
		}

		aChannel[chan]->Unlock();
	}

	/* This call should block until new data is available */
	pFIFO->Dequeue(&packet);

	/* Send a packet to the acquisition (nonblocking) */
	write(COR_2_ACQ_P[WRITE], &packet, sizeof(ms_packet));

	packet_count++;

	if((packet_count % 1000) == 0)
		printf("Packets %d\n",packet_count);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::Correlate()
{
	int32 lcv, leftover, rollover;
	Correlation_S *c;
	Correlator_State_S *s;
	NCO_Command_S *f;
	Measurement_M *m;
	Measurement_M *mb;
	CPX *if_data;

	IncStartTic();

	//	if(packet.measurement)
	//		TakeMeasurement();

	for(lcv = 0; lcv < MAX_CHANNELS; lcv++)
	{
		if(states[lcv].active)
		{
			s = &states[lcv];
			c = &correlations[lcv];
			if_data = &packet.data[0];
			leftover = SAMPS_MS;

			/* If the rollover occurs in this packet */
			if(s->rollover <= SAMPS_MS)
			{
				/* Do the actual accumulation */
				Accum(s, c, if_data, s->rollover);

				/* Remaining number of samples to be processed in this ms packet of data */
				leftover = SAMPS_MS - s->rollover;
				if_data += s->rollover;

				/* Update the code/carrier phase etc */
				UpdateState(s, s->rollover);

				/* Dump the accumulation */
				DumpAccum(s, c, f);

				if(s->active == 0)
					return;

				/* Now process remaining segment of IF data  */
				if(s->rollover <= leftover) /* Rollover occurs in THIS packet of data */
				{
					/* Do the actual accumulation */
					Accum(s, c, if_data, s->rollover);

					/* Remaining number of samples to be processed in this ms packet of data */
					leftover -= s->rollover;
					if_data += s->rollover;

					/* Update the code/carrier phase etc */
					UpdateState(s, s->rollover);

					/* Dump the accumulation */
					DumpAccum(s, c, f);
					if(s->active == 0)
						return;

					/* Do the actual accumulation */
					Accum(s, c, if_data, leftover);

					/* Update the code/carrier phase etc */
					UpdateState(s, leftover);

				}
				else /* Rollover occurs in NEXT packet of data */
				{
					/* Do the actual accumulation */
					Accum(s, c, if_data, leftover);

					/* Update the code/carrier phase */
					UpdateState(s, leftover);
				}

			}
			else /* Just accumulate, no dumping */
			{
				/* Do the actual accumulation */
				Accum(s, c, if_data, SAMPS_MS);

				/* Update the code/carrier phase */
				UpdateState(s, SAMPS_MS);
			}
		} //!< end if active
	} //!< end for

	IncStopTic();

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::TakeMeasurement(Correlator_State_S *s, Measurement_M *m, Measurement_M *mb)
{

	int32 lcv, tic;
	int32 n_dp, n_p, n_c;
	Measurement_M *pmeas;

	tic = packet.measurement;

	/* Step 1, copy in measurement from ICP_TICS ago */
	memcpy(m, &mb[(tic - ICP_TICS + TICS_PER_SECOND) % TICS_PER_SECOND], sizeof(Measurement_M));

	/* Get carrier phase prev from 2*ICP_TICKS ago */
	m->carrier_phase_prev = mb[(tic - 2*ICP_TICS + TICS_PER_SECOND) % TICS_PER_SECOND].carrier_phase;

	/* Get current carrier phase */
	m->carrier_phase = s->carrier_phase;

	/* Store rest of measurement in buffer to do the delay */
	pmeas = &mb[tic % (TICS_PER_SECOND)];
	//pmeas->chan				 = chan;
//	pmeas->code_phase 		 = s->code_phase;
//	pmeas->code_phase_mod 	 = s->code_phase_mod;
//	pmeas->carrier_phase 	 = s->carrier_phase;
//	pmeas->carrier_phase_mod = s->carrier_phase_mod;
//	pmeas->code_nco 		 = s->code_nco;
//	pmeas->carrier_nco 		 = s->carrier_nco;
//	pmeas->_1ms_epoch 		 = s->_1ms_epoch;
//	pmeas->_20ms_epoch 		 = s->_20ms_epoch;
//	pmeas->_z_count 		 = s->_z_count;
//	pmeas->sv				 = s->sv;
//	pmeas->count			 = packet.count;
//	pmeas->navigate			 = s->navigate;

	n_dp = mb[(tic - 2*ICP_TICS + TICS_PER_SECOND) % TICS_PER_SECOND].navigate;
	n_p = mb[(tic - ICP_TICS + TICS_PER_SECOND) % TICS_PER_SECOND].navigate;
	n_c = mb[tic % TICS_PER_SECOND].navigate;

	/* Mark navigate only if all 3 are navigate */
	m->navigate = n_dp && n_p && n_c;

	/* Write over measurement */
	//write(Corr_2_PVT_P[chan][WRITE], m, sizeof(Measurement_M));

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::UpdateState(Correlator_State_S *s, int32 samps)
{

	/* Update phase states */
	s->code_phase 	 		+= samps*s->code_nco/SAMPLE_FREQUENCY;
	s->carrier_phase		+= samps*s->carrier_nco/SAMPLE_FREQUENCY;

	/* Do this to catch code epoch rollovers */
	s->code_phase_mod		+= samps*s->code_nco/SAMPLE_FREQUENCY;
	s->carrier_phase_mod	+= samps*s->carrier_nco/SAMPLE_FREQUENCY;

	/* A double rollover MIGHT occur? */
	if(s->code_phase_mod >= 2.0*(double)CODE_CHIPS)
	{
		s->_1ms_epoch += 2;
		if(s->_1ms_epoch >= 20)
		{
			s->_1ms_epoch %= 20;
			s->_20ms_epoch++;

			if(s->_20ms_epoch >= 300)
			{
				s->_20ms_epoch = 0;
				s->_z_count += 6;

				if(s->_z_count > SECONDS_IN_WEEK)
					s->_z_count = 0;
			}
		}
	} /* If the C/A code rolls over then the 1ms and 20ms counters need incremented */
	else if(s->code_phase_mod >= (double)CODE_CHIPS)
	{
		s->_1ms_epoch++;
		if(s->_1ms_epoch >= 20)
		{
			s->_1ms_epoch %= 20;
			s->_20ms_epoch++;

			if(s->_20ms_epoch >= 300)
			{
				s->_20ms_epoch = 0;
				s->_z_count += 6;

				if(s->_z_count > SECONDS_IN_WEEK)
					s->_z_count = 0;
			}
		}
	}

	/* Update partial phase states */
	s->carrier_phase_mod  	 = fmod(s->carrier_phase_mod, 1.0);
	s->code_phase_mod	  	 = fmod(s->code_phase_mod, CODE_CHIPS);

	s->rollover -= samps;

	/* Update pointers to presampled Doppler and PRN vectors */
	s->psine    += samps;
	s->pcode[0] += samps;
	s->pcode[1] += samps;
	s->pcode[2] += samps;
	s->scount   += samps;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::Accum(Correlator_State_S *s, Correlation_S *c, CPX *data, int32 samps)
{

	CPX_ACCUM EPL[3];

	//SineGen(samps);
	//state.psine = main_sine_rows[chan];

	/* First do the wipeoff */
	sse_cmulsc(data, s->psine, scratch, samps, 14);

	/* Now do the accumulation */
	//sse_prn_accum(scratch, s->pcode[0], s->pcode[1], s->pcode[2], samps, &EPL[0]);
	sse_prn_accum_new(scratch, s->pcode[0], s->pcode[1], s->pcode[2], samps, &EPL[0]);

	c->I[0] += (int32) EPL[0].i;
	c->I[1] += (int32) EPL[1].i;
	c->I[2] += (int32) EPL[2].i;

	c->Q[0] += (int32) EPL[0].q;
	c->Q[1] += (int32) EPL[1].q;
	c->Q[2] += (int32) EPL[2].q;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::DumpAccum(Correlator_State_S *s, Correlation_S *c,  NCO_Command_S *f)
{
	float f1, f2, fix, ang;
	float code_phase;
	int32 bin, offset, lcv, bread;
	float sang, cang, tI, tQ;

	/* First rotate correlation based on nco frequency and actually frequency used for correlation */
	f1 = ((s->sbin - CARRIER_BINS) * CARRIER_SPACING) + IF_FREQUENCY;
	f2 = s->carrier_nco;
	fix = (float)PI*(f2-f1)*(float)s->scount/(float)SAMPLE_FREQUENCY;

	ang = s->carrier_phase_prev*(float)TWO_PI + fix;
	ang = -ang; cang = cos(ang); sang = sin(ang);

	s->carrier_phase_prev = s->carrier_phase_mod;

	tI = c->I[0];	tQ = c->Q[0];
	c->I[0] = (int32)floor(cang*tI - sang*tQ);
	c->Q[0] = (int32)floor(sang*tI + cang*tQ);

	tI = c->I[1];	tQ = c->Q[1];
	c->I[1] = (int32)floor(cang*tI - sang*tQ);
	c->Q[1] = (int32)floor(sang*tI + cang*tQ);

	tI = c->I[2];	tQ = c->Q[2];
	c->I[2] = (int32)floor(cang*tI - sang*tQ);
	c->Q[2] = (int32)floor(sang*tI + cang*tQ);

	/* Get the f */
	//aChannel->Accum(c, f);

	 /* Apply f */
	ProcessFeedback(s, f);

	/* Is this needed? */
	s->count++;

	/* Now clear out accumulation */
	c->I[0] = c->I[1] = c->I[2] = 0;
	c->Q[0] = c->Q[1] = c->Q[2] = 0;

	/* Calculate when next rollover occurs (in samples) */
	s->rollover = (int32) ceil(((double)CODE_CHIPS - s->code_phase_mod)*SAMPLE_FREQUENCY/s->code_nco);

	bin = (int32) floor((s->code_phase_mod + 0.5)*CODE_BINS + 0.5) + CODE_BINS/2;
	if(bin < 0)	bin = 0; if(bin > 2*CODE_BINS) bin = 2*CODE_BINS;
	s->pcode[0] = s->code_rows[bin];
	s->cbin[0] = bin;

	bin = (int32) floor((s->code_phase_mod + 0.0)*CODE_BINS + 0.5) + CODE_BINS/2;
	if(bin < 0)	bin = 0; if(bin > 2*CODE_BINS) bin = 2*CODE_BINS;
	s->pcode[1] = s->code_rows[bin];
	s->cbin[1] = bin;

	bin = (int32) floor((s->code_phase_mod - 0.5)*CODE_BINS + 0.5) + CODE_BINS/2;
	if(bin < 0)	bin = 0; if(bin > 2*CODE_BINS) bin = 2*CODE_BINS;
	s->pcode[2] = s->code_rows[bin];
	s->cbin[2] = bin;

	/* Update pointer to pre-sampled sine vector */
	bin = (int32) floor((s->carrier_nco - IF_FREQUENCY)/CARRIER_SPACING + 0.5) + CARRIER_BINS;

	/* Catch errors if Doppler goes out of range */
	if(bin < 0)	bin = 0; if(bin > 2*CARRIER_BINS) bin = 2*CARRIER_BINS;
	s->psine = main_sine_rows[bin];
	s->sbin = bin;

	/* Remember to nuke this! */
	s->scount = 0;

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::ProcessFeedback(Correlator_State_S *s, NCO_Command_S *f)
{

	s->carrier_nco  = f->carrier_nco;
	s->code_nco 	= f->code_nco;
	s->navigate		= f->navigate;

	nco_phase_inc = (uint32)floor((double)s->carrier_nco*(double)2.097152000000000e+03);

	if(f->reset_1ms)
		s->_1ms_epoch = 0;

	if(f->reset_20ms)
		s->_20ms_epoch = 60;

	if(f->set_z_count)
		s->_z_count = f->z_count;

	/* Update correlator state */
	if(f->kill)
	{
		/* Clear out some buffers */
		memset(s, 		0x0, sizeof(Correlator_State_S));
//		memset(&meas, 		0x0, sizeof(Measurement_M));
//		memset(&meas_buff, 	0x0, TICS_PER_SECOND*sizeof(Measurement_M));

		/* Set correlator status to inactive */
//		pChannels[chan]->Lock();
//		pChannels[chan]->setActive(false);
//		pChannels[chan]->Unlock();
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::Export()
{


}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::SineGen(int32 _samps)
{

	int32 lcv;
	uint32 index;

	for(lcv = 0; lcv < _samps; lcv++)
	{
		index = nco_phase >> 21;
//		main_sine_rows[chan][lcv].i = lookup[index].i;
//		main_sine_rows[chan][lcv].q = lookup[index].q;
		nco_phase += nco_phase_inc;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::SamplePRN()
{
	MIX *row;
	int32 lcv, lcv2, sv, k;
	int32 index;
	float phase_step, phase;

	k = 0;

	for(sv = 0; sv < MAX_SV; sv++)
	{

		code_gen(&scratch[0], sv);

		for(lcv = 0; lcv < 2*CODE_BINS+1; lcv++)
		{

			row = main_code_rows[k];
			k++;

			phase = -0.5 + (float)lcv/(float)CODE_BINS;
			phase_step = CODE_RATE/SAMPLE_FREQUENCY;

			for(lcv2 = 0; lcv2 < 2*SAMPS_MS; lcv2++)
			{
				index  = (int32)floor(phase + CODE_CHIPS) % CODE_CHIPS;

				if(scratch[index].i)
					row[lcv2].i = row[lcv2].ni = 0x0001; /* Map 1 to 0x0000, and 0 to 0xffff for SIMD code */
				else
					row[lcv2].i = row[lcv2].ni = 0xffff;

				row[lcv2].q = row[lcv2].nq = 0x0;

				phase += phase_step;
			}
		}

	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::GetPRN(Correlator_State_S *s)
{

	int32 lcv;

	if((s->sv >= 0) && (s->sv <= 31))
	{
		for(lcv = 0; lcv < (2*CODE_BINS+1); lcv++)
		{
			s->code_rows[lcv] = main_code_rows[lcv + s->sv*(2*CODE_BINS+1)];
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Correlator::InitCorrelator(Correlator_State_S *s)
{
	double code_phase;
	double dt;
	int32 bin;
	int32 inc;

	/* Update delay based on current packet count */
//	dt = (double)packet.count - (double)result.count;
//	dt *= (double).001;
//	dt *= (double)result.doppler * (double)CODE_RATE/(double)L1;

	result.code_phase += (double)CODE_CHIPS + dt;
	result.code_phase = fmod(result.code_phase,(double) CODE_CHIPS);

	s->sv					= result.sv;
	s->navigate				= false;
	s->active 				= 1;
	s->count				= 0;
	s->scount				= 0;
	s->code_phase 			= result.code_phase;
	s->code_phase_mod 		= result.code_phase;
	s->carrier_phase 		= 0;
	s->carrier_phase_mod 	= 0;
	s->code_nco				= CODE_RATE + result.doppler*CODE_RATE/L1;
	s->carrier_nco			= IF_FREQUENCY + result.doppler;
	s->_1ms_epoch 			= 0;
	s->_20ms_epoch			= 0;
	s->rollover 			= (int32) ceil(((double)CODE_CHIPS - s->code_phase)*SAMPLE_FREQUENCY/s->code_nco); /* Calculate rollover point */

	GetPRN(s);

	nco_phase_inc = (uint32)floor((double)s->carrier_nco*(double)2.097152000000000e+03);
	nco_phase = 0;

	//inc = (int32)floor(result.delay*2048.0/1023.0);

	/* Initialize the code bin pointers */
	bin = (int32) floor((s->code_phase_mod + 0.5)*CODE_BINS + 0.5) + CODE_BINS/2;
	if(bin < 0)	bin = 0; if(bin > 2*CODE_BINS) bin = 2*CODE_BINS;
	s->pcode[0] = s->code_rows[bin];
	s->pcode[0] += inc;
	s->cbin[0] = bin;

	bin = (int32) floor((s->code_phase_mod + 0.0)*CODE_BINS + 0.5) + CODE_BINS/2;
	if(bin < 0)	bin = 0; if(bin > 2*CODE_BINS) bin = 2*CODE_BINS;
	s->pcode[1] = s->code_rows[bin];
	s->pcode[1] += inc;
	s->cbin[1] = bin;

	bin = (int32) floor((s->code_phase_mod - 0.5)*CODE_BINS + 0.5) + CODE_BINS/2;
	if(bin < 0)	bin = 0; if(bin > 2*CODE_BINS) bin = 2*CODE_BINS;
	s->pcode[2] = s->code_rows[bin];
	s->pcode[2] += inc;
	s->cbin[2] = bin;

	/* Update pointer to pre-sampled sine vector */
	bin = (int32) floor((s->carrier_nco - IF_FREQUENCY)/CARRIER_SPACING + 0.5) + CARRIER_BINS;

	/* Catch errors if Doppler goes out of range */
	if(bin < 0)	bin = 0; if(bin > 2*CARRIER_BINS) bin = 2*CARRIER_BINS;
	s->psine = main_sine_rows[bin];
	s->sbin = bin;

	//printf("Correlator initialized %d,%d,%f,%f,%f,%d,%d\n",chan,result.sv,state.carrier_nco,state.code_nco,state.code_phase,packet.count,result.count);

}
/*----------------------------------------------------------------------------------------------*/



