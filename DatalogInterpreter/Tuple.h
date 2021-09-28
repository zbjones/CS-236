//
// Created by zacha on 10/24/2020.
//

#ifndef LEXER_TUPLE_H
#define LEXER_TUPLE_H

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

class Tuple {

private:
    vector<string> tupleVector;

public:
    Tuple() = default;
    Tuple(vector<string>& tuples) {
        tupleVector = move(tuples);
    }
    Tuple(const vector<string>& tuples) {
        tupleVector = move(tuples);
    }
    string toString() {
        string strToReturn;
        for (unsigned int i = 0; i < tupleVector.size(); i++) {
            strToReturn += tupleVector.at(i);
            if (i != tupleVector.size() - 1) {
                strToReturn += ", ";
            }
        }
        return strToReturn;
    }

    const vector<string>& getTupleVector() const {
        return tupleVector;
    }

    bool operator<(const Tuple& tuple) const {
        return this->tupleVector < tuple.tupleVector;
    }

    Tuple select(int columnOne, string value) {
        if (tupleVector.at(columnOne) == value) {
            return Tuple(tupleVector);
        }
        return Tuple();
    }

    Tuple select(int columnOne, int columnTwo) {
        if (tupleVector.at(columnOne) == tupleVector.at(columnTwo)) {
            return Tuple(tupleVector);
        }
        return Tuple();
    }

    Tuple select(vector<int> columns)  {
        // Find all column values
        vector<std::string> values;
        for (auto &column : columns) {
            if (column >= 0 && column < static_cast<int>(tupleVector.size())) {
                values.push_back(tupleVector.at(column));
            }
        }
        // If all column values are equal
        if (adjacent_find(values.begin(), values.end(), not_equal_to<string>()) == values.end()) {
            return Tuple(tupleVector);
        }
        return Tuple();
    }

    Tuple project(vector<int> columns) const {
        vector<string> tempVector;
        for (auto &column : columns) {
            tempVector.push_back(tupleVector[column]);
        }
        return Tuple(tempVector);
    }

    void addValueToTuple(string value) {
        tupleVector.push_back(value);
    }

    bool empty() {
        return tupleVector.empty();
    }

};


#endif //LEXER_TUPLE_H
