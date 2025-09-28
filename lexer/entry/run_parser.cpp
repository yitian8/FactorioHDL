#include <iostream>
#include "token.h"
#include "scanner.h"
#include "parser.h"
int main() {
    std::string input{"../data/module_declaration.txt"};
    Scanner scanner{input};
    Parser parser{scanner};
    scanner.print();
    parser.print();
}