//
// Created by zacha on 10/24/2020.
//

#ifndef LEXER_DATABASE_H
#define LEXER_DATABASE_H

#include <map>
#include <iostream>
#include "Relation.h"
#include "Predicate.h"
#include "Rule.h"


class Database {

private:
    map<string, Relation> relations;
    vector<string> orderedKeys;
    vector<unsigned int> tupleSetSizes;

    vector<Predicate> queryVector;
    vector<Rule> rulesVector;

    map<int, string> seenVariables;
    map<int, string> seenConstants;
    vector<string> valueOrder;
    vector<string> newHeader;

    vector<Relation> intermediateResults;

    vector<set<int>> SCCsetVector;


public:
    Database() = default;

    Database(vector<Predicate> schemes, vector<Predicate> facts, vector<set<int>> SCCsetsToAdd) {
        addSchemeVector(schemes);
        addFactVector(facts);
        SCCsetVector = move(SCCsetsToAdd);
    }

    string evaluateAllRules() {
        //update tupleSetSize (first time);
        setSizes();

        string strToReturn;
        unsigned int currIndex = 0;
        do {
            for (unsigned int i = 0; i < rulesVector.size(); i++) {
                strToReturn += evaluateRule(i);
            }
            currIndex++;
        } while (addedNewTuples() || currIndex < 2);

        strToReturn += "\nSchemes populated after " + to_string(currIndex) + " passes through the Rules.\n\n";

        return strToReturn;
    }

    string evaluateAllRulesWithGraph() {
        //update tupleSetSize (first time);
        setSizes();
        string strToReturn;

        for (unsigned int i = 0; i < SCCsetVector.size(); i++) {
            unsigned int currIndex = 0;
            set<int> &currSet = SCCsetVector.at(i);
            if (isTrivial(currSet)) {
                strToReturn += "SCC: " + printSets(currSet) + "\n";
                int j = 0;
                for (auto num : currSet) {
                    strToReturn += evaluateRule(num);
                    j++;
                }
                currIndex++;
            }
            else if (currSet.empty()) {
                return strToReturn;
            }
            else {
                strToReturn += "SCC: " + printSets(currSet) + "\n";
                do {
                    int j = 0;
                    for (auto num : currSet) {
                        strToReturn += evaluateRule(num);
                        j++;
                    }
                    currIndex++;
                } while (addedNewTuples() || currIndex < 2);
            }
            strToReturn += to_string(currIndex) + " passes: " + printSets(currSet) + "\n";
        }

        return strToReturn;
    }

    string evaluateRule(unsigned int location) {
        intermediateResults.clear();
        string strToReturn;
        //get info about rule
        string headPredicateName = rulesVector.at(location).getHeadPredicate().getIdString();
        vector<Parameter*> headPredicateParameters = rulesVector.at(location).getHeadPredicate().getParameterVector();
        vector<Predicate> bodyPredicateList = rulesVector.at(location).getBodyPredicateList();

        //print the rule
        strToReturn += headPredicateName + "(";
        for (unsigned int j = 0; j < headPredicateParameters.size(); j++) {
            strToReturn += headPredicateParameters.at(j)->getParameterValue();
            if (j != headPredicateParameters.size() - 1) {
                strToReturn += ",";
            }
        }
        strToReturn += ") :- ";
        for (unsigned int k = 0; k < bodyPredicateList.size(); k ++) {
            vector<Parameter*> parameterList = bodyPredicateList.at(k).getParameterVector();
            string predicateName = bodyPredicateList.at(k).getIdString();
            strToReturn += predicateName + "(";
            for (unsigned int l = 0; l < parameterList.size(); l++) {
                strToReturn += parameterList.at(l)->getParameterValue();
                if (l != parameterList.size() - 1) {
                    strToReturn += ",";
                }
            }
            if (k != bodyPredicateList.size() - 1) {
                strToReturn += "),";
            }
        }
        strToReturn += ").";

        //Evaluate predicates on right hand side of rule:
        for (unsigned int i = 0; i < bodyPredicateList.size(); i++) {
            vector<Parameter*> parameterList = bodyPredicateList.at(i).getParameterVector();
            Relation workingRelation = getRelation(bodyPredicateList.at(i).getIdString());

            workingRelation = databaseSelect(workingRelation, bodyPredicateList.at(i).getIdString(), parameterList);
            workingRelation = databaseProject(workingRelation, bodyPredicateList.at(i).getIdString(), parameterList);
            workingRelation = databaseRename(workingRelation, bodyPredicateList.at(i).getIdString(), parameterList);

            intermediateResults.push_back(workingRelation);

        }

        //Join all intermediate relations
        Relation workingRelation = intermediateResults.at(0);
        if (intermediateResults.size() >= 1) {
            for (unsigned int i = 1; i < intermediateResults.size(); i++) {
                workingRelation = workingRelation.join(intermediateResults.at(i));
            }
        }

        //Project the columns that appear in the head predicate
        vector<int> savedIndexesToProject;
        for(unsigned int i = 0; i < headPredicateParameters.size(); i++) {
            for (unsigned int j = 0; j < workingRelation.getRelationHeader().getHeader().size(); j++) {
                if (headPredicateParameters.at(i)->getParameterValue() == workingRelation.getRelationHeader().getHeader().at(j)) {
                    savedIndexesToProject.push_back(j);
                }
            }
        }

        //Intermediate step, get header based on scheme
        vector<string> newHeaderIds = getSchemeParamsFromName(headPredicateName);

        //do the projection
        workingRelation = workingRelation.projectForRules(savedIndexesToProject, newHeaderIds);


        //Rename WorkingRelation to match the one in the database
        workingRelation.relationRename(headPredicateName);
        workingRelation.rename(newHeaderIds);

        //Now union with the database.
        strToReturn += unionizer(getRelation(headPredicateName), workingRelation, headPredicateName)  + "\n";;

        return strToReturn;
    }

