//
// Created by zacha on 9/13/2020.
//

#include "Tokenizer.h"

Token Tokenizer::getNextToken() {
    Token tokenToReturn;
    bool wasWhiteSpace = false;
    while (input.size() > 0) {
        tokenToPick = UNDEFINED;
        longestStringLength = 0;
        wasWhiteSpace = false;

        // catch the new line characters and increment the line number
        if (isspace(input[0])) {
            if (input.substr(0, 1) == "\n") {
                ++lineNumber;
            }
            input.erase(input.begin());
            wasWhiteSpace = true;
        }

        if (!wasWhiteSpace) {
            runTokens();
        }
        if (longestStringLength > 0) {
            tokenToReturn = Token(tokenToPick, input.substr(0, longestStringLength), lineNumber);

            //handle multiline strings
            if (multiLineTrack != lineNumber) {
                if (tokenToReturn.getType() == STRING || tokenToReturn.getType() == UNDEFINED ||
                    tokenToReturn.getType() == COMMENT) {
                    lineNumber = multiLineTrack;
                }
            }
            else {
                multiLineTrack = lineNumber;
            }

            //delete the tokens we don't need anymore
            for (int i = 0; i < longestStringLength; i++) {
                input.erase(input.begin());
            }
            return tokenToReturn;
        }
        if (longestStringLength == 0 && !wasWhiteSpace) {
            tokenToReturn = Token(UNDEFINED, input.substr(0, 1), lineNumber);
            input.erase(input.begin());
            return tokenToReturn;
        }
    }
    if (input.empty()) {
        return Token(E_O_F, "", lineNumber);
    }
    return Token(E_O_F, "", lineNumber);
}

void Tokenizer::runTokens() {
    comma();
    period();
    qmark();
    leftparen();
    rightparen();
    multiply();
    add();
    colon();
    colonDash();
    id();
    testString();
    comment();
}

void Tokenizer::comma() {
    if (input.substr(0, 1) == ",") {
        longestString = ",";
        longestStringLength = 1;
        tokenToPick = COMMA;
    }
}

void Tokenizer::period() {
    if (input.substr(0, 1) == ".") {
        longestString = ".";
        longestStringLength = 1;
        tokenToPick = PERIOD;
    }
}

void Tokenizer::qmark() {
    if (input.substr(0, 1) == "?") {
        longestString = "?";
        longestStringLength = 1;
        tokenToPick = Q_MARK;
    }
}

void Tokenizer::leftparen() {
    if (input.substr(0, 1) == "(") {
        longestString = "(";
        longestStringLength = 1;
        tokenToPick = LEFT_PAREN;
    }
}

void Tokenizer::rightparen() {
    if (input.substr(0, 1) == ")") {
        longestString = ")";
        longestStringLength = 1;
        tokenToPick = RIGHT_PAREN;
    }
}

void Tokenizer::multiply() {
    if (input.substr(0, 1) == "*") {
        longestString = "*";
        longestStringLength = 1;
        tokenToPick = MULTIPLY;
    }
}

void Tokenizer::add() {
    if (input.substr(0, 1) == "+") {
        longestString = "+";
        longestStringLength = 1;
        tokenToPick = ADD;
    }
}

void Tokenizer::colon() {
    if (input.substr(0, 1) == ":") {
        longestString = ":";
        longestStringLength = 1;
        tokenToPick = COLON;
    }
}

void Tokenizer::colonDash() {
    if (input.substr(0, 2) == ":-") {
        longestString = ":-";
        longestStringLength = 2;
        tokenToPick = COLON_DASH;
    }
}

