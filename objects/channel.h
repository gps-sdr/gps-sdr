/*! \file OBJECT.h
	Defines the class OBJECT
*/
/************************************************************************************************
Copyright 2008 Gregory W Heckler

This file is part of the GPS Software Defined Radio (GPS-SDR)

The GPS-SDR is free software; you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GPS-SDR is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License along with GPS-SDR; if not,
write to the: 

Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
************************************************************************************************/

#ifndef OBJECT_H
#define OBJECT_H

#include "includes.h"

#define FREQ_LOCK_POINTS (256)

/*! \ingroup CLASSES
 * 
 */
typedef class Channel
{

	friend class SV_Select;

	private:

		/* Status info */
		/*----------------------------------------------------------------------------------------------*/
		FILE *fp;
		int32 len;				//!< accumulation length
		int32 count;			//!< number of accumulations processed
		int32 active;			//!< is this channel active 
		int32 chan;				//!< channel number
		int32 sv;				//!< current SV
		int32 chan_pipe;		//!< named pipe for external correlation peeks
		int32 chan_hand;		//!< named pipe for external correlation peeks
		Chan_Packet_S	packet;		//!< dump to named pipe
		Chan_2_Ephem_S  ephem_packet; //!< dump to ephemeris
		/*----------------------------------------------------------------------------------------------*/

		/* Loop data */
		/*----------------------------------------------------------------------------------------------*/
		Phase_lock_loop aPLL;
		Delay_lock_loop	aDLL;
		double carrier_nco;		//!< Local carrier_nco
		double code_nco;		//!< Local code_nco
		bool frequency_lock;
		bool phase_lock;
		NCO_Command_S feedback;	//!< For feedback
		/*----------------------------------------------------------------------------------------------*/

		/* Correlations */
		/*----------------------------------------------------------------------------------------------*/
		int32 I[3];				//!< Inphase correlations
		int32 Q[3];				//!< Quadrature correlations
		int32 P[3];				//!< Power
		int32 I_prev;			//!< Previous I prompt correlation
		int32 Q_prev;			//!< Previous Q prompt correlation
		float Ip_prev;	
		float I_avg;			//!< Moving average of I
		float Q_var;			//!< Variance of Q
		float P_avg;			//!< Moving average of P
		float CN0;				//!< Current CN0 estimate
		float CN0_old;			//!< Old CN0 estimate (used to detect meta stable loop convergence)
		float NP;				//!< New CN0 estimate thing		
		/*----------------------------------------------------------------------------------------------*/

		/* Bit lock stuff */
		/*----------------------------------------------------------------------------------------------*/
		bool  bit_lock;			//!< Bitlock
		bool  bit_lock_pend;	//!< Pending to send _1ms_epoch reset command to correlator
		int32 bit_lock_ticks;	//!< Number of ticks in attempted bit lock
		int32 bit_lock_reset;	//!< Number of ticks since last change in best epoch
		int32 I_sum20;			//!< I prompt sum for past 20 ms
		int32 Q_sum20;			//!< Q prompt sum for past 20 ms
		int32 I_buff[20];		//!< I for last 20 ms
		int32 Q_buff[20];		//!< Q for last 20 ms
		int32 P_buff[20];		//!< P for last 20 ms
		int32 _20ms_epoch;		//!< _20ms epoch
		int32 _1ms_epoch;		//!< _1ms epoch
		int32 best_epoch;		//!< Best epoch from bit synch
		/*----------------------------------------------------------------------------------------------*/

		/* Data message sheit */
		/*----------------------------------------------------------------------------------------------*/
		bool valid_frame[5];	//!< Are these subframes now valid?
		bool navigate;			//!< Should we navigate on this channel?
		bool z_lock;			//!< Is the Z count synched?
		bool converged;			//!< If the channel reaches frame lock then it is considered converged
		int32 frame_z;			//!< Current z count
		int32 z_count;			//!< Zc incremented by _20ms counter
		int32 z_count_pend;		//!< Has the z count been fed to the correlator yet
		uint32 word_buff[FRAME_SIZE_PLUS_2]; //!< Buffer for 12 GPS words.
		/*----------------------------------------------------------------------------------------------*/
		
		/* Frame synch, process data bit sheit */
		/*----------------------------------------------------------------------------------------------*/
		bool frame_lock;		//!< Frame_lock
		bool frame_lock_pend;	//!< Pending to send _20ms_epoch reset command to correlator
		int32 bit_number;		//!< Ummm, the bit number in the subframe
		int32 subframe;			//!< The current subframe
		/*----------------------------------------------------------------------------------------------*/
	
		/* FFT and buffer for FFT estimate of frequency after initial lock
		/*----------------------------------------------------------------------------------------------*/
		bool freq_lock;				//!< Has the FFT estimate of frequency been completed?
		int32 freq_lock_ticks;
		CPX fft_buff[FREQ_LOCK_POINTS];	//!< Buffer for the 4 ms accumulations
		FFT *pFFT;					//!< This is where the actual FFT lives
		/*----------------------------------------------------------------------------------------------*/

	public:

		Channel(int32 _chan);
		~Channel();
		void Start(int32 sv, Acq_Result_S result, int32 _corr_len);
		void Stop();
		void Clear();
		void DumpAccum();								//!< Dump the accumulation and do rest of processing
		void FrequencyLock();							//!< Use FFT to pull in the PLL
		void PLL_W(float _bw);							//!< Change the PLL bandwidth
		void PLL();										//!< Perform the phase lock loop
		void DLL();										//!< Do the DLL
		void Epoch();									//!< Increase _1ms_epoch, _20ms_epoch		
		void BitLock();									//!< Declare the bit lock?
		void BitStuff();								//!< Get data bits from I_Sum20 and stuff them into data_buff
		void ProcessDataBit();							//!< Process the data bits, how fun!, calls the following 3 functions
			bool FrameSync(uint32 word0, uint32 word1); //!< frame synch?
			bool ParityCheck(uint32 gpsword);			//!< parity check
			bool ValidFrameFormat(uint32 *subframe);	//!< valid frame
		void Error();									//!< look for errors in tracking, killing channel if necessary
		void Export();									//!< Return NCO command to correlator
		Chan_Packet_S getPacket();
		void Accum(Correlation_S *corr, NCO_Command_S *_feedback);	//!< Process an accumulation
		float getCN0(){return(CN0);}
		float getNCO(){return(carrier_nco);}
		float getActive(){return(active);}	
		int32 getSV(){return(sv);}
};

#endif /* Channel_H */
