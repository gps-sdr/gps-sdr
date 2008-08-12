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
									

WX_FLAGS = `wx-config --libs` `wx-config --cxxflags`
LDFLAGS	 = -lpthread -lncurses
CFLAGS   = $(WX_FLAGS) $(CINCPATHFLAGS) 

HEADERS =   
OBJS =		gui.o

EXE =		gui		

All: $(EXE)

#gui: $(OBJS) $(DIS) $(HEADERS)
#	 $(LINK) $(LDFLAGS) -o $@ gui.o $(OBJS)

gui: gui.cpp
	 $(CC) $(CFLAGS) gui.cpp -o $@
	#mv gui ../gps-gui
	
#%.o:%.cpp $(HEADERS)
#	$(CC) $(CFLAGS) -c $< -o $@ 
	
clean:
	@rm -rvf `find . \( -name "*.o" -o -name "*.exe" -o -name "*.dis" -o -name "*.dat" -o -name "*.out" -o -name "*.m~"  -o -name "*.tlm" \) -print`
	@rm -rvf `find . \( -name "*.klm" -o -name "gui" \) -print`	
	@rm -rvf $(EXE)

