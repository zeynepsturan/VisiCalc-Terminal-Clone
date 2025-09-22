#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <unistd.h>
#include "../headers/sheet.h"
#include "../headers/cell.h"
#include "../headers/global.h"
using namespace std;

//constructor
Sheet::Sheet(){
    terminal.clearScreen(); //clears the whole terminal screen
    h_shift=0; //the horizontal shift from the default row in pixels is 0
    v_shift=0; //the vertical shift from the default col in pixels is 0
    crow=1; //default cell is [1][1]
    ccol=1; //default cell is [1][1]
    row=5; //default row position in pixels
    col=4; //default column position in pixels
    srand(time(NULL)); //for randomization
    LoadingScreen(); //loading screen appears first
    printSheet(); //prints the sheet
}

//prints the loading screen
void Sheet::LoadingScreen(){
    vector<string> loadingtexts={ //stores the loading screen texts
        "[INIT] Loading kernel modules...",
        "[SYS] Checking spreadsheet engine...",
        "[OK] Memory check passed.",
        "[ERR] Missing optional plugin: chart.dll",
        "[INFO] Establishing secure session...",
        "[BOOT] Core functions initialized.",
        "[AUTH] Generating session token...",
        "[NET] Connecting to localhost:8080",
        "[DBG] Allocating cell cache...",
        "[OK] Spreadsheet grid created.",
        "[WARN] Deprecated function detected.",
        "[SYS] Running compatibility checks...",
        "[INFO] Enabling ANSI graphics mode...",
        "[INIT] User interface module loaded.",
        "[OK] Function library registered.",
        "[DBG] Parsing command line arguments...",
        "[INFO] Welcome user.",
        "[AUTH] Please enter username and password."};

        //prints the Visicalc text
        terminal.printAt(9, (2*column_width+4),"===========================================");
        terminal.printAt(10, (2*column_width+4),"VISICALC - Terminal Edition");
        terminal.printAt(11, (2*column_width+4),"by github.com/zeynepsturan");
        terminal.printAt(12, (2*column_width+4),"===========================================");

        int barLength=40; //length of the log bar in characters
        int logCount=3; //number of the log texts will be shown
        int logIndex=0; //number of the current shown log texts

        //prints the log bar
        for(int i=0; i<=barLength; i++){ //iterates through the bar
            string bar=string(i, '=') + string(barLength - i, ' '); //sets the loading bar
            terminal.printAt(15, (2*column_width+4), "[" + bar + "]"); //prints the bar

            //prints the log texts
            if(i%(barLength/logCount)==0 && logIndex<logCount){
                int num = rand() % loadingtexts.size(); //chooses a random log text
                terminal.printAt(13, (2*column_width+4), loadingtexts[num]); //log text is printed
                logIndex++; //logindex increased
            }

            usleep(18000); //waits
        }
        usleep(30000); //waits
    terminal.clearScreen(); //clears the screen
}

void Sheet::helpMenu(){
    terminal.clearScreen();
    int i=4;
    terminal.printAt(i++, (column_width+4), "=============================");
    terminal.printAt(i++, (column_width+4), " VisiCalc - Terminal Edition");
    terminal.printAt(i++, (column_width+4), "          HELP MENU");   
    terminal.printAt(i++, (column_width+4), "=============================");
    i++;
    terminal.printAt(i++, (column_width+4), "[ General Usage ]");
    terminal.printAt(i++, (column_width+4), "VisiCalc supports strings, numbers, arithmetic ");
    terminal.printAt(i++, (column_width+4),"operations(+ - / *) and functions.");
    i++;
    terminal.printAt(i++, (column_width+4), "[ Functions ]");
    terminal.printAt(i++, (column_width+4), "Reminder that to be able to use the functions");
    terminal.printAt(i++, (column_width+4), "you need to put @ first.");

    for(int j=0; j<validFunctions.size(); j++){
        terminal.printAt(i+j, (column_width+4), validFunctions[j]+"A1..C1");
    }
    i+=validFunctions.size();

    i++;
    terminal.printAt(i++, (column_width+4),"[ Tips ]");
    terminal.printAt(i++, (column_width+4), "- Formulas can reference other cells (e.g., =B2+C3)");
    terminal.printAt(i++, (column_width+4), "- Empty cells are treated as 0.0");
    terminal.printAt(i++, (column_width+4), "- Invalid functions return 0.0");
    terminal.printAt(i++, (column_width+4), "-----------------------");

    terminal.printAt(i++, (column_width+4), "==================================================");
    terminal.printAt(i++, (column_width+4), "Press any key to return to the spreadsheet...");
    char c=terminal.getSpecialKeyW(); //reads a character
}
//prints the rows
void Sheet::printRows(){
    for(int i=5;i<SHOWN_ROWS+5;i++){ //iterates through the rows
        terminal.printInvertedAtW(i,1,to_string(i-4+v_shift),3); //prints row numbers
    }
}

