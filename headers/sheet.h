#ifndef SHEET_H
#define SHEET_H

#include <iostream>
#include <vector>
#include "cell.h"
#include "global.h"
#include "filemanager.h"
#include "functions.h"
#include "AnsiTerminal.h"
using namespace std;

class Sheet{
public:
    void moveCursor(char key); //to move cell to cell
    void runSheet(); //runs the program
    void LoadingScreen(); //prints the loading screen

    void printSheet(); //a container for the 3 functions below. prints the whole spreadsheet
    void printRows(); //prints row numbers
    void printColumns(); //prints column letters and the top three rows
    void printTable(); //prints the cell values

    Sheet(); //constructor
    ~Sheet(); //destructor
private:
    AnsiTerminal terminal;
    FileManager fileManager;
    Functions function;
    vector<vector<Cell>>table = vector<vector<Cell>>(SHOWN_ROWS, vector<Cell>(SHOWN_COLUMS));
    
    //these are helper functions only used within the class
    void cellReference(char &c, int& flag);
    void OneCellReference(char &c, int& flag);
    void deleteCharacter(); //deletes one character from the cell
    int ExitMenu(); //prnts the exit menu and lets user decide whether or not to exit

    const int column_width=9;

    int row; //row # in terminal grid
    int col; //column # in terminal grid

    int crow; //real row #
    int ccol; //real column #

    int h_shift; //horizontal shift
    int v_shift; //vertical sihft
};

#endif

