#include <stdexcept>
#include <iostream>
#include "node.h"

size_t Entity::next_id_ = 0;


void Entity::addInput(Entity& n) {
    if (&n == this) {
        throw std::logic_error("Cannot connect the I/O port of an entity to itself");
    }
    if (input_.contains(n.getID())) {
        throw std::logic_error("Cannot have multiedges in graphs");
    }
    input_.insert(std::make_pair(n.getID(), std::make_shared<Entity>(n)));
}

void Entity::addOutput(Entity& n) {
    if (&n == this) {
        throw std::logic_error("Cannot connect the I/O port of an entity to itself");
    }
    if (input_.contains(n.getID())) {
        throw std::logic_error("Cannot have multiedges in graphs");
    }
    output_.insert(std::make_pair(n.getID(), std::make_shared<Entity>(n)));
}

void Transportation::addInput(Entity& n) {
    if (n.getType() == "Transfer" || n.getType() == "Transportation") {
        Entity::addInput(n);
    } else {
        throw std::logic_error("Cannot add " + n.getType() + " node to the input port of a " + getType() + " node");
    }
    
}
 
void Transportation::addOutput(Entity& n) {
    if (n.getType() == "Transfer" || n.getType() == "Transportation") {
        Entity::addOutput(n);
    } else {
        throw std::logic_error("Cannot add " + n.getType() + " node to the output port of a " + getType() + " node");
    }
}

void Factory::addInput(Entity& n) {
    if (n.getType() == "Transfer" || n.getType() == "Pipe") {
        Entity::addInput(n);
    } else {
        throw std::logic_error("Cannot add " + n.getType() + " node to the input port of a " + getType() + " node");
    }
}

void Factory::addOutput(Entity& n) {
    if (n.getType() == "Transfer" || n.getType() == "Pipe") {
        Entity::addOutput(n);
    } else {
        throw std::logic_error("Cannot add " + n.getType() + " node to the output port of a " + getType() + " node");
    }
}

void Pipe::addInput(Entity& n) {
    if (n.getType() != "Factory") {
        Entity::addInput(n);
    } else {
        throw std::logic_error("Cannot add " + n.getType() + " node to the input port of a " + getType() + " node");
    }
}

void Pipe::addOutput(Entity& n) {
    if (n.getType() != "Factory") {
        Entity::addOutput(n);
    } else {
        throw std::logic_error("Cannot add " + n.getType() + " node to the output port of a " + getType() + " node");
    }
}

Entity::Entity(std::string entitytype) : id_(next_id_++) {
    entitytype_ = entitytype;
}

Entity::Entity(Entity& rhs) : id_(next_id_++) {
    entitytype_ = rhs.entitytype_;
    input_ = rhs.input_;
    output_ = rhs.output_;
    // for (auto& pair : input_) {
    //     pair.second -> addOutput(*this);
    // }
    // for (auto& pair : output_) {
    //     pair.second -> addInput(*this);
    // }
}


bool connect(Entity& input, Entity& output) {
    try {
        input.addInput(output);
        output.addOutput(input);
    } catch (std::logic_error e) {
        std::cerr<< e.what() << std::endl;
        return false;
    }
    return true;
}

Transportation::Transportation(std::string type) : Entity(type) {}

Transfer::Transfer(std::string type) : Entity(type) {} 

Factory::Factory(std::string type) : Entity(type) {} 

Pipe::Pipe(std::string type) : Entity(type) {}

