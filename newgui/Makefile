CC	= g++
ASM = g++
LINK= g++

CINCPATHFLAGS = -I../accessories	\
				-I../acquisition	\
				-I../includes		\
				-I../main			\
				-I../objects		\
				-I../simd
				
#VPATH		=   ../accessories:	\
#				../acquisition:	\
#				../includes:	\
#				../main:		\
#				../objects:		\
#				../simd:			
									

WX_FLAGS = `wx-config --libs` 
LDFLAGS	 = -lpthread
CFLAGS   = -O2 -D_FORTIFY_SOURCE=0 `wx-config --cxxflags` $(CINCPATHFLAGS) 

HEADERS =   

SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
HEADERS = $(wildcard *.h)

EXE = gps-gui

all: $(EXE)

$(EXE): $(OBJ) $(SRC) $(HEADERS)
	 $(LINK) $(LDFLAGS) $(WX_FLAGS) $(OBJ) -o $@
	 cp $(EXE) ../$(EXE)
	  
%.o : %.cpp $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	@rm -rv *.o
	@rm -rv $(EXE)

test:
	echo $(SRC)
	echo $(OBJ)