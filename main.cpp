#include "connect4Header.h"

using namespace std;

// function declarations


// I'll be real and say this is just to avoid magic numbers
unsigned int EVALPLAYER1 = 1; // for PLAYER1 1
unsigned int EVALPLAYER2 = 2; // for PLAYER1 2
unsigned int EVALPLAYER3 = 3; // for PLAYER1 3
unsigned int PLAYER1 = EVALPLAYER1; // Player 1 default number
unsigned int PLAYER2 = EVALPLAYER2; // Player 2 default number
unsigned int MAX_DEPTH = 2; // the default "difficulty" of the computer controlled AI

bool gameOver = false; // flag for if game is over
unsigned int turns = 0; // count for # turns
unsigned int currPlayer = PLAYER1; // current PLAYER1

/**
 * game playing function
 * loops between PLAYER1s while they take turns
 */
void playGame() {
	//printBoard(gameBoard); // print initial board
	while (!gameOver) { // while no game over state
	    // Player 2 move
		if (currPlayer == PLAYER2)
		{
		    printf("Turn #%d for PLAYER %d is thinking about a move...", turns+1, PLAYER2);
			makeMove(gameBoard, aiMove(PLAYER2), PLAYER2);
		}
        // Player 1 move
		else if (currPlayer == PLAYER1)
        {
		    printf("Turn #%d for PLAYER %d is thinking about a move...", turns+1, PLAYER1);
			makeMove(gameBoard, aiMove(PLAYER1), PLAYER1);
		}
		else if (turns == INT_ROW * INT_COL) { // if max number of turns reached
			gameOver = true;
		}
		gameOver = evalPlayer2(gameBoard, currPlayer); // check if PLAYER1 won
		currPlayer = (currPlayer == PLAYER1) ? PLAYER2 : PLAYER1; // switch PLAYER1
		turns++; // iterate number of turns
		cout << endl;
		printBoard(gameBoard); // print board after successful move
	}
	if (turns == INT_ROW * INT_COL) { // if draw condition
		cout << "Draw!" << endl;
	}
	else { // otherwise, someone won
		printf("Player %d Wins!!!\n", ((currPlayer == PLAYER1) ? PLAYER2 : PLAYER1) );
	}
}

/**
 * function that makes the move for the PLAYER1
 * @param b - the board to make move on
 * @param c - column to drop piece into
 * @param p - the current PLAYER1
 */
void makeMove(vector<vector<int> >& b, int c, unsigned int p) {
	// start from bottom of board going up
	for (unsigned int r = 0; r < INT_ROW; r++) {
		if (b[r][c] == 0) { // first available spot
			b[r][c] = p; // set piece
			break;
		}
	}
}

/**
 * AI "think" algorithm
 * uses minimax to find ideal move
 * @return - the column number for best move
 */
int aiMove(unsigned int player) {
	return miniMax(gameBoard, MAX_DEPTH, 0 - INT_MAX, INT_MAX, player)[1];
}

/**
 * Minimax implementation using alpha-beta pruning
 * @param b - the board to perform MM on
 * @param d - the current depth
 * @param alf - alpha
 * @param bet - beta
 * @param p - current PLAYER1
 */
