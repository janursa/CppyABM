#include <iostream>
#include "bind_agent.h"
#include "bind_env.h"
#include "mesh.h"
#include "bind_patch.h"
#include "common.h"

PYBIND11_MODULE(binds, m) {
	/** Types **/
	py::bind_vector<AgentsBank>(m,"AgentsBank");
	py::bind_map<PatchesBank>(m,"PatchesBank");

	/** Envs **/
	py::class_<Env,PyEnv,std::shared_ptr<Env>>(m,"Env",py::dynamic_attr())
    	.def(py::init<>())
    	.def("check",&Env::check)
		.def("place_agent_randomly",&Env::place_agent_randomly)
    	.def("setup_domain",&Env::setup_domain)
    	.def("step_agents",&Env::step_agents)
    	.def("place_agent",&Env::place_agent)
    	.def("update",&Env::update)
    	.def_readwrite("patches",&Env::patches)
    	.def_readwrite("agents",&Env::agents);
	
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
    	.def_readwrite("coords",&Patch::coords)
    	.def_readwrite("agent",&Patch::agent)
    	.def_readwrite("empty",&Patch::empty)
    	.def_readwrite("disappear",&Patch::disappear)
    	.def("empty_neighbor", &Patch::empty_neighbor,"Return an empty patch around the patch",
    		py::arg("quiet")=false);

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

