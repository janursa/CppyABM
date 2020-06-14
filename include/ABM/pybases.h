#pragma once

#include "bases.h"
#include <algorithm>
#include "pybind11/pybind11.h"
namespace py=pybind11;
struct Env;
template<class PatchBase> 
struct PyPatch : public PatchBase {
    using PatchBase::PatchBase;
    void step() override {
    	PYBIND11_OVERLOAD(
    		void,
    		PatchBase,
    		step
    		);
    }
};
template<class EnvBase> 
struct PyEnv : public EnvBase {
    using EnvBase::EnvBase;
    void setup() override {
        PYBIND11_OVERLOAD(
            void, 
            EnvBase,      
            setup         
        );
    }
    shared_ptr<Patch> generate_patch() override {
        PYBIND11_OVERLOAD_PURE(
            shared_ptr<Patch>, 
            EnvBase,      
            generate_patch         
        );
    }
    shared_ptr<Agent> generate_agent(string agent_name) override {
        PYBIND11_OVERLOAD_PURE(
            shared_ptr<Agent>, 
            EnvBase,      
            generate_agent,
            agent_name         
        );
    }
    void update_repo() override {
        PYBIND11_OVERLOAD(
            void, 
            EnvBase,      
            update_repo
                
        );
    }
};

template<class AgentBase>
struct PyAgent : public AgentBase {
    using AgentBase::AgentBase;
    void step() override {
        PYBIND11_OVERLOAD_PURE(
            void, 
            AgentBase,      
            step         
        );
    }

    void inherit(shared_ptr<Agent> father) override{
        PYBIND11_OVERLOAD(
            void, 
            AgentBase,      
            inherit,
            father         
        );
    };
};
