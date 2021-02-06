
#include <iostream>
#include "../Cpp/cpp_example.h"
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include "cppyabm/bind_tools.h"

//! Defining a trampoline to allow the extension of the functionality of Cell::step
using tramAgent = bind_tools::tramAgent<Domain,Cell,Tissue>; 
struct tramCell : public  tramAgent{
    using tramAgent::tramAgent; // using constructor of the parent class
    void setup() override {
        PYBIND11_OVERLOAD(
            void, 
            Cell,      
            setup    
        );
    }
};
/** Expose agent container**/
EXPOSE_AGENT_CONTAINER(Cell);
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
    env_obj.def(py::init<bool>());
    env_obj.def("setup",&Domain::setup);
    env_obj.def("episode",&Domain::episode);
    // expose agent and add new member
    auto agent_obj = bind_tools::expose_agent<Domain,Cell,Tissue,tramCell>(m,"Cell")
        .def_readwrite("cycle_t",&Cell::cycle_t);
}

