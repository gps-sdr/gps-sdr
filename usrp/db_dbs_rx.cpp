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

#include "db_dbs_rx.h"
#include <math.h>

/*!
	db_dbs_rx(usrp_standard_rx *_urx, int _which)

	_urx must be a pointer to an already invoked usrp_standard_rx object
	_which is either 0 or 1, for RX-A or RX-B, respectively
*/
db_dbs_rx::db_dbs_rx(usrp_standard_rx *_urx, int _which)
{

	/* Get a pointer to the object */
	d_urx = _urx;

	/* Which board (0 for RXA, 1 for RXB) */
	d_which = _which;

	/* Set Default Values */
	d_n = 950;
	d_div2 = 0;
	d_osc = 5;
	d_cp = 3;
	d_r = 4;
	d_r_int = 1;
	d_fdac = 127;
	d_m = 2;
	d_dl = 0;
	d_ade = 0;
	d_adl = 0;
	d_gc2 = 31;
	d_diag = 5;

	/* Gain stages, from 0 to 104 dB of gain */
	d_gc1 = 3227;
	d_gc2 = 31;
	d_dl = 0;
	d_pga = 0;

	d_freq    = 0;	/* LO frequency (Hz) */
	d_bw      = 0;	/* Lowpass filter bandwidth (Hz) */
	d_gain    = 0;	/* Gain (0 to 105 dB) */
	d_rf_gain = 0;	/* RF gain (0 to 56 dB) */
	d_if_gain = 0;	/* IF gain (0 to 49 dB) */

	//usrp_standard_rx *d_urx; /* Pointer to usrp_standard_rx object */

	/* This should be dependant on which RX board this is (0 or 1) */
	d_I2C_ADDR = d_which ? 0x65 : 0x67;

	/* Refclk Address, same deal */
	d_REFCLK_REG = d_which ? FR_RX_B_REFCLK : FR_RX_A_REFCLK;

	/* Set the default master clock freq */
	set_fpga_master_clock_freq(64.0e6);

	/* Set the refclk divisor */
	set_refclk_divisor(16);

	/* Enable the refclk */
	enable_refclk(true);

	/* Bypass ADC Buffers */
	bypass_adc_buffers(true);

	/* Write to the MAX2118 to initialize the board */
	write_MAX2118();

	/* set gain to 0 dB initially */
	gain(0);

}


/*!
	~db_dbs_rx()

	Nothing to do here
*/
db_dbs_rx::~db_dbs_rx()
{






}


/*!
	set_m(int _m)

	Sets M, the register that controls the lowpass filter width
*/
void db_dbs_rx::set_m(int _m)
{
	if((_m>0) && (_m<32))
	{
        d_m = _m;
        send_reg(4);
	}
}


/*!
	set_fdac(int _fdac)

	Sets FADC, the value for the lowpass filter -3 dB cutoff frequency
*/
void db_dbs_rx::set_fdac(int _fdac)
{
	if((_fdac>=0) && (_fdac<128))
	{
		d_fdac = _fdac;
		send_reg(3);
	}
}


/*!
	bandwidth(double _bw)

	Sets the bandwidth of the lowpass filter
*/
double db_dbs_rx::bandwidth(double _bw)
{

	double thresh;
	//double fxtal = refclk_freq();

	if((_bw<1e6) || (_bw>33e6))
		return(0);

	int m_max = 0;

    if(_bw >= 4e6)
	{

		thresh = (double)floor(refclk_freq()/1e6);
		m_max = (int)(31.0 < thresh ? 31.0 : thresh);
	}
    else if (_bw >= 2e6)
	{
		thresh = (double)floor(refclk_freq()/.5e6);
		m_max = (int)(31.0 < thresh ? 31.0 : thresh);
	}
    else
	{
		thresh = (double)floor(refclk_freq()/.25e6);
		m_max = (int)(31.0 < thresh ? 31.0 : thresh);
	}


    int m_min = (int) ceil(refclk_freq()/2.5e6);

    int m_test = m_max;
	int fdac_test = 0;

    while(m_test >= m_min)
	{
        fdac_test = int(floor(((_bw * m_test / refclk_freq())-4)/.145) + 0.5);

        if(fdac_test>127)
            m_test = m_test - 1;
        else
            break;
	}

    if ((m_test>=m_min) && (fdac_test >=0))
	{
        set_m(m_test);
        set_fdac(fdac_test);
		d_bw = refclk_freq()/d_m*(4+0.145*(double)d_fdac);
		return(d_bw);
	}
    else
	{
		d_bw = 0;
		return(0);
	}
}