//prints the columns
void Sheet::printColumns(){

    for(int i=0; i<SHOWN_COLUMS; i++){ //iterates through the columns

        terminal.printInvertedAtW(1, (i*column_width) + column_width/2, " ", column_width); //1st row. prints it full green
        terminal.printInvertedAtW(2, (i*column_width) + column_width/2, " ", column_width); //2nd row. prints it full green
        terminal.printAtW(3, (i*column_width) + column_width/2, " ", column_width); //3rd row. prints it full blank
        terminal.printInvertedAtW(4 ,(i*column_width) + column_width/2," ",column_width); //row of column indicators. prints it full green
    }

    for(int i=1; i<SHOWN_COLUMS+1; i++){ //iterates through the columns
        terminal.printInvertedAt(4, (i*column_width) - 1, col_to_str(i+h_shift)); //prints the column letters centered  
    }
}

//prints the values to the terminal
void Sheet::printTable(){ 
    
    for(int i=0; i<SHOWN_ROWS; i++){ //iterates through the rows

        for(int j=0; j<SHOWN_COLUMS; j++){ //iterates through the columns

            if(table[i+v_shift][j+h_shift].getDtype()!=FUNCTION){ //if the cell isn't a function, simply prints the value of it
                terminal.printAtW(i+5, ((j)*column_width+4), table[i+v_shift][j+h_shift].getValue().substr(0, column_width), column_width); //prints the values (9 characters max)
            }

            else if(table[i+v_shift][j+h_shift].getDtype()==FUNCTION && table[i+v_shift][j+h_shift].getError()){ //if the cell is a function but there is an error in the function
                terminal.printAtW(i+5, ((j)*column_width+4), "ERROR", column_width); //prints the values (9 characters max)
            }

            else{ //cell is function so it calculates the result first and prints to the table
                function.handleFunction(table, i+v_shift,j+h_shift); //calculates the function
                double num=table[i+v_shift][j+h_shift].getNum(); //gets the calculated result as double

                if(num==static_cast<int>(num)){ //if the result is an intiger
                    int inum=num; //converts double num to integer inum
                    terminal.printAtW(i+5, ((j)*column_width+4),to_string(inum), column_width); //prints the result
                }

                else{
                    terminal.printAtW(i+5, ((j)*column_width+4),to_string(num), column_width); //prints the result
                }
            }
        }
    }

    terminal.printAt(3, 0, table[crow-1][ccol-1].getValue());//prints the whole value of the cell to the third row

    //initial cursor position in inverted mode
    if(table[crow-1+v_shift][ccol-1+h_shift].getDtype()!=FUNCTION){ //if the cell isn't a function, simply prints the value of it
                terminal.printInvertedAtW(crow+4, ((ccol-1)*column_width+4), table[crow-1+v_shift][ccol-1+h_shift].getValue().substr(0, column_width), column_width); //prints the values (9 characters max)
            }

            else if(table[crow-1+v_shift][ccol-1+h_shift].getDtype()==FUNCTION && table[crow-1+v_shift][ccol-1+h_shift].getError()){ //if the cell is a function but there is an error in the function
                terminal.printInvertedAtW(crow+4, ((ccol-1)*column_width+4), "ERROR", column_width); //prints the values (9 characters max)
            }

            else{ //cell is function so it calculates the result first and prints to the table
                function.handleFunction(table, crow-1+v_shift,ccol-1+h_shift); //calculates the function
                double num=table[crow-1+v_shift][ccol-1+h_shift].getNum(); //gets the calculated result as double

                if(num==static_cast<int>(num)){ //if the result is an intiger
                    int inum=num; //converts double num to integer inum
                    terminal.printInvertedAtW(crow+4, ((ccol-1)*column_width+4),to_string(inum), column_width); //prints the result
                }

                else{
                    terminal.printInvertedAtW(crow+4, ((ccol-1)*column_width+4),to_string(num), column_width); //prints the result
                }
            }

    terminal.printAtW(SHOWN_ROWS+6 , 0, " ", 50);
    terminal.printAtW(SHOWN_ROWS+7 , 0, " ", 50);
    terminal.printAtW(SHOWN_ROWS+8 , 0, " ", 50);
    terminal.printAt(SHOWN_ROWS+6,0, "CTRL + L to load a sheet"); //prints instructions for loading from a sheet to the bottom
    terminal.printAt(SHOWN_ROWS+7,0, "CTRL + S to save the sheet"); //prints instructions for saving to a sheet to the bottom
    terminal.printAt(SHOWN_ROWS+8,0, "CTRL + H to get help");
}

