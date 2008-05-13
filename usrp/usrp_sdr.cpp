#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <limits.h>
#include "usrp_prims.h"
#include "usrp_spi_defs.h"
#include "usb.h"
#include "usrp_standard.h"
#include "usrp_bytesex.h"
#include "fpga_regs_common.h"
#include "fpga_regs_standard.h"
#include "usrp_i2c_addr.h"
#include "db_dbs_rx.h"

using namespace std; 

typedef struct _CPX
{
	short r;
	short i;
} CPX;

typedef struct _options
{
	int		board_a;
	int		board_b;	
	int 	decimate;
	int		verbose;
	double 	gi;
	double 	gr;
	double 	f_lo;
	double 	f_ddc;	
	double 	bandwidth;
		
} options; 


//ACTCHUNG!!!!!!!!!!!!!!!!!!!!
//Set this to the mainboard's clock frequency
#define F_SAMPLE		(64.0e6)
#define F_SAMPLE_NOM	(64.0e6)
#define DDC_FIX 		(F_SAMPLE_NOM/F_SAMPLE)

#define SAMPS_PER_READ	(2048)
#define BYTES_PER_READ  (SAMPS_PER_READ*4)
#define READ 			(0)
#define WRITE 			(1)

void PostStatus(char *_str) {fprintf(stdout,"%s",_str); fflush(stdout);}
void *record_thread(void *_opt);
void *fifo_thread(void *_opt);
void *key_thread(void *_arg);

bool grun;

int fifo_head;
int fifo_tail;
CPX **fifo_rows;
CPX *fifo_buff;
pthread_mutex_t mFIFO;

#define FIFO_SIZE (1000)

void usage(char *_str)
{

	fprintf(stderr, "usage: [-A] [-B] [-d] [-f] [-gr] [-gi] [-l] [-w]\n");
	fprintf(stderr, "[-A] record from RX board A\n");
	fprintf(stderr, "[-B] record from RX board B\n");
	fprintf(stderr, "[-d] <decimation> USRP decimation (8-128)\n");
	fprintf(stderr, "[-f] <frequency> digital downconvert frequency\n");
	fprintf(stderr, "[-gr] <gain> set rf gain in dB (DBSRX only)\n");
	fprintf(stderr, "[-gi] <gain> set if gain in dB (DBSRX only)\n");
	fprintf(stderr, "[-l] <frequency> RF downconvert frequency (DBSRX only)\n");
	fprintf(stderr, "[-w] <bandwidth> bandwidth of lowpass filter (BDSRX only)\n");
	fflush(stderr);

	exit(1);
}

void echo_options(options *_opt)
{
	
	/* Step one, find out what daughterboards are populated */
	/* Open the base RX class */
	usrp_standard_rx *urx = usrp_standard_rx::make(0, 8, 1, -1, 0, 0, 0);
	if(urx == NULL)
	{
		printf("usrp_standard_rx::make FAILED\n");
		return;
	}
	
	switch(urx->daughterboard_id(0))
	{ 
		case 1:
			fprintf(stdout,"Board A:\t\tBasic RX\n"); break;
		case 2:
			fprintf(stdout,"Board A:\t\tDBS RX\n"); break;
		default:
			fprintf(stdout,"Board A:\t\tUnknown\n"); break;
	}

	switch(urx->daughterboard_id(1))
	{
		case 1:
			fprintf(stdout,"Board A:\t\tBasic RX\n"); break;
		case 2:
			fprintf(stdout,"Board A:\t\tDBS RX\n"); break;
		default:
			fprintf(stdout,"Board A:\t\tUnknown\n"); break;
	}

	fprintf(stdout, "Board A Record:\t\t%d\n",_opt->board_a);
	fprintf(stdout, "Board B Record:\t\t%d\n",_opt->board_b);
	fprintf(stdout, "USRP Decimation:\t%d\n",_opt->decimate);
	fprintf(stdout, "DDC Frequency:\t\t%f\n",_opt->f_ddc);
	fprintf(stdout, "RF Gain:\t\t%f\n",_opt->gr);
	fprintf(stdout, "IF Gain:\t\t%f\n",_opt->gi);
	fprintf(stdout, "DBSRX LO:\t\t%f\n",_opt->f_lo);
	fprintf(stdout, "DBSRX Bandwidth:\t%f\n",_opt->bandwidth);
	
	delete urx;
		
}

