#pragma once
#include "node.h"
#include "scanner.h"

class Parser {
    public:
        Parser(Scanner& source);
    private:
        std::vector<Token> tokens_;
    
};