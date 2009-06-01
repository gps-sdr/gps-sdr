#ifndef DB_DBS_RX_H_

/* -*- c++ -*- */
/*
 * Copyright 2007 Free Software Foundation, Inc.
 * 
 * This file is part of GNU Radio
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
 * ----------------------------------------------------------------------
 * DB-SRX Daughter-card Interface
 *
 * This class allows a user to control the configuration of the DB-SRX
 * daughter-card. A handle to a usrp_standard_rx object, and which RX
 * board this object will control is passed to the constructor. The
 * DB-SRX is initialized with 0 dB of gain. The tune() function allows a
 * user to command the LO downconvert frequency of the Max2118 chip. The
 * gain() functions allows control of the 105 dB of gain on the
 * DB-SRX daughtercard. Further user gain control is provided via the 
 * rf_gain() and if_gain() functions. The rf_gain() function allows a
 * user to control the gain, prior to mixing, from 0 to 56 dB. The
 * if_gain() function provides a further 49 dB of gain. The Max2118
 * also contains a programmable bandwidth low-pass filter after the
 * down-conversion. The low-pass filter's 3 dB cutoff point can be
 * changed from 1 to 33 MHz. The effect total bandwidth of the DB-SRX
 * (assuming the frequency of interest is mixed to DC) is twice the 3 dB
 * cutoff frequency. Appropriate accessor methods are also included.
 * ----------------------------------------------------------------------
 */

#include <usrp_standard.h>
#include <usrp_i2c_addr.h>
#include <fpga_regs_standard.h>

class db_dbs_rx
{

	private:

			/* Max2118 Downcovert chip parameters */
			int d_n;
			int d_div2;
			int d_osc;
			int d_cp;
			int d_r;
			int d_r_int;
			int d_fdac;
			int d_m;
			int d_adl;
			int d_ade;
			int d_diag;

			/* Gain stages, from 0 to 104 dB of gain */
			int	d_gc1;				/* GC1 = 0 to 56 dB gain,   controllable via DAC */
			int	d_gc2;				/* GC2 = 0 to 24 dB gain,   controllable via I2C */
			int	d_dl;				/* dl  = 0 OR 4.58 dB gain, controllable via I2C */
			double d_pga;			/* pga = 0 to 20 dB gain,   controllable via DAC */

			/* Board RXA or Board RXB */
			int d_which;

			/* I2C Address for this daughtercard */
			int d_I2C_ADDR;

			/* Reference Clock Register */
			int d_REFCLK_REG;
			int d_refclk_divisor;

			double d_freq;			/* LO frequency (Hz) */
			double d_bw;			/* Lowpass filter bandwidth (Hz) */
			double d_gain;			/* Total gain (0 to 105 dB) */
			double d_rf_gain;		/* RF gain (0 to 56 dB) */
			double d_if_gain;		/* IF gain (0 to 49 dB) */

			double fpga_master_clock_freq;
			
			usrp_standard_rx *d_urx; /* Pointer to usrp_standard_rx object */

	protected:

			/*! Write to MAX2118 Registers */
			void write_MAX2118();					
			/*! Send a single register */
			bool send_reg(int _reg);				
			/*! Read PLL ADC */
			int  read_adc();						
			/*! Not sure what this is for */
			void bypass_adc_buffers(bool _bypass);	

			/*! Write the GC1 value (FPGA) */
			void set_gc1(int _gc1);					
			/*! Write the GC2 value (register 5) */
			void set_gc2(int _gc2);					
			/*! Write the PGA value (FPGA) */
			void set_pga(double _pga);				
			
			/*! Write the N	   value (registers 0 and 1) */
			void set_n(int _n);						
			/*! Write the DIV2  value (register 0) */
			void set_div2(int _div2);				
			/*! Write the OSC   value (register 2) */
			void set_osc(int _osc);					
			/*! Write the CP    value (register 2) */
			void set_cp(int _cp);					
			/*! Write the R     value (register 2) */
			void set_r(int _r);						
			/*! Write the FDAC  value (register 3) */
			void set_fdac(int _fdac);				
			/*! Write the M     value (register 4) */
			void set_m(int _m);						
			/*! Write the DL    value (register 4) */
			void set_dl(int _dl);					
			/*! Write the ADE   value (register 4) */
			void set_ade(int _ade);					
			/*! Write the ADL   value (register 4) */
			void set_adl(int _adl);					
			/*! Write the DIAG  value (register 5) */
			void set_diag(int _diag);				

	public:

			/*! Constructor */
			db_dbs_rx(usrp_standard_rx *_urx, int _which);		

			/*! Destructor */
			~db_dbs_rx();										

			/* Accessors */
			
			/*! Set clock frequency into the Max2118 */
			void set_refclk_divisor(int _div);		
			/*! Enable the refclk */
			void enable_refclk(bool _enable);	
			/*! Return commanded refclk frequency */
			double refclk_freq();
			/*! Set	fpga_master_clock_freq */	
			void set_fpga_master_clock_freq(double _freq);			
			

			/*!	Return the LO frequency	*/
			double freq()			const {return d_freq;}		
			/*!	Return the lowpass filter bandwidth	*/
			double bw()				const {return d_bw;}		
			/*!	Return the total gain */
			double gain()			const {return d_gain;}		
			/*!	Return the RF gain */
			double rf_gain()		const {return d_rf_gain;}	
			/*!	Return the IF gain */
			double if_gain()		const {return d_if_gain;}	
			/*!	Return the minimum gain */
			double min_gain()		const {return 0.0;}			
			/*!	Return the maximum gain */
			double max_gain()		const {return 105.0;}		
			/*!	Return the minimum RF gain */
			double min_rf_gain()	const {return 0.0;}			
			/*!	Return the maximum RF gain */
			double max_rf_gain()	const {return 56.0;}		
			/*!	Return the minimum IF gain */
			double min_if_gain()	const {return 0.0;}			
			/*!	Return the maximum IF gain */
			double max_if_gain()	const {return 49.0;}		

			/* Modifiers */

			/*! Tune Max2118 to frequency _freq Hz */
			double tune(double _freq);							
			/*! Set lowpass filter in Max2118 to _bw Hz */
			double bandwidth(double _bw);					
			/*! Set the gain */
			double gain(double _gain);						
			/*! Set the RF gain */
			double rf_gain(double _gain);					
			/*! Set the IF gain */
			double if_gain(double _gain);					
			

};
#define DB_DBS_RX_H_

#endif /*DB_DBS_RX_H_*/
