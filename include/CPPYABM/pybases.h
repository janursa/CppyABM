#pragma once

#include "bases.h"
#include <algorithm>
#include "pybind11/pybind11.h"
namespace py=pybind11;
struct Env;

template<class deriveClass>
struct PyBase: public deriveClass{
    using deriveClass::deriveClass;
    void set_data(string tag) override {
        PYBIND11_OVERLOAD(
            void,
            deriveClass,
            set_data,
            tag
            );
    }
    double get_data(string tag) override {
        PYBIND11_OVERLOAD(
            double,
            deriveClass,
            get_data,
            tag
            );
    }

};

template<class deriveClass> 
struct PyPatch : public PyBase<deriveClass> {
    using PyBase<deriveClass>::PyBase;
    void step() override {
    	PYBIND11_OVERLOAD(
    		void,
    		deriveClass,
    		step
    		);
    }

};
template<class EnvBase> 
struct PyEnv : public EnvBase {
    using EnvBase::EnvBase;
    shared_ptr<Patch> generate_patch() override {
        PYBIND11_OVERLOAD(
            shared_ptr<Patch>, 
            EnvBase,      
            generate_patch         
        );
    }
    shared_ptr<Agent> generate_agent(string agent_name) override {
        PYBIND11_OVERLOAD(
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
        PYBIND11_OVERLOAD(
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
