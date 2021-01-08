
#include <iostream>
#include "../cpp/cpp_example.h"
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include "CppyABM/include/ABM/bind_tools.h"
using tramAgent = bind_tools::tramAgent<Domain,Cell,Tissue>;
//! Defining a trampoline to allow the extension of the functionality of Cell::step
struct PyCell : public  tramAgent{
    using tramAgent::tramAgent;
    void step() override {
        PYBIND11_OVERLOAD(
            void, 
            Cell,      
            step    
        );
    }
};
/** Expose agent and patch container**/
EXPOSE_AGENT_CONTAINER(Cell);
EXPOSE_PATCH_CONTAINER(Tissue);
//! Binding function. 
/*!
The name provided as module, i.e. myBinds, must be the same name given in CmakeLists.txt as argument to
pybind11_add_module. 
*/
PYBIND11_MODULE(myBinds, m) {
    // expose defaults
    bind_tools::expose_defaults<Domain,Cell,Tissue>(m);
    // expose env and add new members
    auto env_obj = bind_tools::expose_env<Domain,Cell,Tissue,bind_tools::tramEnv<Domain,Cell,Tissue>>(m,"Domain");
    env_obj.def("setup",&Domain::setup);
    env_obj.def("episode",&Domain::episode);
    // expose agent and add new member
    auto agent_obj = bind_tools::expose_agent<Domain,Cell,Tissue,PyCell>(m,"Cell")
        .def_readwrite("clock",&Cell::clock);
    // expose patch and add new members
    auto patch_obj = bind_tools::expose_patch<Domain,Cell,Tissue>(m,"Tissue")
        .def_readwrite("damage_center",&Tissue::damage_center)
        .def_readwrite("ECM",&Tissue::ECM);
}

