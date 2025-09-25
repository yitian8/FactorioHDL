
#include "token.h"
#include "magic_enum.hpp"
#include <iomanip>

std::string Token::getLiteral() const {
    return literal_;
}

Token::Token(std::string literal, TokenType type, unsigned int line) : type_(type), line_(line){
    literal_ = literal;
}


std::ostream& operator<<(std::ostream& os, const Token& T) {
    std::ios_base::fmtflags flags(os.flags());
    os << std::left 
       << std::setw(20) << T.getLiteral()
       << std::setw(18) << magic_enum::enum_name(T.getType())
       << std::setw(5) << std::to_string(T.getLine());
    os.flags(flags);
    return os;
}

