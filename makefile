# makefile
# updated: Mon Mar 21 23:06:50 PDT 2016

# files to compile
OBJECTS = main.c aed_functions.c

# compiler
CXX = gcc

# compiler flags
FLAGS = -ansi

# linker flags
LINKER_FLAGS = -lSDL2 -lm

#executable name
OUT = aed

all : $(OBJECTS)
	$(CXX) $(OBJECTS) $(FLAGS) $(LINKER_FLAGS) -o $(OUT)

clean :
	-rm $(OUT)


