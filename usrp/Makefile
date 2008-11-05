GNURADIO		= $(HOME)/gnuradio
USRP_LIB_PATH	= $(GNURADIO)/usrp/host/lib
USRP_LIB_PATH2	= $(GNURADIO)/usrp/host/lib/.libs/
USRP_INCLUDES	= $(GNURADIO)/usrp/firmware/include

CC	= g++
ASM = g++
LINK= g++

CINCPATHFLAGS = -I$(USRP_INCLUDES) \
				-I$(USRP_LIB_PATH)

LDFLAGS	= -lpthread -L$(USRP_LIB_PATH) -L$(USRP_LIB_PATH2) -lusrp

CFLAGS = -O3 -D_FORTIFY_SOURCE=0 $(CINCPATHFLAGS)

HEADERS =   db_dbs_rx.h

OBJS =		db_dbs_rx.o

EXE =		gps-usrp

# All Target
all: $(EXE)

$(EXE): $(EXE).o $(OBJS)
	 $(LINK) $(LDFLAGS) -o $@ $(EXE).o $(OBJS)
	mv $(EXE) ../$(EXE)
	
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	@rm -rvf `find . \( -name "*.o" -o -name "*.exe" -o -name "*.dis" -o -name "*.dat" -name "gps-usrp" \) -print`
	@rm -rvf $(EXE)

	