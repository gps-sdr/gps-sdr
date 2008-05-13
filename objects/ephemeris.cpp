/*! \file ephemeris.cpp
	Implements member functions of ephemeris class.
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

#include "ephemeris.h"

/*----------------------------------------------------------------------------------------------*/
void *Ephemeris_Thread(void *_arg)
{
	
	Ephemeris *aEphemeris = pEphemeris;

	while(grun)
	{
		aEphemeris->Import();
		aEphemeris->Export();
	}
	
	pthread_exit(0);
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Ephemeris::Ephemeris()
{
	int32 lcv;

	for(lcv = 0; lcv < NUM_CODES; lcv++)
		iode_master[lcv] = 9999; //some non possible IODE value
		
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_unlock(&mutex);
	
	/* Read in stored ephem/almanac on bootup */
	//ReadEphemeris();
	//ReadAlmanac();
	
	if(gopt.verbose)
		printf("Creating Ephemeris\n");	
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
int32 sbit(uint32 val, int32 nbits)
{
	int32 result = 0x0;

	result = (val << (32 - nbits));
	result = result >> (32 - nbits);  //move the sign bit into the top bit

	return(result);
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


	if((_sv < NUM_CODES) && (_sv >= 0))
	{
		if((IODE[0] == IODE[1]) && (IODE[1] == IODE[2]) && (IODE[0] == IODE[2]))
		{
			if((IODE[0] != iode_master[_sv]) || (ephemerides[_sv].valid == false))
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
					
			} //IODE has changed

		} //IODE matches

	} //if good _sv
	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::Import()
{

	int32 lcv, sv, sv_id;

	read(Chan_2_Ephem_P[READ], &ephem_packet, sizeof(Chan_2_Ephem_S));

	sv = ephem_packet.sv;
	if((sv < NUM_CODES) && (sv >= 0))
	{
		
		switch(ephem_packet.subframe)
		{
			case 1:
			{
				for(lcv = 0; lcv < FRAME_SIZE_PLUS_2; lcv++)
					ephem_data[sv].subframe_1[lcv] = ephem_packet.word_buff[lcv];	
				
				ephem_data[sv].valid[0] = true;
				break;		
			}
			case 2:
			{
				for(lcv = 0; lcv < FRAME_SIZE_PLUS_2; lcv++)
					ephem_data[sv].subframe_2[lcv] = ephem_packet.word_buff[lcv];	
				
				ephem_data[sv].valid[1] = true;
				break;		
			}
			case 3:
			{
				for(lcv = 0; lcv < FRAME_SIZE_PLUS_2; lcv++)
					ephem_data[sv].subframe_3[lcv] = ephem_packet.word_buff[lcv];	
				
				ephem_data[sv].valid[2] = true;
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
				break;		
			}
			default:
			{
				break;
			}
		}
		
		Lock();
		Parse(sv);
		Unlock();
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
Ephemeris_S Ephemeris::getEphemeris(int32 sv)
{

	return(ephemerides[sv]);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::Export()
{
	write(Ephem_2_Telem_P[WRITE], &output_s, sizeof(Ephem_2_Telem_S));
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/

int32 Ephemeris::getIODE(int32 sv)
{

	return(iode_master[sv]);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::Lock()
{
	pthread_mutex_lock(&mutex);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::Unlock()
{
	pthread_mutex_unlock(&mutex);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::Start()
{
	pthread_attr_t tattr;
	pthread_t tid;
	int32 ret;
	sched_param param;
	
	/* Unitialized with default attributes */
	ret = pthread_attr_init (&tattr);
	
	/*Ssafe to get existing scheduling param */
	ret = pthread_attr_getschedparam (&tattr, &param);
	
	/* Set the priority; others are unchanged */
	param.sched_priority = EPHEM_PRIORITY;
	
	/* Setting the new scheduling param */
	ret = pthread_attr_setschedparam(&tattr, &param);
	ret = pthread_attr_setschedpolicy(&tattr, SCHED_FIFO);
	
	/* With new priority specified */
	pthread_create(&thread, NULL, Ephemeris_Thread, NULL);
	
	if(gopt.verbose)
		printf("Ephemeris thread started\n");	
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::Stop()
{
	pthread_join(thread, NULL);
	
	if(gopt.verbose)
		printf("Ephemeris thread stopped\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::ClearSV(int32 _sv)
{
	iode_master[_sv] = 9999; //some non possible IODE value
	memset(&ephemerides[_sv], 0x0, sizeof(Ephemeris_S));
	memset(&ephem_data[_sv], 0x0, sizeof(Ephem_Data_S));
}
/*----------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------*/
void Ephemeris::ParsePage(int32 _sv_id)
{

	/* If the almanac has not been decoded then do it */
	if((_sv_id > 0) && (_sv_id < 33))
	{
		_sv_id = _sv_id - 1;
		if(almanacs[_sv_id].decoded == false)
		{
	
			almanacs[_sv_id].ecc		=	TWO_N21 * (double)( (almanac_data[_sv_id].page[2] >> 6)  & 0x0000FFFF );
			almanacs[_sv_id].toa		=	TWO_P12 * (double)(	(almanac_data[_sv_id].page[3] >> 22) & 0x000000FF );
			almanacs[_sv_id].sqrta		=	TWO_N11 * (double)( (almanac_data[_sv_id].page[5] >> 6)  & 0x00FFFFFF );
			almanacs[_sv_id].in0		=	PI_TWO_N19 * (double)(	sbit(almanac_data[_sv_id].page[3] >> 6,  16) ) + 0.3 * PI;
			almanacs[_sv_id].omd		=	PI_TWO_N38 * (double)(	sbit(almanac_data[_sv_id].page[4] >> 14, 16) );
			almanacs[_sv_id].om0		=	PI_TWO_N23 * (double)(	sbit(almanac_data[_sv_id].page[6] >> 6,  24) );
			almanacs[_sv_id].argp		=	PI_TWO_N23 * (double)(	sbit(almanac_data[_sv_id].page[7] >> 6,  24) );
			almanacs[_sv_id].m0			=	PI_TWO_N23 * (double)(	sbit(almanac_data[_sv_id].page[8] >> 6,  24) );
			almanacs[_sv_id].af0		=	TWO_N20 * (double)(	sbit(almanac_data[_sv_id].page[9] >> 11, 11) );
			almanacs[_sv_id].af1		=	TWO_N38 * (double)(	sbit((almanac_data[_sv_id].page[9] >> 19 & 0x000007F8) + (almanac_data[_sv_id].page[9] >> 8 & 0x00000007),  11) );
			almanacs[_sv_id].decoded = true;
			output_s.avalid[_sv_id] = true;
			
			WriteAlmanac();
		}
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::WriteAlmanac()
{
	int32 lcv;
	Almanac_S *p;
	FILE *fp;
	
	fp = fopen("almanac.txt","wt");

	if(fp != NULL)
	{
		for(lcv = 0; lcv < NUM_CODES; lcv++)
		{
			p = &almanacs[lcv];
			if(p->decoded)
			{
				fprintf(fp,"******** Week %d almanac for PRN-%02d ********\n",p->week,lcv+1);
				fprintf(fp,"ID:                         %02d\n",lcv+1);
				fprintf(fp,"Health:                     %03d\n",p->health);
				fprintf(fp,"Eccentricity:               %.10E\n",p->ecc);
				fprintf(fp,"Time of Applicability(s):   %.4f\n",p->toa);
				fprintf(fp,"Orbital Inclination(rad):   %.10E\n",p->in0);
				fprintf(fp,"Rate of Right Ascen(r/s):   %.10E\n",p->omd);
				fprintf(fp,"SQRT(A)  (m 1/2):           %.6f\n",p->sqrta);
				fprintf(fp,"Right Ascen at Week(rad):   %0.10E\n",p->om0);
				fprintf(fp,"Argument of Perigee(rad):   %.9f\n",p->argp);
				fprintf(fp,"Mean Anom(rad):             %0.10e\n",p->m0);
				fprintf(fp,"Af0(s):                     %0.10G\n",p->af0);
				fprintf(fp,"Af1(s/s):                   %0.10g\n",p->af1);
				fprintf(fp,"week:                       %4d\n",p->week);
				fprintf(fp,"\n");
			}
		}
		
		fclose(fp);
		
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void Ephemeris::ReadAlmanac()	
{
	
	int32 prn, week;
	Almanac_S *p;
	FILE *fp;
	
	fp = fopen("almanac.txt","rt");

	if(fp != NULL)
	{
		while(!feof(fp))
		{
			/* Get the PRN number */
			fscanf(fp,"******** Week %d almanac for PRN-%02d ********\n",&week,&prn);
			p = &almanacs[prn-1];
			p->decoded = true;
			fscanf(fp,"ID:                         %02d\n",&prn);
			fscanf(fp,"Health:                     %03d\n",&p->health);
			fscanf(fp,"Eccentricity:               %lE\n",&p->ecc);
			fscanf(fp,"Time of Applicability(s):   %lf\n",&p->toa);
			fscanf(fp,"Orbital Inclination(rad):   %lE\n",&p->in0);
			fscanf(fp,"Rate of Right Ascen(r/s):   %lE\n",&p->omd);
			fscanf(fp,"SQRT(A)  (m 1/2):           %lf\n",&p->sqrta);
			fscanf(fp,"Right Ascen at Week(rad):   %lE\n",&p->om0);
			fscanf(fp,"Argument of Perigee(rad):   %lf\n",&p->argp);
			fscanf(fp,"Mean Anom(rad):             %lE\n",&p->m0);
			fscanf(fp,"Af0(s):                     %lE\n",&p->af0);
			fscanf(fp,"Af1(s/s):                   %lE\n",&p->af1);
			fscanf(fp,"week:                       %4d\n",&p->week);
			fscanf(fp,"\n");
		}
		
		fclose(fp);
	}
	
	
}
/*----------------------------------------------------------------------------------------------*/

