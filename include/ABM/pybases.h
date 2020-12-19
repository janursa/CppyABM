#pragma once

#include "bases.h"
#include <algorithm>
#include "pybind11/pybind11.h"
namespace py=pybind11;

template<class deriveClass>
struct PyBase: public deriveClass{
    using deriveClass::deriveClass;
    void set_data(string tag, double value) override {
        PYBIND11_OVERLOAD(
            void,
            deriveClass,
            set_data,
            tag,value
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

// template<class deriveClass> 
// struct PyPatch : public PyBase<deriveClass> {
//     using PyBase<deriveClass>::PyBase;
//     void step() override {
//         PYBIND11_OVERLOAD(
//             void,
//             deriveClass,
//             step
//             );
//     }

// };


template<class ENV, class AGENT, class PATCH> 
struct PyEnv : ENV  {
    using ENV::ENV;
    shared_ptr<PATCH> generate_patch() override {
        PYBIND11_OVERLOAD(
            shared_ptr<PATCH>, 
            ENV,      
            generate_patch         
        );
    }
    shared_ptr<AGENT> generate_agent(string agent_name) override {
        PYBIND11_OVERLOAD(
            shared_ptr<AGENT>, 
            ENV,      
            generate_agent,
            agent_name         
        );
    }
    void update_repo() override {
        PYBIND11_OVERLOAD(
            void, 
            ENV,      
            update_repo
                
        );
    }

};

template<class ENV, class AGENT, class PATCH> 
struct PyAgent : AGENT  {
    using AGENT::AGENT;
    void step() override {
        PYBIND11_OVERLOAD(
            void, 
            AGENT,      
            step         
        );
    }

    void inherit(shared_ptr<AGENT> father) override{
        PYBIND11_OVERLOAD(
            void, 
            AGENT,      
            inherit,
            father         
        );
    };
};
template<class ENV, class AGENT, class PATCH> 
struct PyPatch : PATCH {
    using PATCH::PATCH;
    void step() override {
     PYBIND11_OVERLOAD(
         void,
         PATCH,
         step
         );
    }

};
// template<class deriveClass>
// struct PyBase: public deriveClass{
//     using deriveClass::deriveClass;
//     void set_data(string tag, double value) override {
//         PYBIND11_OVERLOAD(
//             void,
//             deriveClass,
//             set_data,
//             tag,value
//             );
//     }
//     double get_data(string tag) override {
//         PYBIND11_OVERLOAD(
//             double,
//             deriveClass,
//             get_data,
//             tag
//             );
//     }

// };


// template<class EnvBase> 
// struct PyEnv : public EnvBase {
//     using EnvBase::EnvBase;
//     shared_ptr<Patch> generate_patch() override {
//         PYBIND11_OVERLOAD(
//             shared_ptr<Patch>, 
//             EnvBase,      
//             generate_patch         
//         );
//     }
//     shared_ptr<Agent> generate_agent(string agent_name) override {
//         PYBIND11_OVERLOAD(
//             shared_ptr<Agent>, 
//             EnvBase,      
//             generate_agent,
//             agent_name         
//         );
//     }
//     void update_repo() override {
//         PYBIND11_OVERLOAD(
//             void, 
//             EnvBase,      
//             update_repo
                
//         );
//     }

// };

// 
