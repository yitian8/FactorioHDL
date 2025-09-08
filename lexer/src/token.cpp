#include "token.h"
#include "magic_enum.hpp"
#include <format>

std::string Token::getLiteral() const {
    return literal_;
}

Token::Token(std::string literal, TokenType type, unsigned int line) : type_(type), line_(line){
    literal_ = literal;
}


std::ostream& operator<<(std::ostream& os, const Token& T) {
    os << std::format("{:<20}", T.getLiteral()) 
        << std::format("{:<18}", magic_enum::enum_name(T.getType())) 
        << std::format("{:<5}", std::to_string(T.getLine()));
    return os;
}

