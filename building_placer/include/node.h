#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>


class Entity {
    public:
        virtual std::string getType() {return "Entity";}
        virtual std::map<size_t, std::shared_ptr<Entity>> getInput() {return input_;}
        virtual std::map<size_t, std::shared_ptr<Entity>> getOutput() {return output_;}
        std::string getEntityType() {return entitytype_;}
        Entity() = delete;
        Entity(std::string entitytype);
        Entity(Entity& rhs);
        size_t getID() const {return id_;}
        friend bool connect(Entity& input, Entity& output);
    protected:

        // Adds n to the input list of the node.
        virtual void addInput(Entity& n);

        // Adds n to the output list of the node.
        virtual void addOutput(Entity& n);

    private:
        static size_t next_id_;
        const size_t id_;
        std::string entitytype_;
        std::map<size_t, std::shared_ptr<Entity>> input_;
        std::map<size_t, std::shared_ptr<Entity>> output_;
};

class Transportation: public Entity { 
    public:
        std::string getType() override {return "Transportation";}
        Transportation(std::string type);
    protected:
        // Adds n to the input list of the node.
        // Only Transfer nodes and Transportation nodes are allowed to be added to input.
        void addInput(Entity& n) override;
        void addOutput(Entity& n) override;
};

class Transfer : public Entity {
    public:
        Transfer(std::string type);
        std::string getType() override {return "Transfer";}
};

class Factory : public Entity {
    public:
        Factory(std::string type);
        std::string getType() override {return "Factory";}
    protected:
        void addInput(Entity& n) override;
        void addOutput(Entity& n) override;
}; 

class Pipe : public Entity {
    public:
        Pipe(std::string type);
        std::string getType() override {return "Pipe";}
    protected:
        void addInput(Entity& n) override;
        void addOutput(Entity& n) override;
};




