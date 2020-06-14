#pragma once

#include "bases.h"
#include <algorithm>
#include "pybind11/pybind11.h"
namespace py=pybind11;

struct PyPatch : public Patch {
    using Patch::Patch;
    void step() override {
    	PYBIND11_OVERLOAD(
    		void,
    		Patch,
    		step
    		);
    }
};

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
