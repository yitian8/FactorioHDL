#include <iostream>
#include "graph.h"
#include "node.h"

int main() {
    Graph g{};
    size_t t1 = g.createNode<Transportation>("belt");
    size_t t2 = g.createNode<Transportation>("belt");
    size_t f1 = g.createNode<Factory>("assembler");
    size_t ins1 = g.createNode<Transfer>("inserter");
    size_t ins2 = g.createNode<Transfer>("inserter");

    // Transportation t1("belt");
    // Transportation t2("belt");
    // Factory f1("assembler");
    bool b0 = g.connect(f1, t2);
    bool b1 = g.connect(t1, t2);
    // Transfer ins1("inserter");
    // Transfer ins2("inserter");
    bool b2 = g.connect(ins1, t1);
    bool b3 = g.connect(ins2, t1);
    bool b5 = g.remove(t2);
    bool b6 = g.remove(34);
    bool b7 = g.remove(t2);
    std::cout <<b0<<b1<<b2<<b3<<b5<<b6<<b7<<std::endl;
    std::cout <<t1<<t2<<f1<<ins1<<ins2<< std::endl;
    return 0;
}

