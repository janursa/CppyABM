#pragma once
#include "bases.h"
#include <map>
#include <algorithm>
#include "pybind11/pybind11.h"
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include "pybases.h"
namespace py=pybind11;

struct expEnv;
struct expPatch;
struct expAgent;
struct expEnv: public baseEnv<expEnv,expAgent,expPatch>{

};
struct expAgent: public baseAgent<expEnv,expAgent,expPatch>{
    expAgent(shared_ptr<expEnv> env , string class_name):baseAgent<expEnv,expAgent,expPatch>(env,class_name){
        
    }
};
struct expPatch: public basePatch<expEnv,expAgent,expPatch>{
    expPatch(shared_ptr<expEnv> env):basePatch<expEnv,expAgent,expPatch>(env){}
};


template<class ENV,class AGENT,class PATCH,typename py_class_name>
py::class_<ENV,baseEnv<ENV,AGENT,PATCH>, py_class_name,std::shared_ptr<ENV>> expose_env(py::module m, string class_name_string){
    auto class_binds_obj = 
    py::class_<ENV,baseEnv<ENV,AGENT,PATCH>,py_class_name,std::shared_ptr<ENV>> (m,class_name_string.c_str(),py::dynamic_attr())
        .def(py::init<>())
        .def("place_agent_randomly",&ENV::place_agent_randomly)
        .def("setup_domain",&ENV::setup_domain)
        .def("step_agents",&ENV::step_agents)
        .def("step_patches",&ENV::step_patches)
        .def("place_agent",&ENV::place_agent)
        .def("update",&ENV::update)
        .def("setup_agents",&ENV::setup_agents)
        .def("count_agents",&ENV::count_agents)
        .def("connect_patch_agent", &ENV::connect_patch_agent)
        .def_readwrite("patches",&ENV::patches)
        .def_readwrite("agents",&ENV::agents);
    return class_binds_obj;
}
template<class ENV,class AGENT,class PATCH>
void expose_base_env(py::module &m, string class_name_string){
    py::class_<baseEnv<ENV,AGENT,PATCH>,PyEnv<ENV,AGENT,PATCH>,std::shared_ptr<baseEnv<ENV,AGENT,PATCH>>> (m,class_name_string.c_str(),py::dynamic_attr())
        .def(py::init<>())
        .def("place_agent_randomly",&baseEnv<ENV,AGENT,PATCH>::place_agent_randomly)
        .def("setup_domain",&baseEnv<ENV,AGENT,PATCH>::setup_domain)
        .def("step_agents",&baseEnv<ENV,AGENT,PATCH>::step_agents)
        .def("step_patches",&baseEnv<ENV,AGENT,PATCH>::step_patches)
        .def("place_agent",&baseEnv<ENV,AGENT,PATCH>::place_agent)
        .def("update",&baseEnv<ENV,AGENT,PATCH>::update)
        .def("setup_agents",&baseEnv<ENV,AGENT,PATCH>::setup_agents)
        .def("count_agents",&baseEnv<ENV,AGENT,PATCH>::count_agents)
        .def_readwrite("patches",&baseEnv<ENV,AGENT,PATCH>::patches)
        .def_readwrite("agents",&baseEnv<ENV,AGENT,PATCH>::agents)
        .def("connect_patch_agent", &baseEnv<ENV,AGENT,PATCH>::connect_patch_agent);

}

// void expose_base_env(py::module &m, string class_name_string){
//     py::class_<expEnv,PyEnv<expEnv,expAgent,expPatch>,std::shared_ptr<expEnv>> (m,class_name_string.c_str(),py::dynamic_attr())
//         .def(py::init<>())
//         .def("place_agent_randomly",&expEnv::place_agent_randomly)
//         .def("setup_domain",&expEnv::setup_domain)
//         .def("step_agents",&expEnv::step_agents)
//         .def("step_patches",&expEnv::step_patches)
//         .def("place_agent",&expEnv::place_agent)
//         .def("update",&expEnv::update)
//         .def("setup_agents",&expEnv::setup_agents)
//         .def("count_agents",&expEnv::count_agents)
//         .def_readwrite("patches",&expEnv::patches)
//         .def_readwrite("agents",&expEnv::agents)
//         .def("connect_patch_agent", &expEnv::connect_patch_agent);

// }

