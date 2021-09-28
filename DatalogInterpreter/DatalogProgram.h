//
// Created by zacha on 9/28/2020.
//

#ifndef LEXER_DATALOGPROGRAM_H
#define LEXER_DATALOGPROGRAM_H

#include <vector>
#include <string>
#include <set>
#include <sstream>
#include "Token.h"
#include "Predicate.h"
#include "Rule.h"

using namespace std;


class DatalogProgram {

private:

    vector<Predicate> schemesVector;
    vector<Predicate> factsVector;
    vector<Predicate> queriesVector;
    vector<Rule> rulesVector;
    set<string> domainSet;

public:

    DatalogProgram() {
        schemesVector = vector<Predicate>();
        factsVector = vector<Predicate>();
        queriesVector = vector<Predicate>();
        rulesVector = vector<Rule>();
        domainSet = set<string>();
    }

    void pushScheme(Predicate predicate) {
        schemesVector.push_back(predicate);
    }

    void pushFact(Predicate predicate) {
        factsVector.push_back(predicate);
        pushDomain(predicate.getParameterVector());
    }

    void pushRule(Rule rule) {
        rulesVector.push_back(rule);
        pushDomain(rule.getHeadPredicate().getParameterVector());
    }
    void pushQuery(Predicate predicate) {
        queriesVector.push_back(predicate);
    }

    void pushDomain(vector<Parameter*> predicateList) {
        for (unsigned int i = 0; i < predicateList.size(); i++) {
            if (predicateList.at(i)->getTokenType() == "STRING") {
                domainSet.insert(predicateList.at(i)->toString());
            }
        }
    }

    string toStringSchemes() {
        string strToReturn = "";
        strToReturn += "Schemes(" + to_string(schemesVector.size()) + "):";
        for (unsigned int i = 0; i < schemesVector.size(); i++) {
            strToReturn += "\n  " + schemesVector.at(i).toString();
        }
        return strToReturn;
    }

    string toStringRules() {
        string strToReturn = "";
        strToReturn += "Rules(" + to_string(rulesVector.size()) + "):";
        for (unsigned int i = 0; i < rulesVector.size(); i++) {
            strToReturn += "\n  " + rulesVector.at(i).toString();
        }
        return strToReturn;
    }

    string toStringFacts() {
        string strToReturn = "";
        strToReturn += "Facts(" + to_string(factsVector.size()) + "):";
        for (unsigned int i = 0; i < factsVector.size(); i++) {
            strToReturn += "\n  " + factsVector.at(i).toString() + ".";
        }
        return strToReturn;
    }

    string toStringQueries() {
        string strToReturn = "";
        strToReturn += "Queries(" + to_string(queriesVector.size()) + "):";
        for (unsigned int i = 0; i < queriesVector.size(); i++) {
            strToReturn += "\n  " + queriesVector.at(i).toString() + "?";
        }
        return strToReturn;
    }

    string toStringDomain() {
        string strToReturn = "";
        strToReturn += "Domain(" + to_string(domainSet.size()) + "):";
        set<string>::iterator setItr;
        for(setItr = domainSet.begin(); setItr != domainSet.end(); setItr++)
            strToReturn += "\n  " + (*setItr);
        return strToReturn;
    }

    string toString() {
        string strToReturn = "Success!\n";
        strToReturn += toStringSchemes() + "\n" + toStringFacts() + "\n" + toStringRules() + "\n" + toStringQueries()
                + "\n" + toStringDomain() + "\n";
        return strToReturn;
    }

    void cleanMemoryDatalog() {
        for (unsigned int i = 0; i < schemesVector.size(); i++) {
            schemesVector.at(i).cleanMemory();
        }
        schemesVector.clear();

        for (unsigned int i = 0; i < factsVector.size(); i++) {
            factsVector.at(i).cleanMemory();
        }
        factsVector.clear();

        for (unsigned int i = 0; i < queriesVector.size(); i++) {
            queriesVector.at(i).cleanMemory();
        }
        queriesVector.clear();

        for (unsigned int i = 0; i < rulesVector.size(); i++) {
            rulesVector.at(i).cleanMemory();
        }
        rulesVector.clear();
    }

    vector<Predicate> getFacts() {
        return factsVector;
    }
    vector<Predicate> getSchemes() {
        return schemesVector;
    }
    vector<Predicate>& getQueries() {
        return queriesVector;
    }
    vector<Rule>& getRules() {
        return rulesVector;
    }

};


#endif //LEXER_DATALOGPROGRAM_H
