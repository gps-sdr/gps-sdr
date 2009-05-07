/*----------------------------------------------------------------------------------------------*/
/*! \file ephemeris.cpp
//
// FILENAME: ephemeris.cpp
//
// DESCRIPTION: Implement the ephemeris object.
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

#include "ephemeris.h"

/*----------------------------------------------------------------------------------------------*/
void *Ephemeris_Thread(void *_arg)
{

	Ephemeris *aEphemeris = pEphemeris;

	while(1)
	{
		aEphemeris->Import();
	}

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::Start()
{

	/* With new priority specified */
	Start_Thread(Ephemeris_Thread, NULL);

	if(gopt.verbose)
		printf("Ephemeris thread started\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Ephemeris::Ephemeris():Threaded_Object("EPHTASK")
{
	int32 lcv;

	object_mem = this;
	size = sizeof(Ephemeris);

	for(lcv = 0; lcv < MAX_SV; lcv++)
		iode_master[lcv] = NON_EXISTENT_IODE; //some non possible IODE value

	/* Zero out structures */
	ClearEphemeris(MAX_SV);
	ClearAlmanac(MAX_SV);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Ephemeris::~Ephemeris()
{
	if(gopt.verbose)
		printf("Destructing Ephemeris\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::ClearUTC()
{
	memset(&utc, 0x0, sizeof(UTC_Parameter_S));
	memset(&klobuchar, 0x0, sizeof(Klobuchar_Model_S));
	memset(&utc_data, 0x0, sizeof(Almanac_Data_S));
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::ClearEphemeris(int32 _sv)
{
	int32 lcv;

	if((_sv >= 0) && (_sv < MAX_SV))
	{
		memset(&ephemerides[_sv], 0x0, sizeof(Ephemeris_M));
		memset(&ephem_data[_sv], 0x0, sizeof(Ephem_Data_S));

		iode_master[_sv] = NON_EXISTENT_IODE; //some non possible IODE value
		ephemerides[_sv].sv = _sv;
		output_s.iode[_sv] = NON_EXISTENT_IODE;
		output_s.valid[_sv] = 0;
	}
	else
	{
		memset(&ephemerides[0], 0x0, MAX_SV*sizeof(Ephemeris_M));
		memset(&ephem_data[0], 0x0, MAX_SV*sizeof(Ephem_Data_S));

		for(lcv = 0; lcv < MAX_SV; lcv++)
		{
			iode_master[lcv] = NON_EXISTENT_IODE; //some non possible IODE value
			ephemerides[lcv].sv = lcv;
			output_s.iode[lcv] = NON_EXISTENT_IODE;
			output_s.valid[lcv] = 0;
		}
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::ClearAlmanac(int32 _sv)
{
	int32 lcv;

	if((_sv >= 0) && (_sv < MAX_SV))
	{
		memset(&almanacs[_sv], 0x0, sizeof(Almanac_M));
		memset(&almanac_data[_sv], 0x0, sizeof(Almanac_Data_S));

		output_s.avalid[_sv] = 0;
		almanacs[_sv].sv = _sv;
	}
	else
	{
		memset(&almanacs[0], 0x0, MAX_SV*sizeof(Almanac_M));
		memset(&almanac_data[0], 0x0, MAX_SV*sizeof(Almanac_Data_S));

		for(lcv = 0; lcv < MAX_SV; lcv++)
		{
			output_s.avalid[lcv] = 0;
			almanacs[lcv].sv = lcv;
		}
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::Import()
{

	int32 lcv, sv, sv_id;
	uint32 bread;

	/* Read a Subframe */
	read(CHN_2_EPH_P[READ], (void *)&ephem_packet, sizeof(Channel_2_Ephemeris_S));

	IncStartTic();

	/* Get the SV number */
	sv = ephem_packet.sv;
	if((sv < MAX_SV) && (sv >= 0))
	{

		switch(ephem_packet.subframe)
		{
			case 1:
			{
				for(lcv = 0; lcv < FRAME_SIZE_PLUS_2; lcv++)
					ephem_data[sv].subframe_1[lcv] = ephem_packet.word_buff[lcv];

				ephem_data[sv].valid[0] = true;

				Lock();
				Parse(sv);
				Unlock();
				break;
			}
			case 2:
			{
				for(lcv = 0; lcv < FRAME_SIZE_PLUS_2; lcv++)
					ephem_data[sv].subframe_2[lcv] = ephem_packet.word_buff[lcv];

				ephem_data[sv].valid[1] = true;

				Lock();
				Parse(sv);
				Unlock();

				break;
			}
			case 3:
			{
				for(lcv = 0; lcv < FRAME_SIZE_PLUS_2; lcv++)
					ephem_data[sv].subframe_3[lcv] = ephem_packet.word_buff[lcv];

				ephem_data[sv].valid[2] = true;

				Lock();
				Parse(sv);
				Unlock();

				break;
			}
			case 4:
			{
				/* Get the sv_id */
				sv_id = (ephem_packet.word_buff[2] >> 22) & 0x0000003F;

				if((sv_id > 0) && (sv_id < 33))
				{
					/* Copy over data */
					for(lcv = 0; lcv < FRAME_SIZE_PLUS_2; lcv++)
						almanac_data[sv_id - 1].page[lcv] = ephem_packet.word_buff[lcv];

					/* Process */
					Lock();
					ParsePage(sv_id);
					Unlock();
				}
				else if(sv_id == 56) /* UTC Info */
				{
					/* Copy over data */
					for(lcv = 0; lcv < FRAME_SIZE_PLUS_2; lcv++)
						utc_data.page[lcv] = ephem_packet.word_buff[lcv];

					Lock();
					ParseUTC();
					Unlock();
				}
				else if(sv_id == 63) /* Health info */
				{
					/* Copy over the data */
					for(lcv = 0; lcv < FRAME_SIZE_PLUS_2; lcv++)
						health_data[0].page[lcv] = ephem_packet.word_buff[lcv];

					Lock();
					ParseHealth();
					Unlock();
				}

				break;
			}
			case 5:
			{
				/* Get the sv_id */
				sv_id = (ephem_packet.word_buff[2] >> 22) & 0x0000003F;

				if((sv_id > 0) && (sv_id < 33))
				{
					/* Copy over data */
					for(lcv = 0; lcv < FRAME_SIZE_PLUS_2; lcv++)
						almanac_data[sv_id - 1].page[lcv] = ephem_packet.word_buff[lcv];

					/* Process */
					Lock();
					ParsePage(sv_id);
					Unlock();
				}
				else if(sv_id == 51) /* Health info */
				{
					/* Copy over the data */
					for(lcv = 0; lcv < FRAME_SIZE_PLUS_2; lcv++)
						health_data[1].page[lcv] = ephem_packet.word_buff[lcv];

					Lock();
					ParseHealth();
					Unlock();
				}
				break;
			}
			default:
			{
				break;
			}
		}


	}

	IncExecTic();

	IncStopTic();
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
int32 sbit(uint32 val, int32 nbits)
{
	int32 result = 0x0;

	result = (val << (32 - nbits));
	result = result >> (32 - nbits);  //move the sign bit into the top bit

	return(result);
}

/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::ParseUTC()
{

	uint32 i_tot;

	i_tot = (int32) TWO_P12 * (int32)(utc_data.page[7] >> 14 & 0x000000FF);

	/* UTC info has changed, decoded it again */
	if((i_tot != utc.tot) || (utc.valid == false) || (klobuchar.valid == false))
	{
		klobuchar.a0     = TWO_N30 * (double)(sbit(utc_data.page[2] >> 12, 8));
		klobuchar.a1     = TWO_N27 * (double)(sbit(utc_data.page[2] >> 6,  8));
		klobuchar.a2     = TWO_N24 * (double)(sbit(utc_data.page[3] >> 22, 8));
		klobuchar.a3     = TWO_N24 * (double)(sbit(utc_data.page[3] >> 14, 8));
		klobuchar.b0     = TWO_P11 * (double)(sbit(utc_data.page[3] >> 6,  8));
		klobuchar.b1     = TWO_P14 * (double)(sbit(utc_data.page[4] >> 22, 8));
		klobuchar.b2     = TWO_P16 * (double)(sbit(utc_data.page[4] >> 14, 8));
		klobuchar.b3     = TWO_P16 * (double)(sbit(utc_data.page[4] >> 6,  8));
		klobuchar.valid = true;

		utc.a0    	= (double) TWO_N30 * (double)(sbit((((utc_data.page[7] >> 22) & 0x000000FF) | ((utc_data.page[6] >> 6) & 0xFFFFFF00)), 32));
		utc.a1    	= (double) TWO_N50 * (double)(sbit(utc_data.page[5] >> 6, 24));
		utc.tot    	= (int32)  TWO_P12 * (int32)      (utc_data.page[7] >> 14 & 0x000000FF);
		utc.wnt    	= (uint32)                        (utc_data.page[7] >> 6  & 0x000000FF);
		utc.dtls  	= (int32)                    (sbit(utc_data.page[8] >> 22,  8));
		utc.wnlsf 	= (uint32)                        (utc_data.page[8] >> 14 & 0x000000FF);
		utc.dn     	= (uint32)                        (utc_data.page[8] >> 6  & 0x000000FF);
		utc.dtlsf 	= (int32)                    (sbit(utc_data.page[9] >> 22,  8));
		utc.valid = true;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::Parse(int32 _sv)
{
	int32 IODE[3];

	if(ephem_data[_sv].valid[0])
		IODE[0] = (ephem_data[_sv].subframe_1[7] >> 22) & 0x0000007F;
	else
		IODE[0] = 9996;

	if(ephem_data[_sv].valid[1])
		IODE[1] = (ephem_data[_sv].subframe_2[2] >> 22) & 0x0000007F;
	else
		IODE[1] = 9997;

	if(ephem_data[_sv].valid[2])
		IODE[2] = (ephem_data[_sv].subframe_3[9] >> 22) & 0x0000007F;
	else
		IODE[2] = 9998;

	if((_sv < MAX_SV) && (_sv >= 0))
	{
		if((IODE[0] == IODE[1]) && (IODE[1] == IODE[2]) && (IODE[0] == IODE[2]))
		{

			iode_master[_sv] = IODE[0];
			ephemerides[_sv].sv = _sv;
			ephemerides[_sv].af0 =		TWO_N31 *		(double) sbit(ephem_data[_sv].subframe_1[9] >> 8, 22);
			ephemerides[_sv].af1 =		TWO_N43 *		(double) sbit(ephem_data[_sv].subframe_1[8] >> 6, 16);
			ephemerides[_sv].af2 =		TWO_N55 *		(double) sbit(ephem_data[_sv].subframe_1[8] >> 22, 8);
			ephemerides[_sv].cic =		TWO_N29 *		(double) sbit(ephem_data[_sv].subframe_3[2] >> 14, 16);
			ephemerides[_sv].cis =		TWO_N29 *		(double) sbit(ephem_data[_sv].subframe_3[4] >> 14, 16);
			ephemerides[_sv].crc =		TWO_N5  *		(double) sbit(ephem_data[_sv].subframe_3[6] >> 14, 16);
			ephemerides[_sv].crs =		TWO_N5  *		(double) sbit(ephem_data[_sv].subframe_2[2] >> 6,  16);
			ephemerides[_sv].cuc =		TWO_N29 *		(double) sbit(ephem_data[_sv].subframe_2[5] >> 14, 16);
			ephemerides[_sv].cus =		TWO_N29 *		(double) sbit(ephem_data[_sv].subframe_2[7] >> 14, 16);
			ephemerides[_sv].deltan =	PI_TWO_N43 *	(double) sbit(ephem_data[_sv].subframe_2[3] >> 14, 16);
			ephemerides[_sv].omd =		PI_TWO_N43 *	(double) sbit(ephem_data[_sv].subframe_3[8] >> 6, 24);
			ephemerides[_sv].idot =		PI_TWO_N43 *	(double) sbit(ephem_data[_sv].subframe_3[9] >> 8, 14);
			ephemerides[_sv].ecc =		TWO_N33 *		(double)	((ephem_data[_sv].subframe_2[5] << 18  & 0xFF000000) | (ephem_data[_sv].subframe_2[6] >> 6  & 0x00FFFFFF));
			ephemerides[_sv].in0 =		PI_TWO_N31 *	(double)	((ephem_data[_sv].subframe_3[4] << 18  & 0xFF000000) | (ephem_data[_sv].subframe_3[5] >> 6  & 0x00FFFFFF));
			ephemerides[_sv].m0 =		PI_TWO_N31 *	(double)	((ephem_data[_sv].subframe_2[3] << 18  & 0xFF000000) | (ephem_data[_sv].subframe_2[4] >> 6  & 0x00FFFFFF));
			ephemerides[_sv].argp =		PI_TWO_N31 *	(double)	((ephem_data[_sv].subframe_3[6] << 18  & 0xFF000000) | (ephem_data[_sv].subframe_3[7] >> 6  & 0x00FFFFFF));
			ephemerides[_sv].om0 =		PI_TWO_N31 *	(double)	((ephem_data[_sv].subframe_3[2] << 18  & 0xFF000000) | (ephem_data[_sv].subframe_3[3] >> 6  & 0x00FFFFFF));
			ephemerides[_sv].sqrta =	TWO_N19 *		(double)	((ephem_data[_sv].subframe_2[7] << 18  & 0xFF000000) | (ephem_data[_sv].subframe_2[8] >> 6  & 0x00FFFFFF));
			ephemerides[_sv].iodc =						(int32)		((ephem_data[_sv].subframe_1[2] << 2   & 0x00000300) | (ephem_data[_sv].subframe_1[7] >> 22 & 0x000000FF));
			ephemerides[_sv].tgd =		TWO_N31 *		(double)	((ephem_data[_sv].subframe_1[6] >> 6)  & 0x000000FF);
			ephemerides[_sv].toc =		TWO_P4 *		(double)	((ephem_data[_sv].subframe_1[7] >> 6)  & 0x0000FFFF);
			ephemerides[_sv].toe =		TWO_P4 *		(double)	((ephem_data[_sv].subframe_2[9] >> 14) & 0x0000FFFF);
			ephemerides[_sv].ura =						(int32)		((ephem_data[_sv].subframe_1[2] >> 14) & 0x0000000F);
			ephemerides[_sv].iode =						(int32)		((ephem_data[_sv].subframe_1[7] >> 22) & 0x000000FF);
			ephemerides[_sv].tow =		4*				(int32)		((ephem_data[_sv].subframe_1[1] >> 13) & 0x0000FFFF);
			ephemerides[_sv].week_number =				(int32)		((ephem_data[_sv].subframe_1[2] >> 20) & 0x000003FF);
			ephemerides[_sv].subframe_1_health =		(int32)		((ephem_data[_sv].subframe_1[2] >> 8)  & 0x0000003F);
			ephemerides[_sv].a = ephemerides[_sv].sqrta * ephemerides[_sv].sqrta;
			ephemerides[_sv].tofxmission = (int32) (ephemerides[_sv].tow * 1.5);
			ephemerides[_sv].n0 = sqrt(GRAVITY_CONSTANT/(ephemerides[_sv].a*ephemerides[_sv].a*ephemerides[_sv].a));
			ephemerides[_sv].zcount = ephemerides[_sv].tow;

			if(ephemerides[_sv].subframe_1_health == 0)
				ephemerides[_sv].valid = true;
			else
				ephemerides[_sv].valid = false;

			output_s.valid[_sv] = ephemerides[_sv].valid;
			output_s.iode[_sv] = iode_master[_sv];

		} //IODE matches

	} //if good _sv
}
/*----------------------------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------------------------*/
void Ephemeris::ParsePage(int32 _sv_id)
{

	/* If the almanac has not been decoded then do it */
	if((_sv_id > 0) && (_sv_id <= MAX_SV))
	{
		_sv_id = _sv_id - 1;
		if(almanacs[_sv_id].valid == false)
		{
			almanacs[_sv_id].sv			= _sv_id;
			almanacs[_sv_id].ecc		=	(double) TWO_N21 * (double)( (almanac_data[_sv_id].page[2] >> 6)  & 0x0000FFFF );
			almanacs[_sv_id].toa		=	(double) TWO_P12 * (double)( (almanac_data[_sv_id].page[3] >> 22) & 0x000000FF );
			almanacs[_sv_id].sqrta		=	(double) TWO_N11 * (double)( (almanac_data[_sv_id].page[5] >> 6)  & 0x00FFFFFF );
			almanacs[_sv_id].in0		=	(double) PI_TWO_N19 * (double)(	sbit(almanac_data[_sv_id].page[3] >> 6,  16) ) + (double)(54.0 * PI / 180.0);
			almanacs[_sv_id].omd		=	(double) PI_TWO_N38 * (double)(	sbit(almanac_data[_sv_id].page[4] >> 14, 16) );
			almanacs[_sv_id].om0		=	(double) PI_TWO_N23 * (double)(	sbit(almanac_data[_sv_id].page[6] >> 6,  24) );
			almanacs[_sv_id].argp		=	(double) PI_TWO_N23 * (double)(	sbit(almanac_data[_sv_id].page[7] >> 6,  24) );
			almanacs[_sv_id].m0			=	(double) PI_TWO_N23 * (double)(	sbit(almanac_data[_sv_id].page[8] >> 6,  24) );
			almanacs[_sv_id].af1		=	(double) TWO_N38 * (double)(	sbit(almanac_data[_sv_id].page[9] >> 11, 11) );
			almanacs[_sv_id].af0		=	(double) TWO_N20 * (double)(	sbit((almanac_data[_sv_id].page[9] >> 19 & 0x000007F8) + (almanac_data[_sv_id].page[9] >> 8 & 0x00000007),  11) );
			almanacs[_sv_id].valid = true;
			output_s.avalid[_sv_id] = true;
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::ParseHealth()
{

	int32 lcv, sv;
	uint32 word;

	sv = 0;

	/* First parse page 25 of subframe 5 for SVs 1 through 24 */
	for(lcv = 3; lcv < 9; lcv++)
	{
		word = health_data[1].page[lcv];
		almanacs[sv+0].health = (word >> 24) & 0x0000003F;
		almanacs[sv+1].health = (word >> 18) & 0x0000003F;
		almanacs[sv+2].health = (word >> 12) & 0x0000003F;
		almanacs[sv+3].health = (word >> 06) & 0x0000003F;
		sv += 4;
	}

	/* Page 24 of subframe 4 is a little more convoluted */
	word = health_data[0].page[7];
	almanacs[sv+0].health = (word >> 06) & 0x0000003F;
	sv++;

	word = health_data[0].page[8];
	almanacs[sv+0].health = (word >> 24) & 0x0000003F;
	almanacs[sv+1].health = (word >> 18) & 0x0000003F;
	almanacs[sv+2].health = (word >> 12) & 0x0000003F;
	almanacs[sv+3].health = (word >> 06) & 0x0000003F;
	sv+=4;

	word = health_data[0].page[9];
	almanacs[sv+0].health = (word >> 24) & 0x0000003F;
	almanacs[sv+1].health = (word >> 18) & 0x0000003F;
	almanacs[sv+2].health = (word >> 12) & 0x0000003F;

}
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Ephemeris_M Ephemeris::getEphemeris(int32 _sv)
{
	return(ephemerides[_sv]);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Almanac_M Ephemeris::getAlmanac(int32 _sv)
{
	return(almanacs[_sv]);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
UTC_Parameter_S Ephemeris::getUTC()
{
	return(utc);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Klobuchar_Model_S Ephemeris::getKlobuchar()
{
	return(klobuchar);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
int32 Ephemeris::getIODE(int32 _sv)
{
	return(iode_master[_sv]);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
int32 Ephemeris::getEphemerisValidity(int32 _sv)
{
	return(ephemerides[_sv].valid);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
int32 Ephemeris::getAlmanacValidity(int32 _sv)
{
	return(almanacs[_sv].valid);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
uint32 Ephemeris::getAlmanacHealth(int32 _sv)
{
	return(almanacs[_sv].health);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Ephemeris_Status_M Ephemeris::getEphemerisStatus()
{
	return(output_s);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::setEphemeris(Ephemeris_M *_e)
{
	int32 sv;

	sv = _e->sv;

	if((sv >= 0) && (sv < MAX_SV))
	{
		memcpy(&ephemerides[sv], _e, sizeof(Ephemeris_M));
		iode_master[sv] = ephemerides[sv].iode;
		output_s.valid[sv] = ephemerides[sv].valid;
		output_s.iode[sv] = iode_master[sv];
	}
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::setAlmanac(Almanac_M *_a)
{
	int32 sv;

	sv = _a->sv;

	if((sv >= 0) && (sv < MAX_SV))
	{
		memcpy(&almanacs[sv], _a, sizeof(Almanac_M));
		almanacs[sv].valid = true;
		almanacs[sv].health = true;
		output_s.avalid[sv] = true;
	}
}
/*----------------------------------------------------------------------------------------------*/

