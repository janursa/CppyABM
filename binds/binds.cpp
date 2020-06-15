#include <iostream>
#include "mesh.h"
#include "tools.h"
#include "common.h"
#include "pybases.h"
#include "bases.h"


PYBIND11_MODULE(binds, m) {
	/** Envs **/

    py::bind_vector<AgentsBank>(m,"AgentsBank");
    py::bind_map<PatchesBank>(m,"PatchesBank");
    link_env<Env,PyEnv<Env>>(m,"Env");
    
	/** Agent **/
    link_agent<Agent,PyAgent<Agent>>(m,"Agent");
    /** Patch **/
    auto bb = py::bind_map<map<string,double>>(m,"PatchDataBank"); //TODO: needs to go
    bb.def("keys",[](map<string,double> &v) {
       std::vector<std::string> retval;
       for (auto const& element : v) {
         retval.push_back(element.first);
       }
       return retval;
    });
    link_patch<Patch,PyPatch<Patch>>(m,"Patch");
    /** Exceptions **/
    register_exceptions(m);
    /** mesh **/
    register_mesh(m);

}

