#include <iostream>
#include "mesh.h"
#include "tools.h"
#include "common.h"
#include "pybases.h"
#include "bases.h"


PYBIND11_MODULE(binds, m) {
	/** Envs **/
    // data types
    py::bind_vector<AgentsBank>(m,"AgentsBank");
    py::bind_map<PatchesBank>(m,"PatchesBank");
    // class
    expose_base_env(m);
	/** Agent **/
    expose_base_agent(m);
    /** Patch **/
    // data types
    auto bb = py::bind_map<map<string,double>>(m,"PatchDataBank"); //TODO: needs to go
    bb.def("keys",[](map<string,double> &v) {
       std::vector<std::string> retval;
       for (auto const& element : v) {
         retval.push_back(element.first);
       }
       return retval;
    });
    // class
    expose_base_patch(m);
    /** Exceptions **/
    register_exceptions(m);
    /** mesh **/
    register_mesh(m);

}

