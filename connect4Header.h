#ifndef CONNECT4HEADER_H_INCLUDED
#define CONNECT4HEADER_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <vector>
#include <limits.h>
#include <array>
#include <sstream>

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

void printBoard(std::vector<std::vector<int> >&);
void makeMove(std::vector<std::vector<int> >&, int, unsigned int);
void errorMessage(int);
int aiMove(unsigned int);
std::vector<std::vector<int> > copyBoard(std::vector<std::vector<int> >);
int scoreSet(std::vector<unsigned int>, unsigned int);
int tabScore(std::vector<std::vector<int> >, unsigned int);
std::array<int, 2> miniMax(std::vector<std::vector<int> >&, unsigned int, int, int, unsigned int);
int heurFunction(unsigned int, unsigned int, unsigned int);
bool evalPlayer1(std::vector<std::vector<int> >&, int);
bool evalPlayer2(std::vector<std::vector<int> >&, unsigned int);
bool evalPlayer3(std::vector<std::vector<int> >&, unsigned int);
int countPieces(int, int, int, int, unsigned int);
int countDiagonal(int, int, int, unsigned int);
unsigned int INT_COL = 7; // how wide is the board
unsigned int INT_ROW = 6; // how tall
std::vector<std::vector<int>> gameBoard(INT_ROW, std::vector<int>(INT_COL)); // the game board


#endif // CONNECT4HEADER_H_INCLUDED
