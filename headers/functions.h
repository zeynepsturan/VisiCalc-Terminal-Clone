#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <vector>
#include <list>
#include "cell.h"
using namespace std;

class Functions{
public:
    double SUM(vector<vector<Cell>>& table, const string& startcell, const string& endcell);
    double AVER(vector<vector<Cell>>& table, const string& startcell, const string& endcell);
    double MAX(vector<vector<Cell>>& table, const string& startcell, const string& endcell);
    double MIN(vector<vector<Cell>>& table, const string& startcell, const string& endcell);
    double STDDEV(vector<vector<Cell>>& table, const string& startcell, const string& endcell);
    void determineCells(vector<vector<Cell>>& table, string sc, string ec, int& startr, int& startc, int& endr, int& endc);
    list<string> parseFunction(vector<vector<Cell>>& table, int r, int c);
    void handleFunction(vector<vector<Cell>>& table, int r, int c);
};

#endif