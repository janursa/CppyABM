#pragma once
#include "bases.h"
#include <map>
#include <algorithm>
#include "pybind11/pybind11.h"
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
namespace py=pybind11;
#define EXPOSE_AGENT_CONTAINER(...) \
namespace pybind11 { namespace detail { \
    template<> class type_caster<vector<shared_ptr<__VA_ARGS__>>> : public type_caster_base<vector<shared_ptr<__VA_ARGS__>>> { }; \
}}
#define EXPOSE_PATCH_CONTAINER(...) \
namespace pybind11 { namespace detail { \
    template<> class type_caster<map<unsigned,shared_ptr<__VA_ARGS__>>> : public type_caster_base<map<unsigned,shared_ptr<__VA_ARGS__>>> { }; \
}}


//!Tools for exposing C++ types and functions to Python
namespace bind_tools{
    //! Template trampoline for Env-based classes
    template<class ENV, class AGENT, class PATCH> 
    struct tramEnv : public ENV  {
        using ENV::ENV;
        shared_ptr<PATCH> generate_patch(MESH_ITEM mesh_item) override {
            PYBIND11_OVERLOAD(
                shared_ptr<PATCH>, 
                ENV,      
                generate_patch,
                mesh_item        
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
        void step() override {
            PYBIND11_OVERLOAD(
                void, 
                ENV,      
                step
                    
            );
        }

    };
    //! Template trampoline for Agent-based classes
    template<class ENV, class AGENT, class PATCH> 
    struct tramAgent : AGENT  {
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
    //! Template trampoline for Patch-based classes
    template<class ENV, class AGENT, class PATCH> 
    struct tramPatch : PATCH {
        using PATCH::PATCH;
        void step() override {
         PYBIND11_OVERLOAD(
             void,
             PATCH,
             step
             );
        }

    };
    //! Expose function for Env-based classes with trampoline. 
    template<class ENV,class AGENT,class PATCH,class tramclass>
    py::class_<ENV, tramclass,std::shared_ptr<ENV>> expose_env(py::module m, string class_name_string){
        auto class_binds_obj = 
        py::class_<ENV,tramclass,std::shared_ptr<ENV>> (m,class_name_string.c_str(),py::dynamic_attr())
            .def(py::init<>())
            .def("place_agent", py::overload_cast<shared_ptr<PATCH>,shared_ptr<AGENT>>(&ENV::place_agent), "Places the agent on the given patch")
            .def("place_agent", py::overload_cast<unsigned,shared_ptr<AGENT>>(&ENV::place_agent), "Places the agent on the given patch index")
            .def("place_agent_randomly",&ENV::place_agent_randomly)
            .def("setup_domain",&ENV::setup_domain)
            .def("step_agents",&ENV::step_agents)
            .def("step_patches",&ENV::step_patches)
            .def("update",&ENV::update)
            .def("step",&ENV::step)
            .def("setup_agents",&ENV::setup_agents)
            .def("count_agents",&ENV::count_agents)
            .def("memory_usage",&ENV::memory_usage)
            .def("activate_serial",&ENV::activate_serial)
            .def("activate_random",&ENV::activate_random)
            .def_readwrite("patches",&ENV::patches)
            .def_readwrite("agents",&ENV::agents);
        return class_binds_obj;
    }
    //! Template trampoline for Env-based classes without trampoline
    template<class ENV,class AGENT,class PATCH>
    py::class_<ENV,std::shared_ptr<ENV>> expose_env(py::module m, string class_name_string){
        auto class_binds_obj = 
        py::class_<ENV,std::shared_ptr<ENV>> (m,class_name_string.c_str(),py::dynamic_attr())
            .def(py::init<>())
            .def("memory_usage",&ENV::memory_usage)
            .def("place_agent_randomly",&ENV::place_agent_randomly)
            .def("setup_domain",&ENV::setup_domain)
            .def("step_agents",&ENV::step_agents)
            .def("step_patches",&ENV::step_patches)
            .def("place_agent",&ENV::place_agent)
            .def("update",&ENV::update)
            .def("step",&ENV::step)
            .def("setup_agents",&ENV::setup_agents)
            .def("count_agents",&ENV::count_agents)
            .def("connect_patch_agent", &ENV::connect_patch_agent)
            .def("activate_serial",&ENV::activate_serial)
            .def("activate_random",&ENV::activate_random)
            .def_readwrite("patches",&ENV::patches)
            .def_readwrite("agents",&ENV::agents);
        return class_binds_obj;
    }
    //! Template trampoline for Agent-based classes with trampoline
    template<class ENV,class AGENT,class PATCH,class tramclass>
    py::class_<AGENT, tramclass,std::shared_ptr<AGENT>>  expose_agent(py::module &m, string class_name_str){
        auto class_binds_obj = py::class_<AGENT, tramclass,std::shared_ptr<AGENT>>(m,class_name_str.c_str(),py::dynamic_attr())
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
            .def("get_patch",&AGENT::get_patch,"Retreives the patch")
            .def_readwrite("disappear",&AGENT::disappear)
            .def_readwrite("env",&AGENT::env)
            .def_readwrite("disappear",&AGENT::disappear)
            .def_readwrite("class_name",&AGENT::class_name);
        return class_binds_obj;
    }
     //! Template trampoline for Agent-based classes without trampoline
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
            .def("get_patch",&AGENT::get_patch,"Retreives the patch")
            .def_readwrite("disappear",&AGENT::disappear)
            .def_readwrite("env",&AGENT::env)
            .def_readwrite("disappear",&AGENT::disappear)
            .def_readwrite("class_name",&AGENT::class_name);
        return class_binds_obj;
    }

     //! Template trampoline for Patch-based classes with trampoline
    template<class ENV,class AGENT,class PATCH,class tramclass>
    py::class_<PATCH,tramclass,std::shared_ptr<PATCH>>  expose_patch(py::module &m, string class_name_ptr){
        auto class_binds_obj =  py::class_<PATCH,tramclass,std::shared_ptr<PATCH>>(m,class_name_ptr.c_str(),py::dynamic_attr())
            .def(py::init<shared_ptr<ENV>,MESH_ITEM>())
            .def("empty_neighbor", &PATCH::empty_neighbor,"Return an empty patch around the patch",
                py::arg("quiet")=false)
            .def("find_neighbor_agents",&PATCH::find_neighbor_agents,"Returns a vector of agents in one patch neighborhood",
                py::arg("include_self")=true)
            .def("get_agents",&PATCH::get_agents)
            .def("add_agent",&PATCH::add_agent)
            .def("remove_agent",&PATCH::remove_agent)
            .def("remove_agents",&PATCH::remove_agents)
            .def("empty",&PATCH::empty)
            .def_readwrite("on_border",&PATCH::on_border)
            .def_readwrite("neighbors",&PATCH::neighbors)
            .def_readwrite("index",&PATCH::index)
            .def_readwrite("coords",&PATCH::coords);
        return class_binds_obj;      
    }
     //! Template for Patch-based classes without trampoline
    template<class ENV,class AGENT,class PATCH>
    py::class_<PATCH,std::shared_ptr<PATCH>>  expose_patch(py::module &m, string class_name_ptr){
        auto class_binds_obj =  py::class_<PATCH,std::shared_ptr<PATCH>>(m,class_name_ptr.c_str(),py::dynamic_attr())
            .def(py::init<shared_ptr<ENV>,MESH_ITEM>())
            .def("empty_neighbor", &PATCH::empty_neighbor,"Return an empty patch around the patch",
                py::arg("quiet")=false)
            .def("find_neighbor_agents",&PATCH::find_neighbor_agents,"Returns a vector of agents in one patch neighborhood",
                py::arg("include_self")=true)
            .def("get_agents",&PATCH::get_agents)
            .def("add_agent",&PATCH::add_agent)
            .def("remove_agent",&PATCH::remove_agent)
            .def("remove_agents",&PATCH::remove_agents)
            .def("empty",&PATCH::empty)
            .def_readwrite("on_border",&PATCH::on_border)
            .def_readwrite("neighbors",&PATCH::neighbors)
            .def_readwrite("index",&PATCH::index)
            .def_readwrite("coords",&PATCH::coords);
        return class_binds_obj;
            
    }
     
     
     //! function to expose exceptions
    void expose_exceptions(py::module m){
         py::register_exception<patch_availibility>(m, "patch_availibility");
         py::register_exception<undefined_member>(m, "undefined_member");
         
    }
     //! function to expose mesh items
    void expose_mesh(py::module &m){
        py::class_<MESH_ITEM>(m,"MESH_ITEM")
            .def(py::init<>()); 
        auto space_sub = m.def_submodule("space");
        space_sub.def("grid2",&space::grid2,"Creates 3D grid mesh", py::arg("length"), py::arg("width"), py::arg("mesh_length"),py::arg("share") = false);
        space_sub.def("grid3",&space::grid3,"Creates 3D grid mesh", py::arg("length"), py::arg("width"),py::arg("height"), py::arg("mesh_length"),py::arg("share") = false);

    }
     //! funtion to expose agent and patch containers
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
        })
        .def("keys",[](PatchesBank& container){
              std::vector<unsigned> keys;
              for (auto const& element : container) {
                keys.push_back(element.first);
              }
            return keys;
        })
        .def("append",[](PatchesBank& container,shared_ptr<PATCH> patch){
            container.insert(std::pair<unsigned,shared_ptr<PATCH>>(patch->index,patch));
        });
    }
     //! Exposes default items 
    template<class ENV,class AGENT, class PATCH>
    void expose_defaults(py::module &m){
        expose_containers<ENV,AGENT,PATCH>(m);
        expose_exceptions(m);
        expose_mesh(m);
    }
    //!< the main class to create binding
    template<class env,class agent,class patch,class tramenv,class tramagent,class trampatch>
    struct Bind {
        using py_env =  py::class_<env,tramenv,std::shared_ptr<env>>;
        using py_agent = py::class_<agent,tramagent,std::shared_ptr<agent>>;
        using py_patch = py::class_<patch,trampatch,std::shared_ptr<patch>>;
        Bind(py::module m,string env_name,string agent_name,string patch_name):
        m_env(bind_tools::expose_env<env,agent,patch,tramenv>(m,env_name)),
        m_agent(bind_tools::expose_agent<env,agent,patch,tramagent>(m,agent_name)),
        m_patch(bind_tools::expose_patch<env,agent,patch,trampatch>(m,patch_name))
        {
            // expose defaults
            bind_tools::expose_defaults<env,agent,patch>(m);
            this->m = m;      
        }
        py_env get_env(){
            return m_env;
        }
        py_agent get_agent(){
            return m_agent;
        }
        py_patch get_patch(){
            return m_patch;
        }
        py::module m;
        py_env m_env;
        py_agent m_agent;
        py_patch m_patch;
    };
}