//container function to print the whole sheet
void Sheet::printSheet(){ 
    
    printColumns(); //prints the columns
    printRows(); //prints the rows
    printTable(); //prints the cells
    //prints the missing parts
    terminal.printInvertedAtW(1,1," ",8);
    terminal.printInvertedAtW(2,1," ",3);
    terminal.printInvertedAtW(4,1," ",3);
    terminal.printInvertedAt(1,1,col_to_str(ccol+h_shift)+to_string(crow+v_shift)); //prints the current cell coordinate at top left

}

//to move from cell to cell
void Sheet::moveCursor(char key){ 
    // Set the current position in normal print mode to show off as the previous cell
    if(table[crow-1+v_shift][ccol-1+h_shift].getDtype()==FUNCTION && !table[crow-1+v_shift][ccol-1+h_shift].getError()){ //if the cell is a function but there is an error in the function
        terminal.printAtW(row, col, "ERROR", column_width);
    }

    else if(table[crow-1+v_shift][ccol-1+h_shift].getDtype()==FUNCTION){ //if the cell is a function
        
        int num=table[crow-1+v_shift][ccol-1+h_shift].getNum(); //gets the number value

        if(floor(num)==num){ //if the result is an integer
            int inum=static_cast<int>(num); //converts double num to integer inum
            terminal.printAtW(row, col, to_string(inum), column_width); //prints the result
        }

        else{
            terminal.printAtW(row, col, to_string(num), column_width); //prints the result
        }
    }

    else{
        terminal.printAtW(row, col, table[crow-1+v_shift][ccol-1+h_shift].getValue().substr(0, column_width), column_width); //prints the current cell to normal text
        terminal.printInvertedAtW(1, 10," ",50); //clears the function area
    }

    // Update position based on arrow key input
    switch(key){
        case UP: //up 

            if(v_shift>0 && crow>1){ //if it's not at the top row
                row-=1; //row decreased
                crow--; //crow decreased
            }

            else if(v_shift>0 && crow==1){ //if there is shifting to top
                v_shift--; //v_shift decreased
                printRows(); //prints the updated rows
            }

            else if(row>5){ //if it's inside the borders
                row-=1; //row decreased
                crow--; //crow decreased
            }

            else{ //if cursor tries to reach out table limits and alert sound plays
                cout<<'\a';
            }
        break;

        case DOWN: //down

            if(crow<SHOWN_ROWS){ //if it's not at the bottom row
                row+=1; //row increased
                crow++; //crow increased
            }

            else if(crow+v_shift<MAX_ROWS){ //if there is shifting to bottom
                v_shift++; //v_shift increased
                
                if(crow+v_shift>table.size()){ //resizes the vector
                    table.resize(crow+v_shift,vector<Cell>(SHOWN_COLUMS+h_shift));
                }
                printRows(); //prints the updated rows
            }

            else{ //if cursor tries to reach out table limits and alert sound plays
                cout<<'\a';
            }

        break;

        case RIGHT: //right
            if(col<65){
                col+=column_width; //col increased
                ccol++; //ccol increased
            }

            else if(ccol+h_shift<MAX_COLUMNS){ //if there is shifting to right
                h_shift++; //h_shift increased

                if(ccol+h_shift>table[crow-1].size()){ //resizing the vector
                    for(auto& i:table){ //resizes the rows
                        i.resize(ccol+h_shift, Cell());
                    }
                }
                printColumns(); //prints the updated columns
            }

            else{ //if cursor tries to reach out table limits and alert sound plays
                cout<<'\a';
            }
        break;

        case LEFT: //left
            if(h_shift>0 && ccol>1){
                col-=column_width; //col decreased
                ccol--; //ccol decreased
                
            }

            else if(h_shift>0 && ccol==1){ //if there is shifting to left
                h_shift--; //h_shift decreased
                printColumns(); //prints the updated columns
            }

            else if(col>4){
                col-=column_width; //col decreased
                ccol--; //ccol decreased
                
            }

            else{ //if cursor tries to reach out table limits and alert sound plays
                cout<<'\a';
            }
        break;
    }
    terminal.printAt(SHOWN_ROWS+6,0, "CTRL + L key to load a sheet"); //prints instructions for loading from a sheet to the bottom
    terminal.printAt(SHOWN_ROWS+7,0, "CTRL + S to save the sheet"); //prints instructions for saving to a sheet to the bottom
    terminal.printAt(SHOWN_ROWS+8,0, "CTRL + H to get help");
    printTable(); //prints the updated table

    terminal.printAtW(3, 0, " ",column_width*SHOWN_COLUMS+3); //clears the third row
    terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row

    terminal.printInvertedAtW(1,1, " ",5); //clears the coordinate area
    terminal.printInvertedAt(1,1, col_to_str(ccol+h_shift)+to_string(crow+v_shift)); //prints the current cell coordinate at top left

    //Display inverted cursor at new position
    if(table[crow-1+v_shift][ccol-1+h_shift].getDtype()==FUNCTION && table[crow-1+v_shift][ccol-1+h_shift].getError()){
        terminal.printInvertedAtW(row, col, "ERROR", column_width);
    }

    else if(table[crow-1+v_shift][ccol-1+h_shift].getDtype()==FUNCTION){ //if the cell is a function
        
        terminal.printInvertedAt(1, 10,"(V)"); //prints the value sign
        terminal.printInvertedAt(1, 14,table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the function
        double num=table[crow-1+v_shift][ccol-1+h_shift].getNum(); //gets the number value

        if(floor(num)==num){ //if it's an integer
            int inum=static_cast<int>(num); //converts double num to integer inum
            terminal.printInvertedAtW(row, col, to_string(inum), column_width); //prints the integer value
        }

        else{
            terminal.printInvertedAtW(row, col, to_string(num), column_width); //prints the double value
        }
    }

    else{ //if it's a string
        terminal.printInvertedAtW(row, col, table[crow-1+v_shift][ccol-1+h_shift].getValue().substr(0, column_width), column_width);
        terminal.printInvertedAtW(1, 10," ",50); //clears the function area
    }
}

void Sheet::cellReference(char &c, int& flag){
    string colname=""; //stores the column name
    string rowname=""; //stores the row name

    if(c>='a' && c<='z'){ //makes the column letter uppercase
        c+='A'-'a';
    }
    colname += c; //adds the letter to the column name

    table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
    terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row

    while(c!='+' && c!='-' && c!='*' && c!='/' && c!=UP && c!=DOWN && c!=RIGHT && c!=LEFT){ //reads until the cell reference ends
        c=terminal.getSpecialKeyW(); //reads a character

        if(c==127 || c==27){ //if backspace or del is pressed

            if(rowname!=""){ //if colname is not empty
                int size=rowname.size(); //gets the size of the row name
                rowname=rowname.substr(0, size-1); //deletes the last character of the row name
            }

            else if(colname!=""){
                int size=colname.size(); //gets the size of the column name
                colname=colname.substr(0, size-1); //deletes the last character of the column name
            }
            
            int size =table[crow-1+v_shift][ccol-1+h_shift].getValue().size();
            terminal.printAtW(3, 0, " ",column_width*SHOWN_COLUMS);
            table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue().substr(0, size-1)); //adds the new character to end of the value
            terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row

            if(rowname=="" && colname==""){
                flag=1;
                return;
            }
        }

        if(isalpha(c)){ //2 letter column

            if(c>='a' && c<='z'){ //makes the column letter uppercase
                c+='A'-'a';
            }

            if(col_to_int((colname+c))-1<=MAX_COLUMNS){
                table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
                terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row
                colname+=c;
            }
        }

        if(isdigit(c)){
            rowname+=c;
            table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
            terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row
            c=terminal.getSpecialKeyW(); //reads a character

            while(isdigit(c)){
                string rn=rowname+c;
                
                if(stoi(rn)<=MAX_ROWS){
                    rowname+=c;
                    table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
                    terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row
                }
                c=terminal.getSpecialKeyW(); //reads a character
            }

            if(c==127 || c==27){ //if backspace or del is pressed

                if(rowname!=""){
                    int size=rowname.size();
                    rowname=rowname.substr(0, size-1);
                }
                
                int size =table[crow-1+v_shift][ccol-1+h_shift].getValue().size();
                terminal.printAtW(3, 0, " ",column_width*SHOWN_COLUMS);
                table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue().substr(0, size-1)); //adds the new character to end of the value
                terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row
            }
        }

        if(c=='+' || c=='-' || c=='*' || c=='/'){

            if(rowname==""){
                c='a';
                flag=0;
            }

            else{
                table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
                terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row
                flag=1;
            }
        }

        else if(c==UP || c==DOWN || c==RIGHT || c==LEFT){

            if(rowname==""){
                c='a';
                flag=0;
            }
            
            else{
                flag=1;
            }
        }
    }
}

