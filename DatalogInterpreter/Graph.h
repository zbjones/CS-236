//
// Created by zacha on 12/5/2020.
//

#ifndef LEXER_GRAPH_H
#define LEXER_GRAPH_H

#include "Node.h"
#include <string>
#include <map>
#include "Rule.h"

class Graph {

private:
    vector<Rule> rulesVector;
    map<int, Node> forwardGraph;
    map<int, Node> reverseGraph;
    vector<set<int>> SCC;
    vector<int> postOrderIndexes;

public:
    Graph() = default;

    Graph(vector<Rule> rules) {
        rulesVector = rules;
        createGraphs();
        //get postorder
        dfsForest();
        //evaluate the rules and get the SCC's
        dfsForestSCC();
    }


    void createGraphs() {
        //Create each node first
        for (unsigned int i = 0; i < rulesVector.size(); i++) {
            Node forward;
            Node reverse;
            forwardGraph.insert(pair<int, Node> (i, forward));
            reverseGraph.insert(pair<int, Node> (i, reverse));
        }

        //populate each node with its dependencies
        for (unsigned int i = 0; i < rulesVector.size(); i++) {
            addDependencies(i);
        }

    }

    void addDependencies(int ruleIndex) {
        Rule dependencyRule = rulesVector.at(ruleIndex);
        int count = 0;
        for (auto dependentRule : rulesVector) {
            for (auto predicate : dependentRule.getBodyPredicateList()) {
                if (dependencyRule.getHeadPredicate().getIdString() == predicate.getIdString()) {
                    forwardGraph.at(count).insertAdjacentNodes(ruleIndex);
                    reverseGraph.at(ruleIndex).insertAdjacentNodes(count);
                }
            }
            count++;
        }
    }

    string printGraph() {
        string strToReturn = "Dependency Graph\n";
        for (auto node : forwardGraph) {
            strToReturn += "R" + to_string(node.first) + ":" + node.second.printAdjacentNodes() + "\n";
        }
        strToReturn += "\n";
        return strToReturn;
    }

    void dfs(int nodeIndex) {
        Node& currentNode = reverseGraph.at(nodeIndex);
//        Node& forwardCurrentNode = forwardGraph.at(nodeIndex);
        currentNode.wasVisited();
        set<int> currAdjSet = currentNode.getAdjacentNodes();

        for (auto setIndex : currAdjSet) {
            if (reverseGraph.at(setIndex).hasBeenVisited() == false) {
                dfs(setIndex);
            }
        }
        currentNode.assignPostOrderNum(static_cast<int>(postOrderIndexes.size()));
//        forwardCurrentNode.assignPostOrderNum(static_cast<int>(postOrderIndexes.size()));
        postOrderIndexes.push_back(nodeIndex);
    }

    void dfsForest() {
        for (auto node : reverseGraph) {
            if (node.second.hasBeenVisited() == false) {
                dfs(node.first);
            }
        }
    }


    vector<int> getReversePostOrder() {
        vector<int> vectorToReturn;
        unsigned int postOrderSize = postOrderIndexes.size();
        for (unsigned int i = 0; i < postOrderSize; i++) {
            vectorToReturn.push_back(postOrderIndexes.at(postOrderSize - 1 - i));
        }
        return vectorToReturn;
    }

    void dfsForestSCC() {
        vector<int> reversePostOrder = getReversePostOrder();
        for (unsigned int i = 0; i < reversePostOrder.size(); i++) {
            set<int> sccSet;
            if (forwardGraph.at(reversePostOrder.at(i)).hasBeenVisited() == false) {
                dfsSCC(reversePostOrder.at(i), sccSet);
            }
            SCC.push_back(sccSet);
        }
    }

    void dfsSCC(int currIndex, set<int>& sccSetPassed) {
        Node& currentNode = forwardGraph.at(currIndex);
        currentNode.wasVisited();
        set<int> currAdjNodes = currentNode.getAdjacentNodes();
        for (auto setIndex : currAdjNodes) {
            if (forwardGraph.at(setIndex).hasBeenVisited() == false) {
                dfsSCC(setIndex, sccSetPassed);
            }
        }
        sccSetPassed.insert(currIndex);
    }

    vector<set<int>> getSCCVector() {
        return SCC;
    }



};


#endif //LEXER_GRAPH_H
