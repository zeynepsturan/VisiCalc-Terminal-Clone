#ifndef CELL_H
#define CELL_H

#include <string>
#include <vector>
#include <iostream>
using namespace std;

enum dtype{
    NONE,VALUE,LABEL,FUNCTION
};

//represents a cell in the sheet
class Cell{
public:
    Cell(); //constructor
    
    string getValue(); //gets the string value of the cell
    void setValue(string text); //sets the string value of the cell

    void setDtype(dtype t); //sets the data type to text, number or function 
    dtype getDtype(); //gets the data type

    void setNum(double d); //gets the numerical value of the cell
    double getNum(); //sets the numerical value of the cell
    
    void setError(bool e); //sets the error status
    bool getError(); //gets the error status

private:
    string value;
    enum dtype type;
    double num;
    bool error;
};

#endif