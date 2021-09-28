//
// Created by zacha on 9/28/2020.
//

#ifndef LEXER_PARAMETER_H
#define LEXER_PARAMETER_H

#include <string>

using namespace std;

class Parameter {

private:

    string tokenType;
    string parameterValue;

public:
    Parameter(){};
    Parameter(string tokType, string paramValue) {
        tokenType = tokType;
        parameterValue = paramValue;
    }
    virtual ~Parameter() {}

    string getParameterValue() {
        return parameterValue;
    }

    string getTokenType() {
        return tokenType;
    }

    virtual string toString() {
        return getParameterValue();
    }



};


#endif //LEXER_PARAMETER_H
