#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

#include "color.h"

namespace Sudoku {

class SudokuBoard
{

public:
    SudokuBoard();
    virtual ~SudokuBoard();
    void print() const;
    void print(int x, int y) const;
    int getValue(int x, int y) const;
    int checkOptions(int x, int y);
    void printOptions(int x, int y) const;
    
private:
    SudokuBoard(const SudokuBoard& toBeCopied);
    SudokuBoard& operator=(const SudokuBoard& toBeCopied);
    void initBoard(int* srcboard);

    class Options {
    public:
        Options() {
            for(int i=0; i<9; i++) options[i] = true;
            val = 0;
        }
        void setValue(int val_) {
            if(val_ == 0) {
                for(int i=0; i<9; i++) options[i] = true;
            } else {
                for(int i=0; i<9; i++) options[i] = false;
                options[val_-1] = true;
            }
            val = val_;
        }
        int getValue() const {
            return val;
        }
        //returns value if known, 0 otherwise
        int remove(int val_) {
            if(val_ == 0) return 0;
            if(val != 0) return val;
            options[val_-1] = false;
            int numOptions = 0;
            int tempVal = 0;
            for(int i=0; i<9; i++) {
                if(options[i]) {
                    numOptions++;
                    tempVal = i+1;
                }
            }
            if(numOptions == 1) {
                val = tempVal;
            }
            return val;
        }
        void print() const {
            for(int i=0; i<9; i++) {
                if(options[i]) {
                    if(val !=0 && val == i+1) printf(ANSI_BGREEN);
                    else printf(ANSI_GREY);
                }
                else printf(ANSI_DARKGREY);
                printf("%d", i+1);
            }
            printf(ANSI_NORMAL"\n");
        }
    private:
        bool options[9];    //true if still possible
        int val;    //0 if not known, value otherwise
    };

    Options options[10][10];
};

}

#endif // SUDOKUBOARD_H

