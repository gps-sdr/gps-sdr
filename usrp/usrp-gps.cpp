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
#include <semaphore.h>
#include <sched.h>
#include <limits.h>
#include <time.h>
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

/*----------------------------------------------------------------------------------------------*/
typedef struct _CPX
{
	short r;
	short i;
} CPX;

typedef struct _options
{
	int		mode;		//!< Run in a  2 antenna mode with 2 DBS-RXs, Run in a  2 antenna mode, board A L1, board B L2
	int		verbose;	//!< Output debug info
	int		decimate;	//!< Decimation level
	int		record;		//!< Dump data to disk
	double	f_lo_a;		//!< LO freq for board A
	double	f_ddc_a;	//!< DDC freq for board A
	double	f_lo_b;		//!< LO freq for board B
	double	f_ddc_b;	//!< DDC freq for board B
	double 	bandwidth;	//!< DB-SRX filter bandwidth
	double 	gi;			//!< IF gain
	double 	gr;			//!< RF gain
	double	f_sample;	//!< Sample rate (pepending on what clock)

} options;
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
#define F_SAMPLE_NOM	(64e6)
#define SAMPS_PER_READ	(4096)
#define BYTES_PER_READ  (SAMPS_PER_READ*4)
#define READ 			(0)
#define WRITE 			(1)
#define F_L1			(1.57542e9)
#define F_L2			(1.2276e9)
#define FIFO_SIZE 		(4000) //!< 2 seconds deep?
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void PostStatus(char *_str) {fprintf(stdout,"%s",_str); fflush(stdout);}
void *record_thread(void *_opt);
void *fifo_thread(void *_opt);
void *key_thread(void *_arg);
void resample(CPX *_in, CPX *_out, options *_opt);		//!< Resample to get in the 2.048 Msps format, also handles de-interleave
void write_pipe(CPX *_buff, int _npipe, int _bytes);
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
bool	grun;			//!< Global run variable
int		fifo_head;		//!< FIFO head position
int		fifo_tail;		//!< FIFO tail position
CPX		**fifo_rows;	//!< FIFO rows
CPX		*fifo_buff;		//!< FIFO buffer
sem_t 	mEMPTY;			//!< # of empty nodes
sem_t 	mFILLED;		//!< # of full nodes
pthread_mutex_t mFIFO;
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void usage(char *_str)
{

	fprintf(stderr, "usage: [-gr] [-gi] [-d] [-l] [-w] [-v] [-c]\n");
	fprintf(stderr, "[-gr] <gain> set rf gain in dB (DBSRX only)\n");
	fprintf(stderr, "[-gi] <gain> set if gain in dB (DBSRX only)\n");
	fprintf(stderr, "[-d] operate in two antenna mode, A & B as L1\n");
	fprintf(stderr, "[-l] operate in L1-L2 mode, A as L1, B as L2\n");
	fprintf(stderr, "[-w] <bandwidth> bandwidth of lowpass filter\n");
	fprintf(stderr, "[-v] output extra debug info\n");
	fprintf(stderr, "[-r] dump data to disk\n");
	fprintf(stderr, "[-c] the USRP samples at a modified 65.536 MHz (default is 64 MHz)\n");
	fflush(stderr);

	exit(1);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
int echo_options(options *_opt)
{

	/* Step one, find out what daughterboards are populated */
	/* Open the base RX class */
	usrp_standard_rx *urx = usrp_standard_rx::make(0, 8, 1, -1, 0, 0, 0);
	if(urx == NULL)
	{
		printf("usrp_standard_rx::make FAILED\n");
		return(-1);
	}

	switch(urx->daughterboard_id(0))
	{
		case 1:
			fprintf(stdout,"Board A:\t\t\tBasic RX\n"); break;
		case 2:
			fprintf(stdout,"Board A:\t\t\t DBS RX\n"); break;
		default:
			fprintf(stdout,"Board A:\t\t\tUnknown\n"); break;
	}

	switch(urx->daughterboard_id(1))
	{
		case 1:
			fprintf(stdout,"Board B:\t\t\tBasic RX\n"); break;
		case 2:
			fprintf(stdout,"Board B:\t\t\t DBS RX\n"); break;
		default:
			fprintf(stdout,"Board B:\t\t\tUnknown\n"); break;
	}

	switch(_opt->mode)
	{
		case 0: fprintf(stdout, "Single L1 mode			(L1->A)\n"); break;
		case 1: fprintf(stdout, "Dual L1 mode			(L1->A L1->B)\n"); break;
		case 2: fprintf(stdout, "Dual L1-L2C mode		(L1->A L2C->B)\n"); break;
	}

	/* Error check based on the mode */
	switch(_opt->mode)
	{
		case 0:
			if(urx->daughterboard_id(0) != 2)
			{
				fprintf(stdout, "Board A is not a DBS-RX!\n");
				delete urx;
				return(-1);
			}
			break;
		case 1:
			if(urx->daughterboard_id(0) != 2)
			{
				fprintf(stdout, "Board A is not a DBS-RX!\n");
				delete urx;
				return(-1);
			}

			if(urx->daughterboard_id(1) != 2)
			{
				fprintf(stdout, "Board B is not a DBS-RX!\n");
				delete urx;
				return(-1);
			}
			break;
		case 2:
			if(urx->daughterboard_id(0) != 2)
			{
				fprintf(stdout, "Board A is not a DBS-RX!\n");
				delete urx;
				return(-1);
			}

			if(urx->daughterboard_id(1) != 2)
			{
				fprintf(stdout, "Board B is not a DBS-RX!\n");
				delete urx;
				return(-1);
			}
				break;
		default:
			fprintf(stdout, "_opt->mode non-valid\n");
			delete urx;
			return(-1);

	}

	fprintf(stdout, "USRP Sample Rate:\t% 15.2f\n",_opt->f_sample);
	fprintf(stdout, "USRP Decimation:\t% 15d\n",_opt->decimate);
	fprintf(stdout, "DBSRX LO A:\t\t% 15.2f\n",_opt->f_lo_a);
	fprintf(stdout, "DBSRX LO B:\t\t% 15.2f\n",_opt->f_lo_b);
//	fprintf(stdout, "DDC Frequency A:\t%f\n",_opt->f_ddc_a);
//	fprintf(stdout, "DDC Frequency B:\t%f\n",_opt->f_ddc_b);
	fprintf(stdout, "RF Gain:\t\t% 15.2f\n",_opt->gr);
	fprintf(stdout, "IF Gain:\t\t% 15.2f\n",_opt->gi);
	fprintf(stdout, "DBSRX Bandwidth:\t% 15.2f\n",_opt->bandwidth);

	delete urx;

	return(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
	int lcv;
	options record_options;
	char *parse;

	/* Set default recording options */
	record_options.mode = 		0;		//!< Single board L1 mode by default
	record_options.decimate = 	16;		//!< Default to work with both 65.536 and 64 MHz clocks
	record_options.gr =			40; 	//!< 40 dB of RF gain
	record_options.gi = 		10; 	//!< 10 dB of IF gain
	record_options.f_lo_a = 	F_L1;	//!< Board A L1 by default
	record_options.f_ddc_a = 	0;		//!< no DDC correction
	record_options.f_lo_b = 	F_L2;	//!< Board B L2 by default
	record_options.f_ddc_b = 	0;		//!< no DDC correction
	record_options.bandwidth =	16.0e6; //!< DBS-RX is set to 10 MHz wide
	record_options.f_sample = 	64.0e6;	//!< Nominal sample rate
	record_options.verbose = 	0;		//!< Output extra debugging info
	record_options.record = 	0;		//!< Record data to disk

	for(lcv = 1; lcv < argc; lcv++)
	{
		switch (argv[lcv][1])
		{

			case 'd':
				record_options.mode = 1;
				break;

			case 'l':
				record_options.mode = 2;
				record_options.f_lo_b = F_L2; /* L2C center frequency */
				break;

			case 'g':
				if(argv[lcv][2] == 'r')
				{
					if(++lcv >= argc)
						usage (argv[0]);

					if(isdigit(argv[lcv][0]))
						record_options.gr = strtod(argv[lcv], &parse);
					else
						usage (argv[0]);
					break;
				}
				else if(argv[lcv][2] == 'i')
				{
					if(++lcv >= argc)
						usage (argv[0]);

					if(isdigit(argv[lcv][0]))
						record_options.gi = strtod(argv[lcv], &parse);
					else
						usage (argv[0]);
					break;
				}
				else
					usage(argv[0]);
				break;

			case 'w':
				if(++lcv >= argc)
					usage (argv[0]);

				if(isdigit(argv[lcv][0]))
					record_options.bandwidth = strtod(argv[lcv], &parse);
				else
					usage (argv[0]);
				break;

			case 'c':
				record_options.f_sample = 65.536e6;
				break;

			case 'v':
				record_options.verbose = 1;
				break;

			case 'r':
				record_options.record = 1;
				break;

			default:
				usage (argv[0]);
		}

	}

	/* Echo the record options, do some bug checks */
	if(echo_options(&record_options) == -1)
	{
		exit(0);
	}

	/* Allocate FIFO buffer */
	fifo_rows = new CPX*[FIFO_SIZE];
	fifo_buff = new CPX[SAMPS_PER_READ*FIFO_SIZE];
	for(lcv = 0; lcv < FIFO_SIZE; lcv++)
	{
		fifo_rows[lcv] = &fifo_buff[lcv*SAMPS_PER_READ];
	}

	fifo_head = 0;
	fifo_tail = 0;

	pthread_t precord_thread;
	pthread_t pfifo_thread;
	pthread_t pkey_thread;
	pthread_attr_t tattr;
	struct sched_param tparam;

	grun = true;

	pthread_mutex_init(&mFIFO, NULL);
	sem_init(&mEMPTY, 0, FIFO_SIZE);	//!< All are empty
	sem_init(&mFILLED, 0, 0);			//!< None are filled yet

	/* Unitialized with default attributes */
	pthread_attr_init(&tattr);
	pthread_attr_setschedpolicy(&tattr, SCHED_FIFO);

	pthread_create(&precord_thread, &tattr, record_thread, (void *)&record_options);
	pthread_create(&pkey_thread, &tattr, key_thread, (void *)&record_options);
	pthread_create(&pfifo_thread, &tattr, fifo_thread, (void *)&record_options);

	pthread_attr_getschedparam(&tattr, &tparam);

//	tparam.sched_priority = sched_get_priority_max(SCHED_FIFO);
//	if(pthread_setschedparam(precord_thread, SCHED_FIFO, &tparam))
//		fprintf(stdout,"Could not elevate priority of USRP thread\n");

//	tparam.sched_priority = sched_get_priority_max(SCHED_FIFO);
//	if(pthread_setschedparam(pfifo_thread, SCHED_FIFO, &tparam))
//		fprintf(stdout,"Could not elevate priority of FIFO thread\n");
//
//	tparam.sched_priority = sched_get_priority_max(SCHED_FIFO);
//	if(pthread_setschedparam(pkey_thread, SCHED_FIFO, &tparam))
//		fprintf(stdout,"Could not elevate priority of KEY thread\n");

	while(grun)
	{
		usleep(100000);
	}

	sem_post(&mFILLED);
	pthread_mutex_unlock(&mFIFO);
	pthread_join(pfifo_thread, NULL);

	sem_post(&mEMPTY);
	pthread_mutex_unlock(&mFIFO);
	pthread_join(precord_thread, NULL);

	pthread_cancel(pkey_thread);
	pthread_join(pkey_thread, NULL);

	pthread_mutex_destroy(&mFIFO);
	sem_destroy(&mEMPTY);
	sem_destroy(&mFILLED);

	delete [] fifo_buff;
	delete [] fifo_rows;

	exit(1);
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void *record_thread(void *opt)
{
	db_dbs_rx *dbs_rx_a = NULL;
	db_dbs_rx *dbs_rx_b = NULL;
	double ddc_correct_a = 0;
	double ddc_correct_b = 0;

	int buff[SAMPS_PER_READ];
	int buffsize = BYTES_PER_READ;

	int bwrote, nbytes, npipe, lcv, filled, empty;
	bool overrun;
	time_t rawtime;
	struct tm * timeinfo;
	options *_opt = (options *)opt;
	CPX *phead;

	/* Make the URX */
	usrp_standard_rx *urx = usrp_standard_rx::make(0, _opt->decimate, 1, -1, 0, 0, 0);
	if(urx == NULL)
	{
		printf("usrp_standard_rx::make FAILED\n");
		pthread_exit(0);
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

	overrun = 0;

	while(grun)
	{

		/* Wait until some nodes are empty */
		sem_wait(&mEMPTY);

		/* Read data from USRP */
		urx->read(&buff[0], buffsize, &overrun);

		/* Do the FIFO stuff */
		pthread_mutex_lock(&mFIFO);
		phead = fifo_rows[fifo_head];
		memcpy(phead, buff, SAMPS_PER_READ*sizeof(CPX));
		fifo_head = fifo_head + 1;
		fifo_head %= FIFO_SIZE;
		pthread_mutex_unlock(&mFIFO);

		/* Added a filled node */
		sem_post(&mFILLED);

		if(overrun)
		{
			time(&rawtime);
			timeinfo = localtime (&rawtime);
			fprintf(stdout, "\nOverflow at time %s",asctime (timeinfo));
			fflush(stdout);
		}

	}

	if(_opt->verbose)
		printf("Stopping USRP\n");
	urx->stop();

	if(_opt->verbose)
		printf("USRP Stopped\n");

	if(_opt->verbose)
		printf("Closing pipe\n");

	close(npipe);

	if(_opt->verbose)
		printf("Pipe closed\n");

	if(dbs_rx_a != NULL)
		delete dbs_rx_a;

	if(dbs_rx_b != NULL)
		delete dbs_rx_b;

	if(urx != NULL)
		delete urx;

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void *fifo_thread(void *arg)
{

	options *_opt = (options *)arg;
	int fifo, npipe, lcv, bwrite, filled, empty;
	FILE *fp_out = NULL;

	CPX buff[16384]; //Base buffer
	CPX db_a[16384]; //Buffer for double buffering
	CPX buff_out[4096]; //Output buffer

	int leftover;
	int sample_mode;

	if(_opt->record)
		fp_out = fopen("gps.dba","wb");

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

	CPX *ptail;
	char *pbuff;
	lcv = 0;

	if(_opt->verbose)
		printf("FIFO thread start\n");

	/* Everything set, now create a disk thread & pipe, and do some recording! */
	fifo = mkfifo("/tmp/GPSPIPE", 0666);
	if ((fifo == -1) && (errno != EEXIST))
        printf("Error creating the named pipe");
//    else
//    	printf("Named pipe created\n");

	printf("Waiting for client\n");
	npipe = -1;
	while((npipe == -1) && grun)
	{
		npipe = open("/tmp/GPSPIPE", O_WRONLY | O_NONBLOCK);
		usleep(1000000);
	}

	if(npipe != -1)
		printf("Client connected\n");
	else
	{
		printf("Killed before client connected\n");
		pthread_exit(0);
	}

	/* Important set this to zero! */
	leftover = 0;

	while(grun)
	{

		/* Wait for a filled node */
		sem_wait(&mFILLED);

		pthread_mutex_lock(&mFIFO);
		ptail = fifo_rows[fifo_tail];
		memcpy(&buff[leftover], ptail, SAMPS_PER_READ*sizeof(CPX));
		fifo_tail = fifo_tail + 1;
		fifo_tail %= FIFO_SIZE;
		pthread_mutex_unlock(&mFIFO);

		/* I just emptied another node */
		sem_post(&mEMPTY);

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
				resample(buff, buff_out, _opt);
				write_pipe(buff_out, npipe, bwrite);

				if(_opt->record)
					fwrite(buff_out, 0x1, bwrite, fp_out);

				leftover = 0;
				break;
			case 1:
				leftover += 2048; leftover %= 4096;
				if(leftover == 0)
				{
					resample(buff, buff_out, _opt);
					write_pipe(buff_out, npipe, bwrite);

					if(_opt->record)
						fwrite(buff_out, 0x1, bwrite, fp_out);
				}
				break;
			case 2:

				leftover += 96;
				resample(buff, buff_out, _opt);
				write_pipe(buff_out, npipe, bwrite);

				if(_opt->record)
					fwrite(buff_out, 0x1, bwrite, fp_out);

				/* Move excess bytes at end of buffer down to the base */
				memcpy(db_a, &buff[4000], leftover*sizeof(int));
				memcpy(buff, db_a, leftover*sizeof(int));

				if(leftover > 4000)
				{
					resample(buff, buff_out, _opt);
					write_pipe(buff_out, npipe, bwrite);

					if(_opt->record)
						fwrite(buff_out, 0x1, bwrite, fp_out);

					leftover -= 4000;
					memcpy(db_a, &buff[4000], leftover*sizeof(int));
					memcpy(buff, db_a, leftover*sizeof(int));
				}
				break;

			case 3:

				leftover += 192;
				resample(buff, buff_out, _opt);
				write_pipe(buff_out, npipe, bwrite);

				if(_opt->record)
					fwrite(buff_out, 0x1, bwrite, fp_out);

				/* Move excess bytes at end of buffer down to the base */
				memcpy(db_a, &buff[8000], leftover*sizeof(int));
				memcpy(buff, db_a, leftover*sizeof(int));

				if(leftover > 8000)
				{

					resample(buff, buff_out, _opt);
					write_pipe(buff_out, npipe, bwrite);

					if(_opt->record)
						fwrite(buff_out, 0x1, bwrite, fp_out);

					leftover -= 8000;
					memcpy(db_a, &buff[8000], leftover*sizeof(int));
					memcpy(buff, db_a, leftover*sizeof(int));
				}
				break;

			default:
				break;
		}

		/* Cut down on output rate */
		if(++lcv >= 250)
		{
			sem_getvalue(&mFILLED, &filled);
			sem_getvalue(&mEMPTY, &empty);

			fprintf(stdout,"FIFO Filled\\Empty: %4d,%4d										\r",filled,empty);
			fflush(stdout);
			lcv = 0;
		}

	}



	close(npipe);

	if(_opt->record)
		fclose(fp_out);

	if(_opt->verbose)
		printf("FIFO thread stop\n");

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
/* Monitor keyboard to kill real-time thread */
void *key_thread(void *_arg)
{

	options *_opt = (options *)_arg;
	int key;

	if(_opt->verbose)
		printf("Key thread start\n");

	while(grun)
	{
		key = getchar();

		if((char)key == 'Q')
			grun = false;

		sleep(1);
	}

	if(_opt->verbose)
		printf("Key thread stop\n");

	pthread_exit(0);

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void kill_program(int _sig)
{
	grun = false;
	printf("Lost GPS-SDR!\n");
}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void write_pipe(CPX *_buff, int _npipe, int _bytes)
{

	int nbytes, bwrote;
	char *pbuff;

	/* Dump to the pipe */
	nbytes = 0; pbuff = (char *)&_buff[0];
	while((nbytes < _bytes) && grun)
	{
		signal(SIGPIPE, kill_program);
		bwrote = write(_npipe, &pbuff[nbytes], PIPE_BUF);

		if(bwrote > 0)
			nbytes += bwrote;

	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void downsample(CPX *_dest, CPX *_source, double _fdest, double _fsource, int _samps)
{

	int lcv, k;
	unsigned int phase_step;
	unsigned int lphase, phase;

	phase_step = (unsigned int)floor((double)4294967296.0*_fdest/_fsource);

	k = lphase = phase = 0;

	for(lcv = 0; lcv < _samps; lcv++)
	{
		if(phase <= lphase)
		{
			_dest[k] = _source[lcv];
			k++;
		}

		lphase = phase;
		phase += phase_step;
	}

}
/*----------------------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------------------------*/
void resample(CPX *_in, CPX *_out, options *_opt)
{

	CPX buff_a[4096]; /* Max size based on 65.536 or 64 Msps */
	CPX buff_b[4096]; /* Max size based on 65.536 or 64 Msps */
	int *p_a;
	int *p_b;
	int *p_in;
	int *p_out;
	int samps_ms;
	int lcv;

	p_a = (int *)&buff_a[0];
	p_b = (int *)&buff_b[0];
	p_in = (int *)_in;
	p_out = (int *)_out;

	samps_ms = (int)floor(_opt->f_sample/_opt->decimate/1e3);

	if(_opt->mode == 0)
	{
		/* Not much to do, just downsample from either 4.096 or 4.0 to 2.048e6 */
		if(_opt->f_sample != 65.536e6)
			downsample(_out, _in, 2.048e6, _opt->f_sample/_opt->decimate, samps_ms);
		else
		{
			for(lcv = 0; lcv < samps_ms; lcv+=2)
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
		downsample(&_out[0],    buff_a, 2.048e6, _opt->f_sample/_opt->decimate, samps_ms);
		downsample(&_out[2048], buff_b, 2.048e6, _opt->f_sample/_opt->decimate, samps_ms);

	}

}
/*----------------------------------------------------------------------------------------------*/



