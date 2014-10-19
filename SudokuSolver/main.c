//
//  main.c
//  SudokuSolver
//
//  Created by Victor Widell on 2014-10-19.
//  Copyright (c) 2014 Victor Widell. All rights reserved.
//

#include <stdio.h>

typedef int Board[9][9];
typedef int Possibilities;


Possibilities rowPossibilities (Board board, int y) {
	
	Possibilities p = ~0;
	
	for (int x = 0; x < 9; ++x) {
		
		int value = board[y][x];
		
		if (value) {
			
			p &= ~(1<<value);
		}
	}
	
	return p;
}


Possibilities colPossibilities (Board board, int x) {
	
	Possibilities p = ~0;
	
	for (int y = 0; y < 9; ++y) {
		
		int value = board[y][x];
		
		if (value) {
			
			p &= ~(1<<value);
		}
	}
	
	return p;
}


Possibilities tilePossibilities (Board board, int tx, int ty) {
	
	Possibilities p = ~0;
	
	for (int y = 0; y < 3; ++y) {
		
		for (int x = 0; x < 3; ++x) {

			
			int value = board[ty*3 + y][tx*3 + x];
			
			if (value) {
				
				p &= ~(1<<value);
			}
		}
	}
	
	return p;
}


Possibilities cellPossibilities (Board board, int x, int y) {
	
	return
	rowPossibilities(board, y) &
	colPossibilities(board, x) &
	tilePossibilities(board, x/3, y/3);
}


void printPossibilities (Possibilities p) {

	int first = 1;
	
	for (int i = 1; i <= 9; ++i) {
		
		if (1<<i & p) {
		
			if (!first) {

				printf(", ");
			}
		
			first = 0;
			
			printf("%d", i);
		}
	}

	printf("\n");
}


void printBoard (Board board) {
	
	for (int y = 0; y < 9; ++y) {
		
		for (int x = 0; x < 9; ++x) {
			
			int cell = board[y][x];
			
			if (cell) {
				
				printf(" %d", board[y][x]);
				
			} else {
				
				printf("  ");
			}
		}
		
		printf("\n");
	}
}


int main (int argc, const char * argv[]) {
	
	int board[9][9] = {
		{0,0,1,  3,2,4,  0,0,0},
		{0,0,0,  0,0,6,  0,0,4},
		{9,2,0,  0,8,0,  3,0,6},

		{1,0,0,  2,0,8,  5,6,0},
		{8,0,0,  0,5,0,  0,0,1},
		{0,6,2,  9,0,1,  0,0,8},

		{3,0,9,  0,1,0,  0,7,2},
		{4,0,0,  8,0,0,  0,0,0},
		{0,0,0,  7,3,9,  4,0,0}
	};

	printBoard(board);
	
	printPossibilities(rowPossibilities(board, 0));
	printPossibilities(colPossibilities(board, 0));
	printPossibilities(tilePossibilities(board, 0, 0));
	printPossibilities(cellPossibilities(board, 0, 0));
	
    return 0;
}

