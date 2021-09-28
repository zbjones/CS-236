//
// Created by zacha on 10/24/2020.
//

#ifndef LEXER_RELATION_H
#define LEXER_RELATION_H

#include "Tuple.h"
#include "Header.h"
#include <set>
#include <algorithm>

class Relation {

private:
    string relationName;
    Header relationHeader;

public:
    set<Tuple> tupleSet;

    Relation() = default;

    Relation(std::string relation_name) {
        relationName = move(relation_name);
    }

    Relation(string name, Header header) {
        relationName = name;
        relationHeader = header;
    }

    Relation(string relation_name, set<Tuple> tupleSet, Header relation_header) {
        relationName = move(relation_name);
        relationHeader = move(relation_header);
        for ( auto &tuple : tupleSet) {
            addTuple(tuple);
        }
    }

    Relation join(Relation& relationToJoinWith) {
        Relation relationToReturn;
        relationToReturn.relationName = this->relationName;
        Header newHeader = joinHeaders(relationHeader, relationToJoinWith.relationHeader);
        relationToReturn.addRelationHeader(newHeader);

        for (auto &tuple1 : tupleSet) {
            for (auto &tuple2 : relationToJoinWith.tupleSet) {
                if (joinable(relationHeader, relationToJoinWith.relationHeader, tuple1, tuple2)) {
                    relationToReturn.addTuple(combineTuples(relationHeader,
                        relationToJoinWith.relationHeader, tuple1, tuple2));
                }
            }
        }

        return relationToReturn;
    }

    Header joinHeaders(Header& one, Header& two) {
        vector<string> vectorToHeader = one.getHeader();
        bool seen = false;
        for (unsigned int i = 0; i < two.getHeader().size(); i++) {
            seen = false;
            for(unsigned j = 0; j < one.getHeader().size(); j++) {
                if (two.getHeader().at(i) == one.getHeader().at(j)) {
                    seen = true;
                    break;
                }
            }
            if (!seen) {
                vectorToHeader.push_back(two.getHeader().at(i));
            }
        }
        return Header(vectorToHeader);
    }

    Tuple combineTuples(Header headerOne, Header headerTwo, const Tuple& tuple1, const Tuple& tuple2) {
        Tuple t = tuple1;
        vector<unsigned int> uniqueIndexesInTwo = getUniqueIndexes(headerOne, headerTwo);
        for (unsigned int i = 0; i < tuple2.getTupleVector().size(); i++) {
            for (unsigned int j = 0; j < uniqueIndexesInTwo.size(); j++) {
                if (i == uniqueIndexesInTwo.at(j)) {
                    t.addValueToTuple(tuple2.getTupleVector().at(i));
                }
            }
        }
        return t;
    }

    vector<unsigned int> getUniqueIndexes(Header headerOne, Header headerTwo) {
        vector<unsigned int> vectorToReturn;
        vector<unsigned int> commonIndices1, commonIndices2;
        for (unsigned int i = 0; i < headerTwo.getHeader().size(); i++) {
            for (unsigned int j = 0; j < headerOne.getHeader().size(); j++) {
                if (headerOne.getHeader().at(j) == headerTwo.getHeader().at(i)) {
                    commonIndices1.push_back(j);
                    commonIndices2.push_back(i);
                }
            }
        }
        for (unsigned int i = 0; i < headerTwo.getHeader().size(); i++) {
            if (!vectorContains(commonIndices2, i)) {
                vectorToReturn.push_back(i);
            }
        }
        return vectorToReturn;
    }

    bool joinable(Header headerOne, Header headerTwo, const Tuple& tuple1, const Tuple& tuple2) {
        vector<unsigned int> commonIndices1, commonIndices2;
        for (unsigned int i = 0; i < headerTwo.getHeader().size(); i++) {
            for (unsigned int j = 0; j < headerOne.getHeader().size(); j++) {
                if (headerOne.getHeader().at(j) == headerTwo.getHeader().at(i)) {
                    commonIndices1.push_back(j);
                    commonIndices2.push_back(i);
                }
            }
        }
        for (unsigned int i = 0; i < commonIndices1.size(); i++) {
            if (tuple1.getTupleVector().at(commonIndices1.at(i)) != tuple2.getTupleVector().at(commonIndices2.at(i))) {
                return false;
            }
        }
        return true;
    }


    Relation select(int position, string value) {
        Relation tempRelation(relationName, relationHeader);
        for (auto tuple : tupleSet) {
            Tuple selected = tuple.select(position, value);
            if (!selected.empty()) {
                tempRelation.addTuple(selected);
            }
        }
        return tempRelation;
    }