int main(int argc, char **argv)
{
	int lcv;
	char *parse;	
	options record_options;
	
	/* Set default recording options */
	record_options.board_a = 1;
	record_options.board_b = 0;
	record_options.decimate = 16;
	record_options.gr = 50;
	record_options.gi = 10;
	record_options.f_lo = 1.57542e9;
	record_options.f_ddc = 604000;
	record_options.bandwidth = 10.0e6;

	for(lcv = 1; lcv < argc; lcv++)
	{
		switch (argv[lcv][1])
		{
			
			case 'A':
				record_options.board_a = 1;
				break;
				
			case 'B':
				record_options.board_b = 1;
				break;
				
			case 'd':
				lcv++;
				if(isdigit(argv[lcv][0]))
					record_options.decimate = strtol(argv[lcv], &parse, 10);
				else
					usage (argv[0]);

				break;

			case 'f':
				lcv++;
				record_options.f_ddc = strtod(argv[lcv], &parse);
				break;

			case 'g':
				if(argv[lcv][2] == 'r')
				{
					lcv++;
					if(isdigit(argv[lcv][0]))
						record_options.gr = strtod(argv[lcv], &parse);
					else
						usage (argv[0]);
					break;
				}
				else if(argv[lcv][2] == 'i')
				{
					lcv++;
					if(isdigit(argv[lcv][0]))
						record_options.gi = strtod(argv[lcv], &parse);
					else
						usage (argv[0]);
					break;
				}
				else
					usage(argv[0]);
				break;
				
			case 'l':
				lcv++;
				if(isdigit(argv[lcv][0]))
					record_options.f_lo = strtod(argv[lcv], &parse);
				else
					usage (argv[0]);
				break;
				
			case 'w':
				lcv++;
				if(isdigit(argv[lcv][0]))
					record_options.bandwidth = strtod(argv[lcv], &parse);
				else
					usage (argv[0]);
				break;

			default:
				usage (argv[0]);
		}

	}

	/* Echo the record options */
	echo_options(&record_options);
	
	fifo_rows = new CPX*[FIFO_SIZE];
	fifo_buff = new CPX[SAMPS_PER_READ*FIFO_SIZE];
	for(lcv = 0; lcv < FIFO_SIZE; lcv++)
	{
		fifo_rows[lcv] = &fifo_buff[lcv*SAMPS_PER_READ];
	}
	
	fifo_head = FIFO_SIZE/2;
	fifo_tail = 0;
	
	pthread_t precord_thread;
	pthread_t pfifo_thread;
	pthread_t pkey_thread;	
	
	grun = true;
	
	pthread_mutex_init(&mFIFO, NULL);
	pthread_mutex_unlock(&mFIFO);
	
	/* with new priority specified */
	pthread_create(&pfifo_thread, NULL, fifo_thread, (void *)&record_options);
	pthread_create(&precord_thread, NULL, record_thread, (void *)&record_options);
	pthread_create(&pkey_thread, NULL, key_thread, (void *)&record_options);
		
	pthread_join(precord_thread, NULL);
	pthread_join(pfifo_thread, NULL);	
	pthread_join(pkey_thread, NULL);
	
	pthread_mutex_destroy(&mFIFO);
	
	delete [] fifo_buff;
	delete [] fifo_rows;
	
	exit(1);
}


