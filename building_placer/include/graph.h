#pragma once
#include <map>
#include <concepts>
#include "node.h"


class Graph{
    public:
        // Adds the output node as an input of the input node.
        // Both nodes must be inside the graph.
        // Returns True if node successfully added, return false if not.
        // @param input The input node
        // @param ouput The output node
        bool connect(Entity& input, Entity& output);

        // Adds the output node as an input of the input node.
        // Both nodes must be inside the graph.
        // Returns True if node successfully added, return false if not.
        // @param id_input  id of the input node
        // @param id_output  id of the output node
        bool connect(size_t id_input, size_t id_output);

        // Disconnects two nodes, where the output node is expected to be the input of the input node.
        // Returns true if successful, false if not.
        // @param input  the input node
        // @param output the output node
        bool disconnect(Entity& input, Entity& output);

        // Disconnects two nodes, where the output node is expected to be the input of the input node.
        // Returns true if successful, false if not.
        // @param id_input  id of the input node
        // @param id_output  id of the output node
        bool disconnect(size_t id_input, size_t id_output);

        bool remove(size_t id);

        // Create a new node with entitytype_ = entitytype.
        template <typename T>
        size_t createNode(std::string entitytype);

        // Create a new node as a copy of rhs.
        template <typename T>
        size_t createNode(T& rhs);

        // size_t createTransportationNode(std::string entitytype);

        // size_t createTransportationNode(Transportation& rhs);
    private:
        std::map<size_t, std::unique_ptr<Entity>> node_list_;
};
 
template <typename T>
size_t Graph::createNode(std::string entitytype) {
    auto ptr = std::make_unique<T>(entitytype);
    size_t id = ptr -> getID();
    node_list_.insert(std::make_pair(ptr -> getID(), std::move(ptr)));
    return id;
}

template <typename T>
size_t Graph::createNode(T& rhs) {
    auto ptr = std::make_unique<T>(rhs);
    size_t id = ptr -> getID();
    node_list_.insert(std::make_pair(ptr -> getID(), std::move(ptr)));
    return id;
}