/*!
	set_gc1(int _gc1)

	Sets GC1, which controls the RF gain
*/
void db_dbs_rx::set_gc1(int _gc1)
{

	d_gc1 = _gc1;
	d_urx->write_aux_dac(d_which, 0, d_gc1);

}


/*!
	set_gc2(int _gc2)

	Sets GC2, which controls the IF gain
*/
void db_dbs_rx::set_gc2(int _gc2)
{

	if((_gc2>=0) && (_gc2<32))
	{
		d_gc2 = _gc2;
		send_reg(5);
	}

}


/*!
	set_dl(int _dl)

	Sets DL, which adds 0 or 4.58 dB of IF gain
*/
void db_dbs_rx::set_dl(int _dl)
{

	if((_dl==0) || (_dl==1))
	{
		d_dl = _dl;
		send_reg(4);
	}

}


/*!
	set_pga(double _pga)

	Sets the PGA on the
*/
void db_dbs_rx::set_pga(double _pga)
{
	if((_pga>0.0) && (_pga<=20.0))
	{
		d_urx->set_pga(d_which, _pga);
		d_urx->set_pga(d_which+1, _pga);
	}
}


/*!
	gain(double _gain)

	Sets the total gain of the DBS-RX daughter board
*/
double db_dbs_rx::gain(double _gain)
{

	/* Check for limits */
	if((_gain < 0) || (_gain > 105))
	{
		d_gain = 0;
		d_rf_gain = 0;
		d_if_gain = 0;
		d_gc1 = 3227;
		d_gc2 = 31;
		d_dl = 0;
		d_pga = 0;
		return(-1);
	}

	if(_gain < 56)
	{
		rf_gain(_gain);
		d_gain = d_rf_gain + d_if_gain;
	}
	else
	{
		rf_gain(56);
		_gain = _gain - 56;
		if_gain(_gain);
		d_gain = d_rf_gain + d_if_gain;
	}

	return(d_gain);

}


/*!
	rf_gain(double _gain)

	Sets only the RF gain of the DBS-RX daughter board
*/
double db_dbs_rx::rf_gain(double _gain)
{

	/* Check for limits */
	if((_gain < 0) || (_gain > 56))
	{
		d_gc1 = 3227;
		return(-1);
	}
	else
		d_rf_gain = _gain;

	/* Calculate voltage */
	d_gc1 = int((2.6 - (_gain*1.85/56.0))*4096.0/3.3);

	/* Write out */
	set_gc1(d_gc1);

	/* Recalculate total gain */
	d_gain = d_if_gain + d_rf_gain;

	return(d_rf_gain);

}


/*!
	if_gain(double _gain)

	Sets only the IF gain of the DBS-RX daughter board
*/
double db_dbs_rx::if_gain(double _gain)
{

	/* Check for limits */
	if((_gain < 0) || (_gain > 49))
	{
		d_if_gain = 0;
		d_gc2 = 31;
		d_dl = 0;
		d_pga = 0;
		return(-1);
	}
	else
		d_if_gain = _gain;

	/* Set to minimum gain */
	d_gc2 = 31;
	d_dl = 0;
	d_pga = 0;

	/* GC2 of Max2118 */
	if(_gain < 24)
	{
		d_gc2 = int(floor(31.0*(1-_gain/24.0)+0.5));
		_gain = 0;
	}
	else
	{
		d_gc2 = 0;
		_gain = _gain - 24;
	}

	/* DL of Max2118 */
	if(_gain > 4.58)
	{
		d_dl = 1;
		_gain = _gain - 4.58;
	}

	/* PGA On AD3841 Chip */
	if(_gain < 20)
	{
		d_pga = _gain;
		_gain = 0;
	}
	else
	{
		d_pga = 20;
		_gain = _gain - 20;
	}

	/* Write out values */
	set_gc2(d_gc2);
	set_dl(d_dl);
	set_pga(d_pga);

	/* Recalculate total gain */
	d_gain = d_if_gain + d_rf_gain;

	return(d_if_gain);

}


/*!
	set_osc(int _osc)

	Chooses which oscillator source to use on Max2118
*/
void db_dbs_rx::set_osc(int _osc)
{
	if((_osc>=0) && (_osc<8))
	{
		d_osc = _osc;
		send_reg(2);
	}
}


