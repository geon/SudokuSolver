
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


// TODO: Use set/unset, since it will always be set via 0.
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


// TODO: Use standard hamming weight function with bitmask.
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
					
					if (boardSolve(board, cellsLeft - 1)) {
						
						return 1;
						
					} else {
						
						// Backtrack.
						boardSetCell(board, x, y, 0);
						
						// Don't try the next cell, since it was already tried in the recursion above.
						return 0;
					}
				}
			}
		}
	}
	
	// Underconstrained, so try out all possible moves.
	for (int y = 0; y < 9; ++y) {
		for (int x = 0; x < 9; ++x) {
			if (!board->cells[y][x]) {
				Possibilities p = boardCellPossibilities(board, x, y);
				for (int i = 1; i <= 9; ++i) {
					if (1<<i & p) {
						
						// Do the move.
						boardSetCell(board, x, y, i);
						
						if (boardSolve(board, cellsLeft - 1)) {
							
							return 1;
						}

						// Backtrack.
						boardSetCell(board, x, y, 0);

						// Try the next possible value...
					}
				}
				
				// No possible move on this cell. Trying elsewhere won't help.
				return 0;
			}
		}
	}

	// No solution found. :(
	return 0;
}


int main (int argc, const char * argv[]) {

	int lineLength = 9*9+1;
	
	FILE *file = fopen("/Users/geon/Desktop/sudoku17 2.txt", "r");
	char line[lineLength];
	
	while (fread(line, 1, lineLength, file) == lineLength) {
		Board board = makeBoardWithString(line);
		boardPrint(&board);
		boardSolve(&board, boardCellsLeft(&board));
		boardPrint(&board);
	}
    
	return 0;
}

