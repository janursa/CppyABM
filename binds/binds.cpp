#include <iostream>
#include "mesh.h"
#include "tools.h"
#include "common.h"
#include "pybases.h"
#include "bases.h"


PYBIND11_MODULE(binds, m) {
	/** Types **/
	py::bind_vector<AgentsBank>(m,"AgentsBank");
	py::bind_map<PatchesBank>(m,"PatchesBank");
	auto bb = py::bind_map<map<string,double>>(m,"PatchDataBank"); //TODO: needs to go
	bb.def("keys",[](map<string,double> &v) {
       std::vector<std::string> retval;
		  for (auto const& element : v) {
		    retval.push_back(element.first);
		  }
		  return retval;
    });
	/** Envs **/
    link_env<Env,PyEnv>(m,"Env");
	/** Agent **/
    link_agent<Agent,PyAgent>(m,"Agent");
    /** Patch **/
    link_patch<Patch,PyPatch>(m,"Patch");
    
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

