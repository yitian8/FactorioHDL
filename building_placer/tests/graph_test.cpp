#include <iostream>
#include "graph.h"
#include "node.h"

int main() {
    Transportation t1("belt");
    Transportation t2("belt");
    Factory f1("assembler");
    bool b0 = connect(f1, t2);
    bool b1 = connect(t1, t2);
    Transfer ins1("inserter");
    Transfer ins2("inserter");
    bool b2 = connect(ins1, t1);
    bool b3 = connect(ins2, t1);
    std::cout <<b0<<b1<<b2<<b3<<std::endl;
    std::cout <<t1.getID() << t2.getID() << f1.getID() << std::endl;
}