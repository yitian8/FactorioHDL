#include "parser.h"
#include "magic_enum.hpp" 

Parser::Parser(Scanner& source) : tokens_(source.getToken()) {
    length_ = tokens_.size();
    root_ = parse_program();
}


Parser::~Parser()
{
    clear(root_);
}

void Parser::print() const {
    if (root_ == nullptr) {
        std::cout << "Empty tree" << std::endl;
        return;
    }
    printNode(root_, "", true);
}

void Parser::printNode(CFGnode* node, std::string prefix, bool isLast) const {
    std::cout << prefix;
    std::cout << (isLast ? "└── " : "├── ");
    
    // Print the current node
    std::cout << getNodeDescription(node) << std::endl;
    
    // Get children
    auto children = node->getChildren();
    
    // Prepare prefix for children
    std::string childPrefix = prefix + (isLast ? "    " : "│   ");
    
    // Print children
    for (size_t i = 0; i < children.size(); ++i) {
        printNode(children[i], childPrefix, i == children.size() - 1);
    }
}

std::string Parser::getNodeDescription(CFGnode* node) const {
    if (node->node_type_ == 0) { // NonTerminal
        auto* nonterm = static_cast<CFGnonTerminal*>(node);
        return "NonTerminal: " + std::string(magic_enum::enum_name(nonterm->type_));
    } 
    else if (node->node_type_ == 1) { // Terminal
        auto* term = static_cast<CFGterminal*>(node);
        return "Terminal: " + term->token_.getLiteral();
    }
    else if (node->node_type_ == 2) { // Error
        return "Error Node";
    }
    return "Unknown Node Type";
}


void Parser::clear(CFGnode* root)
{
    if (root->node_type_ == 1) {
        delete root;
    } else {
        for (const auto& t : root->getChildren()) {
            clear(t);
        }
    }
}


 
bool Parser::match(TokenType expected) {
    return !eof() && top().getType() == expected;
}


bool Parser::match(std::set<TokenType> expected)
{
    return !eof() && expected.contains(top().getType());
}


Token Parser::consume(TokenType expected) {
    if (match(expected)) {
        Token t = top();
        pop();
        return t;
    } else {
        std::string str{magic_enum::enum_name(expected)};
        throw std::logic_error("Unexpected token \"" + top().getLiteral() 
            + "\" at line " + std::to_string(top().getLine()) + ". Expected token: "+ 
            str);
    }
}


Token Parser::consume(std::set<TokenType> expected)
{
    if (!eof() && expected.contains(top().getType())) {
        Token t = top();
        pop();
        return t;
    }
    std::string str;
    for (TokenType t : expected) {
        std::string tmp{magic_enum::enum_name(t)};
        str += (tmp + " | ");
    }
    throw std::logic_error("Unexpected token \"" + top().getLiteral() 
            + "\" at line " + std::to_string(top().getLine()) + ". Expected token: "+ 
            str);
}


void Parser::CFGnonTerminal::add(const Token& token) {
    // Create a new terminal node
    CFGterminal* terminal = new CFGterminal(token);
    // Add it to children
    children_.push_back(terminal);
}

void Parser::CFGnonTerminal::add(const TokenType& type) {
    // Create a new non-terminal node
    CFGnonTerminal* nonterm = new CFGnonTerminal(type);
    // Add it to children
    children_.push_back(nonterm);
}

void Parser::CFGnonTerminal::add(Parser::CFGnode* node) {
    children_.push_back(node);
}

void Parser::CFGterminal::add(const Token& token) {
    // Store the token
    token_ = token;
}

void Parser::CFGterminal::add(const TokenType& type) {
    // Terminals shouldn't add token types
    throw std::logic_error("Cannot add TokenType to terminal node");
}

void Parser::CFGterminal::add(Parser::CFGnode* node) {
    throw std::logic_error("Cannot add another CFG node to terminal node");
}


Parser::CFGerror::CFGerror(std::vector<Token> tokens) : CFGnode(2) {
    for (Token t : tokens) {
        add(t);
    }
}



void Parser::CFGerror::add(CFGnode* node) {
    if (node->node_type_ == 1) {
        children_.push_back(node);
    } else {
        throw std::logic_error("Cannot add a non terminal node to an error node.");
    }
}


void Parser::CFGerror::add(const Token& token) {
    // Create a new terminal node
    CFGterminal* terminal = new CFGterminal(token);
    // Add it to children
    children_.push_back(terminal);
}


void Parser::CFGerror::add(const TokenType& type) {
    throw std::logic_error("Cannot add a non terminal node to an error node.");
}

void Parser::report_error(Token T, std::string msg) {
    throw std::logic_error("Unexpected token on"); 
}


bool Parser::add_terminal(Parser::CFGnode* node, std::set<TokenType> expected_token, std::set<TokenType> fallback)
{  
    try
    {
        node->add(consume(expected_token));
        return true;
    }
    catch(const std::exception& e)
    {
        error_stack.push_back(e);
        node->add(sync(fallback));
        return false;
    }
}
 
inline std::vector<Parser::CFGnode*> Parser::CFGnonTerminal::getChildren() {
    return children_;
}

inline std::vector<Parser::CFGnode*> Parser::CFGterminal::getChildren() {
    return std::vector<Parser::CFGnode*>{};
}

inline std::vector<Parser::CFGnode*> Parser::CFGerror::getChildren() {
    return children_;
}

