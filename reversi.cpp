#include <iostream>
#include <vector>
#include <string>

  /////////////////////////////////////////////////////////////////////////
  //
  // Student Info
  // ------------
  //
  // Name : Kevin Chen
  // St.# : 301306316
  // Email: kca123@sfu.ca
  //
  //
  // Statement of Originality
  // ------------------------
  //
  // All the code and comments below are my own original work. For any non-
  // original work, I have provided citations in the comments with enough
  // detail so that someone can see the exact source and extent of the
  // borrowed work.
  //
  // In addition, I have not shared this work with anyone else, and I have
  // not seen solutions from other students, tutors, websites, books,
  // etc.
  //
  /////////////////////////////////////////////////////////////////////////

using namespace std;

struct point {
  int row;
  int col;
};

struct line {
  point start;
  point end;
};

//struct with vectors meant to store all possibilities for a given turn
struct possibleMoves {
  vector < int > possibleScores;
  vector < string > possibleMoves;
  //for any move, there is the possibility of multiple pieces being able
  //to move to a single end point
  vector < vector < line > > possibleChanges;
  bool canMove;

  void clear() {
    possibleScores.clear();
    possibleMoves.clear();
    possibleChanges.clear();
  }
};

bool operator == (point a, point b) {
  return (a.row == b.row && a.col == b.col);
}

string pointToString(point a) {
  string move = "";
  move.push_back(a.row + 1+'0');
  char cCol = a.col + 65;
  move.push_back(cCol);
  return move;
}

enum class Tile: char {
  player = 'X', computer = 'O', empty = ' '
};

//function to test whether a given change will go out of bounds for the game board
bool withinBounds(vector < vector < Tile > > board, int baseY, int baseX, int changeY = 0, int changeX = 0) {
  return ((baseY + changeY) < board.size() && (baseY + changeY) >= 0 &&
    (baseX + changeX) < board.at(1).size() && (baseX + changeX) >= 0);
}

class board {
  public:
    vector < vector < Tile > > gameBoard;
  int humScore;
  int aiScore;

  board(): gameBoard(8, vector < Tile > (8, Tile::empty)), humScore(0), aiScore(0) {}

  //function to create the starting conditions for reversi in case
  //player decides to have multiple games in one build
  void reinitializeBoard() {
    for (int i = 0; i < gameBoard.size(); i++) {
      for (int j = 0; j < gameBoard[i].size(); j++) {
        gameBoard[i][j] = Tile::empty;
      }
    }
    gameBoard[3][3] = Tile::player;
    gameBoard[3][4] = Tile::computer;
    gameBoard[4][3] = Tile::computer;
    gameBoard[4][4] = Tile::player;
  }

  //method to change pieces from player to computer or vice versa
  void update(possibleMoves moves, int index, int player) {
    if (!moves.canMove) {
      cout << "No moves can be made!" << endl;
      return;
    }
    for (int i = 0; i < moves.possibleChanges.at(index).size(); i++) {
      int startRow = moves.possibleChanges.at(index).at(i).start.row;
      int startCol = moves.possibleChanges.at(index).at(i).start.col;
      int currentRow = startRow;
      int currentCol = startCol;
      int endRow = moves.possibleChanges.at(index).at(i).end.row;
      int endCol = moves.possibleChanges.at(index).at(i).end.col;
      int changeX = 0;
      int changeY = 0;

      if (startRow > endRow) {
        changeY = -1;
      } else if (startRow < endRow) {
        changeY = 1;
      }
      if (startCol > endCol) {
        changeX = -1;
      } else if (startCol < endCol) {
        changeX = 1;
      }

      Tile changeTile;

      if (player) {
        changeTile = Tile::player;
      } else {
        changeTile = Tile::computer;
      }

      while (currentRow != endRow || currentCol != endCol) {
        currentRow += changeY;
        currentCol += changeX;
        gameBoard[currentRow][currentCol] = changeTile;
      }
    }
  }

  //method to printout the board and calculates the score of both player and ai
  void printout() {
    humScore = 0;
    aiScore = 0;
    cout << " ";
    for (int i = 'A'; i < 'I'; i++) {
      cout << " " << char(i);
    }
    cout << endl;
    for (int i = 0; i < gameBoard.size(); i++) {
      cout << i + 1;
      for (int j = 0; j < gameBoard[i].size(); j++) {
        cout << " " << char(gameBoard[i][j]);
        if (gameBoard[i][j] == Tile::player) {
          humScore++;
        } else if (gameBoard[i][j] == Tile::computer) {
          aiScore++;
        }
      }
      cout << endl;
    }
    cout << "Player score: " << humScore << endl;
    cout << "AI score: " << aiScore << endl;
  }
};

