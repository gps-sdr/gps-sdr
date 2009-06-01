/*! \file FFT_Test.cpp
	Used in conjunction with matlab to debug the fixed point FFT
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
#include <time.h>

int main(int32 argc, char** argv)
{

	clock_t time_0, time_1;
	double t1;
	FILE* fp, *fpo;
	CPX *X;
	int N, repeats, lcv;
	
	if(argc != 3)
	{
		N = 1024;
		repeats = 1;
	}
	else
	{
		N = atoi(argv[1]);
		repeats = atoi(argv[2]);
	}

	fprintf(stdout,"%d\n",argc);
	for(lcv = 0; lcv < argc; lcv++)
		fprintf(stdout,"%s\n",argv[lcv]);

	/* Read in the data */
	fp = fopen("input.dat","rb");
	if(fp == NULL)
	{
        return(-1);
	}
	else
	{
		X = (CPX *)malloc(N*sizeof(CPX));
		fread(X, sizeof(CPX), N, fp);
		fclose(fp);
	}
  

	/* FFT */
	/*-------------------------------------------*/
	FFT aFFT(N);
	fprintf(stdout,"FFT Created\n");
	
	time_0 = clock();
	
	/* Do the FFT */
	for(lcv = 0; lcv < repeats; lcv++)
		aFFT.doFFT(X, true);
		
	time_1 = clock();

	t1 = (double)(time_1 - time_0) /CLOCKS_PER_SEC;

	fprintf(stdout,"Time: %f\n",t1);

	fpo = fopen("output.dat","wb");
	if(fpo != NULL)
	{
		fwrite(X, sizeof(CPX), N, fpo);
		fclose(fpo);
	}
	/*-------------------------------------------*/

	free(X);

	return(0);
	
}



