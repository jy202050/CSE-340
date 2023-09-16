// Title: CSE 340 Project 1 input header file
// Desc: Adding three requested token types, REALNUM, BASE08NUM, and BASE16NUM (Copyright (C) Rida Bazzi), implemented by JIAYUAN YU, WENQI LIU
// Date: 09/09/2023


#ifndef __INPUT_BUFFER__H__
#define __INPUT_BUFFER__H__

#include <string>  // String manipulation
#include <vector>  // using vector container

// Struct the class InputBuffer
class InputBuffer {
public:
    void GetChar(char&);                  // Method that fetches a char from the input
    //stream or returns it from the buffer if any
    char UngetChar(char);                 // Pushes a char back into the input buffer
    std::string UngetString(std::string); // Method that pushes a string in reverse into the
    // input buffer. returns the same string
    bool EndOfInput();                    // Method that checks if there is any input left
    // in the buffer. or in the standard input.

private:
    std::vector<char> input_buffer;       // A buffer to store characters which we might
    // want to process again
};

#endif  //__INPUT_BUFFER__H__
