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
											
LDFLAGS	 = -lpthread -lncurses -m32
CFLAGS   = -O2 -D_FORTIFY_SOURCE=0 $(CINCPATHFLAGS)
ASMFLAGS = -masm=intel

SKIP = %main.cpp %simd-test.cpp %fft-test.cpp %acq-test.cpp %sse_new.cpp
SRCC = $(wildcard main/*.cpp simd/*.cpp accessories/*.cpp acquisition/*.cpp objects/*.cpp)
SRC = $(filter-out $(SKIP), $(SRCC)) 
OBJS = $(SRC:.cpp=.o)
HEADERS = $(wildcard accessories/*.h acquisition/*.h main/*.h objects/*.h simd/*.h includes/*.h)			
			
#Uncomment these to look at the disassembly
#DIS = 		x86.dis		\
#			sse.dis		\
#			fft.dis		\
#			acquisition.dis \
#			acq_test.dis 

EXE =	gps-sdr		\
		simd-test

EXTRAS= gps-usrp	\
		gps-gui
		
TEST =	simd-test	\
		fft-test	\
		acq-test
		
all: $(EXE)

extras: $(EXTRAS)

test: $(TEST)

gui: gps-gui

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
	
gps-usrp:
	make --directory=./usrp
	
clean: minclean exclean cleandoxy
	
clean_o:
	@rm -rvf `find . \( -name "*.o" \) -print` 	
	
minclean:
	@rm -rvf `find . \( -name "*.o" -o -name "*.exe" -o -name "*.dis" -o -name "*.dat" -o -name "*.out" -o -name "*.m~"  -o -name "*.tlm" \) -print`
	@rm -rvf `find . \( -name "*.klm" -o -name "fft-test" -o -name "acq-test" -o -name "current.*" -o -name "usrp-gps" -o -name "gps-gui" -o -name "gps-usrp" \) -print`	
	@rm -rvf $(EXE)
	
exclean:	
	@rm -rvf `find . \( -name "*.txt" -o -name "*.png" \) -print`
		
doxy:
	doxygen ./documentation/Doxyfile	

cleanobj:
	@rm *.o

cleandoxy:
	rm -rvf ./documentation/html

install:
	- cp $(EXE) $(EXTRAS) /usr/local/bin/

