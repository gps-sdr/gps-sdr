CC	= g++-4.2
ASM = g++-4.2
LINK= g++-4.2

CINCPATHFLAGS = -Iaccessories	\
				-Iacquisition	\
				-Iincludes		\
				-Imain			\
				-Iobjects		\
				-Isimd
				
VPATH		=   accessories:	\
				acquisition:	\
				includes:		\
				main:			\
				objects:		\
				simd:			
											
LDFLAGS	 = -O3 -lpthread -lncurses -m32
CFLAGS   = -O3 -m32 $(CINCPATHFLAGS)
ASMFLAGS = -masm=intel

HEADERS =   config.h		\
			defines.h 		\
			globals.h		\
			includes.h		\
			macros.h		\
			protos.h		\
			signal.h		\
			structs.h	

OBJS =		init.o			\
			shutdown.o		\
			misc.o			\
			fft.o			\
			cpuid.o			\
			sse.o			\
			x86.o			\
			fifo.o			\
			acquisition.o 	\
			keyboard.o 		\
			correlator.o 	\
			sv_select.o		\
			channel.o		\
			telemetry.o 	\
			ephemeris.o 	\
			pvt.o			\
			post_process.o
			
#Uncomment these to look at the disassembly
#DIS = 		x86.dis		\
#			sse.dis		\
#			fft.dis		\
#			acquisition.dis \
#			acq_test.dis 

EXE =		gps-sdr			\
			simd_test		
#			fft_test		\
#			acq_test		
			

All: $(EXE)

gps-sdr: main.o $(OBJS) $(DIS)
	 $(LINK) $(LDFLAGS) -o $@ main.o $(OBJS)

fft_test: fft_test.o $(OBJS)
	 $(LINK) $(LDFLAGS) -o $@ fft_test.o $(OBJS)

simd_test: simd_test.o $(OBJS)
	 $(LINK) $(LDFLAGS) -o $@ simd_test.o $(OBJS)
	 
acq_test: acq_test.o $(OBJS)
	 $(LINK) $(LDFLAGS) -o $@ acq_test.o $(OBJS)
	 
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

%.dis:%.cpp	
	$(CC) $(CFLAGS) -S $< -o $@ 
	
%.o:%.s
	$(ASM) $(CFLAGS) -c $< -o $@

clean: minclean exclean cleandoxy
	
minclean:
	@rm -rvf `find . \( -name "*.o" -o -name "*.exe" -o -name "*.dis" -o -name "*.dat" -o -name "*.out" -o -name "*.m~"  -o -name "*.tlm" \) -print`
	@rm -rvf `find . \( -name "fft_test" -o -name "acq_test" -o -name "usrp_sdr" \) -print`	
	@rm -rvf $(EXE)
	
exclean:	
	@rm -rvf `find . \( -name "*.txt" -o -name "*.png" \) -print`
		
doxy:
	doxygen ./documentation/Doxyfile	

cleanobj:
	@rm *.o

cleandoxy:
	rm -rvf ./documentation/html
	