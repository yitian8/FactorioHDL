#include <iostream>
#include "token.h"
#include "scanner.h"
int main() {
    std::string input{"../data/syntax.txt"};
    Scanner scanner{input};
    scanner.print();
    
}