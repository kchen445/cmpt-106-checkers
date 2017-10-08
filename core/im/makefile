#
# Internal Messaging Library Makefile
#

# Compiler
CC = g++
CC_FLAGS = -std=c++14 -Wall

# Directories
OUT_DIR = ./out/

LIB = ../../lib/imlib.a
EXE = ../../bin/imexe

SCRIPT_DIR = ../../scripts/dep.sh

# File names
SOURCES = $(filter-out debug.cpp, $(wildcard *.cpp))
OBJECT_FILES = $(SOURCES:.cpp=.o)
OBJECTS = $(addprefix $(OUT_DIR), $(OBJECT_FILES))

# Debug Target
exe: ./out/debug.o lib
	$(CC) -o $(EXE) ./out/debug.o $(LIB)

# Library Target
# Does NOT compile debug.cpp
lib: $(OBJECTS)
	$(info Archiving...)
	@ar rvs $(LIB) $(OBJECTS)

# Debug object
./out/debug.o: debug.cpp $(OBJECTS)
	$(CC) $(CC_FLAGS) -c debug.cpp -o ./out/debug.o

# To build objects
.SECONDEXPANSION:
$(OUT_DIR)%.o: %.cpp $$(shell $(SCRIPT_DIR) %.cpp)
	@ if [ ! -d $(OUT_DIR) ]; then mkdir $(OUT_DIR); fi 
	$(CC) $(CC_FLAGS) -c $< -o $@;

# To remove object files
clean:
	$(info Cleaning...)
	@rm -f $(OBJECTS)
