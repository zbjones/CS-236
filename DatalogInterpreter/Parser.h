//
// Created by zacha on 9/27/2020.
//

#ifndef LEXER_PARSER_H
#define LEXER_PARSER_H

#include "Token.h"
#include "DatalogProgram.h"
#include "Expression.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Parser {

private:
    vector<Token> tokenVector;
    unsigned int currentIndex;
    DatalogProgram data;
    Predicate predToReturn;
    Rule ruleToReturn;

public:
Parser(){};

Parser(vector<Token> theTokenVector) {
    currentIndex = 0;
    for (unsigned int i = 0; i < theTokenVector.size(); i++) {
        tokenVector.push_back(theTokenVector.at(i));
    }
};
~Parser() {
    data.cleanMemoryDatalog();
}

void addTokenVector(vector<Token> theTokenVector) {
    currentIndex = 0;
    for (unsigned int i = 0; i < theTokenVector.size(); i++) {
        tokenVector.push_back(theTokenVector.at(i));
    }
}

    DatalogProgram parse() {
        try {
            parseDatalogProgram();
        } catch (Token& error) {
            cout << "Failure!" << endl << "  " << error.toStringToken();
            data.cleanMemoryDatalog();
            if (error.toStringToken().substr(0,4) != "(QUE") {
                predToReturn.cleanMemory();
                ruleToReturn.cleanMemory();
            }
        }
        return data;
    }

    void parseDatalogProgram() {
        parseSCHEMES();
        parseCOLON();
        parseScheme();
        parseSchemeList();
        parseFACTS();
        parseCOLON();
        parseFactList();
        parseRULES();
        parseCOLON();
        parseRuleList();
        parseQUERIES();
        parseCOLON();
        parseQuery();
        parseQueryList();
    }

    void parseSchemeList() {
        if (getTokenType() == ID) {
            parseScheme();
            parseSchemeList();
        }
        else return;
    }

    void parseFactList() {
        if (getTokenType() == ID) {
            parseFact();
            parseFactList();
        }
        else return;
    }

    void parseRuleList() {
        if (getTokenType() == ID) {
            parseRule();
            parseRuleList();
        }
        else return;
    }

    void parseQueryList() {
        if (getTokenType() == ID) {
            parseQuery();
            parseQueryList();
        }
        else return;
    }

    void parseScheme() {
        predToReturn = Predicate(tokenVector.at(currentIndex).getValue());
        parseID();
        parseLEFT_PAREN();
        if (tokenVector.at(currentIndex).getType() == ID) {
            predToReturn.addParameter(new Parameter(tokenVector.at(currentIndex).getTypeString(),
                                                tokenVector.at(currentIndex).getValue()));
        }
        parseID();
        parseIdList();
        parseRIGHT_PAREN();
        data.pushScheme(predToReturn);
    }

    void parseFact() {
        predToReturn = Predicate(tokenVector.at(currentIndex).getValue());
        parseID();
        parseLEFT_PAREN();
        if (tokenVector.at(currentIndex).getType() == STRING) {
            predToReturn.addParameter(new Parameter(tokenVector.at(currentIndex).getTypeString(),
                                                tokenVector.at(currentIndex).getValue()));
        }
        parseSTRING();
        parseStringList();
        parseRIGHT_PAREN();
        parsePERIOD();
        data.pushFact(predToReturn);
    }

    void parseRule() {
        parseHeadPredicate();
        ruleToReturn = Rule(predToReturn);
        parseCOLON_DASH();
        parsePredicate();
        parsePredicateList();
        parsePERIOD();
        data.pushRule(ruleToReturn);
    }

    void parseQuery() {
        predToReturn = Predicate(tokenVector.at(currentIndex).getValue());
        parsePredicate();
        parseQ_MARK();
        data.pushQuery(predToReturn);
    }

    void parseHeadPredicate() {
        predToReturn = Predicate(tokenVector.at(currentIndex).getValue());
        parseID();
        parseLEFT_PAREN();
        if (tokenVector.at(currentIndex).getType() == ID) {
            predToReturn.addParameter(new Parameter(tokenVector.at(currentIndex).getTypeString(),
                                                tokenVector.at(currentIndex).getValue()));
        }
        parseID();
        parseIdList();
        parseRIGHT_PAREN();
    }

    void parsePredicate() {
        predToReturn = Predicate(tokenVector.at(currentIndex).getValue());
        parseID();
        parseLEFT_PAREN();
        parseParameter();
        parseParameterList();
        parseRIGHT_PAREN();
        ruleToReturn.addPredicate(predToReturn);

    }

    void parsePredicateList() {
        if (getTokenType() == COMMA) {
            parseCOMMA();
            parsePredicate();
            parsePredicateList();
        }
        else return;
    }

    void parseParameterList() {
        if (getTokenType() == COMMA) {
            parseCOMMA();
            parseParameter();
            parseParameterList();
        }
        else return;
    }

    void parseStringList() {
        if (getTokenType() == COMMA) {
            parseCOMMA();
            if (tokenVector.at(currentIndex).getType() == STRING) {
                predToReturn.addParameter(new Parameter(tokenVector.at(currentIndex).getTypeString(),
                                                    tokenVector.at(currentIndex).getValue()));
            }
            parseSTRING();
            parseStringList();
        }
        else return;
    }

    void parseIdList() {
        if (getTokenType() == COMMA) {
            parseCOMMA();
            if (tokenVector.at(currentIndex).getType() == ID) {
                predToReturn.addParameter(new Parameter(tokenVector.at(currentIndex).getTypeString(),
                                                    tokenVector.at(currentIndex).getValue()));
            }
            parseID();
            parseIdList();
        }
        else return;
    }

    void parseParameter() {
        if (getTokenType() == STRING) {
            predToReturn.addParameter(new Parameter(tokenVector.at(currentIndex).getTypeString(),
                                                tokenVector.at(currentIndex).getValue()));
            parseSTRING();
        }
        else if (getTokenType() == ID) {
            predToReturn.addParameter(new Parameter(tokenVector.at(currentIndex).getTypeString(),
                                                tokenVector.at(currentIndex).getValue()));
            parseID();
        }
        else if (getTokenType() == LEFT_PAREN){
            parseExpression();
        }
    }

    void parseExpression() {
        parseLEFT_PAREN();
        Parameter* left = makeParameter();
        string operatorValue = "";
        Parameter* right = nullptr;
        Expression* newExpression = nullptr;

        try {
            operatorValue = parseOperator();
        } catch (Token& errorToken) {
            delete left;
            throw;
        }

        try {
            right = makeParameter();
            parseParameter();
            parseRIGHT_PAREN();
        } catch (Token& errorToken) {
            delete left;
            delete right;
            throw;
        }

        try {
            newExpression = new Expression(left, operatorValue, right);
            predToReturn.addParameter(newExpression);
        } catch (Token& error) {
            delete left;
            delete right;
            delete newExpression;
            throw;
        }
    }

    Parameter* makeParameter() {
        if (getTokenType() == STRING) {
            Parameter* paramToReturn;
            paramToReturn = new Parameter(tokenVector.at(currentIndex).getTypeString(),
                                                    tokenVector.at(currentIndex).getValue());
            parseSTRING();
            return paramToReturn;
        }
        else if (getTokenType() == ID) {
            Parameter* paramToReturn;
            paramToReturn = new Parameter(tokenVector.at(currentIndex).getTypeString(),
                                          tokenVector.at(currentIndex).getValue());
            parseID();
            return paramToReturn;
        }
        else if (getTokenType() == LEFT_PAREN) {
            parseLEFT_PAREN();
            Parameter* left = makeParameter();
            string operatorValue;
            Parameter* right = nullptr;
            Expression* newExpression = nullptr;

            try {
                operatorValue = parseOperator();
            } catch (Token& errorToken) {
                delete left;
                throw;
            }

            try {
                right = makeParameter();
                parseParameter();
                parseRIGHT_PAREN();
            } catch (Token& errorToken) {
                delete left;
                delete right;
                throw;
            }

            try {
                newExpression = new Expression(left, operatorValue, right);
                return newExpression;
            } catch (Token& error) {
                delete left;
                delete right;
                delete newExpression;
                throw;
            }
        }
        else return nullptr;
    }

    string parseOperator() {
        string strToReturn;
        if (getTokenType() == ADD) {
            strToReturn = parseADD();
            ++currentIndex;
            return strToReturn;
        }
        else if (getTokenType() == MULTIPLY) {
            strToReturn = parseMULTIPLY();
            ++currentIndex;
            return strToReturn;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parseSCHEMES() {
        if(getTokenType() == SCHEMES) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parseCOLON() {
        if(getTokenType() == COLON) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parseFACTS() {
        if(getTokenType() == FACTS) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parseRULES() {
        if(getTokenType() == RULES) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parseQUERIES() {
        if(getTokenType() == QUERIES) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parseID() {
        if(getTokenType() == ID) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parseLEFT_PAREN() {
        if(getTokenType() == LEFT_PAREN) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parseRIGHT_PAREN() {
        if(getTokenType() == RIGHT_PAREN) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parseSTRING() {
        if(getTokenType() == STRING) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parsePERIOD() {
        if(getTokenType() == PERIOD) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parseCOLON_DASH() {
        if(getTokenType() == COLON_DASH) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parseQ_MARK() {
        if(getTokenType() == Q_MARK) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    void parseCOMMA() {
        if(getTokenType() == COMMA) {
            ++currentIndex;
        }
        else throw tokenVector.at(currentIndex);
    }

    string parseADD() {
        if(getTokenType() == ADD) {
            return (tokenVector.at(currentIndex).getValue());
        }
        else throw tokenVector.at(currentIndex);
    }

    string parseMULTIPLY() {
        if(getTokenType() == MULTIPLY) {
            return (tokenVector.at(currentIndex).getValue());
        }
        else throw tokenVector.at(currentIndex);
    }

    type getTokenType() {
        return tokenVector.at(currentIndex).getType();
    }


};

#endif //LEXER_PARSER_H

//GRAMMAR
/*datalogProgram	->	SCHEMES COLON scheme schemeList
                        FACTS COLON factList
                        RULES COLON ruleList
                        QUERIES COLON query queryList
scheme   	-> 	ID LEFT_PAREN ID idList RIGHT_PAREN
schemeList	->	scheme schemeList | lambda
idList  	->	COMMA ID idList | lambda
fact    	-> 	ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD
factList	->	fact factList | lambda
rule    	->	headPredicate COLON_DASH predicate predicateList PERIOD
ruleList	->	rule ruleList | lambda
headPredicate	->	ID LEFT_PAREN ID idList RIGHT_PAREN
predicate	->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN
predicateList	->	COMMA predicate predicateList | lambda
parameter	->	STRING | ID | expression
parameterList	-> 	COMMA parameter parameterList | lambda
expression	-> 	LEFT_PAREN parameter operator parameter RIGHT_PAREN
operator	->	ADD | MULTIPLY
query	        ->      predicate Q_MARK
queryList	->	query queryList | lambda
stringList	->	COMMA STRING stringList | lambda */