template<class ENV,class AGENT,class PATCH>
void expose_base_agent(py::module m,string class_name_string){
    py::class_<baseAgent<ENV,AGENT,PATCH>,PyAgent<ENV,AGENT,PATCH>,std::shared_ptr<baseAgent<ENV,AGENT,PATCH>>>(m,class_name_string.c_str(),py::dynamic_attr())
        .def(py::init<shared_ptr<ENV>,string>(),"Initialize",py::arg("env"),py::arg("class_name"))
        .def("move",&baseAgent<ENV,AGENT,PATCH>::move,"Move the agent to a new patch",py::arg("patch")=nullptr,py::arg("quiet")=false)
        .def("order_hatch",&baseAgent<ENV,AGENT,PATCH>::order_hatch,"Hatch request",
            py::arg("patch")=nullptr, py::arg("inherit")=false,
            py::arg("quiet")=false, py::arg("reset")=false)
        .def("order_move",&baseAgent<ENV,AGENT,PATCH>::order_move,"Move request",
            py::arg("patch")=nullptr, 
            py::arg("quiet")=false, py::arg("reset")=false)
        .def("order_switch",&baseAgent<ENV,AGENT,PATCH>::order_switch,"Switch request",
            py::arg("to"))
        .def_readwrite("disappear",&baseAgent<ENV,AGENT,PATCH>::disappear)
        .def_readwrite("env",&baseAgent<ENV,AGENT,PATCH>::env)
        .def_readwrite("patch",&baseAgent<ENV,AGENT,PATCH>::patch)
        .def_readwrite("disappear",&baseAgent<ENV,AGENT,PATCH>::disappear)
        .def_readwrite("class_name",&baseAgent<ENV,AGENT,PATCH>::class_name);
}
template<class ENV,class AGENT,class PATCH,class py_class_name>
py::class_<AGENT,baseAgent<ENV,AGENT,PATCH>, py_class_name,std::shared_ptr<AGENT>>  expose_agent(py::module &m, string class_name_str){
    auto class_binds_obj = py::class_<AGENT,baseAgent<ENV,AGENT,PATCH>, py_class_name,std::shared_ptr<AGENT>>(m,class_name_str.c_str(),py::dynamic_attr())
        .def(py::init<shared_ptr<ENV>,string>(),"Initialize",py::arg("env"),py::arg("class_name"))
        .def("move",&AGENT::move,"Move the agent to a new patch",py::arg("patch")=nullptr,py::arg("quiet")=false)
        .def("order_hatch",&AGENT::order_hatch,"Hatch request",
            py::arg("patch")=nullptr, py::arg("inherit")=false,
            py::arg("quiet")=false, py::arg("reset")=false)
        .def("order_move",&AGENT::order_move,"Move request",
            py::arg("patch")=nullptr, 
            py::arg("quiet")=false, py::arg("reset")=false)
        .def("order_switch",&AGENT::order_switch,"Switch request",
            py::arg("to"))
        .def_readwrite("disappear",&AGENT::disappear)
        .def_readwrite("env",&AGENT::env)
        .def_readwrite("patch",&AGENT::patch)
        .def_readwrite("disappear",&AGENT::disappear)
        .def_readwrite("class_name",&AGENT::class_name);
    return class_binds_obj;
}
template<class ENV,class AGENT,class PATCH>
void  expose_base_patch(py::module &m, string class_name_ptr){
    py::class_<basePatch<ENV,AGENT,PATCH>,PyPatch<ENV,AGENT,PATCH>,std::shared_ptr<basePatch<ENV,AGENT,PATCH>>>(m,class_name_ptr.c_str(),py::dynamic_attr())
        .def(py::init<shared_ptr<ENV>>())
        .def("empty_neighbor", &basePatch<ENV,AGENT,PATCH>::empty_neighbor,"Return an empty patch around the patch",
            py::arg("quiet")=false)
        .def("find_neighbor_agents",&basePatch<ENV,AGENT,PATCH>::find_neighbor_agents,"Returns a vector of agents in one patch neighborhood",
            py::arg("include_self")=true)
        .def_readwrite("coords",&basePatch<ENV,AGENT,PATCH>::coords)
        .def_readwrite("agent",&basePatch<ENV,AGENT,PATCH>::agent)
        .def_readwrite("empty",&basePatch<ENV,AGENT,PATCH>::empty)
        .def_readwrite("on_border",&basePatch<ENV,AGENT,PATCH>::on_border)
        .def_readwrite("neighbors",&basePatch<ENV,AGENT,PATCH>::neighbors);        
}

template<class ENV,class AGENT,class PATCH,class py_class_name>
py::class_<PATCH,basePatch<ENV,AGENT,PATCH>,py_class_name,std::shared_ptr<PATCH>>  expose_patch(py::module &m, string class_name_ptr){
    auto class_binds_obj =  py::class_<PATCH,basePatch<ENV,AGENT,PATCH>,py_class_name,std::shared_ptr<PATCH>>(m,class_name_ptr.c_str(),py::dynamic_attr())
        .def(py::init<shared_ptr<ENV>>())
        .def("empty_neighbor", &PATCH::empty_neighbor,"Return an empty patch around the patch",
            py::arg("quiet")=false)
        .def("find_neighbor_agents",&PATCH::find_neighbor_agents,"Returns a vector of agents in one patch neighborhood",
            py::arg("include_self")=true)
        .def_readwrite("coords",&PATCH::coords)
        .def_readwrite("agent",&PATCH::agent)
        .def_readwrite("empty",&PATCH::empty)
        .def_readwrite("on_border",&PATCH::on_border)
        .def_readwrite("neighbors",&PATCH::neighbors);
    return class_binds_obj;
        
}

void expose_exceptions(py::module m){
     py::register_exception<patch_availibility>(m, "patch_availibility");
     py::register_exception<undefined_member>(m, "undefined_member");
     
}
void expose_mesh(py::module &m){
    py::class_<MESH_ITEM>(m,"MESH_ITEM")
        .def(py::init<>()); 
    m.def("grid2",&grid2,"Creates 3D grid mesh", py::arg("length"), py::arg("width"), py::arg("mesh_length"),py::arg("share") = false);
    m.def("grid3",&grid3,"Creates 3D grid mesh", py::arg("length"), py::arg("width"),py::arg("height"), py::arg("mesh_length"),py::arg("share") = false);

}

template<class ENV,class AGENT,class PATCH>
void expose_containers(py::module &m){
    using PatchesBank = map<unsigned,shared_ptr<PATCH>>;
    py::bind_vector<vector<shared_ptr<AGENT>>>(m,"AgentsBank");
    py::bind_map<PatchesBank>(m,"PatchesBank")
    .def("clear",[](PatchesBank& data){
        data.clear();
    });
}

template<class ENV,class AGENT, class PATCH>
void expose_defaults(py::module &m){
    /** custom types **/

    // data types
    expose_containers<ENV,AGENT,PATCH>(m);
    // * Exceptions *
    expose_exceptions(m);
    // /** mesh **/
    expose_mesh(m);

}


