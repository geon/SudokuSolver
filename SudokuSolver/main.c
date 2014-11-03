//
//  main.c
//  SudokuSolver
//
//  Created by Victor Widell on 2014-10-19.
//  Copyright (c) 2014 Victor Widell. All rights reserved.
//

#include <stdio.h>


typedef int Possibilities;

typedef struct {

	int cells[9][9];
	Possibilities rowPossibilities[9];
	Possibilities colPossibilities[9];
	Possibilities tilePossibilities[3][3];
} Board;



Board makeBoard () {
	
	Board board;
	
	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			
			board.cells[y][x] = 0;
		}
	}
	
	for (int i = 0; i < 9; ++i) {

		board.rowPossibilities[i] = ~0;
		board.colPossibilities[i] = ~0;
	}

	for (int y = 0; y < 3; ++y) {
		for (int x = 0; x < 3; ++x) {
			
			board.tilePossibilities[y][x] = ~0;
		}
	}
	

	return board;
}


void boardSetCell (Board *board, int x, int y, int newValue);


Board makeBoardWithInitialCells (int cells[9][9]) {
	
	Board board = makeBoard();
	
	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			
			boardSetCell(&board, x, y, cells[y][x]);
		}
	}
	
	return board;
}


Board makeBoardWithString (char cells[9*9]) {
	
	Board board = makeBoard();
	
	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			
			boardSetCell(&board, x, y, cells[y*9 + x] - '0');
		}
	}
	
	return board;
}


void boardSetCell (Board *board, int x, int y, int newValue) {
	
	Possibilities oldBit = (1<<(board->cells[y][x]));
	Possibilities newBitInverse = ~(1<<newValue);

	// Re-set the possibility bits of the old value.
	board->rowPossibilities[y] |= oldBit;
	board->colPossibilities[x] |= oldBit;
	board->tilePossibilities[y/3][x/3] |= oldBit;

	// Un-set the possibility bits of the new value.
	board->rowPossibilities[y] &= newBitInverse;
	board->colPossibilities[x] &= newBitInverse;
	board->tilePossibilities[y/3][x/3] &= newBitInverse;
	
	board->cells[y][x] = newValue;
}


Possibilities boardCellPossibilities (Board *board, int x, int y) {
	
	return
	board->rowPossibilities[y] &
	board->colPossibilities[x] &
	board->tilePossibilities[y/3][x/3];
}


int possibilitiesCount (Possibilities p) {
	
	int count = 0;
	
	for (int i = 1; i <= 9; ++i) {
		
		if (1<<i & p) {
			
			++count;
		}
	}
	
	return count;
}


int possibilitiesConstrainedValue (Possibilities p) {
	
	if (possibilitiesCount(p) != 1) {
		
		return 0;
	}
		
	for (int i = 1; i <= 9; ++i) {
		
		if (1<<i & p) {
			
			return i;
		}
	}

	return 0;
}


void possibilitiesPrint (Possibilities p) {

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


void boardPrint (Board *board) {
	
	printf("\n");

	for (int y = 0; y < 9; ++y) {
		
		for (int x = 0; x < 9; ++x) {
			
			int cell = board->cells[y][x];
			
			if (cell) {
				
				printf(" %d", board->cells[y][x]);
				
			} else {
				
				printf("  ");
			}
		}
		
		printf("\n");
	}
}


int boardCellsLeft (Board *board) {

	int count = 0;
	
	for (int y = 0; y < 9; ++y) {
		
		for (int x = 0; x < 9; ++x) {
			
			if (!board->cells[y][x]) {
				
				++count;
			}
		}
	}
	
	return count;
}


int boardSolve(Board *board, int cellsLeft) {

	if(!cellsLeft) {
		
		return 1;
	}

	// Try to solve the board by constraints.
	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			if (!board->cells[y][x]) {
				Possibilities p = boardCellPossibilities(board, x, y);
				int constrained = possibilitiesConstrainedValue(p);
				if (constrained) {
										
					// Do the move.
					boardSetCell(board, x, y, constrained);
					
//					printf("\nConstrained x: %d, y: %d, to: %d", x, y, constrained);
//					boardPrint(board);

					if (boardSolve(board, cellsLeft - 1)) {
						
						return 1;
						
					} else {
						
						// Backtrack.
						boardSetCell(board, x, y, 0);
//						printf("\nBacktracked constrained x: %d, y: %d, possible: %d", x, y, constrained);
						
						// Don't try the next cell, since it was already tried in the recursion above.
						return 0;
					}
				}
			}
		}
	}

//	printf("\nChecked all constrianed.");

	
	// Underconstrained, so try out all possible moves.
	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			if (!board->cells[y][x]) {
				Possibilities p = boardCellPossibilities(board, x, y);
				for (int i = 1; i <= 9; ++i) {
					if (1<<i & p) {
						
						// Do the move.
						boardSetCell(board, x, y, i);
						
//						printf("\nGuessed x: %d, y: %d, to: %d", x, y, i);
//						boardPrint(board);
						
						if (boardSolve(board, cellsLeft - 1)) {
							
							return 1;
						}

						// Backtrack.
						boardSetCell(board, x, y, 0);

//						printf("\nBacktracked guessed x: %d, y: %d", x, y);
						
						// Try the next possible value...
					}
				}
				
				// No possible move on this cell. Trying elsewhere won't help.
				return 0;
			}
		}
	}

//	printf("\nNo solution found. :( %d", cellsLeft);

	// No solution found. :(
	return 0;
}


int main (int argc, const char * argv[]) {

/*
	Board evil = makeBoardWithInitialCells((int[9][9]) {
		{2,0,0,  0,0,0,  4,0,0},
		{0,0,1,  0,0,7,  0,0,0},
		{0,0,0,  4,8,0,  5,0,2},
		
		{1,3,0,  0,4,0,  0,0,0},
		{0,0,8,  2,0,1,  3,0,0},
		{0,0,0,  0,3,0,  0,4,5},
		
		{6,0,7,  0,1,3,  0,0,0},
		{0,0,0,  6,0,0,  8,0,0},
		{0,0,4,  0,0,0,  0,0,1}
	});
*/

	
	int lineLength = 9*9+1;
	
	FILE *file = fopen("/Users/geon/Desktop/sudoku17 2.txt", "r");
	char line[lineLength];
	
	while (fread(line, 1, lineLength, file) == lineLength) {
		Board board = makeBoardWithString(line);
		boardPrint(&board);
		boardSolve(&board, boardCellsLeft(&board));
		boardPrint(&board);
	}
		
	
	//	int x = 6;
	//	int y = 5;
	//
	//	possibilitiesPrint(rowPossibilities(board, y));
	//	possibilitiesPrint(colPossibilities(board, x));
	//	possibilitiesPrint(tilePossibilities(board, x/3, y/3));
	
	//	Possibilities p = boardCellPossibilities(board, x, y);
	//
	//	possibilitiesPrint(p);
	//
	//	printf("%d\n", possibilitiesCount(p));
	//	printf("%d\n", possibilitiesConstrainedValue(p));
	
    
	return 0;
}

