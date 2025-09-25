#pragma once
#include <string>
#include <vector>
#include <map>
#include "token.h"

class Scanner {
    public:
        Scanner() = default;
        Scanner(std::string input_path);
        const std::vector<Token>& getToken() const;
        void print();

    private:
        void scanTokens();
        void scanToken();
        bool eof() {return curr_ >= source_.size();};
        void addToken(std::string literal, TokenType type, unsigned int line);

        //Returns the current character, and adds 1 to curr_
        char pop();

        //Returns the current character, but does not add 1 to curr_
        char peek();

        void forward();

        std::string findString();

        std::string findNumber();

        std::string findIdentifier();


        std::string source_;
        std::vector<Token> tokens_;
        size_t curr_ = 0;
        unsigned int line_ = 1;

        const std::map<std::string, TokenType> token_map_ = {
            {"belt", BELT},
            {"pipe", PIPE},
            {"heat_pipe", HEAT_PIPE},
            {"module", MODULE},
            {"None", NONE},
            {"endmodule", ENDMODULE},
            {"entity", ENTITY},
            {"endentity", ENDENTITY},
            {"building", BUILDING},
            {"param", PARAM},
            {"port", PORT}
        };
};
 