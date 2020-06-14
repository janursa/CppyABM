#include "agents.cpp"
struct PyAgent : public Agent {
    using Agent::Agent;
    void step() override {
        PYBIND11_OVERLOAD_PURE(
            void, 
            Agent,      
            step         
        );
    }

    void inherit(shared_ptr<Agent> father) override{
    	PYBIND11_OVERLOAD(
            void, 
            Agent,      
            inherit,
            father         
        );
    };
};