    vector<string>  getSchemeParamsFromName(string nameOfScheme) {
        vector<string> newHeaderMatchesSchemes;
        Relation matchingRelationScheme = getRelation(nameOfScheme);
        newHeaderMatchesSchemes = matchingRelationScheme.getRelationHeader().getHeader();
        return newHeaderMatchesSchemes;
    }

    Relation joinAllIntermediateRelations() {
        Relation joinedRelation = intermediateResults.at(0);

        for (unsigned int i = 1; i < intermediateResults.size(); i++) {
            joinedRelation = joinedRelation.join(intermediateResults.at(i));
        }

        return joinedRelation;
    }

    string unionizer(Relation databaseTarget, Relation& projectedRelation, string headPredicateName) {
        Relation relToPrint = projectedRelation - databaseTarget;
        for(auto tuple : projectedRelation.tupleSet) {
            databaseTarget.addTuple(tuple);
        }
        addUnionedRelationToDatabase(databaseTarget, headPredicateName);
        return relToPrint.toString();
    }


    string evaluateAllQueries() {
        string strToReturn;
        for (unsigned int i = 0; i < queryVector.size(); i++) {
           strToReturn += evaluateQuery(i) + "\n";
        }
        return strToReturn;
    }

    string evaluateQuery(unsigned int location) {
        string strToReturn;
        //get info about query
        string name = queryVector.at(location).getIdString();
        vector<Parameter*> parameters = queryVector.at(location).getParameterVector();
        //print name of query, the query
        strToReturn += name + "(";
        for (unsigned int j = 0; j < parameters.size(); j++) {
            strToReturn += parameters.at(j)->getParameterValue();
            if (j != parameters.size() - 1) {
                strToReturn += ",";
            }
        }
        strToReturn += ")?";

        //Select tuples
        Relation workingRelation = getRelation(name);

        workingRelation = databaseSelect(workingRelation, name, parameters);
        workingRelation = databaseProject(workingRelation, name, parameters);
        workingRelation = databaseRename(workingRelation, name, parameters);

        if (workingRelation.empty()) {
            strToReturn += " No";
        }
        else {
            strToReturn += " Yes(" + workingRelation.sizeSet() + ")" + workingRelation.toString();
        }

        return strToReturn;
    }

    Relation databaseSelect(Relation &relation, string name, vector<Parameter*> parameters) {
        seenVariables.clear();
        seenConstants.clear();
        valueOrder.clear();
        newHeader.clear();

        for (unsigned int i = 0; i < parameters.size(); i++) {
            if (parameters.at(i)->getTokenType() == "STRING") {
                seenConstants.insert(pair<int, string>(i, parameters.at(i)->getParameterValue()));
                valueOrder.push_back(parameters.at(i)->getParameterValue());
            }
            if (parameters.at(i)->getTokenType() == "ID") {
                string value = parameters.at(i)->getParameterValue();
                bool seen = false;
                for (unsigned int j = 0; j < valueOrder.size(); j++) {
                    if (valueOrder.at(j) == value) {
                        seen = true;
                    }
                }
                if (seen) {
                    valueOrder.push_back(parameters.at(i)->getParameterValue());
                }
                else {
                    seenVariables.insert(pair<int, string>(i, parameters.at(i)->getParameterValue()));
                    valueOrder.push_back(parameters.at(i)->getParameterValue());
                }
            }
        }
        //Select the constants
        for (auto &constant : seenConstants) {
            relation = relation.select(constant.first, constant.second);
        }
        //Select the variables
        for (auto &variable : seenVariables) {
            int timesSeen = 0;
            int position1 = variable.first;
            int position2 = 0;
            for (unsigned int i = 0; i < valueOrder.size(); i++) {
                if (variable.second == valueOrder.at(i)) {
                    timesSeen++;
                    position2 = i;
                }
            }
            if (timesSeen > 1) {
                relation = relation.select(position1, position2);
            }
        }
        return relation;
    }

    Relation databaseProject(Relation &relation, string name, vector<Parameter*> parameters) {
        vector<int> positionsToProject;
        for (unsigned int i = 0; i < valueOrder.size(); i++) {
            if (valueOrder.at(i).substr(0, 1) != "'" && !checkForPreviousOccurence(i)) {
                positionsToProject.push_back(i);
                newHeader.push_back(valueOrder.at(i));
            }
        }
        relation = relation.project(positionsToProject);

        return relation;
    }