array<int, 2> miniMax(vector<vector<int> > &b, unsigned int d, int alf, int bet, unsigned int p) {
	/**
	 * if we've reached minimal depth allowed by the program
	 * we need to stop, so force it to return current values
	 * since a move will never (theoretically) get this deep,
	 * the column doesn't matter (-1) but we're more interested
	 * in the score
	 *
	 * as well, we need to take into consideration how many moves
	 * ie when the board is full
	 */
	if (d == 0 || d >= (INT_COL * INT_ROW) - turns) {
		// get current score to return
		return array<int, 2>{tabScore(b, PLAYER2), -1};
	}
	// if it's player 2
	if (p == PLAYER2)
    {
             // since maximizing, we want lowest possible value
        array<int, 2> moveSoFar = {INT_MIN, -1};
        // if Player 1 is about to win
        // force it to say it's worst possible score, so it knows to avoid move
        // otherwise, business as usual
		switch(p)
		{
            case 1:
                if (evalPlayer1(b, PLAYER1)){ return moveSoFar; }
            case 2:
                if (evalPlayer3(b, PLAYER1)){ return moveSoFar; }
		}
		for (unsigned int c = 0; c < INT_COL; c++)
        { // for each possible move
			if (b[INT_ROW - 1][c] == 0) { // but only if that column is non-full
				vector<vector<int> > newBoard = copyBoard(b); // make a copy of the board
				makeMove(newBoard, c, p); // try the move
				int score = miniMax(newBoard, d - 1, alf, bet, PLAYER1)[0]; // find move based on that new board state
				if (score > moveSoFar[0]) { // if better score, replace it, and consider that best move (for now)
					moveSoFar = {score, (int)c};
				}
				alf = max(alf, moveSoFar[0]);
				if (alf >= bet) { break; } // for pruning
			}
		}
		return moveSoFar; // return best possible move
	}
	else {
		array<int, 2> moveSoFar = {INT_MAX, -1}; // since PLAYER1 is minimized, we want moves that diminish this score
		// if Player 2 is about to win
        // force it to say it's worst possible score, so it knows to avoid move
        // otherwise, business as usual
		switch(p)
		{
            case 1:
                if (evalPlayer1(b, PLAYER2)){ return moveSoFar; }
            case 2:
                if (evalPlayer3(b, PLAYER2)){ return moveSoFar; }
		}
		for (unsigned int c = 0; c < INT_COL; c++) {
			if (b[INT_ROW - 1][c] == 0) {
				vector<vector<int> > newBoard = copyBoard(b);
				makeMove(newBoard, c, p);
				int score = miniMax(newBoard, d - 1, alf, bet, PLAYER2)[0];
				if (score < moveSoFar[0]) {
					moveSoFar = {score, (int)c};
				}
				bet = min(bet, moveSoFar[0]);
				if (alf >= bet) { break; }
			}
		}
		return moveSoFar;
	}
}

/**
 * function to tabulate current board "value"
 * @param b - the board to evaluate
 * @param p - the PLAYER1 to check score of
 * @return - the board score
 */
int tabScore(vector<vector<int> > b, unsigned int p) {
	int score = 0;
	vector<unsigned int> rs(INT_COL);
	vector<unsigned int> cs(INT_ROW);
	vector<unsigned int> set(4);
	/**
	 * horizontal checks, we're looking for sequences of 4
	 * containing any combination of AI, PLAYER1, and empty pieces
	 */
	for (unsigned int r = 0; r < INT_ROW; r++) {
		for (unsigned int c = 0; c < INT_COL; c++) {
			rs[c] = b[r][c]; // this is a distinct row alone
		}
		for (unsigned int c = 0; c < INT_COL - 3; c++) {
			for (int i = 0; i < 4; i++) {
				set[i] = rs[c + i]; // for each possible "set" of 4 spots from that row
			}
			score += scoreSet(set, p); // find score
		}
	}
	// vertical
	for (unsigned int c = 0; c < INT_COL; c++) {
		for (unsigned int r = 0; r < INT_ROW; r++) {
			cs[r] = b[r][c];
		}
		for (unsigned int r = 0; r < INT_ROW - 3; r++) {
			for (int i = 0; i < 4; i++) {
				set[i] = cs[r + i];
			}
			score += scoreSet(set, p);
		}
	}
	// diagonals
	for (unsigned int r = 0; r < INT_ROW - 3; r++) {
		for (unsigned int c = 0; c < INT_COL; c++) {
			rs[c] = b[r][c];
		}
		for (unsigned int c = 0; c < INT_COL - 3; c++) {
			for (int i = 0; i < 4; i++) {
				set[i] = b[r + i][c + i];
			}
			score += scoreSet(set, p);
		}
	}
	for (unsigned int r = 0; r < INT_ROW - 3; r++) {
		for (unsigned int c = 0; c < INT_COL; c++) {
			rs[c] = b[r][c];
		}
		for (unsigned int c = 0; c < INT_COL - 3; c++) {
			for (int i = 0; i < 4; i++) {
				set[i] = b[r + 3 - i][c + i];
			}
			score += scoreSet(set, p);
		}
	}
	return score;
}

