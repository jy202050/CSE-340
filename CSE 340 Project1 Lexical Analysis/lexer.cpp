// Title: CSE 340 Project 1 lexer executable file
// Desc: Adding three requested token types, REALNUM, BASE08NUM, and BASE16NUM (Copyright (C) Rida Bazzi), implemented by JIAYUAN YU, WENQI LIU
// Date: 09/17/2023

#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "inputbuf.cc"
#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
                      "IF", "WHILE", "DO", "THEN", "PRINT",
                      "PLUS", "MINUS", "DIV", "MULT",
                      "EQUAL", "COLON", "COMMA", "SEMICOLON",
                      "LBRAC", "RBRAC", "LPAREN", "RPAREN",
                      "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
                      "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM", "BASE16NUM"
};

#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

// Toupper converts a => A or A => a
bool isPdigit(char c) { return c >= '1' && c <= '9';}
bool isPdigit8(char c) { return c >= '1' && c <= '7';}
bool isPdigit16(char c) { c = toupper(c); return (c >= '1' && c <= '9') || (c >= 'A' && c <= 'F');}
bool isDigit(char c) { return c == '0' || (c >= '1' && c <= '9');}
bool isDigit8(char c) { return c == '0' || (c >= '1' && c <= '7');}
bool isDigit16(char c) { c = toupper(c); return c == '0' || ((c >= '1' && c <= '9') || (c >= 'A' && c <= 'F'));}


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

