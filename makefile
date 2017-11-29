###################
# MASTER MAKEFILE #
###################

# Compiler
CMPL = g++

# Compiler flags
CMPL_FLAGS = -std=c++14 -Wall

# Call to make the dependency script executable.
setup:
	$(shell chmod a+x ./scripts/dep.sh)


# Archives #

all-lib: mllib gamelib

mllib:
	cd ml && $(MAKE) gch

gamelib:
	cd game && $(MAKE) lib
	
# Executables #

game: mllib gamelib
	$(CMPL) $(CMPL_FLAGS) -o bin/game game/main.cpp lib/gamelib.a

train: mllib gamelib
	$(CMPL) $(CMPL_FLAGS) -o bin/train ml/main.cpp lib/gamelib.a
	
# Cleaning #

# Removes all object (.o), archive (.a), and executable files from the project.
purge:
	$(info Preforming binary file purge...)
	$(info Removing object files...)
	@rm -f $(shell find . -name '*.o')
	$(info Removing archive files...)
	@rm -f $(shell find . -name '*.a')
	$(info Removing compiled headers...)
	@rm -f $(shell find . -name '*.gch')
	rm -f bin/game
	rm -f bin/train
	$(info Done)