/**
 * function to find the score of a set of 4 spots
 * @param v - the row/column to check
 * @param p - the PLAYER1 to check against
 * @return - the score of the row/column
 */
int scoreSet(vector<unsigned int> v, unsigned int p) {
	unsigned int good = 0; // points in favor of p
	unsigned int bad = 0; // points against p
	unsigned int empty = 0; // neutral spots
	for (unsigned int i = 0; i < v.size(); i++) { // just enumerate how many of each
		good += (v[i] == p) ? 1 : 0;
		bad += (v[i] == PLAYER1 || v[i] == PLAYER2) ? 1 : 0;
		empty += (v[i] == 0) ? 1 : 0;
	}
	// bad was calculated as (bad + good), so remove good
	bad -= good;
	return heurFunction(good, bad, empty);
}

/**
 * my """heuristic function""" is pretty bad, but it seems to work
 * it scores 2s in a row and 3s in a row
 * @param g - good points
 * @param b - bad points
 * @param z - empty spots
 * @return - the score as tabulated
 */
int heurFunction(unsigned int g, unsigned int b, unsigned int z) {
	int score = 0;
	if (g == 4) { score += 500001; } // preference to go for winning move vs. block
	else if (g == 3 && z == 1) { score += 5000; }
	else if (g == 2 && z == 2) { score += 500; }
	else if (b == 2 && z == 2) { score -= 501; } // preference to block
	else if (b == 3 && z == 1) { score -= 5001; } // preference to block
	else if (b == 4) { score -= 500000; }
	return score;
}

bool evalPlayer1(vector<vector<int> > &board, int player)
{
    // Check column locations for win
    for (int i = 0; i < (int)INT_COL-3; i++)
    {
        for (int j=0; j < (int)INT_ROW; j++)
        {
            if (board[j][i]== player && board[j][i+1]== player && board[j][i+2]== player && board[j][i+3]== player)
                return true;
        }
    }
     // Check row locations for win
    for (int i = 0; i < (int)INT_COL; i++)
    {
        for (int j=0; j < (int)INT_ROW-3; j++)
        {
            if (board[j][i]== player && board[j+1][i]== player && board[j+2][i]== player && board[j+3][i]== player)
                return true;
        }
    }
     // Check negative diagonal locations for win
    for (int i = 0; i < (int)INT_COL-3; i++)
    {
        for (int j=3; j < (int)INT_ROW; j++)
        {
            if (board[j][i]== player && board[j-1][i+1]== player && board[j-2][i+2]== player && board[j-3][i+3]== player)
                return true;
        }
    }
     // Check positive diagonal locations for win
    for (int i = 0; i < (int)INT_COL-3; i++)
    {
        for (int j=0; j < (int)INT_ROW-3; j++)
        {
            if (board[j][i]== player && board[j+1][i+1]== player && board[j+2][i+2]== player && board[j+3][i+3]== player)
                return true;
        }
    }
    return false;
}

/**
 * function to determine if a winning move is made
 * @param b - the board to check
 * @param p - the player to check against
 * @return - whether that player can have a winning move
 */