class player {
  public:
  virtual void findMoves(vector < vector < Tile > > board) = 0;
  virtual void getMove() = 0;
  virtual~player() {}
};

class Human_player: public player {
  public:
  //int to store the index of the players chosen move from a
  //vector storing all possible moves for a given turn
  int indexOfMove;
  possibleMoves playerMoves;

  Human_player(): indexOfMove(0) {
    playerMoves.canMove = true;
  }

    //method that will iterate through the entire board and checking all
    //neighbours of all player tiles and stores valid moves in a
    //possibleMoves struct
void findMoves(vector < vector < Tile > > board) {
    playerMoves.clear();
    for (int row = 0; row < board.size(); row++) {
      for (int col = 0; col < board.at(row).size(); col++) {
        if (board[row][col] == Tile::player) {
		  //scroll through all adjacent points to a player tile
          for (int y = 1; y > -2; y--) {
            for (int x = 1; x > -2; x--) {
              if (x == 0 && y == 0) continue;
              if (withinBounds(board, row, col, y, x)) {
                if (board[row + y][col + x] == Tile::computer) {
                  int currentScore = 0;
                  int currentY = row + y;
                  int currentX = col + x;
                  Tile currentTile = Tile::computer;
				  //continue going through line of computer tiles until we reach an empty tile
                  while (currentTile == Tile::computer && withinBounds(board, currentY, currentX, y, x)) {
                    currentScore++;
                    currentY += y;
                    currentX += x;
                    currentTile = board[currentY][currentX];
                  }
                  if (currentTile == Tile::empty) {
                    bool duplicate = false;

                    point startOfCheck = {row, col};
                    point endOfCheck = {currentY, currentX};
                    line startToEnd = {startOfCheck, endOfCheck};

                    string endPointToString = pointToString(endOfCheck);

                    vector < line > lineVector;
                    lineVector.push_back(startToEnd);
					//ensure that no duplicate moves are entered into the vector
                    for (int index = 0; index < playerMoves.possibleMoves.size(); index++) {
                      if (playerMoves.possibleMoves.at(index) == endPointToString) {
                        duplicate = true;
                        playerMoves.possibleScores.at(index) += currentScore;
                        playerMoves.possibleChanges.at(index).push_back(startToEnd);
                      }
                    }

                    if (!duplicate) {
                      playerMoves.possibleScores.push_back(currentScore);
                      playerMoves.possibleMoves.push_back(endPointToString);
                      playerMoves.possibleChanges.push_back(lineVector);
                    }

                  }
                }
              }
            }
          }
        }
      }
    }
  }

    void getMove() {
    if (playerMoves.possibleMoves.size() < 1) {
      playerMoves.canMove = false;
      return;
    }
    string input;
    bool valid = false;
    while (!valid) {
      cout << "Possible moves: ";
      for (int i = 0; i < playerMoves.possibleMoves.size(); i++) {
        cout << playerMoves.possibleMoves.at(i) << " ";
      }
      cout << endl;
      cout << "enter move:" << endl;
      cin >> input;
      if (input.size() != 2) {
    	  cout << input<< ": ";
        cout << "invalid input" << endl;
      } else {
        input.at(1) = toupper(input.at(1));
        for (int i = 0; i < playerMoves.possibleMoves.size(); i++) {
          if (input == playerMoves.possibleMoves.at(i)) {
            valid = true;
            indexOfMove = i;
          }
        }
        if (!valid) {
          cout << "invalid input" << endl;
        }
      }
    }
  }
};

class Computer_player: public player {
  public: int indexOfMove;
  possibleMoves computerMoves;
  Computer_player(): indexOfMove(0) {
    computerMoves.canMove = true;
  }

