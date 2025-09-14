#include <iostream>
#include "graph.h"
#include "node.h"

bool Graph::connect(Entity& input, Entity& output) {
    if ((!node_list_.contains(input.getID())) || (!node_list_.contains(output.getID()))) {
        return false;
    }
    try {
        input.addInput(output);
        output.addOutput(input);
    } catch (const std::logic_error& e) {
        std::cerr<< e.what() << std::endl;
        return false;
    }
    return true;
}

bool Graph::connect(size_t id_input, size_t id_output) {
    if ((!node_list_.contains(id_input)) || (!node_list_.contains(id_output))) {
        return false;
    }
    try {
        node_list_[id_input] -> addInput(*node_list_[id_output]);
        node_list_[id_output] -> addOutput(*node_list_[id_input]);
    } catch (const std::logic_error& e) {
        std::cerr<< e.what() << std::endl;
        return false;
    }
    return true;
}

bool Graph::disconnect(size_t id_input, size_t id_output) {
    if ((!node_list_.contains(id_input)) || (!node_list_.contains(id_output))) {
        return false;
    }
    try {
        node_list_[id_input] -> removeInput(*node_list_[id_output]);
        node_list_[id_output] -> removeOutput(*node_list_[id_input]);
    } catch(const std::logic_error& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool Graph::disconnect(Entity& input, Entity& output) {
    if ((!node_list_.contains(input.getID())) || (!node_list_.contains(output.getID()))) {
        return false;
    }
    try {
        input.removeInput(output);
        output.removeOutput(input);
    } catch (const std::logic_error& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }
    return true;
}

bool Graph::remove(size_t id) {
    if (!node_list_.contains(id)) {
        return false;
    }
    Entity& n = *node_list_[id];
    for (auto& [key, value] : n.input_) {
        value->removeOutput(n);
    }
    for (auto& [key, value] : n.output_) {
        value->removeInput(n);
    };
    node_list_.erase(id);
    return true;
}