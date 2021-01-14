
#include <iostream>
#include "../cpp/cpp_example.h"
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include "CppyABM/include/ABM/bind_tools.h"


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
}