void Sheet::OneCellReference(char &c, int& flag){
    string colname="";
    string rowname="";

    while(c!='.'){
        c=terminal.getSpecialKeyW();

        if(c==127 || c==27){ //if backspace or del is pressed

            if(rowname!=""){ //if colname is not empty
                int size=rowname.size(); //gets the size of the row name
                rowname=rowname.substr(0, size-1); //deletes the last character of the row name
            }

            else if(colname!=""){
                int size=colname.size(); //gets the size of the column name
                colname=colname.substr(0, size-1); //deletes the last character of the column name
            }
            
            int size =table[crow-1+v_shift][ccol-1+h_shift].getValue().size();
            terminal.printAtW(3, 0, " ",column_width*SHOWN_COLUMS);
            table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue().substr(0, size-1)); //adds the new character to end of the value
            terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row
        
            if(rowname=="" && colname==""){
                flag=1;
                return;
            }
        }

        else if(isalpha(c)){ //2 letter column

            if(c>='a' && c<='z'){
                c+='A'-'a';
            }

            if(col_to_int((colname+c))-1<=MAX_COLUMNS){
                table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
                terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row
                colname+=c;
            }
        }

        if(isdigit(c)){
            rowname+=c;
            table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
            terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row
            c=terminal.getSpecialKeyW(); //reads a character
            while(isdigit(c)){
                string rn=rowname+c;
                 
                if(stoi(rn)<=MAX_ROWS){
                    rowname+=c;
                    table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
                    terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row
                }
                c=terminal.getSpecialKeyW(); //reads a character
            }
        }

        else if(c=='.'){

            if(rowname==""){
                c='a';
                flag=0;
            }
            
            else{
                flag=1;
            }
        }
    }
}

