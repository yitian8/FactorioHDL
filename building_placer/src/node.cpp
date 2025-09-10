#include "node.h"

void InPort::addInput(Node& n) {
    throw std::logic_error("Cannot add input to an InPort");
}

void InPort::addOutput(Node& n) {
    output_.push_back(std::make_shared<Node>(n));
}

void OutPort::addInput(Node& n) {
    input_.push_back(std::make_shared<Node>(n));
}

void OutPort::addOutput(Node& n) {
    throw std::logic_error("Cannot add output to an OutPort");
}

void Entity::addInput(Node& n) {
    input_.push_back(std::make_shared<Node>(n));
}

void Entity::addOutput(Node& n) {
    output_.push_back(std::make_shared<Node>(n));
}

void Transportation::addInput(Node& n) {
    if (n.getType() == "InPort" || "Transfer" || "Transportation") {
        Entity::addInput(n);
    }
    throw std::logic_error("Cannot add " + n.getType() + " node to the input port of a " + getType() + " node");
}

void Transportation::addOutput(Node& n) {
    if (n.getType() == "OutPort" || "Transfer" || "Transportation") {
        Entity::addOutput(n);
    }
    throw std::logic_error("Cannot add " + n.getType() + " node to the output port of a " + getType() + " node");
}

void Factory::addInput(Node& n) {
    if (n.getType() == "Transfer") {
        Entity::addOutput(n);
    }
    throw std::logic_error("Cannot add " + n.getType() + " node to the input port of a " + getType() + " node");
}

void Factory::addOutput(Node& n) {
    if (n.getType() == "Transfer") {
        Entity::addOutput(n);
    }
    throw std::logic_error("Cannot add " + n.getType() + " node to the output port of a " + getType() + " node");
}

Entity::Entity(EntityType entitytype) {
    entitytype_ = entitytype;
}

bool connect(Node& input, Node& output) {
    try {
        input.addInput(output);
        output.addOutput(input);
    } catch (std::logic_error e) {
        return false;
    }
    return true;
}