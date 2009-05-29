/*----------------------------------------------------------------------------------------------*/
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

GPS_Source::GPS_Source(int32 _type) {
	// TODO Auto-generated constructor stub

}

GPS_Source::~GPS_Source() {
	// TODO Auto-generated destructor stub
}


void GPS_Source::Open_USRP_V1()
{

	/* Make the URX */
	urx = usrp_standard_rx::make(0, _opt->decimate, 1, -1, 0, 0, 0);
	if(urx == NULL)
	{
		if(_opt->verbose)
			printf("usrp_standard_rx::make FAILED\n");
	}

	/* Set mux */
	urx->set_mux(0x32103210);

	/* N channels according to which mode we are operating in */
	if(_opt->mode == 0)
		urx->set_nchannels(1);
	else
		urx->set_nchannels(2);

	/* Set the decimation */
	urx->set_decim_rate(_opt->decimate);

	/* Setup board A */
	if(urx->daughterboard_id(0) == 2)
	{
		dbs_rx_a = new db_dbs_rx(urx, 0);

		/* Set the default master clock freq */
		dbs_rx_a->set_fpga_master_clock_freq(_opt->f_sample);
		dbs_rx_a->set_refclk_divisor(16);
		dbs_rx_a->enable_refclk(true);

		/* Program the board */
		dbs_rx_a->bandwidth(_opt->bandwidth);
		dbs_rx_a->if_gain(_opt->gi);
		dbs_rx_a->rf_gain(_opt->gr);
		dbs_rx_a->tune(_opt->f_lo_a);

		/* Add additional frequency to ddc to account for imprecise LO programming */
		ddc_correct_a = dbs_rx_a->freq() - _opt->f_lo_a;

		/* Set the DDC frequency */
		_opt->f_ddc_a += ddc_correct_a;
		_opt->f_ddc_a *= F_SAMPLE_NOM/_opt->f_sample;

		if(_opt->f_ddc_a > (F_SAMPLE_NOM/2.0))
			_opt->f_ddc_a = F_SAMPLE_NOM - _opt->f_ddc_a;

		urx->set_rx_freq(0, _opt->f_ddc_a);

		/* Reset DDC phase to zero */
		urx->set_ddc_phase(0, 0);

		if(_opt->verbose)
		{
			printf("DBS-RX A Configuration\n");
			printf("DBS-RX A BW: \t\t% 15.2f\n",dbs_rx_a->bw());
			printf("DBS-RX A LO: \t\t% 15.2f\n",dbs_rx_a->freq());
			printf("DBS-RX A IF Gain: \t% 15.2f\n",dbs_rx_a->if_gain());
			printf("DBS-RX A RF Gain: \t% 15.2f\n",dbs_rx_a->rf_gain());
			printf("DDC 0: \t\t\t% 15.2f\n",urx->rx_freq(0));
			//printf("DBS-RX A Diff: \t\t%f\n",dbs_rx_a->freq()-_opt->f_lo_a);
		}
	}

	/* Setup board B (if it exists) */
	if(urx->daughterboard_id(1) == 2)
	{
		dbs_rx_b = new db_dbs_rx(urx, 1);

		/* Even if you are not using board B, you need to enable the RF
		 * section else it screws up the CN0 on board A */

		/* Set the default master clock freq */
		dbs_rx_b->set_fpga_master_clock_freq(_opt->f_sample);
		dbs_rx_b->set_refclk_divisor(16);
		dbs_rx_b->enable_refclk(false);

		/* Program the board */
		dbs_rx_b->bandwidth(_opt->bandwidth);
		dbs_rx_b->if_gain(_opt->gi);
		dbs_rx_b->rf_gain(_opt->gr);
		dbs_rx_b->tune(_opt->f_lo_b);

		/* Dual board mode */
		if(_opt->mode)
		{

			/* Add additional frequency to ddc to account for imprecise LO programming */
			ddc_correct_b = dbs_rx_b->freq() - _opt->f_lo_b;

			/* Set the DDC frequency */
			_opt->f_ddc_b += ddc_correct_b;
			_opt->f_ddc_b *= F_SAMPLE_NOM/_opt->f_sample;

			if(_opt->f_ddc_b > (F_SAMPLE_NOM/2.0))
				_opt->f_ddc_b = F_SAMPLE_NOM - _opt->f_ddc_b;

			urx->set_rx_freq(1, _opt->f_ddc_b);

			/* Set mux for both channels */
			urx->set_mux(0x32103210);

			urx->set_ddc_phase(1, 0);

			if(_opt->verbose)
			{
				printf("DBS-RX B Actual Configuration\n");
				printf("DBS-RX B BW: \t\t% 15.2f\n",dbs_rx_b->bw());
				printf("DBS-RX B LO: \t\t% 15.2f\n",dbs_rx_b->freq());
				printf("DBS-RX B IF Gain: \t% 15.2f\n",dbs_rx_b->if_gain());
				printf("DBS-RX B RF Gain: \t% 15.2f\n",dbs_rx_b->rf_gain());
				printf("DDC 1: \t\t\t% 15.2f\n",urx->rx_freq(1));
				//printf("DBS-RX B Diff: \t\t%f\n",dbs_rx_b->freq()-_opt->f_lo_b);
			}

		}
	}

	/* Start collecting data */
	printf("USRP Start\n");

	urx->start();

	/* Clear out any junk data */
	for(lcv = 0; lcv < 10; lcv++)
		urx->read(&buff[0], buffsize, &overrun);


}

void GPS_Source::Read(ms_packet *_p)
{

	if(_opt->f_sample == 65.536e6)
	{
		if(_opt->mode == 0)
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
		if(_opt->mode == 0)
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

	switch(sample_mode)
	{
		case 0:
			resample(buff, buff_out, _opt);
			write_pipe(buff_out, fifo_pipe, bwrite);
			leftover = 0;
			break;
		case 1:
			leftover += 2048; leftover %= 4096;
			if(leftover == 0)
			{
				resample(buff, buff_out, _opt);
				write_pipe(buff_out, fifo_pipe, bwrite);
			}
			break;
		case 2:

			leftover += 96;
			resample(buff, buff_out, _opt);
			write_pipe(buff_out, fifo_pipe, bwrite);

			/* Move excess bytes at end of buffer down to the base */
			memcpy(db_a, &buff[4000], leftover*sizeof(int));
			memcpy(buff, db_a, leftover*sizeof(int));

			if(leftover > 4000)
			{
				resample(buff, buff_out, _opt);
				write_pipe(buff_out, fifo_pipe, bwrite);
				leftover -= 4000;
				memcpy(db_a, &buff[4000], leftover*sizeof(int));
				memcpy(buff, db_a, leftover*sizeof(int));
			}
			break;

		case 3:

			leftover += 192;
			resample(buff, buff_out, _opt);
			write_pipe(buff_out, fifo_pipe, bwrite);
			/* Move excess bytes at end of buffer down to the base */
			memcpy(db_a, &buff[8000], leftover*sizeof(int));
			memcpy(buff, db_a, leftover*sizeof(int));

			if(leftover > 8000)
			{
				resample(buff, buff_out, _opt);
				write_pipe(buff_out, fifo_pipe, bwrite);
				leftover -= 8000;
				memcpy(db_a, &buff[8000], leftover*sizeof(int));
				memcpy(buff, db_a, leftover*sizeof(int));
			}
			break;

		default:
			break;
	}

}
