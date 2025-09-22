#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>
#include <list>
#include "../headers/cell.h"
#include "../headers/functions.h"
#include "../headers/global.h"
using namespace std;

void Functions::determineCells(vector<vector<Cell>>& table, string sc, string ec, int& startr, int& startc, int& endr, int& endc){

    string rownum=""; //stores the row number
    string colname=""; //stores the column name

    for(char i:sc){//stores the start cell's row and column info

        if(isalpha(i)){ //if a letter is read
            colname+=i;
        }

        else{ //if an integer is read
            rownum+=i;
        }
    }
    //converts row and column to intigers
    startr=stoi(rownum)-1;
    startc=col_to_int(colname)-1;

    rownum=""; //row number is cleared for the new cell
    colname=""; //column name is cleared for the new cell

    for(char i:ec){ //stores the end cell's row and column info

        if(isalpha(i)){ //if a letter is read
            colname+=i;
        }

        else{ //if an integer is read
            rownum+=i;
        }
    }

    //converts row and column to intigers
    endr=stoi(rownum)-1;
    endc=col_to_int(colname)-1;
}

double Functions::SUM(vector<vector<Cell>>& table, const string& startcell, const string& endcell){
    int startRow, startCol, endRow, endCol;
    double sum=0.0;

    //converts startcell and endcell's row column infos to intigers
    determineCells(table, startcell, endcell, startRow, startCol, endRow, endCol);

    if(startRow==endRow){ //if operation goes for the same row cells like SUMA8..Z8

        for(int i=startCol; i<=endCol; i++){
            sum+=table[startRow][i].getNum();
        }
    }

    else if(startCol==endCol){

        for(int i=startRow; i<=endRow; i++){
            sum+=table[i][startCol].getNum();
        }
    }

    return sum;
}

double Functions::AVER(vector<vector<Cell>>& table, const string& startcell, const string& endcell){
    int startRow, startCol, endRow, endCol;
    double sum=0.0;
    int count=0;

    determineCells(table, startcell, endcell, startRow, startCol, endRow, endCol);
    
    for(int i=startRow; i<=endRow; i++){
        for(int j=startCol; j<=endCol; j++){
            sum+=table[i][j].getNum();
            count++;
        }
    }
    return sum/count;
}

double Functions::MAX(vector<vector<Cell>>& table, const string& startcell, const string& endcell){
    int startRow, startCol, endRow, endCol;

    determineCells(table, startcell, endcell, startRow, startCol, endRow, endCol);
    int max=table[startRow][startCol].getNum();

    for(int i=startRow; i<=endRow; i++){
        for(int j=startCol; j<=endCol; j++){
            if(table[i][j].getNum()>max){
                max=table[i][j].getNum();
            }
        }
    }
    return max;
}

double Functions::MIN(vector<vector<Cell>>& table, const string& startcell, const string& endcell){
    int startRow, startCol, endRow, endCol;

    determineCells(table, startcell, endcell, startRow, startCol, endRow, endCol);
    int min=table[startRow][startCol].getNum();

    for(int i=startRow; i<=endRow; i++){
        for(int j=startCol; j<=endCol; j++){
            if(table[i][j].getNum()<min){
                min=table[i][j].getNum();
            }
        }
    }
    return min;
}

double Functions::STDDEV(vector<vector<Cell>>& table, const string& startcell, const string& endcell){
    double sum=0.0;
    int count=0;
    int startRow, startCol, endRow, endCol;

    determineCells(table, startcell, endcell, startRow, startCol, endRow, endCol);
    
    //sums the cells and counts the # of cells
    for(int i=startRow; i<=endRow; i++){
        for(int j=startCol; j<=endCol; j++){
            sum+=table[i][j].getNum();
            count++;
        }
    }

    //average
    double mean=0.0;
    
    
    if(count>1){ //if there are more than 1 values
        mean=sum/count;
    }

    else{
        return 0.0;
    }

    //sum of squares of difference
    double varianceSum=0.0;
    for(int i=startRow; i<=endRow; i++){
        for(int j=startCol; j<=endCol; j++) {
            double diff=table[i][j].getNum()-mean;
            varianceSum+=diff*diff;
        }
    }

    // squareroot of variance is the stddev
    if(count>1){ //if there are more than 1 values
       return sqrt(varianceSum/(count-1));
    }
    
    else{
        return 0.0;
    }
}

list<string> Functions::parseFunction(vector<vector<Cell>>& table, int r, int c){
    string func=table[r][c].getValue();
    list<string> funclist;
    char operation;//holds the next operator for next use

    //deletes the = sign first
    if(func[0]=='='){
        func=func.substr(1);
    }

    string token;
    size_t opPos; //holds the position of the operator

    while(!func.empty()){
        //finds the operation and splits the function
        opPos=func.find_first_of("+*/-");

        if(opPos!=string::npos){ //if the operation has found
            token=func.substr(0, opPos); //stores the part before the operation
            operation=func[opPos];
            func=func.substr(opPos+1); //updates the function
            funclist.push_back(token);
            funclist.push_back(string(1,operation));
        }

        else{ //if there is no operation
            token=func; //stores the rest of the function
            funclist.push_back(token);
            func="";
        }
    }
    return funclist;
}

