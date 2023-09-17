// Title: CSE 340 Project 1 inputbuf source code file
// Desc: Adding three requested token types, REALNUM, BASE08NUM, and BASE16NUM (Copyright (C) Rida Bazzi), implemented by JIAYUAN YU, WENQI LIU
// Date: 09/17/2023

#include <iostream>      // For I/O function
#include <istream>       // For standard input stream operations
#include <vector>
#include <string>
#include <cstdio>        // For end of file constant

#include "inputbuf.h"


using namespace std;

bool InputBuffer::EndOfInput()
{
    if (!input_buffer.empty())    // If the input buffer is not empty
        return false;             // return false (means there is still data in the buffer )
    else
        return cin.eof();         // Otherwise, return the status of end-of-file for cin
}

char InputBuffer::UngetChar(char c)
{
    if (c != EOF)                   // If the provided character is not the end-of-file marker
        input_buffer.push_back(c);; // Add the char to the end of the buffer
    return c;                       // Return the char that was added to the buffer
}

void InputBuffer::GetChar(char& c)
{
    if (!input_buffer.empty()) {    // If the input buffer is not empty
        c = input_buffer.back();    // Retrieve the last char from the buffer
        input_buffer.pop_back();    // Remove the last character from the buffer.
    } else {
        cin.get(c);                 // Otherwise, get a character from the standard input.
    }
}

string InputBuffer::UngetString(string s)
{
    for (int i = 0; i < s.size(); i++)
        input_buffer.push_back(s[s.size()-i-1]);   // Push characters from the string in reverse order to the buffer.
    return s;                                      // Return the original string
}
