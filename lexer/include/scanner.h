#pragma once
#include <string>
#include <vector>
#include <map>
#include "token.h"

class Scanner {
    public:
        Scanner() = default;
        Scanner(std::string input_path);
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
            {"assembling_machine", ASSEMBLING_MACHINE},
            {"chemical_plant", CHEMICAL_PLANT},
            {"oil_refinery", OIL_REFINERY},
            {"train_stop", TRAIN_STOP},
            {"inserter", INSERTER},
            {"belt", BELT},
            {"pipe", PIPE},
            {"heat_pipe", HEAT_PIPE},
            {"boiler", BOILER},
            {"steam_engine", STEAM_ENGINE},
            {"offshore_pump", OFFSHORE_PUMP},
            {"pumpjack", PUMPJACK},
            {"furnace", FURNACE},
            {"rocket_silo", ROCKET_SILO},
            {"lab", LAB},
            {"centrifuge", CENTRIFUGE},
            {"pump", PUMP},
            {"storage_tank", STORAGE_TANK},
            {"chest", CHEST},
            {"splitter", SPLITTER},
            {"type", TYPE},
            {"filter", FILTER},
            {"recipe", RECIPE},
            {"input_priority", INPUT_PRIORITY},
            {"output_priority", OUTPUT_PRIORITY}
        };
};