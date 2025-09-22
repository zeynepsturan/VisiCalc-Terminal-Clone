#include <fstream>
#include <sstream>
#include "../headers/filemanager.h"
#include "../headers/global.h"
using namespace std;

void FileManager::saveToFile(AnsiTerminal& terminal, vector<vector<Cell>>& table){
    ofstream file; //output file stream
    string filename=""; //stores the name of the file
    char c; //stores the read character

    //gets the file name
    terminal.printAtW(SHOWN_ROWS+6 , 0, " ", 50); //clears the line where filename is got
    terminal.printAt(SHOWN_ROWS+6, 0, "File name:"); //prints an indicator to the user
    terminal.printAtW(SHOWN_ROWS+7 , 0, " ", 50); //clears the line about the file operaions

    do{
        c=terminal.getSpecialKeyW(); //reads a character

        if(c==127 || c==27){ //if backspace or del is pressed
            if(filename!=""){ //if file name is not empty
                int size=filename.size(); //gets the size of the file name
                filename=filename.substr(0, size-1); //deletes the last character of the file name
            }
        }

        else if(c=='\n') break; //leaves the loop when enter is pressed

        else{
            filename+=c; //adds new character to the file name
        }

        terminal.printAtW(SHOWN_ROWS+6 , 12, " ", 50); //clears the line where filename is got
        terminal.printAt(SHOWN_ROWS + 6, 12, filename); //shows the filename input

    }while(true);

    //adds .csv if the user didn't include it
    if (filename.size()>4 && filename.substr(filename.size()-4)!=".csv"){
        filename+=".csv"; //if the file name doesn't end with .csv
    }
    filename= "../saves/"+filename; //adds saves folder to the beginning of the file
    //opening the file
    file.open(filename); //opens the file

    if(file.is_open()){

        for(auto &row:table){ //iterates through the rows

            bool firstCell=true; //we don't want to add a coma before the first cell

            for(auto &cell:row){ //iterates through the cells in a row
                
                if(!firstCell){
                    file<<","; //puts the seperators
                }

                firstCell=false; //first cell is added so flag is set to false now

                string cellValue=cell.getValue(); //gets the value of the cell
                file<<cellValue; //writes cells to the file
            }

            //adds newline for the next row
            file<<'\n';
        }
        terminal.printAtW(SHOWN_ROWS+6 , 0, " ", 50); //clears the line about the file operaions
        terminal.printAtW(SHOWN_ROWS+7 , 0, " ", 50); //clears the line about the file operaions
        terminal.printAt(SHOWN_ROWS + 6, 0, "File saved successfully"); //prints the success message
    }

    else{
        terminal.printAtW(SHOWN_ROWS+6 , 0, " ", 50); //clears the line about the file operaions
        terminal.printAtW(SHOWN_ROWS+7 , 0, " ", 50); //clears the line about the file operaions
        terminal.printAt(SHOWN_ROWS + 6, 0, "The file could not be opened. Remember that if "); //prints the warning message
        terminal.printAt(SHOWN_ROWS + 7, 0, "the saves folder doesn't exist, sheet won't be saved."); //prints the warning message
    }

    file.close(); //closes the file
}

void FileManager::loadFromFile(AnsiTerminal& terminal, vector<vector<Cell>>& table){
    ifstream file; //input file stream
    string filename=""; //stores the name of the file
    string holdvalue;
    int i=0, j=0; //stores the row and column indexes
    char c; //stores the read character

    //gets the file name
    terminal.printAtW(SHOWN_ROWS+6 , 0, " ", 50); //clears the line about the file operaions
    terminal.printAtW(SHOWN_ROWS+7 , 0, " ", 50); //clears the line about the file operaions
    terminal.printAt(SHOWN_ROWS+6, 0, "\033[32mFile name:\033[0m");

    do{ //reads the filename character by character
        c=terminal.getSpecialKeyW(); //reads a character

        if(c==127 || c==27){ //if backspace or del is pressed
            if(filename!=""){ //if file name is not empty
                int size=filename.size(); //gets the size of the file name
                filename=filename.substr(0, size-1); //deletes the last character of the file name
            }
        }

        else if(c=='\n') break; //leaves the loop when enter is pressed

        else{
            filename+=c; //adds new character to the file name
        }

        terminal.printAtW(SHOWN_ROWS+6 , 12, " ", 50); //clears the line where filename is inputted
        terminal.printAt(SHOWN_ROWS + 6, 12, filename); //shows the filename input

    }while(true);

    //adds .csv if the user didn't include it
    if (filename.size()>4 && filename.substr(filename.size()-4)!=".csv"){ //if the file name doesn't end with .csv
        filename+=".csv"; //adds .csv to the end of the file
    }
    filename= "../saves/"+filename;  //adds saves folder to the beginning of the file

    
    file.open(filename); //opens the file

    if(file.is_open()){
        
        while(getline(file,holdvalue) && i<MAX_ROWS){
            istringstream lineStream(holdvalue);
            string value; //stores the cell value that is got from the file
            j = 0; //j is reset when the loop moves to a new row

            while (getline(lineStream, value, ',') && j < MAX_COLUMNS){
                //sets the vector for rows
                if (table.size()<=i){
                    table.resize(i+1, vector<Cell>(table[0].size())); //resizes the rows
                }

                //sets the vector for columns
                if(table[i].size()<=j){
                    for(auto &row:table){ //iterates through the rows
                        row.resize(j+1, Cell()); //resizes the columns
                    }
                }

                if(value[0]=='=' && value!="="){ //if value starts with = but it's not only a =
                    table[i][j].setDtype(FUNCTION); //sets the data type to function
                }
                table[i][j].setValue(value); //assigns values to cells
                
                j++; //j increases
                
            }
            i++; //i increases
        }
        terminal.printAtW(SHOWN_ROWS+6 , 0, " ", 50); //clears the line about the file operaions
        terminal.printAt(SHOWN_ROWS + 6, 0, "Save loaded successfully. Use arrow keys to see"); //prints the success message
        file.close(); //closes the file
    }

    else{
        terminal.printAtW(SHOWN_ROWS+6 , 0, " ", 50); //clears the line about the file operaions
        terminal.printAt(SHOWN_ROWS+6,0,"The file is not found. Remember that if your file"); //prints the warning message
        terminal.printAt(SHOWN_ROWS + 7, 0, "is not in the saves folder, it won't be opened."); //prints the warning message
    }
}