bool evalPlayer2(vector<vector<int> > &b, unsigned int p) {
	unsigned int winSequence = 0; // to count adjacent pieces
	// for horizontal checks
	for (unsigned int c = 0; c < INT_COL - 3; c++) { // for each column
		for (unsigned int r = 0; r < INT_ROW; r++) { // each row
			for (int i = 0; i < 4; i++) { // recall you need 4 to win
				if ((unsigned int)b[r][c + i] == p) { // if not all pieces match
					winSequence++; // add sequence count
				}
				if (winSequence == 4) { return true; } // if 4 in row
			}
			winSequence = 0; // reset counter
		}
	}
	// vertical checks
	for (unsigned int c = 0; c < INT_COL; c++) {
		for (unsigned int r = 0; r < INT_ROW - 3; r++) {
			for (int i = 0; i < 4; i++) {
				if ((unsigned int)b[r + i][c] == p) {
					winSequence++;
				}
				if (winSequence == 4) { return true; }
			}
			winSequence = 0;
		}
	}
	// the below two are diagonal checks
	for (unsigned int c = 0; c < INT_COL - 3; c++) {
		for (unsigned int r = 3; r < INT_ROW; r++) {
			for (int i = 0; i < 4; i++) {
				if ((unsigned int)b[r - i][c + i] == p) {
					winSequence++;
				}
				if (winSequence == 4) { return true; }
			}
			winSequence = 0;
		}
	}
	for (unsigned int c = 0; c < INT_COL - 3; c++) {
		for (unsigned int r = 0; r < INT_ROW - 3; r++) {
			for (int i = 0; i < 4; i++) {
				if ((unsigned int)b[r + i][c + i] == p) {
					winSequence++;
				}
				if (winSequence == 4) { return true; }
			}
			winSequence = 0;
		}
	}
	return false; // otherwise no winning move
}

bool evalPlayer3(vector<vector<int> > &b, unsigned int p){

    for (int i = 1; i < INT_ROW; i++) {
      for (int j = 1; j < INT_COL; j++) {
        if ((countPieces(i, j, i + 4, j, p) == 3 && countPieces(i, j, i + 4, j, 0) == 1) || (countPieces(i, j, i, j + 4, p) == 3 && countPieces(i, j, i, j + 4, 0) == 1) ||
          (countDiagonal(i, j, 0, p) == 3 && countDiagonal(i, j, 1, 0) == 1)) {
            return true;
        }

        else if ((countPieces(i, j, i + 4, j, p) == 2 && countPieces(i, j, i + 4, j, 0) == 2) || (countPieces(i, j, i, j + 4, p) == 2 && countPieces(i, j, i, j + 4, 0) == 2) ||
          (countDiagonal(i, j, 0, p) == 2 && countDiagonal(i, j, 1, 0) == 2)) {
            return true;
        }

        else if ((countPieces(i, j, i + 4, j, p) == 1 && countPieces(i, j, i + 4, j, 0) == 3) || (countPieces(i, j, i, j + 4, p) == 1 && countPieces(i, j, i, j + 4, 0) == 3) ||
          (countDiagonal(i, j, 0, p) == 1 && countDiagonal(i, j, 1, 0) == 3)) {
            return true;
        }

        else if ((countPieces(i, j, i + 4, j, PLAYER2) == 3 && countPieces(i, j, i + 4, j, 0) == 1) || (countPieces(i, j, i, j + 4, PLAYER2) == 3 && countPieces(i, j, i, j + 4, 0) == 1) ||
          (countDiagonal(i, j, 0, PLAYER2) == 3 && countDiagonal(i, j, 1, 0) == 1)) {
            return true;
        }

        else if ((countPieces(i, j, i + 4, j, PLAYER2) == 2 && countPieces(i, j, i + 4, j, 0) == 2) || (countPieces(i, j, i, j + 4, PLAYER2) == 2 && countPieces(i, j, i, j + 4, 0) == 2) ||
          (countDiagonal(i, j, 0, PLAYER2) == 2 && countDiagonal(i, j, 1, 0) == 2)) {
            return true;
        }

       else if ((countPieces(i, j, i + 4, j, PLAYER2) == 1 && countPieces(i, j, i + 4, j, 0) == 3) || (countPieces(i, j, i, j + 4, PLAYER2) == 1 && countPieces(i, j, i, j + 4, 0) == 3) ||
          (countDiagonal(i, j, 0, PLAYER2) == 1 && countDiagonal(i, j, 1, 0) == 3)) {
            return true;
        }
      }
    }
    return false;
}

