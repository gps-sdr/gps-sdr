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

#define VECTSIZE (1000)
#define REPEATS	 (100)

//#define VECTSIZE (32)
//#define REPEATS	 (1)

//void  x86_add(int16 *A, int16 *B, int32 cnt);	//!< Pointwise vector addition
//void  x86_sub(int16 *A, int16 *B, int32 cnt);	//!< Pointwise vector difference
//void  x86_mul(int16 *A, int16 *B, int32 cnt);	//!< Pointwise vector multiply
//int32 x86_acc(int16 *A, int32 cnt);				//!< Compute vector sum
//int32 x86_dot(int16 *A, int16 *B, int32 cnt);	//!< Compute vector dot product
//
//void  x86_conj(CPX *A, int32 cnt);				//!< Pointwise vector conjugate
//void  x86_crot(CPX *A, CPX *B, int32 cnt);		//!< Pointwise vector rotate
//void  x86_cmul(CPX *A, CPX *B, int32 cnt);		//!< Pointwise vector multiply
//int64 x86_cacc(CPX *A, int32 cnt);				//!< Compute vector sum
//int64 x86_cdot(CPX *A, CPX *B, int32 cnt);		//!< Compute vector dot product

void fill_vect(CPX *_vect, int32 _samps)
{
	int32 lcv;

	/* initialize random seed: */
  	srand(time(NULL));

	/* Fill with values capped to +- 2^15 */
	for(lcv = 0; lcv < _samps; lcv++)
	{
//		_vect[lcv].i = (int16)((rand() & 0x7fff) - 0x4000);
//		_vect[lcv].q = (int16)((rand() & 0x7fff) - 0x4000);

		_vect[lcv].i = (int16)((rand() % 32) - 16);
		_vect[lcv].q = (int16)((rand() % 32) - 16);

	}

}


void fill_prn(CPX *_vect, int32 _samps)
{
	int32 lcv;

	/* initialize random seed: */
  	//srand(time(NULL));

	/* Fill with values 0 or -1 */
	for(lcv = 0; lcv < _samps; lcv++)
	{
		if((rand() & 0x1) == 0)
			_vect[lcv].i = _vect[lcv].q = 0xffff;
		else
			_vect[lcv].i = _vect[lcv].q = 0x0000;
	}

}


void fill_prn_new(MIX *_vect, int32 _samps)
{
	int32 lcv;

	/* initialize random seed: */
  	//srand(time(NULL));

	/* Fill with values 0 or -1 */
	for(lcv = 0; lcv < _samps; lcv++)
	{
		if(rand() & 0x1)
			_vect[lcv].i = _vect[lcv].ni = 0x0001;
		else
			_vect[lcv].i = _vect[lcv].ni = 0xffff;

		_vect[lcv].q =  _vect[lcv].nq = 0;
	}

}

