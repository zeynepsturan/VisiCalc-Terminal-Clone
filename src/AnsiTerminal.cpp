#include <iostream>
#include <unistd.h>   // For read()
#include <termios.h>  // For terminal control
#include <iomanip>
#include "../headers/AnsiTerminal.h"
#include "../headers/global.h"

// Constructor: Configure terminal for non-canonical mode
AnsiTerminal::AnsiTerminal() {
    // Save the original terminal settings
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios new_tio = original_tio;

    // Disable canonical mode and echo for real-time input reading
    new_tio.c_lflag &= ~(ICANON | ECHO);
    new_tio.c_iflag &= ~(IXON | IXOFF);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

// Destructor: Restore the terminal settings to original state
AnsiTerminal::~AnsiTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

// Method to print text at a specified position
void AnsiTerminal::printAt(int row, int col, const std::string &text) {
    std::cout <<START << row << ";" << col << "H"<<GREEN<< text << std::flush;
}

// Method to print text with inverted background at a specified position
void AnsiTerminal::printInvertedAt(int row, int col, const std::string &text) {
    std::cout << START << row << ";" << col << "H"<<GREEN_INVERTED<< text << COLOR_RESET << std::flush;
    // \033[7m enables reverse video mode, \033[0m resets to normal
}

// Method to clear the terminal screen
void AnsiTerminal::clearScreen(){
    std::cout << "\033[2J\033[H" << std::flush; // Clear screen and move cursor to home
}

// Method to get a single keystroke from the terminal
char AnsiTerminal::getKeystroke(){
    char ch;
    read(STDIN_FILENO, &ch, 1);  // Read the first character

    // If the character falls within ASCII range for control characters
    // Ctrl+A to Ctrl+Z are 0x01 to 0x1A, mapping to 'a' - '@'
    /* if (ch >= 1 && ch <= 26) {
        // Map Ctrl+A to Ctrl+Z to a more readable form
        return ch + 'A' - 1;
    } */

    return ch;  // Return the character as-is if it's a regular key
}

// Method to handle arrow key sequences, Alt keys, and other special keys
char AnsiTerminal::getSpecialKey(){
    char ch = getKeystroke();

    if(ch>=1 && ch<=26){
        return ch;
    }

    // If it's an escape character, we might be dealing with an escape sequence
    if(ch == '\033'){
        char next1, next2;
        if(read(STDIN_FILENO, &next1, 1) != 1) return '\033';
        if(next1 != '[') return '\033'; // sadece ESC
        if(read(STDIN_FILENO, &next2, 1) != 1) return '\033';

        switch(next2){
            case 'A': return 'U'; // Up
            case 'B': return 'D'; // Down
            case 'C': return 'R'; // Right
            case 'D': return 'L'; // Left
        }
    }

    // If it's a normal character or Ctrl combination, return as-is
    return ch;
}
