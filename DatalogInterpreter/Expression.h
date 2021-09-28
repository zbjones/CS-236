//
// Created by zacha on 9/28/2020.
//

#ifndef LEXER_EXPRESSION_H
#define LEXER_EXPRESSION_H
#include "Parameter.h"


#include <string>
using namespace std;

class Expression : public Parameter {

private:
    Parameter* paramOne;
    Parameter* paramTwo;
    string operatorStr;

public:
    Expression() {};

    Expression(Parameter* parameterOne, string operStr, Parameter* parameterTwo) {
        paramOne = parameterOne;
        operatorStr = operStr;
        paramTwo = parameterTwo;
    };

    ~Expression() {
        delete paramOne;
        delete paramTwo;
    }

    string toString() {
        string strToReturn = "(" + paramOne->toString() + operatorStr + paramTwo->toString() + ")";
        return strToReturn;
    }

    void pushParamOne(Parameter* param) {
        paramOne = param;
    }

    void pushParamTwo(Parameter* param) {
        paramTwo = param;
    }

    void pushOperator(string str) {
        operatorStr = str;
    }


};


#endif //LEXER_EXPRESSION_H