/*!
	set_cp(int _cp)

	Sets the charge pump register of the Max2118
*/
void db_dbs_rx::set_cp(int _cp)
{
	if((_cp>=0) && (_cp<4))
	{
		d_cp = _cp;
		send_reg(2);
	}
}


/*!
	set_r(int _r)

	Sets the R divider used to phase lock the LO
*/
void db_dbs_rx::set_r(int _r)
{

	/* r_int = log2(r)-1 */
	if((_r >= 0) && (_r < 256))
	{
		d_r = _r;
		_r >>= 1;
		d_r_int = 0;
		while(_r > 1)
		{
			d_r_int++;
			_r >>= 1;
		}
		send_reg(2);
	}

}


/*!
	set_div2(int _div2)

	Sets DIV2 register of Max2118
*/
void db_dbs_rx::set_div2(int _div2)
{

	if((_div2 == 0) || (_div2 == 1))
	{
		d_div2 = _div2;
		send_reg(0);
	}

}


/*!
	set_n(int _n)

	Sets the N divider used to phase lock the LO
*/
void db_dbs_rx::set_n(int _n)
{

	if((_n > 256) && (_n < 32768))
	{
		d_n = _n;
		send_reg(0);
		send_reg(1);
	}

}


/*!
	set_ade(int _ade)

	Sets the AD enable bit
*/
void db_dbs_rx::set_ade(int _ade)
{

	if((_ade==0) || (_ade==1))
	{
		d_ade = _ade;
		send_reg(4);
	}

}


/*!
	set_adl(int _adl)

	Latches the AD value
*/
void db_dbs_rx::set_adl(int _adl)
{

	if((_adl==0) || (_adl==1))
	{
		d_adl = _adl;
		send_reg(4);
	}

}


/*!
	set_diag(int _diag)

	Sets the DIAG register
*/
void db_dbs_rx::set_diag(int _diag)
{

	if((_diag>=0) && (_diag<8) && (_diag!=4))
	{
		d_diag = _diag;
		send_reg(5);
	}

}


/*!
	tune(double _freq)

	Sets the LO frequency of the DBS-RX daughterbaord
*/
double db_dbs_rx::tune(double _freq)
{

	int vco = 0;
	int n_guess = 0;
	int best_r = 0;
	int best_n = 0;
	double vcofreq = 0;
	double best_delta = 100e6;
	double delta = 0;

	d_freq = 0;

	/* Make sure the _freq is in range of the Max2118 */
	if((_freq<500e6) || (_freq>2.6e9))
		return(-1);

	/* Set the VCO divider */
	if(_freq < 1125e6)
	{
		set_div2(0);
		vcofreq = 4*_freq;
	}
	else
	{
		set_div2(1);
		vcofreq = 2*_freq;
	}


	/* Set maximum value of r, to gaurantee that the comparator frequency is at least 250 kHz
		(anything less than this introduces instability in the Max2118 PLL) */
	int max_r = 256;
	while((refclk_freq()/(double)max_r) < 250000.0)
		max_r >>= 1;

	/* Find r and n to produce to closest possible frequency */
	for(; max_r >= 2; max_r >>= 1)
	{

		/* Find n for this r */
		n_guess = (int) floor(((double)max_r*_freq)/refclk_freq() + 0.5);

		/* Calculate difference from desired and actual */
		delta = (double)n_guess*refclk_freq()/(double)max_r;
		delta -= _freq;
		delta < 0 ? delta = -delta : delta = delta;

		if((n_guess > 256) && (n_guess < 32768))
		{
			if(delta < best_delta)
			{
				best_r = max_r;
				best_n = n_guess;
				best_delta = delta;
			}
		}

	}

	/* Set the value of r and n */
	set_r(best_r);
	set_n(best_n);

	 /* Choose the VCO depending on frequency */
	if(vcofreq < 2433e6)
		vco = 0;
	else if(vcofreq < 2711e6)
		vco = 1;
	else if(vcofreq < 3025e6)
		vco = 2;
	else if(vcofreq < 3341e6)
		vco = 3;
	else if(vcofreq < 3727e6)
		vco = 4;
	else if(vcofreq < 4143e6)
		vco = 5;
	else if(vcofreq < 4493e6)
		vco = 6;
	else
		vco = 7;

	/* Set the chosen VCO */
	set_osc(vco);

	/* Enable the PLL ADC */
	set_ade(1);

    /* Set charge pump current */
    int adc_val = 0;
	while((adc_val==0) || (adc_val==7))
	{
		adc_val = read_adc();
		if(adc_val==0)
		{
			if(vco == 0)
				return(0);
			else
			{
				vco = vco - 1;
				set_osc(vco);
			}
		}

		if(adc_val==7)
		{
			if(vco == 7)
				return(0);
			else
			{
				vco = vco + 1;
				set_osc(vco);
			}
		}
	}

	/* Disable the PLL ADC */
	set_ade(0);

	/* Set the charge pump value */
    if((adc_val==1) || (adc_val==2))
        set_cp(1);
	else if((adc_val==3) || (adc_val==4))
        set_cp(2);
    else
        set_cp(3);

	/* Actual frequency */
	d_freq = (double)d_n*refclk_freq()/double(d_r);

	return (d_freq);

}


