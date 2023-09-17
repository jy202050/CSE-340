// Title: CSE 340 Project 1 lexer header file
// Desc: Adding three requested token types, REALNUM, BASE08NUM, and BASE16NUM (Copyright (C) Rida Bazzi), implemented by JIAYUAN YU, WENQI LIU
// Date: 09/17/2023

#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.h"

using namespace std;
// ------- token types -------------------

typedef enum { END_OF_FILE = 0,
    IF, WHILE, DO, THEN, PRINT,
    PLUS, MINUS, DIV, MULT,
    EQUAL, COLON, COMMA, SEMICOLON,
    LBRAC, RBRAC, LPAREN, RPAREN,
    NOTEQUAL, GREATER, LESS, LTEQ, GTEQ,
    DOT, NUM, ID, ERROR, REALNUM, BASE08NUM, BASE16NUM // TODO: Add labels for new token types here
} TokenType;

class Token {
public:
    void Print();

    std::string lexeme;
    TokenType token_type;
    int line_no;
};

class LexicalAnalyzer {
public:
    Token GetToken();
    TokenType UngetToken(Token);
    LexicalAnalyzer();

private:
    std::vector<Token> tokens;
    int line_no;
    Token tmp;
    InputBuffer input;

    bool IsSequenceAlign(string);
    bool SkipSpace();
    bool IsKeyword(string);

    string ScanNumType();
    string ScanRNandBASE08Type(string, bool (*DigitType1)(char), bool (*DigitType2)(char));
    string ScanBASE16Type(string, bool (*DigitType1)(char), bool (*DigitType2)(char));

    TokenType FindKeywordIndex(string);
    Token ScanIdOrKeyword();
    Token ScanTokenType();
};

#endif  //__LEXER__H__