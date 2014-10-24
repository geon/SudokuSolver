//
//  main.c
//  SudokuSolver
//
//  Created by Victor Widell on 2014-10-19.
//  Copyright (c) 2014 Victor Widell. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

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


// If I put i in this cell, how many cells do I constrain?
int numConstrainedByMove(Board *board, int x, int y, int i) {
	
	int count = 0;
	
	// Row
	for (int vx = 0; vx < 9; ++vx) {
		
		if (vx == x) {
			
			continue;
		}
		
		if (board[y][vx] && 1<<i) {
			
			++count;
		}
	}
	
	// Collumn
	for (int vy = 0; vy < 9; ++vy) {
		
		if (vy == y) {
			
			continue;
		}
		
		if (board[vy][x] && 1<<i) {
			
			++count;
		}
	}
	
	// The rest of the tile
	for (int vy = 0; vy < 3; ++vy) {
		for (int vx = 0; vx < 3; ++vx) {
			
			if (vx == x || vy == y) {
				
				continue;
			}
			
			if (board[vy][vx] && 1<<i) {
				
				++count;
			}
		}
	}

	return 0;
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


int boardNumPossibilities (Possibilities p) {
	
	int count = 0;
	
	for (int i = 1; i <= 9; ++i) {
		
		if (1<<i & p) {
			
			++count;
		}
	}
	
	return count;
}


int constrainedValue (Possibilities p) {
	
	if (boardNumPossibilities(p) != 1) {
		
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
				
				printf(" .");
			}
		}
		
		printf("\n");
	}
}


void printNumPossibilities (Board board) {
	
	printf("\n");
	
	for (int y = 0; y < 9; ++y) {
		
		for (int x = 0; x < 9; ++x) {
			
			if (!board[y][x]) {
				
				printf(" %d", boardNumPossibilities(cellPossibilities(board, x, y)));
				
			} else {
				
				printf(" .");
			}
		}
		
		printf("\n");
	}
}


void printBoardPossibilities (Board board) {
	
	printf("\n");
	
	for (int y = 0; y < 9; ++y) {
		
		for (int x = 0; x < 9; ++x) {
			
			if (!board[y][x]) {
				
				Possibilities p = cellPossibilities(board, x, y);
				
				const int maxPosiibilities = 3;
				int numPossibilities = boardNumPossibilities(p);
				if (numPossibilities <= maxPosiibilities) {
					
					int numPrinted = 0;
					for (int i = 1; i <= 9; ++i) {
						
						if (1<<i & p) {
							
							printf("%d", i);
							++numPrinted;
							if (numPrinted >= maxPosiibilities) {
								
								break;
							}
						}
					}
					
					for (int i = 0; i < 3 - numPossibilities; ++i) {
						
						printf(" ");
					}
					
				} else {
					
					printf(".  ");
				}
								
			} else {
				
				printf("%d  ", board[y][x]);
			}
			
			printf(" ");
		}
		
		printf("\n\n");
	}
}


int setConstrainedCells (Board board) {

	int changes = 0;
	
	for (int y = 0; y < 9; ++y) {
		
		for (int x = 0; x < 9; ++x) {
			
			if (!board[y][x]) {
				
				Possibilities p = cellPossibilities(board, x, y);
				
				int constrained = constrainedValue(p);
				
				if (constrained) {
					
//					printf("\nConstrained x: %d, y: %d, to: %d", x, y, constrained);
					
					board[y][x] = constrained;
					
					++changes;
				}
			}
		}
	}
	
	return changes;
}


int isSolved (Board board) {

	for (int y = 0; y < 9; ++y) {
		
		for (int x = 0; x < 9; ++x) {
			
			if (!board[y][x]) {
				
				return 0;
			}
		}
	}
	
	return 1;
}


void copyBoard (Board from, Board to) {
	
	for (int y = 0; y < 9; ++y) {
		
		for (int x = 0; x < 9; ++x) {
			
			to[y][x] = from[y][x];
		}
	}
}


struct ConstraintsPerMove {
	
	int x;
	int y;
	int value;
	int numConstraints;
};

int compareConstraintsPerMove (const struct ConstraintsPerMove * a, const struct ConstraintsPerMove * b) {

	// DESC
	return
	b->numConstraints -
	a->numConstraints;
}


int solveBoard (Board board) {
	
	// Try to solve the board by constraints.
	while (setConstrainedCells(board));
	
	if(isSolved(board)) {
		
		return 1;
	}

	struct ConstraintsPerMove constraintsPerMove[9*9*9];
	
	// Underconstrained, so find all possible moves.
	int numFoundConstraints = 0;
	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {

			if (!board[y][x]) {

				Possibilities p = cellPossibilities(board, x, y);
				for (int i = 1; i <= 9; ++i) {

						
					if (p & 1<<i) {
											
						constraintsPerMove[numFoundConstraints] =
						(struct ConstraintsPerMove) {
							.x = x,
							.y = y,
							.value = i,
							.numConstraints =
							numConstrainedByMove(board, x, y, i)
						};
						
						++numFoundConstraints;
					}
				}
			}
		}
	}

	// Do the possible moves in the most constraining order.
	qsort(&constraintsPerMove,
		  numFoundConstraints,
		  sizeof(struct ConstraintsPerMove),
		  (int (*)(const void *, const void *)) compareConstraintsPerMove);
	
	for (int j = 0; j < numFoundConstraints; ++j) {

		int x = constraintsPerMove[j].x;
		int y = constraintsPerMove[j].y;
		int value = constraintsPerMove[j].value;
						
		// Use a copy.
		Board copy;
		copyBoard(board, copy);
		
		// Do the move.
		copy[y][x] = value;
		
		printBoard(copy);
		
		if (solveBoard(copy)) {
			
			// Found a complete solution, so use it.
			
			copyBoard(copy, board);
			return 1;
		}
	}
	
	// No solution found. :(
	return 0;
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
	
	Board hardest = {
		{8,0,0,  0,0,0,  0,0,0},
		{0,0,3,  6,0,0,  0,0,0},
		{0,7,0,  0,9,0,  2,0,0},
		
		{0,5,0,  0,0,7,  0,0,0},
		{0,0,0,  0,4,5,  7,0,0},
		{0,0,0,  1,0,0,  0,3,0},
		
		{0,0,1,  0,0,0,  0,6,8},
		{0,0,8,  5,0,0,  0,1,0},
		{0,9,0,  0,0,0,  4,0,0}
	};
	
	Board *board = &medium;

	printBoard(*board);
	solveBoard(*board);
	printBoard(*board);

	
	// printBoard(*board);
	// while (setConstrainedCells(*board));
	// printBoard(*board);
	// printBoardPossibilities(*board);
	// printNumPossibilities(*board);

	
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

