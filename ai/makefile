#
# Neural Framework Libray Makefile
#

# Compiler
CC = g++
CC_FLAGS = -std=c++14 -Wall

# Library Dependencies
LIB_DEP = ../lib/imlib.a

# Excluded Files
EXCLUDE = debug.cpp NNetwork.cpp

# Directories
OUT_DIR = ./out/

LIB = ../lib/ailib.a
EXE = ../bin/aiexe

# File names
SOURCES = $(filter-out $(EXCLUDE), $(wildcard *.cpp))
OBJECT_FILES = $(SOURCES:.cpp=.o)
OBJECTS = $(addprefix $(OUT_DIR), $(OBJECT_FILES))

# Debug Target
exe: ./out/debug.o lib
	$(CC) -o $(EXE) ./out/debug.o $(LIB) $(LIB_DEP)

# Library Target
# Does NOT compile debug.cpp
lib: $(OBJECTS)
	$(info Archiving...)
	@ar rvs $(LIB) $(OBJECTS)

# Debug object
./out/debug.o: debug.cpp $(OBJECTS)
	$(CC) $(CC_FLAGS) -c debug.cpp -o ./out/debug.o

# To build objects
$(OUT_DIR)%.o: %.cpp
	@ if [ ! -d $(OUT_DIR) ]; then mkdir $(OUT_DIR); fi 
	$(CC) $(CC_FLAGS) -c $< -o $@;

# To remove object files
clean:
	$(info Cleaning...)
	@rm -f $(OBJECTS)
