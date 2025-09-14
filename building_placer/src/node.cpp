#include <stdexcept>
#include <iostream>
#include "node.h"

size_t Entity::next_id_ = 0;

void Entity::removeInput(Entity& n) {
    if (&n == this) {
        throw std::logic_error("Cannot remove oneself from its input nodes");
    }
    if (!input_.contains(n.getID())) {
        throw std::logic_error("Node being removed from input is not connected to the current node");
    }
    input_.erase(n.id_);
}

void Entity::removeOutput(Entity& n) {
    if (&n == this) {
        throw std::logic_error("Cannot remove oneself from its output nodes");
    }
    if (!output_.contains(n.getID())) {
        throw std::logic_error("Node being removed from output is not connected to the current node");
    }
    output_.erase(n.id_);
}

void Entity::addInput(Entity& n) {
    if (&n == this) {
        throw std::logic_error("Cannot connect the I/O port of an entity to itself");
    }
    if (input_.contains(n.id_)) {
        throw std::logic_error("Cannot have multiedges in graphs");
    }
    input_.insert(std::make_pair(n.id_, &n));
}

void Entity::addOutput(Entity& n) {
    if (&n == this) {
        throw std::logic_error("Cannot connect the I/O port of an entity to itself");
    }
    if (input_.contains(n.id_)) {
        throw std::logic_error("Cannot have multiedges in graphs");
    }
    output_.insert(std::make_pair(n.id_, &n));
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

void Transfer::addInput(Entity& n) {
    if (n.getType() == "Pipe") {
        throw std::logic_error("Cannot add " + n.getType() + " node to the input port of a " + getType() + " node");
    } else {
        Entity::addInput(n);
    }
}

void Transfer::addOutput(Entity& n) {
    if (n.getType() == "Pipe") {
        throw std::logic_error("Cannot add " + n.getType() + " node to the output port of a " + getType() + " node");
    } else {
        Entity::addOutput(n);
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
    if (n.getType() != "Factory" && n.getType() != "Pipe") {
        throw std::logic_error("Cannot add " + n.getType() + " node to the input port of a " + getType() + " node");
    } else {
        Entity::addInput(n);
    }
}

void Pipe::addOutput(Entity& n) {
    if (n.getType() != "Factory" && n.getType() != "Pipe") {
        throw std::logic_error("Cannot add " + n.getType() + " node to the output port of a " + getType() + " node");
    } else {
        Entity::addOutput(n);
    }
}

Entity::Entity(std::string entitytype) : id_(next_id_++) {
    entitytype_ = entitytype;
}

Entity::Entity(Entity& rhs) : id_(next_id_++) {
    entitytype_ = rhs.entitytype_;
    input_ = rhs.input_;
    output_ = rhs.output_;
    for (auto& pair : input_) {
        pair.second -> addOutput(*this);
    }
    for (auto& pair : output_) {
        pair.second -> addInput(*this);
    }
}




Transportation::Transportation(std::string type) : Entity(type) {}

Transfer::Transfer(std::string type) : Entity(type) {} 

Factory::Factory(std::string type) : Entity(type) {} 

Pipe::Pipe(std::string type) : Entity(type) {}

Transportation::Transportation(Transportation& rhs) : Entity(rhs) {}

Transfer::Transfer(Transfer& rhs) : Entity(rhs) {}

Factory::Factory(Factory& rhs) : Entity(rhs){}

Pipe::Pipe(Pipe& rhs) : Entity(rhs) {}