Parser::CFGnode* Parser::sync(std::set<TokenType> sync_words) {
    std::vector<Token> result;
    while (!eof() && !sync_words.contains(top().getType())) {
        result.push_back(top());
        pop();
    }
    CFGnode* error = new CFGerror(result);
    return error;
}


Parser::CFGnode* Parser::parse_program() {
    CFGnode* result = new CFGnonTerminal(program);
    while (!eof()) {
        if (match(MODULE)) {
            result->add(parse_module_block());
        } else if (match(ENTITY)) {
            // result->add(parse_entity_block());
        } else {
            result->add(sync({MODULE, ENTITY}));
        }
    }
    return result;
}

Parser::CFGnode* Parser::parse_module_block() {
    CFGnode* result = new CFGnonTerminal(module_block);
    result->add(consume(MODULE));
    result->add(parse_module_declaration());
    try {
        result->add(consume(COLON));
    } catch(const std::exception& e) {
        error_stack.push_back(e);
        result->add(sync({MODULE, ENTITY}));
        return result;
    }
    // result->add(parse_module_statement_list());
    try {
        result->add(consume(ENDMODULE));
    } catch(const std::exception& e) {
        error_stack.push_back(e);
        result->add(sync({MODULE, ENTITY}));
        return result;
    }
    return result;
}


Parser::CFGnode* Parser::parse_module_declaration() {
    CFGnode* result = new CFGnonTerminal(module_declaration);
    if (!add_terminal(result, {IDENTIFIER}, {COLON, MODULE, ENTITY, ENDMODULE})) return result;
    if (!add_terminal(result, {LEFT_PAREN}, {COLON, MODULE, ENTITY, ENDMODULE})) return result;
    result->add(parse_module_io_list());
    if (!add_terminal(result, {RIGHT_PAREN}, {COLON, MODULE, ENTITY, ENDMODULE})) return result;
    return result;
}


Parser::CFGnode* Parser::parse_module_io_list()
{
    CFGnode* result = new CFGnonTerminal(module_io_list);
    result->add(parse_declaration_list());
    if (!add_terminal(result, {ARROW}, {RIGHT_PAREN, SEMICOLON, MODULE, ENTITY, ENDMODULE})) return result;
    result->add(parse_declaration_list());
    return result;
}

Parser::CFGnode* Parser::parse_declaration_list()
{
    CFGnode* result = new CFGnonTerminal(declaration_list);
    result->add(parse_declaration_statement());
    if (!add_terminal(result, {SEMICOLON}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) {
        return result;
    }
    while (!match({RIGHT_PAREN, ARROW})) {
        result->add(parse_declaration_statement());
        if (!add_terminal(result, {SEMICOLON}, {RIGHT_PAREN, ARROW, SEMICOLON, MODULE, ENTITY, ENDMODULE})) {
            return result;
        }
    }
    return result;
}

Parser::CFGnode* Parser::parse_declaration_statement()
{
    CFGnode* result = new CFGnonTerminal(declaration_statement);
    if (!add_terminal(result, {BELT, PIPE, HEAT_PIPE, IDENTIFIER}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) {
        return result;
    }
    result->add(parse_bus_operator());
    result->add(parse_identifier_list_no_bus());
    if (match(COLON)) {
        if (!add_terminal(result, {COLON}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) {
            return result;
        }
        result->add(parse_param_list());
    }
    return result;
}


Parser::CFGnode* Parser::parse_bus_operator()
{
    CFGnode* result = new CFGnonTerminal(bus_operator);
    if (match({COLON, IDENTIFIER, ARROW, SEMICOLON, COMMA})) {
        return result;
    } else {
        if (!add_terminal(result, {LEFT_BRACKET}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) return result;
        if (!add_terminal(result, {NUMBER}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) return result;
        if (match(RIGHT_BRACKET)) {
            result->add(consume(RIGHT_BRACKET));
            return result;
        } else if (match(COLON)) {
            result->add(consume(COLON));
            if (!add_terminal(result, {NUMBER}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) return result;
            if (!add_terminal(result, {RIGHT_BRACKET}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) return result;
        }
    }
    return result;
}


Parser::CFGnode* Parser::parse_identifier_list_no_bus()
{
    CFGnode* result = new CFGnonTerminal(identifier_list_no_bus);
    if (!add_terminal(result, {IDENTIFIER}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) return result;
    while (match(COMMA)) {
        if (!add_terminal(result, {COMMA}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) return result;
        if (!add_terminal(result, {IDENTIFIER}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) return result;
    }
    return result;
}


Parser::CFGnode* Parser::parse_param_list()
{
    CFGnode* result = new CFGnonTerminal(param_list);
    if (match(SEMICOLON)) {
        return result;
    }
    result->add(parse_param_assignment());
    while (!match(SEMICOLON)) {
        if (!add_terminal(result, {COMMA}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) return result;
        result->add(parse_param_assignment());
    }
    return result;
}


Parser::CFGnode* Parser::parse_param_assignment()
{
    CFGnode* result = new CFGnonTerminal(param_assignment);
    if (!add_terminal(result, {STRING}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) return result;
    if (!add_terminal(result, {EQUAL}, {SEMICOLON, MODULE, ENTITY, ENDMODULE})) return result;
    add_terminal(result, {STRING, NUMBER}, {SEMICOLON, MODULE, ENTITY, ENDMODULE});
    return result;
}
