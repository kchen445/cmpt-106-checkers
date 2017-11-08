#
# Generic Local Makefile v2
#
# Created by Jeremy Schwartz, 10/12/17
#

#                  #
# Compiler Options #
#                  #

# Compiler
CMPL = g++

# Compiler flags
CMPL_FLAGS = -std=c++14 -Wall -Wnon-virtual-dtor

# Archiver
ARC = ar

# Archiver flags
ARC_FLAGS = rvs



#                     #
# Source File Options #
#                     #

# Source file exension
#
# The language specific extension of the source files.
EXT = .cpp

# Output file extension
#
# The file extension of compiled code
OUT_EXT = .o

# Main source file
# 
# Must have a single file defined to create an executable.
# As this file will be excluded from the library only the
# int main function should be defined in this file.
MAIN = debug.cpp

# File to exclude from building
#
# Files listed here will not be built. Examples of files
# to add here would be incompatable code during debuging.
EXCLUDE = 



#                   #
# Directory Options #
#                   #

# Compiled binary output directory
#
# The directory where compiled code will be placed.
OUT_DIR = ./

# Depencency script path
#
# The path to the unix script for determining depenencies.
DEP_SCRIPT = ../scripts/dep.sh



#                #
# Output Options #
#                #

# Output archive
#
# The archive file to be built.
LIB = ../lib/mllib.a

# Output executable
#
# The executable to be built. The MAIN varabile must have
# a valid file as the entry point for the executable.
EXE = ../bin/mlexe

# Library dependencies
#
# Library files listed here will be linked with the executable.
# These libraries will not be linked with the archive.
LIB_DEP = 



########################
#    Implementation    #
#      DON'T EDIT      #
########################

# All the source files in this directory with the file
# extension as defined above.
SOURCES = $(filter-out $(MAIN) $(EXCLUDE), $(wildcard *$(EXT)))

# A list of object files to be created from the soucers.
OBJECTS = $(addprefix $(OUT_DIR), $(SOURCES:$(EXT)=$(OUT_EXT)))

# The object file for the main source file.
MAIN_OBJ = $(addprefix $(OUT_DIR), $(MAIN:$(EXT)=$(OUT_EXT)))

# Cleaning target.
# Removes all files with the compiled code extension as
# defined above in the variable OUT_EXT
clean:
	$(info Cleaning...)
	@rm -f $(wildcard *.gch)

# Executable build target.
exe: $(MAIN_OBJ)
	$(info Creating Executable...)
	@$(CMPL) -o $(EXE) $(MAIN_OBJ) $(LIB) $(LIB_DEP)

# Archive/library target.
# lib: $(OBJECTS)
# 	$(info Archiving...)
# 	@$(ARC) $(ARC_FLAGS) $(LIB) $(OBJECTS)

gch: TargetedLearning.hpp.gch CompetitiveLearning.hpp.gch

# Main source file target.
$(MAIN_OBJ): $(MAIN) TargetedLearning.hpp.gch CompetitiveLearning.hpp.gch
	$(info Compiling: $(MAIN)...)
	@$(CMPL) $(CMPL_FLAGS) -c $< -o $@

# Generic source file target.
# .SECONDEXPANSION:
# $(OUT_DIR)%$(OUT_EXT): %$(EXT) $$(shell $(DEP_SCRIPT) %$(EXT))
# 	@ if [ ! -d $(OUT_DIR) ]; then mkdir $(OUT_DIR); fi 
# 	$(CMPL) $(CMPL_FLAGS) -c $< -o $@;

#########################
# End of Implementation #
#########################

# Any custom targets may be defined below.

# Precompiled header targets
MLFramework.hpp.gch: $(filter-out $(wildcard *.hpp.gch), $(wildcard *.hpp))
	$(info Compiling header: MLFramework.hpp...)
	@$(CMPL) $(CMPL_FLAGS) MLFramework.hpp

TargetedLearning.hpp.gch: $(filter-out $(wildcard tl/*.hpp.gch), $(wildcard tl/*.hpp)) MLFramework.hpp.gch
	$(info Compiling header: TargetedLearning.hpp...)
	@$(CMPL) $(CMPL_FLAGS) TargetedLearning.hpp

CompetitiveLearning.hpp.gch: $(filter-out $(wildcard cl/*.hpp.gch), $(wildcard cl/*.hpp)) MLFramework.hpp.gch
	$(info Compiling header: CompetitiveLearning.hpp...)
	@$(CMPL) $(CMPL_FLAGS) CompetitiveLearning.hpp
