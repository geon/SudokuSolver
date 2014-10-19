//
//  main.c
//  SudokuSolver
//
//  Created by Victor Widell on 2014-10-19.
//  Copyright (c) 2014 Victor Widell. All rights reserved.
//

#include <stdio.h>

typedef int Board[9][9];

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



int main(int argc, const char * argv[])
{


	
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
	
	
    return 0;
}