void *record_thread(void *opt)
{
	db_dbs_rx *dbs_rx_a = NULL;
	db_dbs_rx *dbs_rx_b = NULL;
	double ddc_correct = 0;
	int *pbuff;
	int buff1[4*SAMPS_PER_READ];
	int buff2[2*SAMPS_PER_READ];
	int buff3[1*SAMPS_PER_READ];	
	int buffsize = BYTES_PER_READ;
	int bwrote, nbytes, npipe, lcv;
	int leftover;
	unsigned int index;
		
	bool overrun;
	options *_opt = (options *)opt;
	CPX *phead;
	
	/* Make the URX */
	usrp_standard_rx *urx = usrp_standard_rx::make(0, _opt->decimate, 1, -1, 0, 0, 0);
	if(urx == NULL)
	{
		printf("usrp_standard_rx::make FAILED\n");
		pthread_exit(0);
	}
	
	/* N channels according to which boards are enabled */
	urx->set_nchannels(_opt->board_a + _opt->board_b);
	
	/* Create DB_DBS_RX object if the daughter board ID matches */
	if(urx->daughterboard_id(0) == 2) 
	{
		dbs_rx_a = new db_dbs_rx(urx, 0);
		
		/* Set the default master clock freq */
		dbs_rx_a->set_fpga_master_clock_freq(F_SAMPLE);
		dbs_rx_a->set_refclk_divisor(16);
		dbs_rx_a->enable_refclk(true);	
				
		dbs_rx_a->bandwidth(_opt->bandwidth);
		dbs_rx_a->if_gain(_opt->gi);
		dbs_rx_a->rf_gain(_opt->gr);
		dbs_rx_a->tune(_opt->f_lo);
		
		if(_opt->verbose)
		{
			printf("DBS-RX A BW: \t\t%f\n",dbs_rx_a->bw());
			printf("DBS-RX A LO: \t\t%f\n",dbs_rx_a->freq());
			printf("DBS-RX A IF Gain: \t%f\n",dbs_rx_a->if_gain());
			printf("DBS-RX A RF Gain: \t%f\n",dbs_rx_a->rf_gain());
			printf("DBS-RX A Diff: \t\t%f\n",dbs_rx_a->freq()-_opt->f_lo);
		}
				
		/* Add additional frequency to ddc to account for imprecise LO programming */
		ddc_correct = dbs_rx_a->freq() - _opt->f_lo;
	}


	/* Create DB_DBS_RX object if the daughter board ID matches */	
	if(urx->daughterboard_id(1) == 2) 
	{
		dbs_rx_b = new db_dbs_rx(urx, 1);
		
		/* Set the default master clock freq */
		dbs_rx_b->set_fpga_master_clock_freq(F_SAMPLE);
		dbs_rx_b->set_refclk_divisor(16);
		dbs_rx_b->enable_refclk(true);	
				
		dbs_rx_b->bandwidth(_opt->bandwidth);
		dbs_rx_b->if_gain(_opt->gi);
		dbs_rx_b->rf_gain(_opt->gr);
		dbs_rx_b->tune(_opt->f_lo);

		if(_opt->verbose)
		{
			printf("DBS-RX B BW: \t\t%f\n",dbs_rx_b->bw());
			printf("DBS-RX B LO: \t\t%f\n",dbs_rx_b->freq());
			printf("DBS-RX B IF Gain: \t%f\n",dbs_rx_b->if_gain());
			printf("DBS-RX B RF Gain: \t%f\n",dbs_rx_b->rf_gain());
			printf("DBS-RX B Diff: \t\t%f\n",dbs_rx_b->freq()-_opt->f_lo);
		}
		
		/* Add additional frequency to ddc to account for imprecise LO programming */
		ddc_correct = dbs_rx_b->freq() - _opt->f_lo;
	}
	
	/* Set the mux */ 
	if((_opt->board_a + _opt->board_b) > 1)
		urx->set_mux(0x32103210);	//for both channels
	else if(_opt->board_a)
		urx->set_mux(0x32103210);	//board A only
	else
		urx->set_mux(0x10321032);	//board B only

	/* Set the decimation */
	urx->set_decim_rate(_opt->decimate);

	/* Set the DDC frequency */
	_opt->f_ddc += ddc_correct;
	_opt->f_ddc *= DDC_FIX;
	
	if(_opt->f_ddc > (F_SAMPLE_NOM/2.0))
		_opt->f_ddc = F_SAMPLE_NOM - _opt->f_ddc;
		
	urx->set_rx_freq(0, _opt->f_ddc);
	urx->set_rx_freq(1, _opt->f_ddc);
	
	/* Reset DDC phase to zero */
	urx->set_ddc_phase(0, 0);
	urx->set_ddc_phase(1, 0);		
	
	printf("DDC 0: \t\t\t%f\n",urx->rx_freq(0));
	printf("DDC 1: \t\t\t%f\n",urx->rx_freq(1));

	urx->start();

	printf("USRP Start\n");
	
	/* Clear out any junk data */
	for(lcv = 0; lcv < 10; lcv++)
		urx->read(&buff1[0], buffsize, &overrun);
	
	overrun = 0;
	leftover = 0;
	
	while(grun)
	{
		/* Read in 4096 samples */		
		urx->read(&buff1[leftover], buffsize, &overrun);
		urx->read(&buff1[leftover+2048], buffsize, &overrun);

		/* Resample 4 Msps to 2.048 Msps */
		index = 0;
		for(lcv = 0; lcv < SAMPS_PER_READ; lcv++)
		{
			buff3[lcv] = buff1[index >> 11];
			index += 4000;
		}
		
		/* Leftover samples */
		leftover += 96;
		
		/* Move excess bytes at end of buffer down to the base */
		memcpy(buff2, &buff1[4000], leftover*sizeof(int));
		memcpy(buff1, buff2, leftover*sizeof(int));
		
		/* Enqueue to FIFO */
		pthread_mutex_lock(&mFIFO);
		if((fifo_head+1) % FIFO_SIZE == fifo_tail)
		{	
			//printf("FIFO Overflow!\n");
			pthread_mutex_unlock(&mFIFO);
		}
		else
		{
			phead = fifo_rows[fifo_head];
			memcpy(phead, buff3, SAMPS_PER_READ*sizeof(CPX));
			fifo_head = fifo_head + 1;
			fifo_head %= FIFO_SIZE;
			pthread_mutex_unlock(&mFIFO);
		}
	
		if(overrun)
		{
			fprintf(stderr, "o");
			fflush(stderr);
		}
		
		/* An extra ms has snuck in */
		if(leftover > 4000)
		{
			/* Resample 4 Msps to 2.048 Msps */
			index = 0;
			for(lcv = 0; lcv < SAMPS_PER_READ; lcv++)
			{
				buff3[lcv] = buff1[index >> 11];
				index += 4000;
			}

			leftover -= 4000;
			
			/* Move excess bytes at end of buffer down to the base */
			memcpy(buff2, &buff1[4000], leftover*sizeof(int));
			memcpy(buff1, buff2, leftover*sizeof(int));			
			
			/* Enqueue to FIFO */
			pthread_mutex_lock(&mFIFO);
			if((fifo_head+1) % FIFO_SIZE == fifo_tail)
			{	
				//printf("FIFO Overflow!\n");
				pthread_mutex_unlock(&mFIFO);
			}
			else
			{
				phead = fifo_rows[fifo_head];
				memcpy(phead, buff3, SAMPS_PER_READ*sizeof(CPX));
				fifo_head = fifo_head + 1;
				fifo_head %= FIFO_SIZE;
				pthread_mutex_unlock(&mFIFO);
			}
			
		}		
		
	}
	
	urx->stop();

	printf("USRP Stop\n");
	
	grun = false;

	printf("Closing pipe\n");
	close(npipe);
	printf("Pipe closed\n");		
			
	if(dbs_rx_a != NULL)
		delete dbs_rx_a; 

	if(dbs_rx_b != NULL)
		delete dbs_rx_b; 

	if(urx != NULL)
		delete urx;

	pthread_exit(0);
	
}