    Relation databaseRename(Relation &relation, string name, vector<Parameter*> parameters) {
        return relation.rename(newHeader);
    }

    void storeQueries(vector<Predicate>& queries) {
        queryVector = queries;
    }

    void storeRules(vector<Rule> rules) {
        rulesVector = rules;
    }

    void addScheme(Predicate scheme) {
        string relationName = scheme.getIdString();
        vector<string> headers = transformParamToStr(scheme.getParameterVector());
        if (!exists(relationName)) {
            Relation relationToAdd(relationName, headers);
            relations.insert(pair<string,Relation>(relationName, relationToAdd));
            orderedKeys.push_back(relationName);
        }
    }

    void addSchemeVector(vector<Predicate> schemesVector) {
        for (auto &parameter : schemesVector) {
            addScheme(parameter);
        }
    }

    void addFact(Predicate fact) {
        string relationName = fact.getIdString();
        if (exists(relationName)) {
            relations.at(relationName).add(transformParamToStr(fact.getParameterVector()));
        }
    }

    void addFactVector(vector<Predicate> factsVector) {
        for (auto &parameter : factsVector) {
            addFact(parameter);
        }
    }

    bool exists(string relationName) {
        auto iter = relations.find(relationName);
        return iter != relations.end();
    }

    Relation getRelation(string relationName) {
        if (exists(relationName)) {
            for(const auto& pair : relations) {
                string name = pair.first; //gets key
                Relation rel = pair.second; //gets value
                if (name == relationName) {
                    return rel;
                }
            }
        }
        return Relation();
    }

    //stringify the parameters
    vector<string> transformParamToStr(vector<Parameter*> parameterVector) {
        vector<string> vectorToReturn;
        for (unsigned int i = 0; i < parameterVector.size(); i++) {
            vectorToReturn.push_back(parameterVector.at(i)->toString());
        }
        return vectorToReturn;
    }

    string printAllRelations() {
        string strToReturn;
        //= "PRINTED RELATIONS:\n";
        for(unsigned int i = 0; i < orderedKeys.size(); i++) {
            Relation workingRelation = relations.find(orderedKeys.at(i))->second;
            strToReturn += workingRelation.toString() + "\n";
        }
        return strToReturn;
    }

    bool checkForPreviousOccurence(int currIndex) {
        string currIndexValue = valueOrder.at(currIndex);
        for (int i = 0; i < currIndex; i++) {
            if (valueOrder.at(i) == currIndexValue) {
               return true;
            }
        }
        return false;
    }

    void addUnionedRelationToDatabase(Relation& unionedRel, string relationName) {
        relations.at(relationName) = unionedRel;
    }

    bool addedNewTuples() {
        vector<unsigned int> currentSetSizes;
        for (unsigned int i = 0; i < tupleSetSizes.size(); i++) {
            currentSetSizes.push_back(tupleSetSizes.at(i));
        }
        for (unsigned int i = 0; i < orderedKeys.size(); i++) {
            currentSetSizes.at(i) = getRelation(orderedKeys.at(i)).tupleSet.size();
        }
        for (unsigned int i = 0; i < tupleSetSizes.size(); i++) {
            if (tupleSetSizes.at(i) != currentSetSizes.at(i)) {
                updateSizes();
                return true;
            }
        }
        updateSizes();
        return false;
    }

    void updateSizes() {
        for (unsigned int i = 0; i < orderedKeys.size(); i++) {
            tupleSetSizes.at(i) = getRelation(orderedKeys.at(i)).tupleSet.size();
        }
    }
    void setSizes() {
        for (unsigned int i = 0; i < orderedKeys.size(); i++) {
            tupleSetSizes.push_back(getRelation(orderedKeys.at(i)).tupleSet.size());
        }
    }
    unsigned int getNumTuples(string relationName) {
        return getRelation(relationName).tupleSet.size();
    }

    string printSets(set<int> currSet) {
        string strToReturn;
        int j = 0;
        for (auto num : currSet) {
            if (j > 0) {
                strToReturn += ",";
            }
            strToReturn += "R" + to_string(num);
            j++;
        }
        return strToReturn;
    }

    bool isTrivial(set<int> setToCheck) {
        if (setToCheck.size() == 1) {
            if (doesNotDependOnSelf(setToCheck)) {
                return true;
            }
        }
        return false;
    }

    bool doesNotDependOnSelf(set<int> setToCheck) {
        int i;
        if (setToCheck.size() == 1) {
            for (auto num : setToCheck) {
                i = num;
            }
            for (unsigned int j = 0; j < rulesVector.at(i).getBodyPredicateList().size(); j++) {
                if (rulesVector.at(i).getHeadPredicate().getIdString() == rulesVector.at(i).getBodyPredicateList().at(j).getIdString()) {
                    return false;
                }
            }
        }
        return true;
    }

};



#endif //LEXER_DATABASE_H
