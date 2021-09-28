//
// Created by zacha on 12/5/2020.
//

#ifndef LEXER_NODE_H
#define LEXER_NODE_H

#include <string>
#include <vector>
#include <set>

using namespace std;


class Node {

private:
//    int nodeID;
    int postOrderNum;
    bool visited;
    set<int> dependencies;

public:

    Node() {
//        nodeID = = -1;
        postOrderNum = -1;
        visited = false;
    }
    Node(int ruleID) {
//        nodeID = ruleID;
        postOrderNum = -1;
        visited = false;
    }


    ~Node() {
        dependencies.clear();
    }

    void wasVisited() {
        visited = true;
    }

    void assignPostOrderNum(int num) {
        postOrderNum = num;
    }

    void insertAdjacentNodes(int ruleIndex) {
        dependencies.insert(ruleIndex);
    }

    set<int> getAdjacentNodes() {
        return dependencies;
    }

    string printAdjacentNodes() {
        string strToReturn;
        int count = 0;
        for (auto node : dependencies) {
            count++;
            if (count > 1) {
                strToReturn += ",";
            }
            strToReturn += "R" + to_string(node);
        }
        return strToReturn;
    }

//    int getNodeID() {
//        return nodeID;
//    }

    int getPostOrderNum() {
        return postOrderNum;
    }

    bool hasBeenVisited() {
        return visited;
    }
};


#endif //LEXER_NODE_H