    //this method is essentially the same as the player findMoves except
    //this will find the possible moves for the AI
    void findMoves(vector < vector < Tile > > board) {
    computerMoves.clear();
    for (int row = 0; row < board.size(); row++) {
      for (int col = 0; col < board.at(row).size(); col++) {
        if (board[row][col] == Tile::computer) {
          for (int y = 1; y > -2; y--) {
            for (int x = 1; x > -2; x--) {
              if (x == 0 && y == 0) continue;
              if (withinBounds(board, row, col, y, x)) {
                if (board[row + y][col + x] == Tile::player) {

                  int currentScore = 0;
                  int currentY = row + y;
                  int currentX = col + x;
                  Tile currentTile = Tile::player;
                  while (currentTile == Tile::player && withinBounds(board, currentY, currentX, y, x)) {
                    currentScore++;
                    currentY += y;
                    currentX += x;
                    currentTile = board[currentY][currentX];
                  }
                  if (currentTile == Tile::empty) {

                    bool duplicate = false;

                    point startOfCheck = {row, col};
                    point endOfCheck = {currentY, currentX};
                    line startToEnd = {startOfCheck, endOfCheck};

                    string endPointToString = pointToString(endOfCheck);
                    vector < line > lineVector;
                    lineVector.push_back(startToEnd);

                    for (int index = 0; index < computerMoves.possibleMoves.size(); index++) {
                      if (computerMoves.possibleMoves.at(index) == endPointToString) {
                        duplicate = true;
                        computerMoves.possibleScores.at(index) += currentScore;
                        computerMoves.possibleChanges.at(index).push_back(startToEnd);
                      }
                    }

                    if (!duplicate) {
                      computerMoves.possibleScores.push_back(currentScore);
                      computerMoves.possibleMoves.push_back(endPointToString);
                      computerMoves.possibleChanges.push_back(lineVector);
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

    //this game has a simplistic ai which will choose the moves that yields the highest increase in score
    void getMove() {
    if (computerMoves.possibleMoves.size() < 1) {
      computerMoves.canMove = false;
      return;
    }
    int max = 0;
    int maxIndex = 0;
    for (int i = 0; i < computerMoves.possibleScores.size(); i++) {
      if (computerMoves.possibleScores.at(i) > max) {
        max = computerMoves.possibleScores.at(i);
        maxIndex = i;
      }
    }
    cout << endl;
    indexOfMove = maxIndex;
  }
};

//function that determines if the player wants to play another round
bool retry() {
  string playAgain;
  cout << "Game over! Play again? (Y/N)" << endl;
  while (true) {
    cin >> playAgain;
    if (playAgain.size() == 1) {
      if (toupper(playAgain.at(0)) == 'Y') {
        return true;
      } else if (toupper(playAgain.at(0)) == 'N') {
        return false;
      } else {
        cout << "Invalid input" << endl;
      }
    } else {
      cout << "Invalid input" << endl;
    }
  }
}

int main() {
  int human = 1;
  int computer = 0;

  bool stillPlay = true;
  board gameBoard;

  Human_player hum;
  Computer_player ai;

  //this while loop creates the conditions necessary for a new game to be played
  while (stillPlay) {
    hum.playerMoves.canMove = true;
    ai.computerMoves.canMove = true;
    gameBoard.reinitializeBoard();
    cout << "X is the player" << endl;
    cout << "O is the computer" << endl;
    cout << "Enter moves in the form of column,row i.e 6E" << endl;
    //actual game loop
    while (ai.computerMoves.canMove || hum.playerMoves.canMove) {
      cout << endl;
      if (ai.computerMoves.possibleMoves.size()) {
        cout << "AI moved to " << ai.computerMoves.possibleMoves.at(ai.indexOfMove) << endl;
      }
      cout << "Player's move" << endl;
      gameBoard.printout();
      hum.findMoves(gameBoard.gameBoard);
      hum.getMove();
      gameBoard.update(hum.playerMoves, hum.indexOfMove, human);
      cout << endl;
      if (hum.playerMoves.possibleMoves.size()) {
        cout << "You moved to " << hum.playerMoves.possibleMoves.at(hum.indexOfMove) << endl;
      }
      cout << "Computer's move" << endl;
      gameBoard.printout();
      ai.findMoves(gameBoard.gameBoard);
      ai.getMove();
      gameBoard.update(ai.computerMoves, ai.indexOfMove, computer);
    }
    if(gameBoard.humScore > gameBoard.aiScore){
        cout << endl << "Player wins!" << endl << endl;
    }else if (gameBoard.aiScore > gameBoard.humScore){
        cout << endl << "Computer wins!" << endl << endl;
    }else{
        cout << endl << "Tie!" << endl;
    }
    stillPlay = retry();
  }
}