void Tokenizer::id() {
    char currentChar;
    if (isalpha(input[0])) {
        ++longestStringLength;
        for (unsigned int i = 1; i < input.size(); i++) {
            currentChar = input[i];
            if (isalpha(currentChar) || isdigit(currentChar)) {
                longestString = input.substr(0, i);
                ++longestStringLength;
            } else {
                break;
            }
        }
        tokenToPick = ID;
    }
    if (input.substr(0, 5) == "Rules") {
        if (!isalpha(input[5])) {
            longestString = "Rules";
            longestStringLength = 5;
            tokenToPick = RULES;
        }
    }
    if (input.substr(0, 5) == "Facts") {
        if (!isalpha(input[5])) {
            longestString = "Facts";
            longestStringLength = 5;
            tokenToPick = FACTS;
        }
    }
    if (input.substr(0, 7) == "Schemes") {
        if (!isalpha(input[7])) {
            longestString = "Schemes";
            longestStringLength = 7;
            tokenToPick = SCHEMES;
        }
    }
    if (input.substr(0, 7) == "Queries") {
        if (!isalpha(input[7])) {
            longestString = "Queries";
            longestStringLength = 7;
            tokenToPick = QUERIES;
        }
    }
}

void Tokenizer::testString() {
    char currentChar;
    bool doubleCase = false;
    multiLineTrack = lineNumber;
    if (input.substr(0, 1) == "'") {
        ++longestStringLength;
        for (unsigned int i = 1; i < input.size(); i++) {
            currentChar = input[i];
            if (currentChar == '\n') {
                ++multiLineTrack;
            }
            longestString = input.substr(0, i);
            ++longestStringLength;
            if (currentChar == '\'') {
                if (input[i + 1] == '\'' && input[i] != '\n') {
                    doubleCase = true;
                    continue;
                }
                if (input[i + 1] == '\n' && evenNumQuotes(longestStringLength)) {
                    tokenToPick = STRING;
                    return;
                }
                if(!doubleCase && input[i + 1] != '\'') {
                    tokenToPick = STRING;
                    return;
                }

                if (input[i + 1] == '\n' && !evenNumQuotes(multiLineTrack)) {
                    continue;
                }
            }
            if (i == input.size() - 1) {
                tokenToPick = UNDEFINED;
                return;
            }
            doubleCase = false;

        }
        tokenToPick = STRING;
    }
}

void Tokenizer::comment() {
    char currentChar;
    if (input.substr(0, 1) == "#") {
        ++longestStringLength;
        if (input.substr(1,1) == "|") {
            ++longestStringLength;
            longestString = input.substr(0, 2);
            multiLineTrack = lineNumber;
            for(unsigned int i = 1; i < input.size(); i++) {
                currentChar = input[i];
                if(currentChar == '\n' && input.size() - 1 != i) {
                    ++multiLineTrack;
                    longestString = input.substr(0, i + 2);
                    ++longestStringLength;
                    continue;
                }
                else if(currentChar == '\n') {
                    ++multiLineTrack;
                    longestString = input.substr(0, i+2);
                    continue;
                }
                if(currentChar == '|') {
                    if(input[i + 1] == '#') {
                        ++longestStringLength;
                        longestString = input.substr(0, i + 2);
                        tokenToPick = COMMENT;
                        return;
                    }
                }
                if(i == input.size() - 1) {
                    tokenToPick = UNDEFINED;
                    return;
                }
                longestString = input.substr(0, i + 2);
                ++longestStringLength;
            }
        }
        else {
            for(unsigned int i = 1; i < input.size(); i++) {
                currentChar = input[i];
                if(currentChar == '\n') {
                    tokenToPick = COMMENT;
                    return;
                }
                if(i == input.size() - 1) {
                    ++longestStringLength;
                    tokenToPick = COMMENT;
                    return;
                }
                longestString = input.substr(0, i);
                ++longestStringLength;
            }
        }

    }
}

bool Tokenizer::evenNumQuotes(int currentIndex) {
    string currentString = input.substr(0, currentIndex);
    int numQuotes = 0;
    for (char i : currentString) {
        if (i == '\'') {
            ++numQuotes;
        }
    }
    if (numQuotes % 2 == 0) {
        return true;
    }
    return false;
}

