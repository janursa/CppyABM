#include <iostream>
#include "example.h"
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include "CPPYABM/include/ABM/bind_tools.h"

template<class derivedEnv,class derivedAgent,class derivedPatch>
class PyhealingEnv: public PyEnv<derivedEnv,derivedAgent,derivedPatch>{
    using PyEnv<derivedEnv,derivedAgent,derivedPatch>::PyEnv;
    shared_ptr<derivedAgent> generate_agent(string agent_name) override {
        PYBIND11_OVERLOAD(
            shared_ptr<derivedAgent>, 
            derivedEnv,      
            generate_agent,
            agent_name         
        );
    }
    void update() override {
        PYBIND11_OVERLOAD(
            void, 
            derivedEnv,      
            update
                     
        );
    }
};
struct PyCell : public Cell {
    using Cell::Cell;
    void step() override {
        PYBIND11_OVERLOAD(
            void, 
            Cell,      
            step
                     
        );
    }
};
EXPOSE_AGENT_CONTAINER(Cell);
EXPOSE_PATCH_CONTAINER(myPatch);
PYBIND11_MODULE(myBinds, m) {
    binds_tools::expose_defaults<healingEnv,Cell,myPatch>(m);
    auto env_obj = binds_tools::expose_env<healingEnv,Cell,myPatch,PyhealingEnv<healingEnv,Cell,myPatch>>(m,"healingEnv");
    env_obj.def("setup",&healingEnv::setup);
    env_obj.def("step",&healingEnv::step);
    auto agent_obj = binds_tools::expose_agent<healingEnv,Cell,myPatch,PyCell>(m,"Cell")
        .def_readwrite("clock",&Cell::clock);
    auto patch_obj = binds_tools::expose_patch<healingEnv,Cell,myPatch>(m,"myPatch")
        .def_readwrite("damage_center",&myPatch::damage_center)
        .def_readwrite("tissue",&myPatch::tissue);
}

