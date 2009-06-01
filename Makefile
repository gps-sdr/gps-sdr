CC	= g++
ASM = g++
LINK= g++

CINCPATHFLAGS = -Iaccessories	\
				-Iacquisition	\
				-Iincludes		\
				-Imain			\
				-Iobjects		\
				-Isimd			\
				-Iusrp
				
VPATH		=   accessories:	\
				acquisition:	\
				includes:		\
				main:			\
				objects:		\
				simd:			\
				usrp:			
											
LDFLAGS	 = -lpthread -lusrp -m32
CFLAGS   = -O2 -D_FORTIFY_SOURCE=0 -g3 $(CINCPATHFLAGS)
ASMFLAGS = -masm=intel

SKIP = %main.cpp %simd-test.cpp %fft-test.cpp %acq-test.cpp %sse_new.cpp %gps-usrp.cpp
SRCC = $(wildcard main/*.cpp simd/*.cpp accessories/*.cpp acquisition/*.cpp objects/*.cpp usrp/*.cpp)
SRC = $(filter-out $(SKIP), $(SRCC)) 
OBJS = $(SRC:.cpp=.o)
HEADERS = $(wildcard accessories/*.h acquisition/*.h main/*.h objects/*.h simd/*.h includes/*.h)			
			
#Uncomment these to look at the disassembly
#DIS = 		x86.dis		\

EXE =	gps-sdr		\
		gps-gse

EXTRAS= gps-usrp
		
TEST =	simd-test

all: $(EXE)
	@echo ---- Build Complete ----

extras:	extraclean $(EXTRAS)

test: testclean $(TEST)

gps-sdr: main.o $(OBJS) $(DIS) $(HEADERS)
	 $(LINK) $(LDFLAGS) -o $@ main.o $(OBJS)

simd-test: simd-test.o $(OBJS)
	 $(LINK) $(LDFLAGS) -o $@ simd-test.o $(OBJS)

%.o:%.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ 

%.dis:%.cpp	
	$(CC) $(CFLAGS) -S $< -o $@ 
	
%.o:%.s
	$(ASM) $(CFLAGS) -c $< -o $@

gps-gse:
	make --directory=./gse
	
gps-usrp:
	make --directory=./usrp
	
clean: distclean execlean testclean extraclean doxyclean

minclean: oclean
	
oclean:
	@rm -rvf `find . \( -name "*.o" \) -print` 	
	
distclean:
	@rm -rvf `find . \( -name "*.o" -o -name "*.dis" -o -name "*.dat" -o -name "*.klm" -o -name "*.m~" -o -name "*.tlm" -o -name "*.log" \) -print`
	
execlean:
	@rm -rvf $(EXE)
	
testclean:
	@rm -rvf $(TEST)
	
extraclean:
	@rm -rvf $(EXTRA)	
	
doxyclean:
	rm -rvf ./documentation/html

doxy:
	doxygen ./documentation/Doxyfile	

install:
	mkdir -p /usr/share/gps
	cp gps-sdr /usr/share/gps
	cp gps-gse /usr/share/gps
	ln -f -s /usr/share/gps/gps-sdr /usr/sbin/gps-sdr
	ln -f -s /usr/share/gps/gps-gse /usr/sbin/gps-gse


