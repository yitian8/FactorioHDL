#include <string>
#include <vector>
#include <memory>

enum EntityType {
    ASSEMBLER, BELT, INSERTER, FURNACE, CHEST
};

class Node {
    public:
        virtual std::string getType() = 0;
        virtual std::vector<std::shared_ptr<Node>> getInput() = 0;
        virtual std::vector<std::shared_ptr<Node>> getOutput() = 0;
        virtual void addInput(Node& n) = 0;
        virtual void addOutput(Node& n) = 0;
};

class InPort : public Node{
    public:
        virtual std::string getType() override {return "InPort";};
        virtual std::vector<std::shared_ptr<Node>> getInput() override{return std::vector<std::shared_ptr<Node>>{};};
        virtual std::vector<std::shared_ptr<Node>> getOutput() override{return output_;};
        void addInput(Node& n) override;
        void addOutput(Node& n) override;
    private:
        std::vector<std::shared_ptr<Node>> output_;
};

class OutPort : public Node{
    public:
        virtual std::string getType() override {return "OutPort";};
        virtual std::vector<std::shared_ptr<Node>> getInput() override{return input_;}
        virtual std::vector<std::shared_ptr<Node>> getOutput() override{return std::vector<std::shared_ptr<Node>>{};}
        void addInput(Node& n) override;
        void addOutput(Node& n) override;
    private:
        std::vector<std::shared_ptr<Node>> input_;
};

class Entity : public Node {
    public:
        virtual std::string getType() {return "Entity";}
        virtual std::vector<std::shared_ptr<Node>> getInput() {return input_;}
        virtual std::vector<std::shared_ptr<Node>> getOutput() {return output_;}
        virtual void addInput(Node& n) override;
        virtual void addOutput(Node& n) override;
        EntityType getEntityType() {return entitytype_;}
        Entity() = delete;
        Entity(EntityType entitytype);
    private:
        
        EntityType entitytype_;
        std::vector<std::shared_ptr<Node>> input_;
        std::vector<std::shared_ptr<Node>> output_;
};

class Transportation: public Entity {
    public:
        std::string getType() override {return "Transportation";}
        void addInput(Node& n) override;
        void addOutput(Node& n) override;
};

class Transfer : public Entity {
    public:
        std::string getType() override {return "Transfer";}
};

class Factory : public Entity {
    public:
        std::string getType() override {return "Factory";};
        void addInput(Node& n) override;
        void addOutput(Node& n) override;
};

bool connect(Node& input, Node& output);

