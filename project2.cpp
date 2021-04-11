#include <iostream>
using namespace std;

const int COLUMNS = 7;
const int ROWS = 6;

char board[ROWS][COLUMNS];

void initalizeBoard();
void displayBoard(char [ROWS][COLUMNS]);

int main(){

    initalizeBoard();
    displayBoard(board);
    
	return 0;
}

void initalizeBoard(){
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLUMNS; c++){
            board[r][c] = 0;
        }
    }
}

void displayBoard(char b[ROWS][COLUMNS]){
    
    for(int i = 1; i <= COLUMNS; i++){
        cout << " " << i;
    }
    cout << endl << "---------------" << endl;
    for(int r = 0; r < ROWS; r++){
        for(int c = 0; c < COLUMNS; c++){
            cout << "|";
            switch(board[ROWS - r - 1][c]){
                case 0: cout << " ";
                    break;
                case 1: cout << "O";
                    break;
                case 2: cout << "X";
                    break;
            }
            if(c + 1 == COLUMNS){
                cout << "|";
            }
        }
        cout << endl;
    }
    cout << "---------------" << endl << endl;
}