int countPieces(int i, int j, int i2, int j2, unsigned int player){
    int pieces = 0;
    
    for(int i = i; i < i2; i++){
        for(int j = j; j < j2; j++){
            if(gameBoard[i][j] == player){
                pieces += 1;
            }
        }
    }
    return pieces;
}

int countDiagonal(int i, int j, int direction, unsigned int player){
    int pieces = 0;
    
    for(int x = 0; x < 4; x++){
        if(direction == 1){
            if((i + x < INT_ROW) && (j + x < INT_COL)){
                if(gameBoard[i + x][j + x] == player){
                    pieces += 1;
                }
            }
        }
        else{
            if((i + x < INT_ROW) && (j - x < INT_COL) && (j - x > 0)){
                if(gameBoard[i + x][j - x] == player){
                    pieces += 1;
                }
            }
        }
    }
    return pieces;
}

/**
 * sets up the board to be filled with empty spaces
 * also used to reset the board to this state
 */
void initBoard() {
	for (unsigned int r = 0; r < INT_ROW; r++) {
		for (unsigned int c = 0; c < INT_COL; c++) {
			gameBoard[r][c] = 0; // make sure board is empty initially
		}
	}
}

/**
 * function to copy board state to another 2D vector
 * ie. make a duplicate board; used for mutating copies rather
 * than the original
 * @param b - the board to copy
 * @return - said copy
 */
vector<vector<int> > copyBoard(vector<vector<int> > b) {
	vector<vector<int>> newBoard(INT_ROW, vector<int>(INT_COL));
	for (unsigned int r = 0; r < INT_ROW; r++) {
		for (unsigned int c = 0; c < INT_COL; c++) {
			newBoard[r][c] = b[r][c]; // just straight copy
		}
	}
	return newBoard;
}

/**
 * prints the board to console out
 * @param b - the board to print
 */
void printBoard(vector<vector<int> > &board) {
	cout << "-----------------------------" << endl;
	for ( int r = 0; r < (int)INT_ROW; r++) {
		for ( int c = 0; c < (int)INT_COL; c++) {
			cout << "| ";
			switch (board[INT_ROW - r - 1][c]) {
			case 0: cout << "  "; break; // no piece
			case 1: cout << "O "; break; // one player's piece
			case 2: cout << "X "; break; // other player's piece
			}
			if (c + 1 == (int)INT_COL)
            {
                cout << "| R" << 5-r+1;
            }
		}
		cout << endl;
	}
	cout << "-----------------------------" << endl;
    for ( int i = 0; i < (int)INT_COL; i++) {
		cout << "  C" << i+1;
	}
	cout << endl;
	cout << endl;
}

/**
 * handler for displaying error messages
 * @param t - the type of error to display
 */
void errorMessage(int t) {
	if (t == 1) { // non-int input
		cout << "Use a value 0.." << INT_COL - 1 << endl;
	}
	else if (t == 2) { // out of bounds
		cout << "That is not a valid column." << endl;
	}
	else if (t == 3) { // full column
		cout << "That column is full." << endl;
	}
	cout << endl;
}

/**
 * main driver
 */
int main(int argc, char** argv) {
	int i = -1; bool flag = false;
	if (argc == 2) {
		istringstream in(argv[1]);
		if (!(in >> i)) { flag = true; }
		if (i > (int)(INT_ROW * INT_COL) || i <= -1) { flag = true; }
		if (flag) { cout << "Invalid command line argument, using default depth = 5." << endl; }
		else { MAX_DEPTH = i; }
	}
	initBoard(); // initial setup
	playGame(); // begin the game
	return 0; // exit state
}
