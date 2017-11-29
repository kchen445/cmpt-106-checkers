# Compiler
CMPL = g++

# Compiler flags
CMPL_FLAGS = -std=c++14 -Wall

# Archiver
ARC = ar

# Archiver flags
ARC_FLAGS = rvs

OUT_DIR = out

SRC = controller.cpp AIPlayer.cpp Display.cpp HumanPlayer.cpp Player.cpp Board.cpp Helpers.cpp Piece.cpp
OBJECTS = $(addprefix out/, $(SRC:.cpp=.o))

lib: $(OBJECTS) ../ml/singleton_def.o
	$(info $(OBJECTS))
	$(ARC) $(ARC_FLAGS) ../lib/gamelib.a $^

../ml/singleton_def.o: ../ml/singleton_def.cpp
	cd ../ml && $(CMPL) $(CMPL_FLAGS) -c singleton_def.cpp

# Generic source file target.
.SECONDEXPANSION:
out/%.o: %.cpp
	if [ ! -d $(OUT_DIR) ]; then mkdir $(OUT_DIR); fi 
	$(CMPL) $(CMPL_FLAGS) -c $< -o $@;
#$$(shell ../scripts/dep.sh %.cpp)