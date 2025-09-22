#include <iostream>
#include <unistd.h>   // For read()
#include <termios.h>  // For terminal control
#include <iomanip>
#include "../headers/AnsiTerminal.h"
#include "../headers/global.h"



// Method to print text at a specified position with width
void AnsiTerminal::printAtW(int row, int col, const std::string &text, int w){
    std::cout << START << row << ";" << col << "H"<<GREEN<<std::setw(w)<< text << std::flush;
}

void AnsiTerminal::printInvertedAtW(int row, int col, const std::string &text, int w){
    std::cout << START << row << ";" << col << "H"<<GREEN_INVERTED<<std::setw(w)<< text << COLOR_RESET << std::flush;
    // \033[7m enables reverse video mode, \033[0m resets to normal
}

char AnsiTerminal::getSpecialKeyW(){
    char ch;
    read(STDIN_FILENO, &ch, 1);

    // If it's an escape character, we might be dealing with an escape sequence
    if (ch == '\033'){
        // Peek at the next character to see if it's an arrow key or Alt key combination
        char next_ch;
        if (read(STDIN_FILENO, &next_ch, 1) == 0) return '\033';  // If no more input, it's just ESC

        // Arrow keys and some function keys produce sequences starting with '[' after '\033'
        if(next_ch == '['){
            char arrow_key;
            if(read(STDIN_FILENO, &arrow_key, 1) == 1){
                switch(arrow_key){
                    case 'A': return UP; // Up arrow
                    case 'B': return DOWN; // Down arrow
                    case 'C': return RIGHT; // Right arrow
                    case 'D': return LEFT; // Left arrow
                    case 'H': return HOME; // Home tuşu
                    case 'F': return END;  // End tuşu
                    // Add cases for other keys like Home, End, PgUp, PgDn, if needed
                }
            }
        }
        
        else{
            // If it's not an arrow sequence, it could be an Alt+Key combination
            // Return the second character as the key pressed with Alt
            return next_ch | 0x80;  // Set high bit to distinguish Alt
        }
    }

    // If it's a normal character or Ctrl combination, return as-is
    return ch;
}