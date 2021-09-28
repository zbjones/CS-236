//
// Created by zacha on 9/12/2020.
//

#ifndef LEXER_TOKEN_H
#define LEXER_TOKEN_H

#include <string>

using namespace std;
enum type {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES, FACTS,
    RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, E_O_F};

class Token {

private:
    string tokenValue;
    int token_line_number;
    type tokenType;


public:
    Token() {};

    Token(type tokenType, string tokenValue, int token_line_number) {
        this->tokenType = tokenType;
        this->tokenValue = tokenValue;
        this->token_line_number = token_line_number;
    };

    virtual ~Token() {};

    string getTypeString();;

    string toStringToken();;

    string getValue() {
        return tokenValue;
    }

    type getType() {
        return tokenType;
    };

};


#endif //LEXER_TOKEN_H
