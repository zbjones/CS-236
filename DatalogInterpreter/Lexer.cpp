//
// Created by zacha on 9/13/2020.
//

#include "Lexer.h"

void Lexer::run() {
    bool proceed = true;
    Token nextToken;
    while (proceed) {
        nextToken = tokenizer.getNextToken();
        storedTokens.push_back(nextToken);
        if(nextToken.getType() == E_O_F) {
            proceed = false;
        }
    }
    removeComments();
}
