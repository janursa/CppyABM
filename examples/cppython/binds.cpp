#include <iostream>
#include "example.h"
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include "CPPYABM/include/ABM/bind_tools.h"
using tramAgent = bind_tools::tramAgent<Domain,Cell,Tissue>;
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

EXPOSE_AGENT_CONTAINER(Cell);
EXPOSE_PATCH_CONTAINER(Tissue);
PYBIND11_MODULE(myBinds, m) {
    bind_tools::expose_defaults<Domain,Cell,Tissue>(m);
    auto env_obj = bind_tools::expose_env<Domain,Cell,Tissue,bind_tools::tramEnv<Domain,Cell,Tissue>>(m,"Domain");
    env_obj.def("setup",&Domain::setup);
    env_obj.def("episode",&Domain::episode);
    auto agent_obj = bind_tools::expose_agent<Domain,Cell,Tissue,PyCell>(m,"Cell")
        .def_readwrite("clock",&Cell::clock);
    auto patch_obj = bind_tools::expose_patch<Domain,Cell,Tissue>(m,"Tissue")
        .def_readwrite("damage_center",&Tissue::damage_center)
        .def_readwrite("ECM",&Tissue::ECM);
}