void Functions::handleFunction(vector<vector<Cell>>& table, int r, int c){
    list<string> func=parseFunction(table, r, c);

    for(auto it = func.begin(); it != func.end(); ++it){

        if(!it->empty() && isalpha((*it)[0]) && (*it).rfind("SUM") && (*it).rfind("MAX") &&
        (*it).rfind("MIN") && (*it).rfind("AVER") && (*it).rfind("STDDEV")){ //cell reference
            string colname="";
            string rowname="";
            int i=0;

            //holds the column name
            while(i<(*it).size() && isalpha((*it)[i])){
                colname+=(*it)[i];
                i++;
            }

            //holds the row number
            while(i<(*it).size() && isdigit((*it)[i])){
                rowname+=(*it)[i];
                i++;
            }

            //converting row and column strings to integer
            int ref_row=stoi(rowname)-1;
            int ref_col=col_to_int(colname)-1;
            double value;

            if(ref_row>=table.size() || ref_col>=table[0].size()){ //if the memory for that cell is not allocated yet
                value=0.0;
            }

            else if(table[ref_row][ref_col].getValue()!=""){ //if the cell is emty

                if(table[ref_row][ref_col].getDtype()==VALUE){
                    value=stod(table[ref_row][ref_col].getValue());
                }

                /*------------potantial error----------*/
                else if(table[ref_row][ref_col].getDtype()==FUNCTION){
                    value=table[ref_row][ref_col].getNum();
                }
                /*---------------------------------------*/

                else{
                    value=0.0;
                }
            }

            else{
                value=0.0;
            }

            *it=to_string(value);
        }
    }

    for(auto it = func.begin(); it != func.end(); ++it){
        if((*it).find("SUM")==0){
            size_t dotdotPos=(*it).find("..");
            string startCell=(*it).substr(3, dotdotPos-3);
            string endCell=(*it).substr(dotdotPos + 2);
            double value=SUM(table, startCell, endCell);
            *it=to_string(value);
        }

        else if((*it).find("AVER")==0){
            size_t dotdotPos=(*it).find("..");
            string startCell=(*it).substr(4, dotdotPos-4);
            string endCell=(*it).substr(dotdotPos + 2);
            double value=AVER(table, startCell, endCell);
            *it=to_string(value);
        }

        else if((*it).find("MAX")==0){
            size_t dotdotPos=(*it).find("..");
            string startCell=(*it).substr(3, dotdotPos-3);
            string endCell=(*it).substr(dotdotPos + 2);
            double value=MAX(table, startCell, endCell);
            *it=to_string(value);
        }

        else if((*it).find("MIN")==0){
            size_t dotdotPos=(*it).find("..");
            string startCell=(*it).substr(3, dotdotPos-3);
            string endCell=(*it).substr(dotdotPos + 2);
            double value=MIN(table, startCell, endCell);
            *it=to_string(value);
        }

        else if((*it).find("STDDEV")==0){
            size_t dotdotPos=(*it).find("..");
            string startCell=(*it).substr(6, dotdotPos-6);
            string endCell=(*it).substr(dotdotPos + 2);
            double value=STDDEV(table, startCell, endCell);
            *it=to_string(value);
        }
    }
    

    for(auto it = func.begin(); it != func.end();){

        if(*it=="*" || *it=="/"){
            auto leftIt=prev(it);
            auto rightIt= next(it);
            double left= stod(*leftIt);
            double right= stod(*rightIt);

            if(*it=="/" && right==0){
            table[r][c].setError(true);
                return;
            }

            double result=(*it=="*") ? left*right : left/right;
            auto eraseStart = leftIt;
            auto eraseEnd = next(rightIt);
            it = func.erase(eraseStart, eraseEnd);
            it = func.insert(it, std::to_string(result));        
            
        }

        else{
            ++it;
        }
    }

    for(auto it = func.begin(); it != func.end();){

        if(*it=="+" || *it=="-"){
            auto leftIt=prev(it);
            auto rightIt= next(it);
            double left= stod(*leftIt);
            double right= stod(*rightIt);

            double result=(*it=="+") ? left+right : left-right;
            auto eraseStart = leftIt;
            auto eraseEnd = next(rightIt);
            it = func.erase(eraseStart, eraseEnd); 
            it = func.insert(it, std::to_string(result)); 
        }

        else{
            ++it;
        }
    }

    
    //saves the result
    table[r][c].setNum(stod(*(func.begin())));
}
