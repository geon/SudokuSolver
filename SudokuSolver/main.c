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


int numPossibilities (Possibilities p) {
	
	int count = 0;
	
	for (int i = 1; i <= 9; ++i) {
		
		if (1<<i & p) {
			
			++count;
		}
	}
	
	return count;
}


int constrainedValue (Possibilities p) {
	
	if (numPossibilities(p) != 1) {
		
		return 0;
	}
		
	for (int i = 1; i <= 9; ++i) {
		
		if (1<<i & p) {
			
			return i;
		}
	}

	return 0;
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
	
	printf("\n");

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


int setConstrainedCells (Board board) {

	printf("\n");

	int changes = 0;
	
	for (int y = 0; y < 9; ++y) {
		
		for (int x = 0; x < 9; ++x) {
			
			if (!board[y][x]) {
				
				Possibilities p = cellPossibilities(board, x, y);
				
				int constrained = constrainedValue(p);
				
				if (constrained) {
					
					printf("Constrained x: %d, y: %d, to: %d\n", x, y, constrained);
					
					board[y][x] = constrained;
					
					++changes;
				}
			}
		}
	}
	
	return changes;
}


int main (int argc, const char * argv[]) {
	
	// Puzzles from www.websudoku.com
	
	Board easy = {
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
	
	Board medium = {
		{0,4,0,  1,2,0,  0,9,0},
		{0,0,3,  0,5,0,  0,0,0},
		{9,5,0,  8,6,0,  0,0,0},
		
		{8,0,9,  0,0,0,  0,0,3},
		{0,0,0,  3,0,6,  0,0,0},
		{6,0,0,  0,0,0,  5,0,2},
		
		{0,0,0,  0,3,8,  0,2,1},
		{0,0,0,  0,7,0,  6,0,0},
		{0,9,0,  0,1,5,  0,3,0}
	};
	
	Board hard = {
		{8,0,1,  0,3,6,  2,0,0},
		{2,0,0,  0,0,4,  0,9,0},
		{0,0,9,  0,0,2,  0,0,0},
		
		{0,1,0,  0,0,0,  0,0,2},
		{0,0,0,  6,0,8,  0,0,0},
		{6,0,0,  0,0,0,  0,7,0},
		
		{0,0,0,  8,0,0,  1,0,0},
		{0,2,0,  7,0,0,  0,0,4},
		{0,0,5,  2,4,0,  6,0,3}
	};
	

	Board evil = {
		{2,0,0,  0,0,0,  4,0,0},
		{0,0,1,  0,0,7,  0,0,0},
		{0,0,0,  4,8,0,  5,0,2},
		
		{1,3,0,  0,4,0,  0,0,0},
		{0,0,8,  2,0,1,  3,0,0},
		{0,0,0,  0,3,0,  0,4,5},
		
		{6,0,7,  0,1,3,  0,0,0},
		{0,0,0,  6,0,0,  8,0,0},
		{0,0,4,  0,0,0,  0,0,1}
	};
	
	Board *board = &hard;
	
	do {

		printBoard(*board);

	} while (setConstrainedCells(*board));

	
	//	int x = 6;
	//	int y = 5;
	//
	//	printPossibilities(rowPossibilities(board, y));
	//	printPossibilities(colPossibilities(board, x));
	//	printPossibilities(tilePossibilities(board, x/3, y/3));
	
	//	Possibilities p = cellPossibilities(board, x, y);
	//
	//	printPossibilities(p);
	//
	//	printf("%d\n", numPossibilities(p));
	//	printf("%d\n", constrainedValue(p));
	
    
	return 0;
}

