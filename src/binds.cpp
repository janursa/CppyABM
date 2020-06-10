#include <iostream>
#include "mesh.h"
#include "common.h"
#include "bases.h"
//!  Binds classes and modules
/*!
  Create py classes for each abstract class. Defines py module. 
*/
struct PyAgent : public Agent {
    using Agent::Agent;
    void step() override {
        PYBIND11_OVERLOAD_PURE(
            void, 
            Agent,      
            step         
        );
    }

    void inherit(shared_ptr<Agent> father) override{
    	PYBIND11_OVERLOAD(
            void, 
            Agent,      
            inherit,
            father         
        );
    };
};
struct PyEnv : public Env {
    using Env::Env;
    void setup() override {
        PYBIND11_OVERLOAD(
            void, 
            Env,      
            setup         
        );
    }
    shared_ptr<Patch> generate_patch() override {
        PYBIND11_OVERLOAD_PURE(
            shared_ptr<Patch>, 
            Env,      
            generate_patch         
        );
    }
    shared_ptr<Agent> generate_agent(string agent_name) override {
        PYBIND11_OVERLOAD_PURE(
            shared_ptr<Agent>, 
            Env,      
            generate_agent,
            agent_name         
        );
    }
    void update_repo() override {
        PYBIND11_OVERLOAD(
            void, 
            Env,      
            update_repo
                
        );
    }
};
struct PyPatch : public Patch {
    using Patch::Patch;
    void step() override {
    	PYBIND11_OVERLOAD(
    		void,
    		Patch,
    		step
    		);
    }
};

// test 
PYBIND11_MAKE_OPAQUE(vector<int>)
PYBIND11_MAKE_OPAQUE(vector<vector<int>>)


PYBIND11_MODULE(binds, m) {
	/** Types **/
	// py::bind_vector<VectorFloat>(m,"VectorFloat");
	py::bind_vector<vector<int>>(m,"vector_int");
	py::bind_vector<vector<vector<int>>>(m,"vector_vector_int");
	py::bind_vector<AgentsBank>(m,"AgentsBank");
	py::bind_map<PatchesBank>(m,"PatchesBank");
	auto bb = py::bind_map<map<string,double>>(m,"PatchDataBank");
	bb.def("keys",[](map<string,double> &v) {
       std::vector<std::string> retval;
		  for (auto const& element : v) {
		    retval.push_back(element.first);
		  }
		  return retval;
    });
    // bb.def("update",[](map<string,double> &v, map<string,vector<double>> pp) {
    // 		for (auto const &[key,value]:pp){
    // 			v[key] = value;
    // 		}
    // });
	/** Envs **/
	py::class_<Env,PyEnv,std::shared_ptr<Env>>(m,"Env",py::dynamic_attr())
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
		// .def_readwrite("data",&Env::data);	
	/** Agent **/
    py::class_<Agent,PyAgent,std::shared_ptr<Agent>>(m,"Agent",py::dynamic_attr())
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

    
    /** Patch **/
    py::class_<Patch,PyPatch,std::shared_ptr<Patch>>(m,"Patch",py::dynamic_attr())
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
    	

    /** Exceptions **/
    py::register_exception<patch_availibility>(m, "patch_availibility");
    /** mesh **/
    py::class_<mesh_tools>(m,"mesh_tools")
    	.def(py::init<>())
    	.def("grid",&mesh_tools::grid);
    /** custom types **/
    py::class_<MESH_ITEM>(m,"MESH_ITEM")
    	.def(py::init<>());	
    py::class_<HATCH_CONFIG>(m,"HATCH_CONFIG")
    	.def(py::init<>());	

}

