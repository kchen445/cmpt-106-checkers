#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct point {
	int row;
	int col;
};

struct line {
	point start;
	point end;
};

//helper function to see where pieces should go
//pieces are positioned from top to bottom such that
//adding the row and column (0 indexed ) together of 
//the piece add up to an odd number
bool isOdd(int x){
	return ((x%2) == 1);
}

bool operator== (point a, point b) {
	return (a.row == b.row && a.col == b.col);
}

bool operator= (piece a, piece b){
	
}

string pointToString(point a) {
	string move = "";
	move.push_back(a.row + 1 +'0');
	char cCol = a.col + 65;
	move.push_back(cCol);
	return move;
}

enum class Tile: char {
	player1 = 'x', player2 = 'o', empty = ' ', player1King = 'X', player2King = 'O'
};

//function to test whether a given change will go out of bounds for the game board
bool withinBounds(vector < vector < Tile > > board, int baseY, int baseX, int changeY = 0, int changeX = 0) {
	return ((baseY + changeY) < board.size() && (baseY + changeY) >= 0 &&
		(baseX + changeX) < board.at(1).size() && (baseX + changeX) >= 0);
}

class piece{
	public:
	point position;
	bool isKing;
	//determines which player the piece belongs to
	int player;
	//board will be a 2d vector of checker objects
	//use this to determine whether a point on the board
	//has a proper piece or not
	bool isEmpty;
	
	piece(point initialPosition, int playerOrAI)
	:point(initialPosition), isKing(false), player(playerOrAI), isEmpty(false)
	{}
	piece(point initialPosition)
	:point(initialPosition), isKing(false), player(0), isEmpty(true)
	{}
	
	vector<line> findMove(vector<Tile> board);
}

class display{
	vector < vector<Tile> > displayBoard;
	
	display()
	:displayBoard(8, vector< Tile>(8,Tile::Empty) )
	{}
	
	displayBoard(vector <vector <pieces>> gameBoard);
	
	void displayGame();
}

class board {
	public:
	vector < vector < pieces > > gameBoard;

	board(): gameBoard(8, vector < piece > (8, 1) {}

	//function to create the starting conditions for checkers in case
	//player decides to have multiple games in one build
	void initializeBoard(vector <player> listOfPlayers){
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < gameBoard[i].size;j++){
					
					
				}
			}
		}
	}
	
	//update the board after a player's move is taken
	void update(player player);
};

class player {
	public:
	virtual void findMoves(vector < vector < Tile > > board) = 0;
	virtual void reinitializePlayer();
	virtual vector< pieces > getVectorOfPieces() = 0;
	virtual void findMoves();
	virtual void getMove() = 0;
	virtual~player() {}
};

class Human_player: public player {
	public:
	//int to store the index of the players chosen move from a
	//vector storing all possible moves for a given turn
	int player;
	int indexOfMove;
	bool canMove;
	vector< pieces > pieces;
	
	//inner vector refers to how a piece can jump multiple times
	//by taking more than 1 piece
	//outer vector is a list of all possible moves a player can make
	vector< vector < lines > > possibleMoves;
	
	
	Human_player(int whichPlayer)
	:player(whichPlayer), indexOfMove(0), canMove(true)
	{}
	//set up a player's piece vector
	void reinitializePlayer();
	
	vector< pieces > getVectorOfPieces();
	
	//find all possible moves by finding the possible moves
	//of each individual piece and add all of it to a vector
	void findMoves();

    void getMove() {}
};

class AI_player: public player{
	
}

//function that determines if the player wants to play another round
bool retry() {}

//get input to see if the game will be human vs human or human vs ai
//return string to prevent errors/complications
string findGameMode(){
	string gameMode;
	while (true){
		getline(cin, gameMode);
	}
} 

int main() {
	bool stillPlay = true;
	display display;
	board gameBoard;
	vector< players > listOfPlayers;
	cout << "1. Human Vs Human" << endl;
	cout << "2. Human Vs AI" << endl;
	cout << "Input your choice:" << endl;
	char gameMode = findGameMode().at(0);
	
	//add players to a vector of player objects
	//so it will be easier to change between a human vs human
	//and human vs ai mode
	if(gameMode = '1'){
		Human_player player1;
		Human_player player2;
		listOfPlayers.push_back(player1);
		listOfPlayers.push_back(player2);
	}else{
		Human_player player1;
		AI_player player2;
		listOfPlayers.push_back(player1);
		listOfPlayers.push_back(player2);
	}
	

  //this while loop creates the conditions necessary for a new game to be played
  while (stillPlay) {
    listOfPlayers[0].canMove = true;
    listOfPlayers[1].canMove = true;
    gameBoard.initializeBoard(listOfPlayers);
    cout << "X is player1" << endl;
    cout << "O is player2" << endl;
    cout << "Enter moves in the form of column,row i.e 6E" << endl;
    //actual game loop
    while (listOfPlayers[0].canMove || listOfPlayers[1].canMove) {
      cout << endl;
      if (listOfPlayers[1].possibleMoves.size()) {
        cout << "AI moved to " << listOfPlayers[1].possibleMoves.at(listOfPlayers[1].indexOfMove) << endl;
      }
      cout << "Player's move" << endl;
      display.DisplayGame();
      listOfPlayers[0].findMoves();
      listOfPlayers[0].getMove();
      gameBoard.update(listOfPlayers[0]);
      cout << endl;
      if (listOfPlayers[0].possibleMoves.size()) {
        cout << "You moved to " << listOfPlayers[0].possibleMoves.at(listOfPlayers[0].indexOfMove) << endl;
      }
      cout << "Computer's move" << endl;
      display.DisplayGame();
      listOfPlayers[1].findMoves();
      listOfPlayers[1].getMove();
      gameBoard.update(listOfPlayers[1]);
    }
    if(!listOfPlayers[1].canMove){
        cout << endl << "Player wins!" << endl << endl;
    }else if (!listOfPlayers[0].canMove){
        cout << endl << "Computer wins!" << endl << endl;
    }else{
        cout << endl << "Tie!" << endl;
    }
    stillPlay = retry();
  }
}
