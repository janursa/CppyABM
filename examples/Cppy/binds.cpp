
#include <iostream>
#include "../Cpp/cpp_example.h"
// #include <pybind11/stl.h>
// #include <pybind11/stl_bind.h>
// #include <pybind11/complex.h>
// #include <pybind11/functional.h>
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
EXPOSE_AGENT_CONTAINER(Cell);
EXPOSE_PATCH_CONTAINER(Tissue);

//! Binding function. 
/*!
The name provided as module, i.e. myBinds, must be the same name given in CmakeLists.txt as argument to
pybind11_add_module. 
*/
PYBIND11_MODULE(myBinds, m) {

    using tramEnv = bind_tools::tramEnv<Domain,Cell,Tissue>;
    using tramPatch = bind_tools::tramPatch<Domain,Cell,Tissue>;
    // 
    auto bind_obj = bind_tools::Bind<Domain,Cell,Tissue,tramEnv,tramCell,tramPatch>(m,"Domain","Cell","Tissue");
    auto env_obj = bind_obj.get_env();
    env_obj.def(py::init<bool>());
    env_obj.def("setup",&Domain::setup);
    env_obj.def("episode",&Domain::episode);
    auto agent_obj = bind_obj.get_agent();
    agent_obj.def_readwrite("cycle_t",&Cell::cycle_t);
}


