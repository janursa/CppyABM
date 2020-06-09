#pragma once
#include "pybind11/pybind11.h"
#include "bases.h"
namespace py=pybind11;
using namespace std;

struct PyEnv : public Env {
    using Env::Env;
    void setup() override {
        PYBIND11_OVERLOAD(
            void, 
            Env,      
            setup         
        );
    }
    shared_ptr<Patch> generate_patch() override {
        PYBIND11_OVERLOAD_PURE(
            shared_ptr<Patch>, 
            Env,      
            generate_patch         
        );
    }
    shared_ptr<Agent> generate_agent(string agent_name) override {
        PYBIND11_OVERLOAD_PURE(
            shared_ptr<Agent>, 
            Env,      
            generate_agent,
            agent_name         
        );
    }
    void update_repo() override {
        PYBIND11_OVERLOAD(
            void, 
            Env,      
            update_repo
                
        );
    }
};
