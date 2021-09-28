//
// Created by zacha on 10/24/2020.
//

#ifndef LEXER_HEADER_H
#define LEXER_HEADER_H

#include <vector>
#include <string>

using namespace std;


class Header {

private:
    vector<string> headerVector;

public:
    Header() {}

    Header(vector<string> attributes) {
        headerVector = attributes;
    }

    vector<string> getHeader() {
        return headerVector;
    }

    bool empty() {
        return headerVector.empty();
    }


};


#endif //LEXER_HEADER_H
