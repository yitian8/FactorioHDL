#pragma once
#include <string>
#include <iostream>


enum TokenType {

    //Operation signs
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACKET, RIGHT_BRACKET, LEFT_BRACES, RIGHT_BRACES,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, EQUAL, COLON, STAR, ARROW, 

    //Literals
    IDENTIFIER, STRING, NUMBER,

    //Hard coded buildings
    BELT, PIPE, HEAT_PIPE,

    //Keywords
    NONE, MODULE, ENDMODULE, ENTITY, ENDENTITY, BUILDING, PARAM, PORT,

    // Epsilon character
    EPSILON,

    //Non-terminals
    program, module_block, entity_block, module_declaration, module_statement_list, module_io_list,
    declaration_list, bus_operator, param_list,
    declaration_statement, identifier_list_no_bus, param_assignment,

};



class Token {
    public:
        Token(std::string literal, TokenType type, unsigned int line);
        Token() = delete;
        Token(const Token& rhs);
        Token& operator=(const Token& rhs);
        std::string getLiteral() const;
        TokenType getType() const {return type_;};
        unsigned int getLine() const {return line_;};
    private:
        std::string literal_;
        TokenType type_;
        unsigned int line_;

};

std::ostream& operator<<(std::ostream& os, const Token& T);

// class OperationSign : public Token {
//     public: 
//         OperationSign(std::string literal, TokenType type, unsigned int line);
// };

// class Literal : public Token {
//     public: 
//         Literal(std::string literal, TokenType type, unsigned int line);
// };

// class Building : public Token {
//     public: 
//         Building(std::string literal, TokenType type, unsigned int line);
// };

// class Keyword : public Token {
//     public: 
//         Keyword(std::string literal, TokenType type, unsigned int line); 
// };





