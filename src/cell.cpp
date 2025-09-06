#include <iostream>
#include <cstring>
#include "../headers/cell.h"
using namespace std;

Cell::Cell(){
    value="";
    error=false;
    type=NONE;
    num=0.0;
}

void Cell::setValue(string text){
    value=text;
}

string Cell::getValue(){
    return value;
}

void Cell::setDtype(dtype t){
    type=t;
}

dtype Cell::getDtype(){
    return type;
}

void Cell::setNum(double d){
    num=d;
}

double Cell::getNum(){
    return num;
}

void Cell::setError(bool e){
    error=e;
}

bool Cell::getError(){
    return error;
}