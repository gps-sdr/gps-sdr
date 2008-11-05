CC	= g++
ASM = g++
LINK= g++

CINCPATHFLAGS = -I../accessories	\
				-I../acquisition	\
				-I../includes		\
				-I../main			\
				-I../objects		\
				-I../simd
				
VPATH		=   ../accessories:	\
				../acquisition:	\
				../includes:	\
				../main:		\
				../objects:		\
				../simd:			
									

WX_FLAGS = `wx-config --libs` 
LDFLAGS	 = -lpthread
CFLAGS   = `wx-config --cxxflags` $(CINCPATHFLAGS) 

HEADERS =   

SRC = gui.cpp gui_classes.cpp gui_toplevel.cpp gui_default.cpp

OBJ = $(SRC:.cpp=.o)

all: gui

gui: $(OBJ)
	 $(LINK) $(LDFLAGS) $(WX_FLAGS) $(OBJ) -o gps-gui
	 mv gps-gui ../gps-gui
	 
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	@rm -rvf `find . \( -name "*.o" -o -name "*.exe" -o -name "*.dis" -o -name "*.dat" -o -name "*.out" -o -name "*.m~"  -o -name "*.tlm" \) -print`
	@rm -rvf `find . \( -name "*.klm" -o -name "gui" \) -print`	
	@rm -rvf $(EXE)

