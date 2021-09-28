//
// Created by zacha on 9/28/2020.
//

#ifndef LEXER_PREDICATE_H
#define LEXER_PREDICATE_H


#include <vector>
#include <string>
#include "Parameter.h"


using namespace std;

class Predicate {

private:

    vector<Parameter*> parameterVector;
    string idString;


public:
    Predicate() {
        idString = "";
        parameterVector = vector<Parameter*>();
    }
    Predicate(string idValue) {
        idString = idValue;
        parameterVector = vector<Parameter*>();
    }
    Predicate(string idValue, vector<Parameter*> vectorParameters) {
        idString = idValue;
        parameterVector = vectorParameters;
    }
    ~Predicate() {}

     vector<Parameter*> getParameterVector() {
        return parameterVector;
     }

     string getIdString() {
        return idString;
     }

     void addParameter(Parameter* parameter) {
         parameterVector.push_back(parameter);
     }

     string toString() {
         string strToReturn = idString + "(";
         for(unsigned int i = 0; i < parameterVector.size(); i++) {
             if (i == 0) {
                 strToReturn += parameterVector.at(i)->toString();
             }
             else {
                 strToReturn += "," + parameterVector.at(i)->toString();
             }
         }
         strToReturn += ")";
         return strToReturn;
     }

    void cleanMemory() {
        for (unsigned int i = 0; i < parameterVector.size(); i++) {
            delete parameterVector.at(i);
        }
        parameterVector.clear();
    }


};


#endif //LEXER_PREDICATE_H