/*!
	write_MAX2118()

	Writes the Max2118 registers
*/
void db_dbs_rx::write_MAX2118()
{

	send_reg(0);
	send_reg(1);
	send_reg(2);
	send_reg(3);
	send_reg(4);
	send_reg(5);

}


/*!
	read_adc()

	Read the PLL ADC value, used to tune the LO
*/
int db_dbs_rx::read_adc()
{

	std::string read_buff;
	int _adc;

	/* Latch the PLL ADC Value */
	set_adl(1);

	/* Read PLL ADC Value */
	read_buff = d_urx->read_i2c(d_I2C_ADDR, 2);

	/* Unlatch PLL ADC Value */
	set_adl(0);

	if(read_buff.length() != 2)
		return(0);

	_adc = (int)read_buff[0];
	_adc >>= 2;
	_adc &= 0x7;

	return(_adc);
}


/*!
	send_reg(int _reg)

	Send a Max2118 register
*/
bool db_dbs_rx::send_reg(int _reg)
{

	std::string buf;


	if((_reg>=0) && (_reg<6))
	{

		buf.append(1, (char)_reg);

		switch(_reg)
		{
			case 0:
				buf.append(1, (char)((d_div2<<7) + ((d_n>>8) & 0x7f)));
				break;
			case 1:
				buf.append(1, (char)(d_n & 0xff));
				break;
			case 2:
				buf.append(1, (char)(((d_osc) + (d_cp<<3) + (d_r_int<<5)) & 0xff));
				break;
			case 3:
				buf.append(1, (char)(d_fdac & 0xff));
				break;
			case 4:
				buf.append(1, (char)((d_m + (d_dl<<5) + (d_ade<<6) + (d_adl<<7)) & 0xff));
				break;
			case 5:
				buf.append(1, (char)((d_gc2 + (d_diag<<5)) & 0xff));
				break;
			default:
				break;
		}

		return(d_urx->write_i2c(d_I2C_ADDR, buf));

	}
	else
		return(false);




}


/*!
	set_refclk_divisor(int _div)
	Sets the refclk_divisor, which divides down the 64 MHz board clock
*/
void db_dbs_rx::set_refclk_divisor(int _div)
{
	d_refclk_divisor = _div;
}


/*!
	enable_refclk(bool _enable)
	Enables the refclk input to the DBS-RX daughterboard
*/
void db_dbs_rx::enable_refclk(bool _enable)
{

	unsigned int CLOCK_OUT = 1; //# Clock is on lowest bit
	unsigned int REFCLK_ENABLE = 0x80;
	unsigned int REFCLK_DIVISOR_MASK = 0x7f;
	if(_enable)
	{
		d_urx->_write_oe(d_which,CLOCK_OUT,CLOCK_OUT);
		d_urx->_write_fpga_reg(d_REFCLK_REG,((d_refclk_divisor & REFCLK_DIVISOR_MASK)|REFCLK_ENABLE));
	}
	else
	{
		d_urx->_write_fpga_reg(d_REFCLK_REG, 0x0);
	}
}

/*!
	set_fpga_master_clock_freq()
	Sets the fpga_master_clock_freq
*/
void db_dbs_rx::set_fpga_master_clock_freq(double _freq)
{
	fpga_master_clock_freq = _freq;
}

/*!
	refclk_freq()
	Returns the refclk frequency
*/
double db_dbs_rx::refclk_freq()
{
	return(fpga_master_clock_freq/d_refclk_divisor);
}


/*!
	bypass_adc_buffers(bool _bypass)

	Bypasses ADC buffers, not sure why, but this
	was done in the Python driver
*/
void db_dbs_rx::bypass_adc_buffers(bool _bypass)
{

	d_urx->set_adc_buffer_bypass(2*d_which+0, _bypass);
	d_urx->set_adc_buffer_bypass(2*d_which+1, _bypass);

}

