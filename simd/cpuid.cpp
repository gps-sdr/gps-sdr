/*! \file CPUID.cpp
	Functions which probe the CPU to discover what SIMD functionality is present
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

#include "includes.h"

bool CPU_MMX()
{
	
	__asm
	(
		".intel_syntax noprefix	\n"
		"mov eax, 0x1			\n"	
		"cpuid					\n"			
		"sar edx, 0x17			\n"
		"and edx, 0x1			\n"
		"mov eax, edx			\n"
		"leave					\n"
		"ret					\n"
		".att_syntax			\n"
	);
	
}


bool CPU_SSE()
{

	__asm
	(
		".intel_syntax noprefix	\n"
		"mov eax, 0x1			\n"	
		"cpuid					\n"			
		"sar edx, 0x19			\n"
		"and edx, 0x1			\n"
		"mov eax, edx			\n"
		"leave					\n"
		"ret					\n"
		".att_syntax			\n"
	);

}


bool CPU_SSE2()
{
	
	__asm
	(
		".intel_syntax noprefix	\n"
		"mov eax, 0x1			\n"	
		"cpuid					\n"			
		"sar edx, 0x1A			\n"
		"and edx, 0x1			\n"
		"mov eax, edx			\n"
		"leave					\n"
		"ret					\n"
		".att_syntax			\n"
	);

}


bool CPU_SSE3()
{

	__asm
	(
		".intel_syntax noprefix \n"
		"mov eax, 0x1 			\n"
		"cpuid;					\n"
		"and ecx, 0x1 			\n"
		"mov eax, ecx 			\n"
		"leave					\n"
		"ret					\n"
		".att_syntax			\n"

	);

}

bool CPU_SSSE3()
{

	__asm
	(
		".intel_syntax noprefix \n"
		"mov eax, 0x1 			\n"
		"cpuid;					\n"
		"sar ecx, 9				\n"
		"and ecx, 0x1 			\n"
		"mov eax, ecx 			\n"
		"leave					\n"
		"ret					\n"
		".att_syntax			\n"

	);

}

bool CPU_SSE41()
{

	__asm
	(
		".intel_syntax noprefix \n"
		"mov eax, 0x1 			\n"
		"cpuid;					\n"
		"sar ecx, 19			\n"
		"and ecx, 0x1 			\n"
		"mov eax, ecx 			\n"
		"leave					\n"
		"ret					\n"
		".att_syntax			\n"

	);

}


bool CPU_SSE42()
{

	__asm
	(
		".intel_syntax noprefix \n"
		"mov eax, 0x1 			\n"
		"cpuid;					\n"
		"sar ecx, 20			\n"
		"and ecx, 0x1 			\n"
		"mov eax, ecx 			\n"
		"leave					\n"
		"ret					\n"
		".att_syntax			\n"

	);

}


void Init_SIMD()
{

//	if(CPU_SSE3())
//	{
//		simd_add = &sse_add;
//		simd_sub = &sse_sub;
//		simd_mul = &sse_mul;
//		simd_dot = &sse_dot;
//
//		simd_conj = &sse_conj;
//		simd_cacc = &sse_cacc;
//		simd_cmul = &sse_cmul;
//		simd_cmuls = &sse_cmuls;
//		simd_cmulsc = &sse_cmulsc;	
//	}
//	else
//	{
//		simd_add = &x86_add;
//		simd_sub = &x86_sub;
//		simd_mul = &x86_mul;
//		simd_dot = &x86_dot;
//
//		simd_conj = &x86_conj;
//		simd_cacc = &x86_cacc;
//		simd_cmul = &x86_cmul;
//		simd_cmuls = &x86_cmuls;
//		simd_cmulsc = &x86_cmulsc;
//		
//	}
//	
//	simd_cmag = &x86_cmag;
//	simd_max = &x86_max;
	

}

