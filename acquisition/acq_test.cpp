/*! \file Main.cpp
	Entry point of program
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

#define GLOBALS_HERE

#include "includes.h"

typedef struct _Acquisition_test_options
{
	int32 realtime;			//!< loop forever using USRP
	int32 sv;				//!< search for a single sv
	int32 type;				//!< strong, medium, or weak
	int32 doppler_min;		//!< doppler min (Hz)
	int32 doppler_max;		//!< doppler max (Hz) 
	char filename[1024]; 	//!< filename of raw data
} Acquisition_test_options;

void run_acq(Acquisition_test_options *_opt);
void print_results(Acq_Result_S *_results);

void usage(char *_str)
{

    fprintf(stderr, "usage: [-sv] [-s] [-m] [-w] [-min] [-max] [-f] [-r]\n");
    fprintf(stderr, "[-r] repeat forever \n"); 
    fprintf(stderr, "[-min] <Doppler> minimum Doppler (Hz) \n");
    fprintf(stderr, "[-max] <Doppler> maximum Doppler (Hz) \n"); 
    fprintf(stderr, "[-f] <filename> GPS signal file\n");
    fprintf(stderr, "[-sv] <SV#> search for an individual SV (1:32)\n");
    fprintf(stderr, "[-s] Strong signal, 1 ms  coherent integration \n");
    fprintf(stderr, "[-m] Medium signal, 10 ms coherent integration \n");
    fprintf(stderr, "[-w] Weak signal, 10 ms  coherent integration + 15 non-coherent integrations \n");
    fflush(stderr);

    exit(1);
}

void echo_options(Acquisition_test_options *_opt)
{
    fprintf(stderr, "SV:\t\t\t%d\n",_opt->sv);
    fprintf(stderr, "Realtime:\t\t%d\n",_opt->realtime);
    fprintf(stderr, "Filename:\t\t%s\n",_opt->filename);
    fprintf(stderr, "Minimum Doppler (Hz):\t%d\n",_opt->doppler_min);
    fprintf(stderr, "Maximum Doppler (Hz):\t%d\n",_opt->doppler_max);
    fprintf(stderr, "Type:\t\t\t%d\n",_opt->type); 
    switch(_opt->type)
    {
    	case 0:
    		fprintf(stderr, "Type:\t\t\tStrong signal\n");
			break;
    	case 1:
    		fprintf(stderr, "Type:\t\t\tMedium signal\n");
			break;
    	case 2:
    		fprintf(stderr, "Type:\t\t\tWeak signal\n");
			break;
    }
    
    fflush(stderr);
}

int main(int32 argc, char* argv[])
{
	
	int32 lcv;
	
	printf("Acquisition_Test\n");
	
	Init_SIMD();
	
	Acquisition_test_options acq_options;
	char *parse;
	    
	/* Set default recording options */
	acq_options.realtime = 0;
	acq_options.sv = 0;
	acq_options.type = 0;
	acq_options.doppler_min = -10000;
	acq_options.doppler_max = 10000;
	strcpy(acq_options.filename, "data.dat");
	
	for(int lcv = 1; lcv < argc; lcv++)
	{
		if(!strcmp(argv[lcv], "-sv"))
		{
			lcv++;
            if(isdigit(argv[lcv][0]))
				acq_options.sv = (int32)strtof(argv[lcv], &parse);
            else
				usage(argv[0]);
		}
		else if(!strcmp(argv[lcv], "-s"))
		{
			acq_options.type = 0;
		}
		else if(!strcmp(argv[lcv], "-m"))
		{
			acq_options.type = 1;
		}	
		else if(!strcmp(argv[lcv], "-w"))
		{
			acq_options.type = 2;
		}	
		else if(!strcmp(argv[lcv], "-min"))
		{
			lcv++;
			acq_options.doppler_min = (int32) strtof(argv[lcv], &parse);
		}	
		else if(!strcmp(argv[lcv], "-max"))
		{
			lcv++;
			acq_options.doppler_max = (int32)strtof(argv[lcv], &parse);
		}
		else if(!strcmp(argv[lcv], "-f"))
		{
			lcv++;
			strcpy(acq_options.filename, argv[lcv]);
		}
		else if(!strcmp(argv[lcv], "-r"))
		{
			acq_options.realtime = 1;
		}
		else
			usage(argv[0]);
	}

	
	/* Echo the record options */
	echo_options(&acq_options);
	
	/* Call the recording function */
	run_acq(&acq_options);
	
	exit(1);

}

