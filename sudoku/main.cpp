#include <stdio.h>

#include "SudokuBoard.h"

using Sudoku::SudokuBoard;

int main(int argc, char* argv[])
{
    SudokuBoard board;
    bool colTodo[10];
    bool rowTodo[10];
    for(int i=1; i<10; i++) colTodo[i] = rowTodo[i] = true;

    board.print();
    printf("checking board\n");
    bool progress = true;
    while(progress) {
        progress = false;
        for(int col=1; col<10; col++) {
            if(colTodo[col]) {
                for(int row=1; row<10; row++) {
                    int val = board.checkOptions(col, row);
                    if(val) {
                        progress = true;
                        rowTodo[row] = true;
                        board.print(col, row);
                    }
                }
                colTodo[col] = false;
            }
        }
        for(int row=1; row<10; row++) {
            if(rowTodo[row]) {
                for(int col=1; col<10; col++) {
                    int val = board.checkOptions(col, row);
                    if(val) {
                        progress = true;
                        colTodo[col] = true;
                        board.print(col, row);
                    }
                }
                rowTodo[row] = false;
            }
        }
    }

    return 0;
}

