//
// Created by zacha on 9/12/2020.
//

#ifndef LEXER_LEXER_H
#define LEXER_LEXER_H

#include <string>
#include <vector>

#include "Token.h"
#include "Tokenizer.h"
using namespace std;

class Lexer {

private:
    vector<Token> storedTokens;
    Tokenizer tokenizer;

public:
    Lexer(Tokenizer tokenizer) : tokenizer(tokenizer) {};

    int getNumTokens() {
        return storedTokens.size();
    };

    void run();;

    string printTokens() {
        string strToReturn;
        for(unsigned int i = 0; i < storedTokens.size(); i++) {
            strToReturn += storedTokens[i].toStringToken();
        }
        return strToReturn;
    };

    void removeComments() {
        for(unsigned int i = 0; i < storedTokens.size();) {
            if (isComment(storedTokens[i])) {
                storedTokens.erase(storedTokens.begin() + i);
            }
            else {
                ++i;
            }
        }
    }

    bool isComment(Token theToken) {
        if (theToken.getType() == COMMENT) {
            return true;
        }
        else return false;
    }

    vector<Token> getVector() {
        return storedTokens;
    }

};


#endif //LEXER_LEXER_H