void *fifo_thread(void *_arg)
{
	
	int fifo, npipe, lcv, nbytes, bwrote;
	CPX buff[SAMPS_PER_READ];
	CPX *ptail;
	char *pbuff;
	
	printf("FIFO thread start\n");
	
	/* Everything set, now create a disk thread & pipe, and do some recording! */
	fifo = mkfifo("/tmp/GPSPIPE", 0666);
	if ((fifo == -1) && (errno != EEXIST))
        printf("Error creating the named pipe");
    else
    	printf("Named pipe created\n");
	
	printf("Waiting for client\n");
	npipe = open("/tmp/GPSPIPE", O_WRONLY);
	printf("Client connected\n");
		
		
	while(grun)
	{
		
		pthread_mutex_lock(&mFIFO);
		if((fifo_tail+1) % FIFO_SIZE == fifo_head)
		{	
			//printf("FIFO Underflow!\n");
			pthread_mutex_unlock(&mFIFO);
			usleep(100);
		}
		else
		{
			ptail = fifo_rows[fifo_tail];
			memcpy(buff, ptail, SAMPS_PER_READ*sizeof(CPX));
			fifo_tail = fifo_tail + 1;
			fifo_tail %= FIFO_SIZE;
			pthread_mutex_unlock(&mFIFO);
			
			/* Dump to the pipe */
			nbytes = 0; pbuff = (char *)&buff[0];
			while((nbytes < BYTES_PER_READ) && grun)
			{
				bwrote = write(npipe, &pbuff[nbytes], PIPE_BUF);
				signal(SIGPIPE, SIG_IGN);
				if(bwrote > 0)
					nbytes += bwrote;
					
				usleep(100);
			}	
		}
	}	
	
	close(npipe);
	
	printf("FIFO thread stop\n");
	
	pthread_exit(0);
	
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

