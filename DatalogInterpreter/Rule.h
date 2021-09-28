//
// Created by zacha on 9/28/2020.
//

#ifndef LEXER_RULE_H
#define LEXER_RULE_H


#include "Predicate.h"
#include <vector>

using namespace std;

class Rule {

private:
    Predicate headPredicate;
    vector<Predicate> bodyPredicateList;

public:
    Rule(){
        headPredicate = Predicate();
        bodyPredicateList = vector<Predicate>();
    }

    Rule(Predicate p){
        headPredicate = p;
        bodyPredicateList = vector<Predicate> ();
    }

    Rule (Predicate predicate, vector<Predicate> listPredicates){
        headPredicate = predicate;
        bodyPredicateList = listPredicates;
    }

    void addPredicate(Predicate predicate) {
        bodyPredicateList.push_back(predicate);
    }

    Predicate getHeadPredicate() {
        return headPredicate;
    }

    vector<Predicate> getBodyPredicateList() {
        return bodyPredicateList;
    }

    string toString() {
        string strToReturn = headPredicate.toString() + " :- ";
        for(unsigned int i = 0; i < bodyPredicateList.size(); i++) {
            if (i == 0) {
                strToReturn += bodyPredicateList.at(i).toString();
            }
            else {
                strToReturn += "," + bodyPredicateList.at(i).toString();
            }
        }
        strToReturn += ".";
        return strToReturn;
    }

    void cleanMemory() {
        for (unsigned int i = 0; i < bodyPredicateList.size(); i++) {
            bodyPredicateList.at(i).cleanMemory();
        }
        bodyPredicateList.clear();
        headPredicate.cleanMemory();

    }

};


#endif //LEXER_RULE_H
