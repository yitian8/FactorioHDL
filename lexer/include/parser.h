#pragma once
#include "scanner.h"
#include <set>

class Parser {
    public:
        Parser(Scanner& source);
        ~Parser();
        void print() const;
    private:
        class CFGnode {
            public:
                CFGnode(int i) : node_type_(i){}
                virtual ~CFGnode() = default;
                virtual void add(const Token& token) = 0;
                virtual void add(const TokenType& type) = 0;
                virtual void add(CFGnode* node) = 0;
                virtual std::vector<CFGnode*> getChildren() = 0;
                const int node_type_;
        };

        class CFGnonTerminal : public CFGnode {
            public:
                CFGnonTerminal(TokenType type):type_(type), CFGnode(0){}

                void add(CFGnode* node) override;
                void add(const Token& token) override;
                void add(const TokenType& type) override;
                std::vector<CFGnode*> getChildren() override;
                std::vector<CFGnode*> children_;
                TokenType type_;
        };

        class CFGterminal : public CFGnode {
            public:
                CFGterminal(Token token):token_(token), CFGnode(1){}

                void add(CFGnode* node) override;
                void add(const Token& token) override;
                void add(const TokenType& type) override;
                std::vector<CFGnode*> getChildren() override;
                Token token_;
        };

        class CFGerror : public CFGnode {
            public:
                CFGerror():CFGnode(2){}
                CFGerror(std::vector<Token> tokens); 
                void add(CFGnode* node) override;
                void add(const Token& token) override;
                void add(const TokenType& type) override;
                std::vector<CFGnode*> getChildren() override;
                std::vector<CFGnode*> children_;
        };

        std::vector<Token> tokens_;
        CFGnode* root_;
        size_t length_;
        size_t curr_ = 0;

        std::vector<std::exception> error_stack;

        Token top() {return tokens_[curr_];}
        void pop() {curr_++;}
        bool eof() {return curr_ >= length_;}
        
        // Helper functions

        void printNode(CFGnode* node, std::string prefix, bool isLast) const;
        std::string getNodeDescription(CFGnode* node) const;

        // Clears the CFG tree rooted at root
        void clear(CFGnode* root);

        // Check if the current token matches the expected type
        bool match(TokenType expected);
        bool match(std::set<TokenType> expected);
        
        // Also checks if the current token matches the expected type, but consumes the token and returns it
        // Throws errors
        Token consume(TokenType expected);
        Token consume(std::set<TokenType> expected);

        bool add_terminal(CFGnode* node, std::set<TokenType> expected_token, std::set<TokenType> fallback);

        CFGnode* sync(std::set<TokenType> sync_words);

        void report_error(Token T, std::string msg);

        // Functions for implementing CFG


        CFGnode* parse_program();
        CFGnode* parse_module_block();
        CFGnode* parse_entity_block();

        CFGnode* parse_module_declaration();
        CFGnode* parse_module_statement_list();

        CFGnode* parse_module_io_list();

        CFGnode* parse_declaration_list();
        CFGnode* parse_declaration_statement();
        CFGnode* parse_bus_operator();
        CFGnode* parse_identifier_list_no_bus();
        CFGnode* parse_param_list();
        CFGnode* parse_param_assignment();
};