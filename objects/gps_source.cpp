/*----------------------------------------------------------------------------------------------*/
/*! \file gps_source.cpp
//
// FILENAME: gps_source.cpp
//
// DESCRIPTION: Implements member functions of the GPS_Source class.
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

#include "gps_source.h"

GPS_Source::GPS_Source(Options_S *_opt)
{

	memcpy(&opt, _opt, sizeof(Options_S));
	Open_USRP_V1();

	if(gopt.verbose)
		fprintf(stdout,"Creating GPS Source\n");
}

GPS_Source::~GPS_Source()
{
	Close_USRP_V1();

	if(gopt.verbose)
		fprintf(stdout,"Destructing GPS Source\n");
}

void GPS_Source::Open_USRP_V1()
{

	double ddc_correct_a = 0;
	double ddc_correct_b = 0;

	leftover = 0;

	if(opt.f_sample == 65.536e6)
	{
		if(opt.mode == 0)
		{
			sample_mode = 0;
			bwrite = 2048 * sizeof(CPX);
		}
		else
		{
			sample_mode = 1;
			bwrite = 4096 * sizeof(CPX);
		}
	}
	else
	{
		if(opt.mode == 0)
		{
			sample_mode = 2;
			bwrite = 2048 * sizeof(CPX);
		}
		else
		{
			sample_mode = 3;
			bwrite = 4096 * sizeof(CPX);
		}
	}

	/* Make the URX */
	urx = usrp_standard_rx::make(0, opt.decimate, 1, -1, 0, 0, 0);
	if(urx == NULL)
	{
		if(opt.verbose)
			fprintf(stdout,"usrp_standard_rx::make FAILED\n");
	}

	/* Set mux */
	urx->set_mux(0x32103210);

	/* N channels according to which mode we are operating in */
	if(opt.mode == 0)
		urx->set_nchannels(1);
	else
		urx->set_nchannels(2);

	/* Set the decimation */
	urx->set_decim_rate(opt.decimate);

	/* Setup board A */
	if(urx->daughterboard_id(0) == 2)
	{
		dbs_rx_a = new db_dbs_rx(urx, 0);

		/* Set the default master clock freq */
		dbs_rx_a->set_fpga_master_clock_freq(opt.f_sample);
		dbs_rx_a->set_refclk_divisor(16);
		dbs_rx_a->enable_refclk(true);

		/* Program the board */
		dbs_rx_a->bandwidth(opt.bandwidth);
		dbs_rx_a->if_gain(opt.gi);
		dbs_rx_a->rf_gain(opt.gr);
		dbs_rx_a->tune(opt.f_lo_a);

		/* Add additional frequency to ddc to account for imprecise LO programming */
		ddc_correct_a = dbs_rx_a->freq() - opt.f_lo_a;

		/* Set the DDC frequency */
		opt.f_ddc_a += ddc_correct_a;
		opt.f_ddc_a *= F_SAMPLE_NOM/opt.f_sample;

		if(opt.f_ddc_a > (F_SAMPLE_NOM/2.0))
			opt.f_ddc_a = F_SAMPLE_NOM - opt.f_ddc_a;

		urx->set_rx_freq(0, opt.f_ddc_a);

		/* Reset DDC phase to zero */
		urx->set_ddc_phase(0, 0);

		if(opt.verbose)
		{
			fprintf(stdout,"DBS-RX A Configuration\n");
			fprintf(stdout,"BW:      %15.2f\n",dbs_rx_a->bw());
			fprintf(stdout,"LO:      %15.2f\n",dbs_rx_a->freq());
			fprintf(stdout,"IF Gain: %15.2f\n",dbs_rx_a->if_gain());
			fprintf(stdout,"RF Gain: %15.2f\n",dbs_rx_a->rf_gain());
			fprintf(stdout,"DDC 0:   %15.2f\n",urx->rx_freq(0));
		}
	}

	/* Setup board B (if it exists) */
	if(urx->daughterboard_id(1) == 2)
	{
		dbs_rx_b = new db_dbs_rx(urx, 1);

		/* Even if you are not using board B, you need to enable the RF
		 * section else it screws up the CN0 on board A */

		/* Set the default master clock freq */
		dbs_rx_b->set_fpga_master_clock_freq(opt.f_sample);
		dbs_rx_b->set_refclk_divisor(16);
		dbs_rx_b->enable_refclk(false);

		/* Program the board */
		dbs_rx_b->bandwidth(opt.bandwidth);
		dbs_rx_b->if_gain(opt.gi);
		dbs_rx_b->rf_gain(opt.gr);
		dbs_rx_b->tune(opt.f_lo_b);

		/* Dual board mode */
		if(opt.mode)
		{

			/* Add additional frequency to ddc to account for imprecise LO programming */
			ddc_correct_b = dbs_rx_b->freq() - opt.f_lo_b;

			/* Set the DDC frequency */
			opt.f_ddc_b += ddc_correct_b;
			opt.f_ddc_b *= F_SAMPLE_NOM/opt.f_sample;

			if(opt.f_ddc_b > (F_SAMPLE_NOM/2.0))
				opt.f_ddc_b = F_SAMPLE_NOM - opt.f_ddc_b;

			urx->set_rx_freq(1, opt.f_ddc_b);

			/* Set mux for both channels */
			urx->set_mux(0x32103210);

			urx->set_ddc_phase(1, 0);

			if(opt.verbose)
			{
				fprintf(stdout,"DBS-RX B Configuration\n");
				fprintf(stdout,"BW:      %15.2f\n",dbs_rx_b->bw());
				fprintf(stdout,"LO:      %15.2f\n",dbs_rx_b->freq());
				fprintf(stdout,"IF Gain: %15.2f\n",dbs_rx_b->if_gain());
				fprintf(stdout,"RF Gain: %15.2f\n",dbs_rx_b->rf_gain());
				fprintf(stdout,"DDC 0:   %15.2f\n",urx->rx_freq(0));

			}

		}
	}

	/* Start collecting data */
	fprintf(stdout,"USRP Start\n");

	urx->start();

}