// Method to verify whether the next series of char in the input matches a specific string that passes as the parameter digit type(ScanTokenType)
bool LexicalAnalyzer::IsSequenceAlign(string digit)
{
    char c;

    for (int i = 0; i < digit.length(); i++) {        // Loop to iterate through each character in the string digit
        input.GetChar(c);                         // Retrieve char c on each iteration
        if (input.EndOfInput() || c != digit[i]) {   // Check if the end of input stream has been reached
            if (!input.EndOfInput()) {       // or the current input does not match the current char c in the string digit
                input.UngetChar(c);          // Call UngetChar method to putting it back
            }
            for (int j = 0; j < i; j++) {      // Unget all the char if there is a mismatch between string digit and input stream
                input.UngetChar(digit[i-j-1]);
            }
            return false;     // Return false if the function couldn't find a matching sequence in the input
        }
    }
    return true;         // Return true if match
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

// Scan to verify if the token type is NUM or REALNUM or BASE08NUM or BASE16NUM.
Token LexicalAnalyzer::ScanTokenType()
{
        string TokenType;

        // Recall ScanRNandBASE08Type string method to check if it is NUM token type
        TokenType = ScanRNandBASE08Type(".",isDigit,isPdigit);
        if (TokenType.length() > 0)
        {
            tmp.lexeme = TokenType;
            tmp.token_type = REALNUM;
            tmp.line_no = line_no;
            return tmp;
        }

        // Recall ScanRNandBASE08Type string method to check if it is NUM token type
        TokenType = ScanRNandBASE08Type("x08",isDigit8,isPdigit8);
        if (TokenType.length() > 0)
        {
            tmp.lexeme = TokenType;
            tmp.token_type = BASE08NUM;
            tmp.line_no = line_no;
            return tmp;
        }

        // Recall ScanBASE16Type string method to check if it is NUM token type
        TokenType = ScanBASE16Type("x16",isDigit16 ,isPdigit16);
        if (TokenType.length() > 0)
        {
            tmp.lexeme = TokenType;
            tmp.token_type = BASE16NUM;
            tmp.line_no = line_no;
            return tmp;
        }

        // Recall ScanNumType string method to check if it is NUM token type
        TokenType = ScanNumType();
        if (TokenType.length() > 0)
        {
            tmp.lexeme = TokenType;
            tmp.token_type = NUM;
            tmp.line_no = line_no;
            return tmp;
        }

    tmp.lexeme = "";
    tmp.token_type = ERROR;
    tmp.line_no = line_no;
    return tmp;
}


// Method to check if the string is NUM type
string LexicalAnalyzer::ScanNumType()
{
    char c;
    string NumLexeme = "";        // Initialie the string NumLexeme

    input.GetChar(c);          // Get next char
    NumLexeme += c;               // Append the first char
    if (isDigit(c))
    {
        // Check if it is digit in range 0 - 9
        if (c == '0')
        {
            return NumLexeme;           // if it is 0, return 0 as a num type(isolate 0)
        }  else
              {                         // Else loop to check 1-9
                input.GetChar(c);    // Read next char
                // Set while loop to append all the digit 0-9 if start with pdigit 1-9
                    while (!input.EndOfInput() && isDigit(c))
                    {
                        NumLexeme += c;             // Append char to the NumLexeme on each iteration
                        input.GetChar(c);        // Read next char on each iteration
                    }
                        if (!input.EndOfInput())
                        {  // If the next char is not a digit 1-9 and not the end of input
                            input.UngetChar(c);     // Call UngetChar method
                        }
            return NumLexeme;                       // Return the current lexeme
              }
    }
    // If not satisfy the entry 0-9, Recall UngetString method to unget all char in the NumLexeme
    input.UngetString(NumLexeme);
    return "";
}


// Method to check if the string is either RealNum or BASE08NUM type
string LexicalAnalyzer::ScanRNandBASE08Type(string MatchRNN8String, bool (*DigitType1)(char), bool (*DigitType2)(char))
{
        char c;
        string RNN8Lexeme  = ""; // Initialize the RNN8Lexeme(Realnum and base08)
        bool ScanDigit;

        input.GetChar(c);  // Read the first char
        RNN8Lexeme += c;        // Append the first char to RNN8Lexeme
        // Check if the first char is 0
        if(c == '0') {
            // Recall IsSequenceAlign to check the parameter MatchRNN8String if the next set of char in the input match the MatchRNN8String(RNN8Lexeme)
            if (IsSequenceAlign(MatchRNN8String)) {
                RNN8Lexeme += MatchRNN8String;  // Append to RNN8Lexeme if MatchRNN8String is found
                // Check if the MatchRNN8String is a dot(.)
                if (MatchRNN8String == ".") {
                    input.GetChar(c);
                    // Append digit if the current digit is 0 and not end of input
                    while (!input.EndOfInput() && c == '0') {
                        RNN8Lexeme += c;              // Append digit
                        input.GetChar(c);        // Read next char
                            // Check if a newline character is encountered
                            if(c== '\n') {
                                RNN8Lexeme += c;       // Append to the RNN8Lexeme
                                input.UngetString(RNN8Lexeme);   // Put back into the input
                                return "";                        // Return empty string
                            }
                    }
                    // Check if the current char satisfies is pdigit
                    if (!input.EndOfInput() && isPdigit(c))
                    {
                        while(!input.EndOfInput() && isDigit(c))   // Append all the digit 0-9
                        {
                            RNN8Lexeme += c;                         // Append all char on each iteration
                            input.GetChar(c);                   // Read next char(each iteration)
                        }
                        if (!input.EndOfInput())                   // UngetChar if not in range of 0-9 and not end of the input
                            input.UngetChar(c);
                    }
                }
                return RNN8Lexeme;      //Return RNN8Lexeme if found
            }
        } else if(DigitType2(c)) {
            input.GetChar(c);  // Read the first char
            // Append all the digit 0-9 after the first char(1-9)
            while (!input.EndOfInput() && DigitType1(c)) {
                RNN8Lexeme += c;
                input.GetChar(c);
            }
            // Call UngetChar method if not in range of 0-9 and not end of input
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
            // Check again if the next char in the input match MatchRNN8String
            if (IsSequenceAlign(MatchRNN8String)) {
                RNN8Lexeme += MatchRNN8String;     // Append it to RNN8Lexeme
                if (MatchRNN8String == ".") {    // Check if the current char is a dot(.)

                    // If it is a dot(.), set ScanDigit to false(hasnt found a valid digit after the dot yet)
                    ScanDigit = false;
                    input.GetChar(c);       // Read next char

                    // Append all digit 0-9
                    while (!input.EndOfInput() && isDigit(c)) {
                        ScanDigit = true;     // Set ScanPdigit to true and appends the digit character to RNN8Lexeme
                        RNN8Lexeme += c;
                        input.GetChar(c);
                    }
                    // Unget char if not in range of 0-9
                    if (!input.EndOfInput())
                        input.UngetChar(c);

                    // If ScanDigit is true, return the RNN8Lexeme string(RNN8Lexeme)
                    if (ScanDigit)
                        return RNN8Lexeme;
                }
                // If  not a dot(.), return RNLexeme string
                else
                    return RNN8Lexeme;
            }
        }

    input.UngetString(RNN8Lexeme);    // Unget the whole string if no valid RNN8Lexeme was found
    return "";                           // Return empty string
}



// Method to check if the string is BASE16NUM type
string LexicalAnalyzer::ScanBASE16Type(string MatchBASE16String, bool (*DigitType1)(char), bool (*DigitType2)(char))
{
    char c;
    string BASE16Lexeme  = ""; // Initialize the BASE16Lexeme
    bool ScanDigit16;

    input.GetChar(c);  // Read the first char
    BASE16Lexeme += c;    // Append the first char to BASE16Lexeme
    // Check if the first char is 0
    if(c == '0') {
        // Recall IsSequenceAlign to check the parameter MatchBASE16String if the next set of char in the input match the MatchBASE16String(BASE16Lexeme)
        if (IsSequenceAlign(MatchBASE16String)) {
            BASE16Lexeme += MatchBASE16String;  // Append to BASE16Lexeme if MatchRNString is found
            // Check if the MatchBASE16String is a 'x'
            if (MatchBASE16String == "x") {
                input.GetChar(c);
                // Append digit if the current digit is 0 and not end of input
                while (!input.EndOfInput() && c == '0') {
                    BASE16Lexeme += c;           // Append digit
                    input.GetChar(c);        // Read next char
                    // Check if a newline character is encountered
                    if(c== '\n') {
                        BASE16Lexeme += c;       // Append to the BASE16Lexeme
                        input.UngetString(BASE16Lexeme);   // Put back into the input
                        return "";                             // Return empty string
                    }
                }

                // Check if the current char satisfies is pdigit16 1-9 and A-F
                if (!input.EndOfInput() && isPdigit16(c))
                {
                    while(!input.EndOfInput() && isDigit16(c))   // Append all the digit 0-9 and A-F
                    {
                        BASE16Lexeme += c;                      // Append all char on each iteration
                        input.GetChar(c);                   // Read next char(each iteration)
                    }
                    if (!input.EndOfInput())                   // UngetChar if not in range of 0-9 or A-F and not end of the input
                        input.UngetChar(c);
                }
            }
            return BASE16Lexeme;      //Return BASE16Lexeme if found
        }
    } else if(DigitType2(c)) {
        input.GetChar(c);  // Read the first char

        // Append all the digits (0-9 && a-f） after the first char(1-9 && a-f)
        while (!input.EndOfInput() && DigitType1(c)) {
            BASE16Lexeme += c;
            input.GetChar(c);
        }
        // Call UngetChar method if not in range of (1-9 && a-f) and not end of input
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        // Check again if the next char in the input match MatchRNString
        if (IsSequenceAlign(MatchBASE16String)) {
            BASE16Lexeme += MatchBASE16String;     // Append it to BASE16Lexeme
            if (MatchBASE16String == "x") {    // Check if the current char is a 'x'

                // If it is a 'x'', set ScanDigit16 to false(hasnt found a valid digit after the 'x' yet)
                ScanDigit16 = false;
                input.GetChar(c);       // Read next char

                // Append all digit 0-9 && a-f
                while (!input.EndOfInput() && isDigit16(c)) {
                    ScanDigit16 = true;     // Set ScanDigit16 to true and appends the digit character to BASE16Lexeme
                    BASE16Lexeme += c;
                    input.GetChar(c);
                }
                // Unget char if not in range of 0-9 && a-f
                if (!input.EndOfInput())
                    input.UngetChar(c);

                // If ScanDigit16 is true, return the BASE16NUM string(BASE16Lexeme)
                if (ScanDigit16)
                    return BASE16Lexeme;
            }
                // If not a 'x'', return BASE16Lexeme string
            else
                return BASE16Lexeme;
        }
    }

    input.UngetString(BASE16Lexeme);    // Unget the whole string if no valid RNLexeme was found
    return "";
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
                return ScanTokenType();
            } else if (isalpha(c)) {
                input.UngetChar(c);
                return ScanIdOrKeyword();
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

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