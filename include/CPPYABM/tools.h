
#pragma once
#include "bases.h"
#include <map>
#include <algorithm>
#include "pybases.h"
// #include <pybind11/stl.h>
#include "pybind11/pybind11.h"
namespace py=pybind11;
using namespace std;

void expose_base_env(py::module m){
    py::class_<Env,PyEnv<Env>,std::shared_ptr<Env>> (m,"Env",py::dynamic_attr())
        .def(py::init<>())
        .def("check",&Env::check)
        .def("place_agent_randomly",&Env::place_agent_randomly)
        .def("setup_domain",&Env::setup_domain)
        .def("step_agents",&Env::step_agents)
        .def("step_patches",&Env::step_patches)
        .def("place_agent",&Env::place_agent)
        .def("update",&Env::update)
        .def("setup_agents",&Env::setup_agents)
        .def("count_agents",&Env::count_agents)
        .def("collect_from_patches",&Env::collect_from_patches)
        .def_readwrite("patches",&Env::patches)
        .def_readwrite("agents",&Env::agents);
}

template<typename class_name,typename py_class_name>
py::class_<class_name,Env,py_class_name,std::shared_ptr<class_name>> expose_env(py::module m, string class_name_string){
    auto class_binds_obj = 
    py::class_<class_name,Env,py_class_name,std::shared_ptr<class_name>> (m,class_name_string.c_str(),py::dynamic_attr())
        .def(py::init<>())
        .def("check",&class_name::check)
        .def("place_agent_randomly",&class_name::place_agent_randomly)
        .def("setup_domain",&class_name::setup_domain)
        .def("step_agents",&class_name::step_agents)
        .def("step_patches",&class_name::step_patches)
        .def("place_agent",&class_name::place_agent)
        .def("update",&class_name::update)
        .def("setup_agents",&class_name::setup_agents)
        .def("count_agents",&class_name::count_agents)
        .def("collect_from_patches",&class_name::collect_from_patches)
        .def_readwrite("patches",&class_name::patches)
        .def_readwrite("agents",&class_name::agents);
    return class_binds_obj;
}

void expose_base_agent(py::module m){
    py::class_<Agent,PyAgent<Agent>,std::shared_ptr<Agent>>(m,"Agent",py::dynamic_attr())
        .def(py::init<shared_ptr<Env>,string>(),"Initialize",py::arg("env"),py::arg("class_name"))
        .def("move",&Agent::move,"Move the agent to a new patch")
        .def("order_hatch",&Agent::order_hatch,"Hatch request",
            py::arg("patch")=nullptr, py::arg("inherit")=false,
            py::arg("quiet")=false, py::arg("reset")=false)
        .def("order_move",&Agent::order_move,"Move request",
            py::arg("patch")=nullptr, 
            py::arg("quiet")=false, py::arg("reset")=false)
        .def("order_switch",&Agent::order_switch,"Switch request",
            py::arg("to"))
        .def_readwrite("disappear",&Agent::disappear)
        .def_readwrite("env",&Agent::env)
        .def_readwrite("data",&Agent::data)
        .def_readwrite("patch",&Agent::patch)
        .def_readwrite("class_name",&Agent::class_name);
}

template<typename class_name,typename py_class_name>
py::class_<class_name,Agent,py_class_name,std::shared_ptr<class_name>>  expose_agent(py::module &m, string class_name_str) {
    /** Agent **/
    auto class_binds_obj =  py::class_<class_name,Agent,py_class_name,std::shared_ptr<class_name>>(m,class_name_str.c_str(),py::dynamic_attr())
        .def(py::init<shared_ptr<Env>,string>(),"Initialize",py::arg("env"),py::arg("class_name"))
        .def("move",&class_name::move,"Move the agent to a new patch")
        .def("order_hatch",&class_name::order_hatch,"Hatch request",
            py::arg("patch")=nullptr, py::arg("inherit")=false,
            py::arg("quiet")=false, py::arg("reset")=false)
        .def("order_move",&class_name::order_move,"Move request",
            py::arg("patch")=nullptr, 
            py::arg("quiet")=false, py::arg("reset")=false)
        .def("order_switch",&class_name::order_switch,"Switch request",
            py::arg("to"))
        .def_readwrite("disappear",&class_name::disappear)
        .def_readwrite("env",&class_name::env)
        .def_readwrite("data",&class_name::data)
        .def_readwrite("patch",&class_name::patch)
        .def_readwrite("class_name",&class_name::class_name);
    return class_binds_obj;

}

void expose_base_patch(py::module m){
    py::class_<Patch,PyPatch<Patch>,std::shared_ptr<Patch>>(m,"Patch",py::dynamic_attr())
        .def(py::init<shared_ptr<Env>>())
        .def("empty_neighbor", &Patch::empty_neighbor,"Return an empty patch around the patch",
            py::arg("quiet")=false)
        .def("find_neighbor_agents",&Patch::find_neighbor_agents,"Returns a vector of agents in one patch neighborhood",
            py::arg("include_self")=true)
        .def_readwrite("coords",&Patch::coords)
        .def_readwrite("agent",&Patch::agent)
        .def_readwrite("empty",&Patch::empty)
        .def_readwrite("disappear",&Patch::disappear)
        .def_readwrite("data",&Patch::data)
        .def_readwrite("neighbors",&Patch::neighbors);
}

template<typename class_name,typename py_class_name>
py::class_<class_name,Patch,py_class_name,std::shared_ptr<class_name>>  expose_patch(py::module &m, string class_name_ptr){
    auto class_binds_obj =  py::class_<class_name,Patch,py_class_name,std::shared_ptr<class_name>>(m,class_name_ptr.c_str(),py::dynamic_attr())
        .def(py::init<shared_ptr<Env>>())
        .def("empty_neighbor", &class_name::empty_neighbor,"Return an empty patch around the patch",
            py::arg("quiet")=false)
        .def("find_neighbor_agents",&class_name::find_neighbor_agents,"Returns a vector of agents in one patch neighborhood",
            py::arg("include_self")=true)
        .def_readwrite("coords",&class_name::coords)
        .def_readwrite("agent",&class_name::agent)
        .def_readwrite("empty",&class_name::empty)
        .def_readwrite("disappear",&class_name::disappear)
        .def_readwrite("data",&class_name::data)
        .def_readwrite("neighbors",&class_name::neighbors);
    return class_binds_obj;
        
}

void register_exceptions(py::module m){
     py::register_exception<patch_availibility>(m, "patch_availibility");
}
void register_mesh(py::module m){
    py::class_<MESH_ITEM>(m,"MESH_ITEM")
        .def(py::init<>()); 
    py::class_<mesh_tools>(m,"mesh_tools")
        .def(py::init<>())
        .def("grid",&mesh_tools::grid);
}