

// Title: CSE 340 Project 1 lexer source code file
// Desc: Adding three requested token types, REALNUM, BASE08NUM, and BASE16NUM (Copyright (C) Rida Bazzi), implemented by JIAYUAN YU, WENQI LIU
// Date: 09/09/2023

#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype> // For toupper function, it converts a given character to its uppercase equivalent.

#include "lexer.h"
#include "inputbuf.h"
#include "inputbuf.cc"

using namespace std;

string reserved[] = { "END_OF_FILE",
                      "IF", "WHILE", "DO", "THEN", "PRINT",
                      "PLUS", "MINUS", "DIV", "MULT",
                      "EQUAL", "COLON", "COMMA", "SEMICOLON",
                      "LBRAC", "RBRAC", "LPAREN", "RPAREN",
                      "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
                      "DOT", "NUM", "ID", "ERROR",
                      "REALNUM", "BASE08NUM", "BASE16NUM"// TODO: Add labels for new token types here (as string)
};




#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

// isdigit is defined in the system
// Check if the char is a positive digit (1-9)
bool LexicalAnalyzer::isPdigit(char c){return c >= '1' && c <= '9';}
// Check if the char is a valid hex digit (0-9, A-F, a-f)
bool LexicalAnalyzer::isDigit16(char c){return (c >= '0' && c <= '9') || (toupper(c) >= 'A' && toupper(c) <= 'F');}
// Check if the char is a valid positive hex digit (1-9, A-F, a-f)
bool LexicalAnalyzer::isPdigit16(char c){return (c >= '1' && c <= '9') || (toupper(c) >= 'A' && toupper(c) <= 'F');}
// Check if the character is a valid octal digit (0-7)
bool LexicalAnalyzer::isDigit8(char c) {return c >= '0' && c <= '7';}
// Check if the character is a valid positive octal digit (1-7)
bool LexicalAnalyzer::isPdigit8(char c) {return c >= '1' && c <= '7';}


void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}




Token LexicalAnalyzer::ScanNumber() {
    char c;
    c = toupper(c);    // Convert c to uppercase

    input.GetChar(c);  // Get the first char

    // Take 0-9 and A-F as char entry
    if (isDigit16(c)) {
        if (c == '0') {            // If the first char is 0, isolated it
            tmp.lexeme = "0";      // Append 0 to the lexeme
            tmp.token_type = NUM; // Set default token_type to NUM
            input.GetChar(c);  // Get next char

            // Check if it is a dot
            if (!input.EndOfInput() && c == '.') {        // If the next character is a dot
                tmp.lexeme += '.';                        // Append the . to the lexeme
                input.GetChar(c);                      // Read the next character after the dot
                if (isdigit(c)) {                      // Confirm it's a digit after the dot
                    do {
                        tmp.lexeme += c;                  // Append the digit to the lexeme
                        input.GetChar(c);                 // Read next character
                    } while (!input.EndOfInput() && isdigit(c));

                    if (!input.EndOfInput()) {
                        input.UngetChar(c);
                    }
                    tmp.token_type = REALNUM;             // Set type to REALNUM;
                } else {
                    input.UngetChar(c);                  // Unget the current digit
                    input.UngetChar('.');                  // Unget the current digit
                    tmp.token_type = NUM;
                }
                
                // Check if it is 'x' for base08num and base16num
            } else if (!input.EndOfInput() && c == 'x') {
                tmp.lexeme += c;  //  Append 'x'
                input.GetChar(c);  // Read next char, check 0

                if (c == '0') {
                    tmp.lexeme += c;  // Append '0'
                    input.GetChar(c);  // Read next char, check 8
                    if (c == '8') {
                        tmp.lexeme += c;  // Append '8'
                        tmp.token_type = BASE08NUM;  // Define a BASE08NUM token type
                    } else {
                        input.UngetChar(c);  // Unget char if not 8
                        input.UngetChar('0');  // Also unget the '0'
                        input.UngetChar('x');  // Also unget the 'x'
                        tmp.lexeme = "0";  // Reset lexeme to just "0"
                        tmp.token_type = NUM;  // Set token type to NUM
                        return tmp;
                    }
                } else if (c == '1') {
                    tmp.lexeme += c;  // Append '1'
                    input.GetChar(c);  // Read next char, check 6
                    if (c == '6') {
                        tmp.lexeme += c;  // Append '6'
                        tmp.token_type = BASE16NUM;  // Define a BASE16NUM token type
                    } else {
                        input.UngetChar(c);  // Unget char if not 6
                        input.UngetChar('1');  // Also unget the '1'
                        input.UngetChar('x');  // Also unget the 'x'
                        tmp.lexeme = "0";  // Reset lexeme to just "0"
                        tmp.token_type = NUM;  // Set token type to NUM
                        return tmp;
                    }
                } else {
                    input.UngetChar(c);  // Unget the current char if not 0 or 1
                    input.UngetChar('x');  // Also unget the 'x'
                    tmp.lexeme = "0";  // Reset lexeme to just "0"
                    tmp.token_type = NUM;  // Set token type to NUM
                }
                
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
            }
            return tmp;
        }




        // Check if  it starts with 1-9
    } else if (isPdigit(c)) {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) {
                tmp.lexeme += c;
                input.GetChar(c);
            }

    } else if (c >= 'A' && c <= 'F') {

    } else {
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
            tmp.lexeme = "";
            tmp.token_type = ERROR;
            tmp.line_no = line_no;
            return tmp;
        }

}


















Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
            tmp.lexeme += c;
            input.GetChar(c);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
            if (isdigit(c)) {
                input.UngetChar(c);
                return ScanNumber();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else {
                tmp.token_type = ERROR;
            }
            return tmp;
    }
}

int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}
