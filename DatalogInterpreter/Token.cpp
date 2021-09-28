//
// Created by zacha on 9/13/2020.
//
#include "Token.h"

string Token::getTypeString() {
    switch(tokenType) {
        case 0:
            return "COMMA";
            break;
        case 1:
            return "PERIOD";
            break;
        case 2:
            return "Q_MARK";
            break;
        case 3:
            return "LEFT_PAREN";
            break;
        case 4:
            return "RIGHT_PAREN";
            break;
        case 5:
            return "COLON";
            break;
        case 6:
            return "COLON_DASH";
            break;
        case 7:
            return "MULTIPLY";
            break;
        case 8:
            return "ADD";
            break;
        case 9:
            return "SCHEMES";
            break;
        case 10:
            return "FACTS";
            break;
        case 11:
            return "RULES";
            break;
        case 12:
            return "QUERIES";
            break;
        case 13:
            return "ID";
            break;
        case 14:
            return "STRING";
            break;
        case 15:
            return "COMMENT";
            break;
        case 16:
            return "WHITESPACE";
            break;
        case 17:
            return "UNDEFINED";
            break;
        case 18:
            return "EOF";
            break;
    }
    return "";
}

string Token::toStringToken() {
    string strToReturn;
    strToReturn = "(" + getTypeString() + ",";
    strToReturn += "\"" + tokenValue + "\"" + ",";
    strToReturn += to_string(token_line_number) + ")" + "\n";
    return strToReturn;
}
