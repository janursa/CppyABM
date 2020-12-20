#pragma once
#include "bases.h"
#include <map>
#include <algorithm>
#include "pybind11/pybind11.h"
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include "pybases.h"
namespace py=pybind11;
#define EXPOSE_AGENT_CONTAINER(...) \
namespace pybind11 { namespace detail { \
    template<> class type_caster<vector<shared_ptr<__VA_ARGS__>>> : public type_caster_base<vector<shared_ptr<__VA_ARGS__>>> { }; \
}}
#define EXPOSE_PATCH_CONTAINER(...) \
namespace pybind11 { namespace detail { \
    template<> class type_caster<map<unsigned,shared_ptr<__VA_ARGS__>>> : public type_caster_base<map<unsigned,shared_ptr<__VA_ARGS__>>> { }; \
}}
#define MAKE_OPAQUE(...) \
namespace pybind11 { namespace detail { \
    template<> class type_caster<__VA_ARGS__> : public type_caster_base<__VA_ARGS__> { }; \
}}


namespace binds_tools{
    template<class ENV,class AGENT,class PATCH,typename py_class_name>
    py::class_<ENV, py_class_name,std::shared_ptr<ENV>> expose_env(py::module m, string class_name_string){
        auto class_binds_obj = 
        py::class_<ENV,py_class_name,std::shared_ptr<ENV>> (m,class_name_string.c_str(),py::dynamic_attr())
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
    py::class_<ENV,std::shared_ptr<ENV>> expose_env(py::module m, string class_name_string){
        auto class_binds_obj = 
        py::class_<ENV,std::shared_ptr<ENV>> (m,class_name_string.c_str(),py::dynamic_attr())
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

    template<class ENV,class AGENT,class PATCH,class py_class_name>
    py::class_<AGENT, py_class_name,std::shared_ptr<AGENT>>  expose_agent(py::module &m, string class_name_str){
        auto class_binds_obj = py::class_<AGENT, py_class_name,std::shared_ptr<AGENT>>(m,class_name_str.c_str(),py::dynamic_attr())
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
    py::class_<AGENT,std::shared_ptr<AGENT>>  expose_agent(py::module &m, string class_name_str){
        auto class_binds_obj = py::class_<AGENT, std::shared_ptr<AGENT>>(m,class_name_str.c_str(),py::dynamic_attr())
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


    template<class ENV,class AGENT,class PATCH,class py_class_name>
    py::class_<PATCH,py_class_name,std::shared_ptr<PATCH>>  expose_patch(py::module &m, string class_name_ptr){
        auto class_binds_obj =  py::class_<PATCH,py_class_name,std::shared_ptr<PATCH>>(m,class_name_ptr.c_str(),py::dynamic_attr())
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
    template<class ENV,class AGENT,class PATCH>
    py::class_<PATCH,std::shared_ptr<PATCH>>  expose_patch(py::module &m, string class_name_ptr){
        auto class_binds_obj =  py::class_<PATCH,std::shared_ptr<PATCH>>(m,class_name_ptr.c_str(),py::dynamic_attr())
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
        using AgentsBank = vector<shared_ptr<AGENT>>;
        using PatchesBank = map<unsigned,shared_ptr<PATCH>>;
        py::bind_vector<AgentsBank>(m,"AgentsBank")
        .def("append",[](AgentsBank& container,shared_ptr<AGENT> agent){
            container.push_back(agent);
        });
        py::bind_map<PatchesBank>(m,"PatchesBank")
        .def("clear",[](PatchesBank& container){
            container.clear();
        });
    }

    template<class ENV,class AGENT, class PATCH>
    void expose_defaults(py::module &m){
        expose_containers<ENV,AGENT,PATCH>(m);
        expose_exceptions(m);
        expose_mesh(m);
    }
}


