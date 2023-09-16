// Title: CSE 340 Project 1 lexer header file
// Desc: Adding three requested token types, REALNUM, BASE08NUM, and BASE16NUM (Copyright (C) Rida Bazzi), implemented by JIAYUAN YU, WENQI LIU
// Date: 09/09/2023

#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.h"

// ------- token types -------------------

typedef enum { END_OF_FILE = 0,
    IF, WHILE, DO, THEN, PRINT,
    PLUS, MINUS, DIV, MULT,
    EQUAL, COLON, COMMA, SEMICOLON,
    LBRAC, RBRAC, LPAREN, RPAREN,
    NOTEQUAL, GREATER, LESS, LTEQ, GTEQ,
    DOT, NUM, ID, ERROR,
    REALNUM, BASE08NUM, BASE16NUM   // TODO: Add labels for new token types here
} TokenType;

class Token {
public:
    void Print();                   // method to print the token's details

    std::string lexeme;             // The actual text (substring) identified as the token
    TokenType token_type;           // The type/category of the token (from TokenType enumeration).
    int line_no;                    // The line number in the source code where the token was found.
};


class LexicalAnalyzer {
public:
    Token GetToken();               // Method to fetch the next token from the input.
    TokenType UngetToken(Token);    // Method to 'unget' a token (push it back to be read again).
    LexicalAnalyzer();              // Constructor to initialize the lexical analyzer.

private:
    std::vector<Token> tokens;      // Store a list of tokens identified.
    int line_no;                    // Current line number being processed.
    Token tmp;                      // Temporary storage for the current token being processed.
    InputBuffer input;              // Input buffer to read characters from the source code.

    bool SkipSpace();               // Helper method to skip white spaces.
    bool IsKeyword(std::string);    // Method to check if a given lexeme is a keyword.
    bool isPdigit(char c);          // Check if the character is a positive digit(1-9)
    bool isDigit16(char c);         // Check if the character is a valid hex digit(0-9, A-F, a-f)
    bool isPdigit16(char c);        // Check if the char is a valid positive hex digit (1-9, A-F, a-f)
    bool isDigit8(char c);          // Check if the char is a valid octal digit (0-7)
    bool isPdigit8(char c);         // Check if the char is a valid positive octal digit(1-7)
    TokenType FindKeywordIndex(std::string); // Fetch the token type of a keyword.
    Token ScanIdOrKeyword();        // Identify identifiers or keywords in the input.
    Token ScanNumber();             // Identify number tokens in the input.
    Token ScanRealNum();            // Identify real num tokens in the input.
    Token ScanBase08Num();          // Identify base 08 tokens in the input.
    Token ScanBase16Num();          // Identify base 16 tokens in the input.
};

#endif  //__LEXER__H__