//deletes one character from the cell
void Sheet::deleteCharacter(){
    int size=table[crow-1+v_shift][ccol-1+h_shift].getValue().size(); //gets the size of the current value in the current cell
    
    //if cell is not empty deletes the last character
    if(size>0){
        table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue().substr(0, size-1));
    }
    //prints the cell
    terminal.printInvertedAtW(row, col, table[crow-1+v_shift][ccol-1+h_shift].getValue().substr(0, column_width), column_width);
    terminal.printAtW(3, 0, " ",column_width*SHOWN_COLUMS); //clears the third row
    terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row
}

//shows the exit menu and lets user decide whether or not to exit
int Sheet::ExitMenu(){
    int flagLeave=0;
    while(1){ //while enter hasn't been pressed
        
        char key=terminal.getSpecialKeyW(); //reads input
    
        switch(key){
            case RIGHT: //Right means cursor on the YES button
                terminal.printAtW(13, (5*column_width+4), "No", column_width);
                terminal.printInvertedAtW(13, (6*column_width+4)+column_width, "Yes", column_width);
                flagLeave=1; //if user presses enter program will end

            break;

            case LEFT: //Left means cursor on the NO button
                terminal.printInvertedAtW(13, (5*column_width+4), "No", column_width);
                terminal.printAtW(13, (6*column_width+4)+column_width, "Yes", column_width);
                flagLeave=0; //if user presses enter program will continue

            break;

            case '\n': //Press Enter
                return flagLeave; //returns the decision
            break;
        }
    }
    return flagLeave; //returns the decision
}

