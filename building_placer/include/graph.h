#pragma once
#include <map>
#include "node.h"


class Graph{
    public:
        // Adds the source node as an input node of the target node.
        // Target must be inside the graph.
        // Returns True if node successfully added, return false if not.
        bool addNodeAsInput(std::shared_ptr<Entity> source, std::shared_ptr<Entity> target);

        // Adds the source node as an output node of the target node
        // Target must be inside the graph.
        // Returns True if node successfully added, return false if not.
        bool addNodeAsOutput(std::shared_ptr<Entity> source, std::shared_ptr<Entity> target);
    private:
        std::map<int, std::unique_ptr<Entity>> node_list_;
        std::map<int, std::weak_ptr<Entity>> entry_points_;
        std::map<int, std::weak_ptr<Entity>> exit_points_;
};