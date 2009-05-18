CC	= g++
ASM = g++
LINK= g++

VPATH = include:src

INCPATH	=	-I. \
			-Iinclude \
			-I../includes \
			-I../objects

WX_FLAGS = `wx-config --libs` 
LDFLAGS	 = -lpthread
CFLAGS   = -D_FORTIFY_SOURCE=0 `wx-config --cxxflags` -g3 $(INCPATH) 

SRC = $(wildcard src/*.cpp)
SRC += gui_classes.cpp
OBJ = $(SRC:.cpp=.o)
HEADERS = $(wildcard include/*.h)
HEADERS += gui_classes.h

EXE = gps-gse

all: $(EXE)

$(EXE): $(OBJ) $(SRC) $(HEADERS)
	 $(LINK) $(LDFLAGS) $(WX_FLAGS) $(OBJ) -o $@
	 cp $(EXE) ../$(EXE)
	  
%.o : %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	@rm -rvf *.o
	@rm -rvf src/*.o
	@rm -rvf $(EXE)

test:
	echo $(SRC)
	echo $(OBJ)