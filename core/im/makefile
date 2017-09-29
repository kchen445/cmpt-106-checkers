###############################################################################
# Makefile for building on Unix machines without an IDE
#
# Object files will be created in a 'out' directory within the current
# directory.
#
# Usage:
#   $ make all
#       Build the executable with name defined in EXEC.
#
#   $ make clean
#       Remove all .o files created during building.
#
# Note: This generic makefile is extreamly inefficient, having to build all
#       source files even if only a single file is changed.
###############################################################################

# Compiler
CC = g++
CC_FLAGS = -std=c++14 -Wall

# Directories
OUT_DIR = ./out/

# Output executable name
# Value may be safely changed.
EXEC = imexe 

# File names
SOURCES = $(wildcard *.cpp)
OBJECT_FILES = $(SOURCES:.cpp=.o)
OBJECTS = $(addprefix $(OUT_DIR), $(OBJECT_FILES))

# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

# Convenience build option
all:
	@make $(EXEC)

# To build objects
$(OUT_DIR)%.o: %.cpp
	@[[ -d $(OUT_DIR) ]] || mkdir $(OUT_DIR) 
	$(CC) $(CC_FLAGS) -c $< -o $@;

# To remove object files
clean:
	$(info Cleaning...)
	@rm -f $(OBJECTS)
