CC	= g++
ASM = g++
LINK= g++

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
			signaldef.h		\
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
			simd-test		
#			fft-test		\
#			acq-test		
			

All: $(EXE)

gps-sdr: main.o $(OBJS) $(DIS) $(HEADERS)
	 $(LINK) $(LDFLAGS) -o $@ main.o $(OBJS)

simd-test: simd-test.o $(OBJS)
	 $(LINK) $(LDFLAGS) -o $@ simd-test.o $(OBJS)

fft-test: fft-test.o $(OBJS)
	 $(LINK) $(LDFLAGS) -o $@ fft-test.o $(OBJS)
	 
acq-test: acq-test.o $(OBJS)
	 $(LINK) $(LDFLAGS) -o $@ acq-test.o $(OBJS)
	 
%.o:%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ 

%.dis:%.cpp	
	$(CC) $(CFLAGS) -S $< -o $@ 
	
%.o:%.s
	$(ASM) $(CFLAGS) -c $< -o $@

gps-gui:
	make --directory=./gui
	
usrp-gps:
	make --directory=./usrp
	
clean: minclean exclean cleandoxy
	
minclean:
	@rm -rvf `find . \( -name "*.o" -o -name "*.exe" -o -name "*.dis" -o -name "*.dat" -o -name "*.out" -o -name "*.m~"  -o -name "*.tlm" \) -print`
	@rm -rvf `find . \( -name "*.klm" -o -name "fft_test" -o -name "acq_test" -o -name "current.*" -o -name "usrp-gps" \) -print`	
	@rm -rvf $(EXE)
	
exclean:	
	@rm -rvf `find . \( -name "*.txt" -o -name "*.png" \) -print`
		
doxy:
	doxygen ./documentation/Doxyfile	

cleanobj:
	@rm *.o

cleandoxy:
	rm -rvf ./documentation/html
	