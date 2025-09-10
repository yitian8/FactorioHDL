#include <string>
#include <vector>
#include <memory>

enum EntityType {
    ASSEMBLER, BELT, INSERTER, FURNACE, CHEST
};

class Node {
    public:
        virtual std::string getType() = 0;
        virtual int getID() = 0;
        virtual std::vector<std::shared_ptr<Node>> getInput() = 0;
        virtual std::vector<std::shared_ptr<Node>> getOutput() = 0;
        friend bool connect(Node& input, Node& output);
    protected:
        virtual void addInput(Node& n) = 0;
        virtual void addOutput(Node& n) = 0;
};

class InPort : public Node{
    public:
        virtual std::string getType() override {return "InPort";};
        virtual std::vector<std::shared_ptr<Node>> getInput() override{return std::vector<std::shared_ptr<Node>>{};};
        virtual std::vector<std::shared_ptr<Node>> getOutput() override{return output_;};
        int getID() {return id_;}
        InPort();
    protected:
        void addInput(Node& n) override;
        void addOutput(Node& n) override;
    private:  
        std::vector<std::shared_ptr<Node>> output_;
        int id_;
};

class OutPort : public Node{
    public:
        virtual std::string getType() override {return "OutPort";};
        virtual std::vector<std::shared_ptr<Node>> getInput() override{return input_;}
        virtual std::vector<std::shared_ptr<Node>> getOutput() override{return std::vector<std::shared_ptr<Node>>{};}
        int getID() {return id_;}
        OutPort();
    protected:
        void addInput(Node& n) override;
        void addOutput(Node& n) override;
    private:
        std::vector<std::shared_ptr<Node>> input_;
        int id_;
};

class Entity : public Node {
    public:
        virtual std::string getType() {return "Entity";}
        virtual std::vector<std::shared_ptr<Node>> getInput() {return input_;}
        virtual std::vector<std::shared_ptr<Node>> getOutput() {return output_;}
        EntityType getEntityType() {return entitytype_;}
        Entity() = delete;
        Entity(EntityType entitytype);
        int getID() {return id_;}
    protected:
        virtual void addInput(Node& n) override;
        virtual void addOutput(Node& n) override;
    private:
        int id_;
        EntityType entitytype_;
        std::vector<std::shared_ptr<Node>> input_;
        std::vector<std::shared_ptr<Node>> output_;
};

class Transportation: public Entity { 
    public:
        std::string getType() override {return "Transportation";}
    protected:
        void addInput(Node& n) override;
        void addOutput(Node& n) override;
};

class Transfer : public Entity {
    public:
        std::string getType() override {return "Transfer";}
};

class Factory : public Entity {
    public:
        std::string getType() override {return "Factory";}
    protected:
        void addInput(Node& n) override;
        void addOutput(Node& n) override;
};

bool connect(Node& input, Node& output);

