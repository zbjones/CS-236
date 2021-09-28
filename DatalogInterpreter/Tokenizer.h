//
// Created by zacha on 9/12/2020.
//

#ifndef LEXER_TOKENIZER_H
#define LEXER_TOKENIZER_H

#include "Token.h"
#include <string>
#include <sstream>
#include <cctype>

using namespace std;

class Tokenizer {
private:
    std::string input;
    int lineNumber;
    int multiLineTrack;
    string longestString;
    int longestStringLength;
    type tokenToPick;

public:
    Tokenizer(std::string &input) {
        this->input = input;
        lineNumber = 1;
    };

    Token getNextToken();;

    void runTokens();;

    void comma();;

    void period();;

    void qmark();;

    void leftparen();;

    void rightparen();;

    void multiply();;

    void add();;

    void colon();;

    void colonDash();;

    void id();;

    void testString();;

    void comment();

    bool evenNumQuotes(int currentIndex);

};





#endif //LEXER_TOKENIZER_H
