//
// Created by zacha on 10/24/2020.
//

#ifndef LEXER_INTERPRETER_H
#define LEXER_INTERPRETER_H

#include "Database.h"
#include "DatalogProgram.h"
#include "Lexer.h"
#include "Parser.h"
#include "Graph.h"

class Interpreter {

private:
    DatalogProgram datalog;
    Database database;
    string fileInput;
    Parser parserObj;
    Graph graphObj;

public:
    Interpreter() = default;
    Interpreter(string &input) {
            fileInput = move(input);
    }
    Interpreter(DatalogProgram& datalogProgram) {
        datalog = move(datalogProgram);
    }

    //Interpreter now runs the Lexer and Parser, converts Datalog to a database
    Database buildDatabase() {
        Lexer lexerObj(fileInput);
        lexerObj.run();
        parserObj.addTokenVector(lexerObj.getVector());
        datalog = parserObj.parse();
        graphObj = Graph(datalog.getRules());
        database = Database(datalog.getSchemes(), datalog.getFacts(), graphObj.getSCCVector());
        return database;
    }

    void evaluateQueries() {
        database.storeQueries(datalog.getQueries());
        cout <<  "Query Evaluation\n" << database.evaluateAllQueries();
    }

    //THIS IS WHERE I CAN NOW INSERT MY NEW FUNCTIONS TO FIND THE NEW RULES VECTOR
    void evaluateRules() {
        database.storeRules(datalog.getRules());
        cout << graphObj.printGraph();
        cout << "Rule Evaluation\n" << database.evaluateAllRulesWithGraph() << "\n";


    }

    //Builds database and evaluates all queries
    Database run() {
        buildDatabase();
        evaluateRules();
        evaluateQueries();
        return database;
    }

};


#endif //LEXER_INTERPRETER_H