void GPS_Source::Close_USRP_V1()
{

	urx->stop();

	if(dbs_rx_a != NULL)
		delete dbs_rx_a;

	if(dbs_rx_b != NULL)
		delete dbs_rx_b;

	if(urx != NULL)
		delete urx;

	if(opt.verbose)
		fprintf(stdout,"Destructing USRP\n");

}

void GPS_Source::Read(ms_packet *_p)
{

	bool overrun;

	/* There may be a packet waiting already, if so copy and return immediately */
	switch(sample_mode)
	{
		case 2:

			if(leftover > 4000)
			{
				Resample_USRP_V1(buff, buff_out);
				memcpy(_p->data, buff_out, SAMPS_MS*sizeof(CPX));
				leftover -= 4000;
				memcpy(db_a, &buff[4000], leftover*sizeof(int32));
				memcpy(buff, db_a, leftover*sizeof(int32));
				return;
			}
			break;

		case 3:

			if(leftover > 8000)
			{
				Resample_USRP_V1(buff, buff_out);
				memcpy(_p->data, buff_out, SAMPS_MS*sizeof(CPX));
				leftover -= 8000;
				memcpy(db_a, &buff[8000], leftover*sizeof(int32));
				memcpy(buff, db_a, leftover*sizeof(int32));
				return;
			}
			break;

		default:
			break;
	}

	/* Read data from USRP */
	urx->read(&buff[leftover], BYTES_PER_READ, &overrun);

//	if(overrun && _opt->verbose)
//	{
//		time(&rawtime);
//		timeinfo = localtime (&rawtime);
//		ffprintf(stdout,stdout, "\nUSRP overflow at time %s",asctime (timeinfo));
//		fflush(stdout);
//	}


	/* Now we have SAMPS_PER_READ samps, 4 possible things to do depending on the state:
	 * 0) mode == 0 && f_sample == 65.536e6: This mode is the easiest, 1 ms of data per FIFO node,
	 * hence just call resample() and stuff in the pipe
	 * 1) mode == 1/2 && f_sample == 65.536e6: This mode is also easy, 2 nodes = 1 ms, hence buffer 2 nodes together
	 * and then call resample();
	 * 2) mode == 0 && f_sample == 64.0e6: 1 node is slightly larger (4096 vs 4000) than 1 ms, must first double
	 * buffer the data to extract continuous 4000 sample packets then call resample()
	 * 3) mode == 1/2 && f_sample == 64.0e6: must take 2 nodes, create 8192 sample buffer, and similarly extract 8000
	 * sample packet and call resample(), requires double buffering */
	switch(sample_mode)
	{
		case 0:
			Resample_USRP_V1(buff, buff_out);
			memcpy(_p->data, buff_out, SAMPS_MS*sizeof(CPX));
			leftover = 0;
			break;
		case 1:
			leftover += 2048; leftover %= 4096;
			if(leftover == 0)
			{
				Resample_USRP_V1(buff, buff_out);
				memcpy(_p->data, buff_out, SAMPS_MS*sizeof(CPX));
			}
			break;
		case 2:

			leftover += 96;
			Resample_USRP_V1(buff, buff_out);
			memcpy(_p->data, buff_out, SAMPS_MS*sizeof(CPX));

			/* Move excess bytes at end of buffer down to the base */
			memcpy(db_a, &buff[4000], leftover*sizeof(int32));
			memcpy(buff, db_a, leftover*sizeof(int32));
			break;

		case 3:

			leftover += 192;
			Resample_USRP_V1(buff, buff_out);
			memcpy(_p->data, buff_out, SAMPS_MS*sizeof(CPX));

			/* Move excess bytes at end of buffer down to the base */
			memcpy(db_a, &buff[8000], leftover*sizeof(int32));
			memcpy(buff, db_a, leftover*sizeof(int32));
			break;

		default:
			break;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void GPS_Source::Resample_USRP_V1(CPX *_in, CPX *_out)
{

	CPX buff_a[4096]; /* Max size based on 65.536 or 64 Msps */
	CPX buff_b[4096]; /* Max size based on 65.536 or 64 Msps */
	int32 *p_a;
	int32 *p_b;
	int32 *p_in;
	int32 *p_out;
	int32 samps_ms;
	int32 lcv;

	p_a = (int32 *)&buff_a[0];
	p_b = (int32 *)&buff_b[0];
	p_in = (int32 *)_in;
	p_out = (int32 *)_out;

	samps_ms = (int32)floor(opt.f_sample/opt.decimate/1e3);

	if(opt.mode == 0)
	{
		/* Not much to do, just downsample from either 4.096 or 4.0 to 2.048e6 */
		if(opt.f_sample != 65.536e6)
			downsample(_out, _in, 2.048e6, opt.f_sample/opt.decimate, samps_ms);
		else
		{
			for(lcv = 0; lcv < samps_ms; lcv += 2)
				*p_out++ = p_in[lcv];
		}
	}
	else //!< 2 boards are being used, must first de-interleave data before downsampling
	{

		/* De-interleave */
		for(lcv = 0; lcv < samps_ms; lcv++)
		{
			p_a[lcv] = *p_in++;
			p_b[lcv] = *p_in++;
		}

		/* Downsample (and copy!) into appropriate location */
		downsample(&_out[0],    buff_a, 2.048e6, opt.f_sample/opt.decimate, samps_ms);
		downsample(&_out[2048], buff_b, 2.048e6, opt.f_sample/opt.decimate, samps_ms);

	}

}
