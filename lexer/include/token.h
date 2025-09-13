#pragma once
#include <string>
#include <iostream>


enum TokenType {

    //Operation signs
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET, RIGHT_BRACKET, LEFT_BRACES, RIGHT_BRACES,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, EQUAL,

    //Literals
    IDENTIFIER, STRING, NUMBER,

    //Buildings
    ASSEMBLING_MACHINE, CHEMICAL_PLANT, OIL_REFINERY, TRAIN_STOP, INSERTER, BELT, BOILER,
    STEAM_ENGINE, OFFSHORE_PUMP, PUMPJACK, FURNACE, ROCKET_SILO, LAB, CENTRIFUGE, PUMP,
    STORAGE_TANK, CHEST, SPLITTER, PIPE, HEAT_PIPE,

    //Keywords
    TYPE, FILTER, RECIPE, INPUT_PRIORITY, OUTPUT_PRIORITY

};



class Token {
    public:
        Token(std::string literal, TokenType type, unsigned int line);
        std::string getLiteral() const;
        TokenType getType() const {return type_;};
        unsigned int getLine() const {return line_;};
    private:
        std::string literal_;
        const TokenType type_;
        const unsigned int line_;

};

class OperationSign : public Token {
    public: 
        OperationSign(std::string literal, TokenType type, unsigned int line);
};

class Literal : public Token {
    public: 
        Literal(std::string literal, TokenType type, unsigned int line);
};

class Building : public Token {
    public: 
        Building(std::string literal, TokenType type, unsigned int line);
};

class Keyword : public Token {
    public: 
        Keyword(std::string literal, TokenType type, unsigned int line);
};



std::ostream& operator<<(std::ostream& os, const Token& T);