//runs the program
void Sheet::runSheet(){
    char c; //reads from the terminal
    int flag=0; //prevents getting an input twice in a row
    int typeflag=0; //indicates the type of data for valid input

    while(1){

        if(flag==0) //if input hasn't got 
            c=terminal.getSpecialKeyW(); //gets the input
        
        if(c==8){ //when ctrl + H is pressed to see the help menu
            helpMenu();
            terminal.clearScreen();
            printSheet();
        }

        if(c==12){ //when ctrl + L is pressed to load a file
            fileManager.loadFromFile(terminal, table);
            flag=0;
        }

        if(c==17){ //when ctrl + Q is pressed to quit
            terminal.printInvertedAtW(10, (5*column_width+4), "Do you want to leave?",3*column_width);
            terminal.printInvertedAtW(11, (5*column_width+4),"If you didn't save,",3*column_width);
            terminal.printInvertedAtW(12, (5*column_width+4),"all the table will be lost.",3*column_width);
            terminal.printInvertedAtW(13, (5*column_width+4), "No", column_width);
            terminal.printAtW(13, (6*column_width+4)+column_width, "Yes", column_width);
            
            int flagLeave=ExitMenu();

            if(flagLeave){
                usleep(200000);
                return;
            }

            else{
                printTable();
            }
        }

        if(c==19){ //when ctrl + S is pressed to save 
            fileManager.saveToFile(terminal, table);
            flag=0;
        }
        
        if(c==UP || c==DOWN || c==RIGHT || c==LEFT){ //if arrow keys are entered
            terminal.printInvertedAtW(2,0," ",10);
            moveCursor(c);
            flag=0; //flag is set to 0 so it can get input
            typeflag=0;
        }
        
        else if(c==127 || c==27){ //if backspace or del is pressed
            deleteCharacter();
        }

        //Sheet
        else if(c=='='){

            table[crow-1+v_shift][ccol-1+h_shift].setValue(""); //clears the cells value
            terminal.printAtW(3, 0, " ",column_width*SHOWN_COLUMS); //clears the third row
            table[crow-1+v_shift][ccol-1+h_shift].setDtype(FUNCTION);

            //adds = to cell
            table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
            terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row

            while(c!=UP && c!=DOWN && c!=RIGHT && c!=LEFT){//gets input character by character

                c=terminal.getSpecialKeyW();

                if(c==127 || c==27){ //if backspace or del is pressed
                    deleteCharacter();
                    if(table[crow-1+v_shift][ccol-1+h_shift].getValue()==""){
                        table[crow-1+v_shift][ccol-1+h_shift].setDtype(NONE);
                        break;
                    }
                }
                
                //COMPLEX FUNCTIONS
                if(c=='@'){

                    string holdfunc="";

                    while(holdfunc!="SUM" && holdfunc!="AVER" && holdfunc!="MAX" && holdfunc!="MIN" && holdfunc!="STDDEV"){ //gets input character by character
                        c=terminal.getSpecialKeyW();

                        if(c==127 || c==27){ //if backspace or del is pressed
                            deleteCharacter();
                            if(holdfunc==""){
                                break;
                            }
                        }

                        else if(isalpha(c)){
                            
                            if(c>='a' && c<='z'){
                                c+='A'-'a';
                            }
                            
                            holdfunc+=c;
                            bool isValid=false;
                            
                            for(const auto& func:validFunctions){
                                if(func.substr(0, holdfunc.size())==holdfunc){ // Sadece eşleşen fonksiyonlar için kontrol
                                    isValid=true;
                                    table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue()+c);
                                    break;
                                }
                            }

                            if(isValid==false){
                                holdfunc.pop_back();
                            }

                            terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); // Display the valid function being typed
                            
                        }
                    }
                    //---------------------------------
                    OneCellReference(c,flag);

                    table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + ".."); //adds the new character to end of the value
                    terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row

                    c=terminal.getSpecialKeyW();
                    cellReference(c,flag);
                    //--------------------------------
                }

                //çalışıyo ama sonsuz eksi basılabiliyp
                else if(c=='-'){
                    table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
                    terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row
                }

                //----------------------
                //FOR CELL REFERENCE
                else if(isalpha(c)){
                    cellReference(c, flag); 
                }

                //FOR NUMBER REFERENCE
                else if(isdigit(c)){
                    
                    table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
                    terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row

                    while(c!='+' && c!='-' && c!='*' && c!='/' && c!=UP && c!=DOWN && c!=RIGHT && c!=LEFT){
                        c=terminal.getSpecialKeyW();

                        if(isdigit(c)){
                            table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
                            terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row
                            //number+=c;
                        }

                        if(c=='+' || c=='-' || c=='*' || c=='/'){
                            table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
                            terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row 
                        }

                        else if(c==UP || c==DOWN || c==RIGHT || c==LEFT){
                            flag=1;
                        }
                    }

                }
                //-----------------------------
            }
            string cellValue = table[crow-1+v_shift][ccol-1+h_shift].getValue();

            if (!cellValue.empty()){ 
                char endch=*(cellValue.end()-1);
                if(endch=='+' || endch =='-' || endch =='*' || endch=='/'){
                    cellValue.pop_back();
                    table[crow-1+v_shift][ccol-1+h_shift].setValue(cellValue);
                }
            }

            if(cellValue.empty() || cellValue=="="){
                table[crow-1+v_shift][ccol-1+h_shift].setDtype(NONE);
                table[crow-1+v_shift][ccol-1+h_shift].setValue(cellValue);
            }

            if(table[crow-1+v_shift][ccol-1+h_shift].getDtype()==FUNCTION){
                function.handleFunction(table, crow-1+v_shift,ccol-1+h_shift);
            }
        }

        //GETTING REGULAR INPUT
        else{
            table[crow-1+v_shift][ccol-1+h_shift].setValue(""); //clears the cells value
            terminal.printAtW(3, 0, " ",column_width*SHOWN_COLUMS); //clears the third row

            if(isdigit(c) || c=='.'){
                typeflag=1;
                table[crow-1+v_shift][ccol-1+h_shift].setDtype(VALUE);
            }

            else{
                table[crow-1+v_shift][ccol-1+h_shift].setDtype(LABEL);
            }

            while(c!=UP && c!=DOWN && c!=RIGHT && c!=LEFT){ //gets input character by character

                if(typeflag && isalpha(c)){
                    //does nothing
                }

                else if(c==127 || c==27){ //if backspace or del is pressed
                    deleteCharacter();
                }

                else{

                    table[crow-1+v_shift][ccol-1+h_shift].setValue(table[crow-1+v_shift][ccol-1+h_shift].getValue() + c); //adds the new character to end of the value
                    terminal.printInvertedAtW(row, col, table[crow-1+v_shift][ccol-1+h_shift].getValue().substr(0, column_width), column_width); //prints the current cell
                    terminal.printAt(3, 0, table[crow-1+v_shift][ccol-1+h_shift].getValue()); //prints the whole value of the cell to the third row

                    if(table[crow-1+v_shift][ccol-1+h_shift].getDtype()==VALUE){
                        terminal.printInvertedAt(2, 0, "Value");
                    }

                    else if(table[crow-1+v_shift][ccol-1+h_shift].getDtype()==LABEL){
                        terminal.printInvertedAt(2, 0, "Label");
                    }
                }
                c=terminal.getSpecialKeyW();
                
            }
            flag=1; //flag is set to 1 bc the input is already got and it's an arrow key
            typeflag=0;

            if(table[crow-1+v_shift][ccol-1+h_shift].getDtype()==VALUE && table[crow-1+v_shift][ccol-1+h_shift].getValue()!=""){
                table[crow-1+v_shift][ccol-1+h_shift].setNum(stod(table[crow-1+v_shift][ccol-1+h_shift].getValue()));
            }
            
        }
    }
}

//destructor
Sheet::~Sheet(){
    ofstream file;
    file.open("../saves/backup.csv");

    if(file.is_open()){

        for(auto &row:table){

            bool firstCell=true; //we don't want to add a coma before the first cell

            for(auto &cell:row){
                
                if(!firstCell){
                    file<<",";
                }

                firstCell=false;

                string cellValue=cell.getValue();
                file<<cellValue; //writes cells to the file
            }

            //adds newline for the next row
            file<<'\n';
        }
        terminal.printAtW(SHOWN_ROWS+6 , 0, " ", 50);
        terminal.printAt(SHOWN_ROWS + 6, 0, "File saved successfully");
    }
    
    terminal.clearScreen(); // clears the screen on exit
}