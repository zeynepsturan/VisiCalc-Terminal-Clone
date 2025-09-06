/*
  GLOBAL VARIABLES AND FUNCTIONS
  FOR THE VISICALC TERMINAL CLONE
  BY github.com/zeynepsturan
*/
#ifndef GLOBAL_H
#define GLOBAL_H
#include <string>
using namespace std;

const int SHOWN_ROWS=21;
const int SHOWN_COLUMS=8;
const int MAX_ROWS=256;
const int MAX_COLUMNS=63;

const string START="\033[";
const string GREEN="\033[32m";
const string GREEN_INVERTED="\033[32;7m";
const string COLOR_RESET="\033[0m";

string col_to_str(int num); //converts numbers to column letters
int col_to_int(string str); //converts column letters to numvers

#endif
