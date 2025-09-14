#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>


class Entity {
    public:
        friend class Graph;
        virtual std::string getType() {return "Entity";}
        virtual const std::map<size_t, Entity*>& getInput() const {return input_;}
        virtual const std::map<size_t, Entity*>& getOutput() const {return output_;} 
        
        std::string getEntityType() {return entitytype_;}
        size_t getID() const {return id_;} 

    protected:
        Entity() = delete;
        Entity(std::string entitytype);
        Entity(Entity& rhs);
        
        // Adds n to the input list of the node.
        virtual void addInput(Entity& n);

        // Adds n to the output list of the node.
        virtual void addOutput(Entity& n);

        // Removes n from the input list of the node
        void removeInput(Entity& n);

        // Removes n from the output list of the node.
        void removeOutput(Entity& n);

    private:
        static size_t next_id_;
        const size_t id_;
        std::string entitytype_;
        std::map<size_t, Entity*> input_;
        std::map<size_t, Entity*> output_; 
};

class Transportation: public Entity { 
    public:
        std::string getType() override {return "Transportation";}
        Transportation(std::string type);
        Transportation(Transportation& rhs);
    protected:
        // Adds n to the input list of the node.
        // Only Transfer nodes and Transportation nodes are allowed to be added to input.
        void addInput(Entity& n) override;
        void addOutput(Entity& n) override;
};

class Transfer : public Entity {
    public:
        Transfer(std::string type);
        Transfer(Transfer& rhs);
        std::string getType() override {return "Transfer";}
    protected:
        // Adds n to the input list of the node.
        // Only Transfer nodes and Transportation nodes are allowed to be added to input.
        void addInput(Entity& n) override;
        void addOutput(Entity& n) override;
};

class Factory : public Entity {
    public:
        Factory(std::string type);
        Factory(Factory& rhs);
        std::string getType() override {return "Factory";}
    protected:
        void addInput(Entity& n) override;
        void addOutput(Entity& n) override;
}; 

class Pipe : public Entity {
    public:
        Pipe(std::string type);
        Pipe(Pipe& rhs);
        std::string getType() override {return "Pipe";}
    protected:
        void addInput(Entity& n) override;
        void addOutput(Entity& n) override;
};




