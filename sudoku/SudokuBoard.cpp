#include <stdio.h>
#include <assert.h>

#include "SudokuBoard.h"

using namespace Sudoku;


SudokuBoard::SudokuBoard()
{
    int initboard[9][9] = {
              {0,6,0,1,0,4,0,5,0},
              {0,0,8,3,0,5,6,0,0},
              {2,0,0,0,0,0,0,0,1},
              {8,0,0,4,0,7,0,0,6},
              {0,0,6,0,0,0,3,0,0},
              {7,0,0,9,0,1,0,0,4},
              {5,0,0,0,0,0,0,0,2},
              {0,0,7,2,0,6,9,0,0},
              {0,4,0,5,0,8,0,7,0} };
    initBoard(initboard[0]);
}


SudokuBoard::~SudokuBoard()
{
}


void SudokuBoard::print() const
{
    print(10,10);
}


void SudokuBoard::print(int x, int y) const
{
    for(int row=1; row<10; row++)
    {
        if((row-1) %3 == 0) printf("-------------------------------\n");
        for(int col=1; col<10; col++)
        {
            if((col-1)%3 == 0) printf("|");
            int val = options[col][10-row].getValue();
            if(val == 0) printf("   ");
            else
            {
                if( col==x && 10-row==y ) printf(ANSI_BGREEN);
                printf(" %d ", val);
                if( col==x && 10-row==y ) printf(ANSI_NORMAL);
            }
        }
        printf("|\n");
    }
    printf("-------------------------------\n");
}


int SudokuBoard::getValue(int x, int y) const
{
    assert(x <= 9);
    assert(y <= 9);
    return options[x][y].getValue();
}


void SudokuBoard::initBoard(int* srcboard)
{
    for(int row=0; row<9; row++)
    {
        for(int col=0; col<9; col++)
        {
            int val = *(srcboard + col*9 + row);
            options[row+1][col+1].setValue(val);
        }
    }
}


int SudokuBoard::checkOptions(int x, int y)
{
    int value = options[x][y].getValue();
    if(value!=0) return 0;  //already done
    //check row options
    for(int row=1; row<10; row++) {
        if(row==x) continue;
        value = options[row][y].getValue();
        options[x][y].remove(value);
    }
    //check col options
    for(int col=1; col<10; col++) {
        if(col==y) continue;
        value = options[x][col].getValue();
        options[x][y].remove(value);
    }
    //check quadrant
    int colstart = (((x-1)/3)*3)+1;
    int rowstart = (((y-1)/3)*3)+1;
    for(int col=colstart; col<colstart+3; col++) {
        for(int row=rowstart; row<rowstart+3; row++) {
            if(col==y && row==x) continue;
            value = options[col][row].getValue();
            options[x][y].remove(value);
        }
    }
    return options[x][y].getValue();
}


void SudokuBoard::printOptions(int x, int y) const
{
    printf("options (%d,%d): ", x, y);
    options[x][y].print();
}


