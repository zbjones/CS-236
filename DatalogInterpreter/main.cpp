#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Interpreter.h"

using namespace std;


int main(int argc, char* argv[]) {

    string fileInput;

    ifstream in;
    in.open(argv[1]);
    if(in) {
        stringstream ss;
        ss << in.rdbuf();
        fileInput = ss.str();
    }

    Interpreter interpreter(fileInput);
    interpreter.run();


    return 0;
}