int main(int32 argc, char* argv[])
{

	printf("SIMD_Test\n");

	CPX *testvecta;
	CPX *testvectb;
	CPX *testvectc;
	CPX *testvectd;
	CPX *testvecte;
	MIX *testvectf;
	MIX *testvectg;
	MIX *testvecth;

	int32 err;
	int32 lcv;
	int32 lcv2;
	int32 pts;
	int32 val1;
	int32 val2;
	int32 shift;
	int32 ai1, aq1;
	int32 ai2, aq2;

	testvecta = new CPX[VECTSIZE];
	testvectb = new CPX[VECTSIZE];
	testvectc = new CPX[VECTSIZE];
	testvectd = new CPX[VECTSIZE];
	testvecte = new CPX[VECTSIZE];

	testvectf = new MIX[VECTSIZE];
	testvectg = new MIX[VECTSIZE];
	testvecth = new MIX[VECTSIZE];



	/* SIMD ADD */
	/*----------------------------------------------------------------------------------------------*/
	err = 0;

	for(lcv = 0; lcv < REPEATS; lcv++)
	{

		pts = rand() % VECTSIZE;

		fill_vect(testvecta, pts);
		fill_vect(testvectb, pts);

		memcpy(testvectc, testvecta, pts*sizeof(CPX));

		x86_add((int16 *)testvecta, (int16 *)testvectb, pts*2);
		sse_add((int16 *)testvectc, (int16 *)testvectb, pts*2);

		for(lcv2 = 0; lcv2 < pts; lcv2++)
		{

			if(testvecta[lcv2].i != testvectc[lcv2].i)
				err++;

			if(testvecta[lcv2].q != testvectc[lcv2].q)
				err++;
		}

	}

	if(err)
		printf("INT16 ADD \t\t\tFAILED: %d\n",err);
	else
		printf("INT16 ADD \t\t\tPASSED\n",err);
	/*----------------------------------------------------------------------------------------------*/


	/* SIMD SUB */
	/*----------------------------------------------------------------------------------------------*/
	err = 0;

	for(lcv = 0; lcv < REPEATS; lcv++)
	{

		pts = rand() % VECTSIZE;

		fill_vect(testvecta, pts);
		fill_vect(testvectb, pts);

		memcpy(testvectc, testvecta, pts*sizeof(CPX));

		x86_sub((int16 *)testvecta, (int16 *)testvectb, pts*2);
		sse_sub((int16 *)testvectc, (int16 *)testvectb, pts*2);

		for(lcv2 = 0; lcv2 < pts; lcv2++)
		{
			if(testvecta[lcv2].i != testvectc[lcv2].i)
				err++;

			if(testvecta[lcv2].q != testvectc[lcv2].q)
				err++;
		}

	}

	if(err)
		printf("INT16 SUB \t\t\tFAILED: %d\n",err);
	else
		printf("INT16 SUB \t\t\tPASSED\n",err);
	/*----------------------------------------------------------------------------------------------*/


	/* SIMD MUL */
	/*----------------------------------------------------------------------------------------------*/
	err = 0;

	for(lcv = 0; lcv < REPEATS; lcv++)
	{

		pts = rand() % VECTSIZE;

		fill_vect(testvecta, pts);
		fill_vect(testvectb, pts);

		memcpy(testvectc, testvecta, pts*sizeof(CPX));

		x86_mul((int16 *)testvecta, (int16 *)testvectb, pts*2);
		sse_mul((int16 *)testvectc, (int16 *)testvectb, pts*2);

		for(lcv2 = 0; lcv2 < pts; lcv2++)
		{
			if(testvecta[lcv2].i != testvectc[lcv2].i)
				err++;

			if(testvecta[lcv2].q != testvectc[lcv2].q)
				err++;
		}

	}
	if(err)
		printf("INT16 MUL \t\t\tFAILED: %d\n",err);
	else
		printf("INT16 MUL \t\t\tPASSED\n",err);
	/*----------------------------------------------------------------------------------------------*/


	/* SIMD DOT */
	/*----------------------------------------------------------------------------------------------*/
	err = 0;

	for(lcv = 0; lcv < REPEATS; lcv++)
	{

		pts = rand() % VECTSIZE;

		fill_vect(testvecta, pts);
		fill_vect(testvectb, pts);

		memcpy(testvectc, testvecta, pts*sizeof(CPX));

		val1 = x86_dot((int16 *)testvecta, (int16 *)testvectb, pts*2);
		val2 = sse_dot((int16 *)testvecta, (int16 *)testvectc, pts*2);

		if(val1 != val2)
		{
			printf("x86,SSE: %d,%d\n",val1,val2);
			err++;
		}

	}
	if(err)
		printf("INT16 DOT \t\t\tFAILED: %d\n",err);
	else
		printf("INT16 DOT \t\t\tPASSED\n",err);
	/*----------------------------------------------------------------------------------------------*/


	/* SIMD CONJ */
	/*----------------------------------------------------------------------------------------------*/
	err = 0;

	for(lcv = 0; lcv < REPEATS; lcv++)
	{

		pts = rand() % VECTSIZE;

		fill_vect(testvecta, pts);
		fill_vect(testvectb, pts);

		memcpy(testvectc, testvecta, pts*sizeof(CPX));

		x86_conj(testvecta, pts);
		sse_conj(testvectc, pts);

		for(lcv2 = 0; lcv2 < pts; lcv2++)
		{

			//printf("[%d,%d] [%d,%d]\n",testvecta[lcv2].i,testvecta[lcv2].q,testvectc[lcv2].i,testvectc[lcv2].q);

			if(testvecta[lcv2].i != testvectc[lcv2].i)
				err++;

			if(testvecta[lcv2].q != testvectc[lcv2].q)
				err++;
		}

	}
	if(err)
		printf("CPX CONJ \t\t\tFAILED: %d\n",err);
	else
		printf("CPX CONJ \t\t\tPASSED\n",err);
	/*----------------------------------------------------------------------------------------------*/


	/* SIMD CMUL */
	/*----------------------------------------------------------------------------------------------*/
	err = 0;

	for(lcv = 0; lcv < REPEATS; lcv++)
	{

		pts = rand() % VECTSIZE;

		fill_vect(testvecta, pts);
		fill_vect(testvectb, pts);

		memcpy(testvectc, testvecta, pts*sizeof(CPX));

		x86_cmul(testvecta, testvectb, pts);
		sse_cmul(testvectc, testvectb, pts);

		for(lcv2 = 0; lcv2 < pts; lcv2++)
		{

			//printf("[%d,%d] [%d,%d]\n",testvecta[lcv2].i,testvecta[lcv2].q,testvectc[lcv2].i,testvectc[lcv2].q);

			if(testvecta[lcv2].i != testvectc[lcv2].i)
				err++;

			if(testvecta[lcv2].q != testvectc[lcv2].q)
				err++;
		}

	}
	if(err)
		printf("CPX MUL \t\t\tFAILED: %d\n",err);
	else
		printf("CPX MUL \t\t\tPASSED\n",err);
	/*----------------------------------------------------------------------------------------------*/


	/* SIMD CMUL w SHIFT*/
	/*----------------------------------------------------------------------------------------------*/
	err = 0;

	for(lcv = 0; lcv < REPEATS; lcv++)
	{

		pts = rand() % VECTSIZE;
		shift = 1;

		fill_vect(testvecta, pts);
		fill_vect(testvectb, pts);

		memcpy(testvectc, testvecta, pts*sizeof(CPX));

		x86_cmuls(testvecta, testvectb, pts, shift);
		sse_cmuls(testvectc, testvectb, pts, shift);

		for(lcv2 = 0; lcv2 < pts; lcv2++)
		{

			//printf("[%d,%d] [%d,%d]\n",testvecta[lcv2].i,testvecta[lcv2].q,testvectc[lcv2].i,testvectc[lcv2].q);

			if(testvecta[lcv2].i != testvectc[lcv2].i)
				err++;

			if(testvecta[lcv2].q != testvectc[lcv2].q)
				err++;
		}

	}
	if(err)
		printf("CPX MUL SHIFT \t\t\tFAILED: %d\n",err);
	else
		printf("CPX MUL SHIFT \t\t\tPASSED\n",err);
	/*----------------------------------------------------------------------------------------------*/


	/* SIMD CMUL w SHIFT and Preserve */
	/*----------------------------------------------------------------------------------------------*/
	err = 0;

	for(lcv = 0; lcv < REPEATS; lcv++)
	{

		pts = rand() % VECTSIZE;
		shift = 1;

		fill_vect(testvecta, pts);
		fill_vect(testvectb, pts);


		x86_cmulsc(testvecta, testvectb, testvectc, pts, shift);
		sse_cmulsc(testvecta, testvectb, testvectd, pts, shift);

		for(lcv2 = 0; lcv2 < pts; lcv2++)
		{

			//printf("[%d,%d] [%d,%d]\n",testvecta[lcv2].i,testvecta[lcv2].q,testvectc[lcv2].i,testvectc[lcv2].q);

			if(testvectc[lcv2].i != testvectd[lcv2].i)
				err++;

			if(testvectc[lcv2].q != testvectd[lcv2].q)
				err++;
		}

	}
	if(err)
		printf("CPX MUL SHIFT PRESERVE \t\tFAILED: %d\n",err);
	else
		printf("CPX MUL SHIFT PRESERVE \t\tPASSED\n",err);
	/*----------------------------------------------------------------------------------------------*/


	/* SIMD CACC */
	/*----------------------------------------------------------------------------------------------*/
	err = 0;

	for(lcv = 0; lcv < REPEATS; lcv++)
	{

		pts = rand() % VECTSIZE;

		fill_vect(testvecta, pts);
		fill_vect(testvectb, pts);

		memcpy(testvectc, testvecta, pts*sizeof(CPX));

		pts = pts/2;

		x86_cacc(testvecta, (MIX *)testvectb, pts, &ai1, &aq1);
		sse_cacc(testvecta, (MIX *)testvectb, pts, &ai2, &aq2);

		if(ai1 != ai2)
		{
			printf("Low32: %8x,%8x\n",ai1, ai2);
			err++;
		}

		if(aq1 != aq2)
		{
			printf("High32: %8x,%8x\n",aq1, aq2);
			err++;
		}

	}
	if(err)
		printf("CPX ACCUM \t\t\tFAILED: %d\n",err);
	else
		printf("CPX ACCUM \t\t\tPASSED\n",err);
	/*----------------------------------------------------------------------------------------------*/

	/* SIMD x86_prn_accum */
	/*----------------------------------------------------------------------------------------------*/
	err = 0;

	for(lcv = 0; lcv < REPEATS; lcv++)
	{

		CPX accuma[3];
		CPX accumb[3];


		pts = rand() % VECTSIZE;

		fill_vect(testvecta, pts);


		fill_prn(testvectb, pts);
		fill_prn(testvectc, pts);
		fill_prn(testvectd, pts);

		sse_prn_accum(testvecta, testvectb, testvectc, testvectd, pts, &accuma[0]);
		x86_prn_accum(testvecta, testvectb, testvectc, testvectd, pts, &accumb[0]);

		for(lcv2 = 0; lcv2 < 3; lcv2++)
			if(accuma[lcv2].i != accumb[lcv2].i)
				err++;

		for(lcv2 = 0; lcv2 < 3; lcv2++)
			if(accuma[lcv2].q != accumb[lcv2].q)
				err++;

		if(err)
		{
			for(lcv2 = 0; lcv2 < 3; lcv2++)
				printf("%d.%d,%d.%d\n",accuma[lcv2].i,accuma[lcv2].q,accumb[lcv2].i,accumb[lcv2].q);
		}

	}
	if(err)
		printf("CPX PRN ACCUM \t\t\tFAILED: %d\n",err);
	else
		printf("CPX PRN ACCUM \t\t\tPASSED\n",err);
	/*----------------------------------------------------------------------------------------------*/


	/* SIMD x86_prn_accum new */
	/*----------------------------------------------------------------------------------------------*/
	err = 0;

	for(lcv = 0; lcv < REPEATS; lcv++)
	{

		CPX_ACCUM caccuma[3];
		CPX_ACCUM caccumb[3];

		pts = rand() % VECTSIZE;

		fill_vect(testvecta, pts);

		fill_prn_new(testvectf, pts);
		fill_prn_new(testvectg, pts);
		fill_prn_new(testvecth, pts);

		x86_prn_accum_new(testvecta, testvectf, testvectg, testvecth, pts, &caccuma[0]);
		sse_prn_accum_new(testvecta, testvectf, testvectg, testvecth, pts, &caccumb[0]);


		for(lcv2 = 0; lcv2 < 3; lcv2++)
			if(caccuma[lcv2].i != caccumb[lcv2].i)
				err++;

		for(lcv2 = 0; lcv2 < 3; lcv2++)
			if(caccuma[lcv2].q != caccumb[lcv2].q)
				err++;

		if(err)
		{
			for(lcv2 = 0; lcv2 < 3; lcv2++)
				printf("%d.%d,%d.%d\n",caccuma[lcv2].i,caccuma[lcv2].q,caccumb[lcv2].i,caccumb[lcv2].q);
		}

	}
	if(err)
		printf("CPX PRN ACCUM NEW \t\tFAILED: %d\n",err);
	else
		printf("CPX PRN ACCUM NEW\t\tPASSED\n",err);
	/*----------------------------------------------------------------------------------------------*/












	delete [] testvecta;
	delete [] testvectb;
	delete [] testvectc;
	delete [] testvectd;
	delete [] testvecte;
	delete [] testvectf;
	delete [] testvectg;
	delete [] testvecth;

	return(1);

}