    Relation select(int columnOne, int columnTwo) {
        Relation tempRelation(relationName, relationHeader);
        for (auto tuple : tupleSet) {
            Tuple selected = tuple.select(columnOne, columnTwo);
            if (!selected.empty()) {
                tempRelation.addTuple(selected);
            }
        }
        return tempRelation;
    }

    Relation select(vector<int> columns) {
        Relation tempRelation(relationName, relationHeader);
        for (auto tuple : tupleSet) {
            Tuple selected = tuple.select(columns);
            if (!selected.empty()) {
                tempRelation.addTuple(selected);
            }
        }
        return tempRelation;
    }


    Relation project(vector<int> columns) {
        Relation workingRelation1(relationName, relationHeader);
        for (auto &tuple : tupleSet) {
            Tuple projectedTuple = tuple.project(columns);
            workingRelation1.addTuple(projectedTuple);
        }
        return workingRelation1;
    }

    Relation projectForRules(vector<int> columns, vector<string> newHeader) {
        Relation workingRelation1(relationName, newHeader);
        for (auto &tuple : tupleSet) {
            Tuple projectedTuple = tuple.project(columns);
            workingRelation1.addTuple(projectedTuple);
        }
        return workingRelation1;
    }

    Relation rename(vector<string> newHeader) {
        Relation workingRelation(relationName, tupleSet, newHeader);
        return workingRelation;
    }

    string toString() {
        string strToReturn;
        if (!tupleSet.empty()) {
            bool printedNew = false;
            int setSize = tupleSet.size();
            if (setSize != 1) {
                strToReturn += "\n  ";
                printedNew = true;
            }
            for(const Tuple& t : tupleSet) {
                for (unsigned int i = 0; i < t.getTupleVector().size(); i++) {
                    if (tupleSet.size() == 1 && !printedNew) {
                        strToReturn += "\n  ";
                        printedNew = true;
                    }
                    strToReturn += relationHeader.getHeader().at(i) + "=" + t.getTupleVector().at(i);
                    if (i != t.getTupleVector().size() - 1) {
                        strToReturn += ", ";
                    }
                }
                setSize--;
                if (setSize > 0 && tupleSet.size() !=  1) {
                    strToReturn += "\n  ";
                }
            }
        }
        return strToReturn;
    }

    void addTuple(const Tuple& tupleToAdd) {
        tupleSet.insert(tupleToAdd);
    }

    void add(const vector<string>& stringVector) {
        addTuple(Tuple(stringVector));
    }

    void clearRelation() {
        tupleSet.clear();
    }

    bool empty() {
        return tupleSet.empty();
    }

    bool vacant() {
        return relationHeader.empty();
    }

    string sizeSet() {
        return to_string(tupleSet.size());
    }

    bool operator<(const Relation& r)const {
        return tupleSet > r.tupleSet;
    }

    void addRelationHeader(Header newHeader) {
        relationHeader = newHeader;
    }

    Header getRelationHeader() {
        return relationHeader;
    }

    void relationRename(string newName) {
        relationName = newName;
    }

    bool vectorContains(vector<unsigned int>& vectorToCheck, unsigned int element) {
        for (auto e : vectorToCheck) {
            if (e == element) {
                return true;
            }
        }
        return false;
    }

    Relation operator-(const Relation r)const {
        set<Tuple> setToReturn;
        for (auto tuple1 : tupleSet) {
            vector<string> currTupleVector = tuple1.getTupleVector();
            if (!tupleExists(currTupleVector, r.tupleSet)) {
                setToReturn.insert(tuple1);
            }
        }
        return Relation(relationName, setToReturn, relationHeader);
    }

    bool tupleExists(const vector<string>& tupleToCheck, const set<Tuple>& setOfTuplesToCheck)const {
        for (auto tuple : setOfTuplesToCheck) {
            if (equivalentVectors(move(tuple.getTupleVector()), tupleToCheck)) {
                return true;
            }
        }
        return false;
    }

    bool equivalentVectors(const vector<string>& v1, const vector<string>& v2) const {
        if (v1.size() != v2.size()) {
            return false;
        }
        bool foundDiscrepancy = false;
        for (unsigned int i = 0; i < v1.size(); i++) {
            if (v1.at(i) != v2.at(i)) {
                foundDiscrepancy = true;
                break;
            }
        }
        if (foundDiscrepancy) {
            return false;
        }
        return true;
    }


};


#endif //LEXER_RELATION_H
