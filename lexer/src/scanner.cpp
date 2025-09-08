#include "scanner.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <cctype>
Scanner::Scanner(std::string input_path) {
    std::ifstream ifs {input_path};
    if (!ifs.is_open()) {
        throw std::runtime_error("Critical error: cannot open input file.");
    }
    while (!ifs.eof()) {
        if (ifs.good()) {
            char c = ifs.get();
            // std::cout << c;
            if (!ifs.eof()) {
                source_ += c;
            }
        } else {
            throw std::runtime_error("Critical error: input file broken");
        }
    }
    // std::cout << source_ << std::endl;
    scanTokens();
}

void Scanner::scanTokens() {
    curr_ = 0;
    tokens_.clear();
    line_ = 1;
    std::cout << source_.size() << std::endl;
    while (!eof()) {
        // std::cout << curr_ << std::endl;
        scanToken();
    }
}

void Scanner::scanToken() {
    char c = peek();
    // std::cout << c << std::endl;
    switch (c) {
        case '(': addToken("(", TokenType::LEFT_PAREN, line_); break;
        case ')': addToken(")", TokenType::RIGHT_PAREN, line_); break;
        case '{': addToken("{", TokenType::LEFT_BRACES, line_); break;
        case '}': addToken("}", TokenType::RIGHT_BRACES, line_); break;
        case '[': addToken("[", TokenType::LEFT_BRACKET, line_); break;
        case ']': addToken("]", TokenType::RIGHT_BRACKET, line_); break;
        case ',': addToken(",", TokenType::COMMA, line_); break;
        case ';': addToken(";", TokenType::SEMICOLON, line_); break;
        case '=': addToken("=", TokenType::EQUAL, line_); break;
        case '+': addToken("+", TokenType::PLUS, line_); break;
        case '/':
            if (pop() == '/') {
                while(!eof() && peek() != '\n') forward();
                line_++;
            } else {
                throw std::runtime_error("Invalid syntax at line " + std::to_string(line_) + ": single '/' character");
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;

        case '\n':
            line_++;
            break;
        case '"':
            addToken(findString(), STRING, line_); break;
        default:
            if (std::isdigit(peek())) {
                addToken(findNumber(), NUMBER, line_);
            } else if (std::isalpha(peek())) {
                std::string result = findIdentifier();
                // std::cout <<result << std::endl;
                if (token_map_.contains(result)) {
                    addToken(result, token_map_.at(result), line_);
                } else {
                    addToken(result, IDENTIFIER, line_);
                }
                
            } else {
                std::string s;
                s+= c;
                throw std::runtime_error("unexpected token " +s+" on line " + std::to_string(line_));
            }
            break;
    }
    forward();
}

std::string Scanner::findIdentifier() {
    std::string output;
    while(!eof() && (std::isdigit(peek()) || std::isalpha(peek()) || peek() == '_' || peek() == '-')) {
        // std::cout << peek();
        output += peek();
        forward();
    }
    // std::cout << '\n';
    return output;
}



std::string Scanner::findNumber() {
    std::string output;
    while(!eof() && isdigit(peek())) {
        output += peek();
        forward();
    }
    return output;
}

std::string Scanner::findString() {
    std::string output;
    forward();
    while(!eof() && peek() != '"') {
        if (peek() == '\n') {
            line_++;
        }
        output += peek();
        forward();
    }
    if (eof()) {
        throw std::runtime_error("Critical error: non-terminated string at line " + std::to_string(line_));
    }
    return output;
}

char Scanner::pop() {
    if (curr_ < source_.size()) {
        return source_[curr_++];
    }
    return '\0';
}

char Scanner::peek() {
    if (curr_ < source_.size()) {
        return source_[curr_];
    }
    return '\0';
}

void Scanner::addToken(std::string literal, TokenType type, unsigned int line) {
    std::string classtype = classifyToken(type);
    if (classtype == "Operation signs") {
        OperationSign t{literal, type, line};
        tokens_.push_back(t);
    } else if (classtype == "Literals") {
        Literal t{literal, type, line};
        tokens_.push_back(t);
    } else if (classtype == "Buildings") {
        Building t{literal, type, line};
        tokens_.push_back(t);
    } else {
        Keyword t{literal, type, line};
        tokens_.push_back(t);
    }
}

void Scanner::forward() {
    curr_++;
}

void Scanner::print() {
    std::cout << tokens_.size() << std::endl;
    for (const Token& t : tokens_) {
        std::cout << t << std::endl;
    }
}

OperationSign::OperationSign(std::string literal, TokenType type, unsigned int line) :
    Token(literal, type, line){}

Literal::Literal(std::string literal, TokenType type, unsigned int line) :
    Token(literal, type, line){}

Building::Building(std::string literal, TokenType type, unsigned int line) :
    Token(literal, type, line){}

Keyword::Keyword(std::string literal, TokenType type, unsigned int line) :
    Token(literal, type, line){}

std::string classifyToken(TokenType token) {
    switch (token) {
        // Operation Signs
        case LEFT_PAREN:
        case RIGHT_PAREN:
        case LEFT_BRACKET:
        case RIGHT_BRACKET:
        case LEFT_BRACES:
        case RIGHT_BRACES:
        case COMMA:
        case DOT:
        case MINUS:
        case PLUS:
        case SEMICOLON:
        case EQUAL:
            return "Operation signs";

        // Literals
        case IDENTIFIER:
        case STRING:
        case NUMBER:
            return "Literals";

        // Buildings
        case ASSEMBLING_MACHINE:
        case CHEMICAL_PLANT:
        case OIL_REFINERY:
        case TRAIN_STOP:
        case INSERTER:
        case BELT:
        case BOILER:
        case STEAM_ENGINE:
        case OFFSHORE_PUMP:
        case PUMPJACK:
        case FURNACE:
        case ROCKET_SILO:
        case LAB:
        case CENTRIFUGE:
        case PUMP:
        case STORAGE_TANK:
        case CHEST:
        case SPLITTER:
        case PIPE:
        case HEAT_PIPE:
            return "Buildings";

        // Keywords
        case TYPE:
        case FILTER:
        case RECIPE:
        case INPUT_PRIORITY:
        case OUTPUT_PRIORITY:
            return "Keywords";

        default:
            throw std::runtime_error("Unknown TokenType");
    }
}