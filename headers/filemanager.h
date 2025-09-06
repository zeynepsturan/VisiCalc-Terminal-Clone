/*
    FILE MANAGER CLASS FOR THE
    VISICAL TERMINAL CLONE
    BY github.com/zeynepsturan
*/
#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <vector>
#include "AnsiTerminal.h"
#include "cell.h"
using namespace std;

class FileManager{
public:
    void loadFromFile(AnsiTerminal& terminal, vector<vector<Cell>>& table);
    void saveToFile(AnsiTerminal& terminal, vector<vector<Cell>>& table);
};

#endif
