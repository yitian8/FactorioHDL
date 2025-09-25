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
    std::cout << "Total characters: "<<source_.size() << std::endl;
    while (!eof()) {
        // std::cout << curr_ << std::endl;
        scanToken();
    }
}

void Scanner::scanToken() {
begin:
    if (eof()) {
        return;
    }
    char c = peek();
    switch (c) {
        case '(': addToken("(", TokenType::LEFT_PAREN, line_); break;
        case ')': addToken(")", TokenType::RIGHT_PAREN, line_); break;
        case '{': addToken("{", TokenType::LEFT_BRACES, line_); break;
        case '}': addToken("}", TokenType::RIGHT_BRACES, line_); break;
        case '[': addToken("[", TokenType::LEFT_BRACKET, line_); break;
        case ']': addToken("]", TokenType::RIGHT_BRACKET, line_); break;
        case ',': addToken(",", TokenType::COMMA, line_); break;
        case ';': addToken(";", TokenType::SEMICOLON, line_); break;
        case ':': addToken(":", TokenType::COLON, line_); break;
        case '=': addToken("=", TokenType::EQUAL, line_); break;
        case '+': addToken("+", TokenType::PLUS, line_); break;
        case '-':
            forward();
            if (peek() == '>') {
                addToken("->", TokenType::ARROW, line_); break;
            } else {
                addToken("-", TokenType::MINUS, line_);
                goto begin;
            }
            
        case '*': addToken("*", TokenType::STAR, line_); break;
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
            // std::cout <<curr_<<std::endl;
            // std::cout <<peek()<<std::endl;
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
                throw std::runtime_error("unexpected token \'" +std::to_string(peek())+"\' on line " + std::to_string(line_));
            }
            
            goto begin;
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
    // std::cout << literal << std::endl;
    Token tk{literal, type, line};
    tokens_.push_back(tk);
}

void Scanner::forward() {
    curr_++;
}

void Scanner::print() {
    std::cout << "Total tokens: "<<tokens_.size() << std::endl;
    for (const Token& t : tokens_) {
        std::cout << t << std::endl;
    }
}

const std::vector<Token>& Scanner::getToken() const {
    return tokens_;
}