/* Monitor keyboard to kill real-time thread */
void *key_thread(void *_arg)
{
	
	int key;
		
	printf("Key thread start\n");
			
	while(grun)
	{
		key = getchar();
		printf("%c",(char)key);
		
		if((char)key == 'Q')
			grun = false;
		
	}
	
	printf("Key thread stop\n");
	
	pthread_exit(0);
		
}

void run_acq(Acquisition_test_options *_opt)
{
	pthread_t pkey_thread;
	FILE *fp = NULL;
	CPX *buff_in;
	CPX *buff;
	char *p;
	int *ip;
	Acq_Result_S results[NUM_CODES];
	Acquisition *pAcquisition;
	int32 sv, lcv, npipe;
	int32 nbytes, bread, bytes_per_read, ms_per_read, agc_scale;
	
	agc_scale = 1300;
	
	clock_t c0, c1;
	time_t start,end;
    double dif;

	memset(results, 0x0, sizeof(Acq_Result_S)*NUM_CODES);
	buff = new CPX[310*SAMPS_MS];
	buff_in = new CPX[310*IF_SAMPS_MS];
	
	/* Take care of file stuff */
	if(!_opt->realtime)
	{
		fp = fopen(_opt->filename, "rb");
		if(fp == NULL)
		{
			fprintf(stderr, "Could not open GPS source file!\n");
			exit(-1);
		}
		else
		{

			fread(buff_in, sizeof(CPX), 310*IF_SAMPS_MS, fp);
			fclose(fp);
					
			/* Downsample to 2048 samps/ms */
			downsample(buff, buff_in, SAMPLE_FREQUENCY, IF_SAMPLE_FREQUENCY, IF_SAMPS_MS*310); 

			/* Init AGC scale value */		
			init_agc(&buff[0], 10*SAMPS_MS, AGC_BITS, &agc_scale);
		
			/* Now run it */						
			for(lcv = 0; lcv < 310; lcv++)
				run_agc(&buff[lcv*SAMPS_MS], SAMPS_MS, AGC_BITS, &agc_scale);
		}
	
		/* Now do the hard work? */
		pAcquisition = new Acquisition(IF_SAMPLE_FREQUENCY, IF_FREQUENCY);
	
		pAcquisition->doPrepIF(_opt->type, buff);
	
		/* A single SV */
		if(_opt->sv)	
		{
			sv = _opt->sv - 1;
			
			switch(_opt->type)
			{
				case 0:
					results[sv] = pAcquisition->doAcqStrong(sv, _opt->doppler_min, _opt->doppler_max);
					break;
				case 1:
					results[sv] = pAcquisition->doAcqMedium(sv, _opt->doppler_min, _opt->doppler_max);
					break;
				case 2:
					results[sv] = pAcquisition->doAcqWeak(sv, _opt->doppler_min, _opt->doppler_max);
					break;
				default:
					fprintf(stderr, "Bad GPS acquisition type!\n");
					exit(-1);
			}
		}
		else	/* Loop over all SVs */
		{
			for(lcv = 0; lcv < NUM_CODES; lcv++)
			{
				sv = lcv;
				switch(_opt->type)
				{
					case 0:
						results[sv] = pAcquisition->doAcqStrong(sv, _opt->doppler_min, _opt->doppler_max);
						break;
					case 1:
						results[sv] = pAcquisition->doAcqMedium(sv, _opt->doppler_min, _opt->doppler_max);
						break;
					case 2:
						results[sv] = pAcquisition->doAcqWeak(sv, _opt->doppler_min, _opt->doppler_max);
						break;
					default:
						fprintf(stderr, "Bad GPS acquisition type!\n");
						exit(-1);
				}
				
				printf("SV: %02d\t%02d\t%10.2f\t%10.0f\t%15.0f\n",lcv+1, results[sv].type,results[sv].delay,results[sv].doppler,results[sv].magnitude);				
				
			}
		}
		
		print_results(&results[0]);
		
	}
	else
	{
		grun = true;
		pthread_create(&pkey_thread, NULL, key_thread, (void *)_opt);
			
		switch(_opt->type)
		{
			case 0:
				ms_per_read = 1;
				break;			
			case 1:
				ms_per_read = 20;
				break;			
			case 2:
				ms_per_read = 310;
				break;			
			default:
				ms_per_read = 310;
		}
			
		bytes_per_read = sizeof(CPX)*ms_per_read*IF_SAMPS_MS;

		/* Now do the hard work? */
		pAcquisition = new Acquisition(IF_SAMPLE_FREQUENCY, IF_FREQUENCY);

		while(grun)
		{
			
			npipe = open("/tmp/GPSPIPE",O_RDONLY);
					
			// Get data from pipe (310 ms)
			nbytes = 0; p = (char *)&buff[0];
			while((nbytes < bytes_per_read) && grun)
			{
				bread = read(npipe, &p[nbytes], PIPE_BUF);
				
				if(bread >= 0)
					nbytes += bread;
			}
			
			close(npipe);

			for(lcv = 0; lcv < ms_per_read; lcv++)		
				run_agc(&buff[lcv*SAMPS_MS], SAMPS_MS, AGC_BITS, &agc_scale);
				
			/* Prep the IF Data */
			pAcquisition->doPrepIF(_opt->type, buff);
			
			/* A single SV */
			if(_opt->sv)	
			{
				sv = _opt->sv - 1;
				switch(_opt->type)
				{
					case 0:
						results[sv] = pAcquisition->doAcqStrong(sv, _opt->doppler_min, _opt->doppler_max);
						break;
					case 1:
						results[sv] = pAcquisition->doAcqMedium(sv, _opt->doppler_min, _opt->doppler_max);
						break;
					case 2:
						results[sv] = pAcquisition->doAcqWeak(sv, _opt->doppler_min, _opt->doppler_max);
						break;
					default:
						fprintf(stderr, "Bad GPS acquisition type!\n");
						exit(-1);
				}
				print_results(&results[0]);
			}
			else
			{
				/* Loop over all SVs */
				for(lcv = 0; lcv < NUM_CODES; lcv++)
				{
					switch(_opt->type)
					{
						case 0:
							results[lcv] = pAcquisition->doAcqStrong(lcv, _opt->doppler_min, _opt->doppler_max);
							break;
						case 1:
							results[lcv] = pAcquisition->doAcqMedium(lcv, _opt->doppler_min, _opt->doppler_max);
							break;
						case 2:
							results[lcv] = pAcquisition->doAcqWeak(lcv, _opt->doppler_min, _opt->doppler_max);
							break;
						default:
							fprintf(stderr, "Bad GPS acquisition type!\n");
							exit(-1);
					}
					print_results(&results[0]);
					if(grun == false)
						break;
				}
				
			}
			
		}//end while
		
		close(npipe);
		
	}
	
	delete [] buff;
	delete [] buff_in;
	delete pAcquisition;	
	
}


void print_results(Acq_Result_S *_results)
{
	int32 lcv;
	FILE *fp = NULL;
	Acq_Result_S *p;
	
	fp = fopen("Acq.txt","wa");
	if(fp != NULL)
	{
		fseek(fp, 0x0, SEEK_SET);
		for(lcv = 0; lcv < 	NUM_CODES; lcv++)
		{
			p = &_results[lcv];
			fprintf(fp, "%02d,%02d,%10.2f,%10.0f,%15.0f,%1d\n",p->type,lcv+1,p->delay,p->doppler,p->magnitude,p->success);
		}
		fclose(fp);
	}
}


