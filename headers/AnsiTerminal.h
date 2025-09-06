/*
    THE LIBRARY FOR THE TERMINAL OPERATIONS
*/
#ifndef ANSI_TERMINAL_H
#define ANSI_TERMINAL_H

#include <string>
#include <termios.h> //added

//these are used for control of arrow keys and Home, End keys
//asked ChatGPT to give some unprintable characters for check
const char UP = '\x01';
const char DOWN = '\x02';
const char LEFT = '\x03';
const char RIGHT = '\x04';
const char HOME = '\x05';
const char END = '\x06';

class AnsiTerminal {
public:
    // Constructor: Sets up the terminal for capturing keystrokes
    AnsiTerminal();

    // Destructor: Restores the terminal settings to the original state
    ~AnsiTerminal();

    // Print text at a specified row and column
    void printAt(int row, int col, const std::string &text);

    // Print text with inverted background at a specified row and column
    void printInvertedAt(int row, int col, const std::string &text);

    //print functions with setw()
    void printAtW(int row, int col, const std::string &text, int w);

    void printInvertedAtW(int row, int col, const std::string &text, int w);
    
    //revised input function
    char getSpecialKeyW();

    // Clear the terminal screen
    void clearScreen();

    // Get a single keystroke from the terminal
    char getKeystroke();

    // Get the arrow key or special key input ('U', 'D', 'L', 'R' for Up, Down, Left, Right),
    // or detect other key combinations such as Alt+Key, Ctrl+Key, etc.
    char getSpecialKey();

    

private:
    struct termios original_tio; // Holds the original terminal settings
};

#endif // ANSI_TERMINAL_H
