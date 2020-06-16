#include <iostream>
#include "CPPYABM/mesh.h"
#include "CPPYABM/tools.h"
#include "CPPYABM/common.h"
#include "CPPYABM/pybases.h"
#include "CPPYABM/bases.h"


PYBIND11_MODULE(CPPYABM, m) {
	/** Envs **/
    // data types
    py::bind_vector<AgentsBank>(m,"AgentsBank");
    py::bind_map<PatchesBank>(m,"PatchesBank");
    // class
    expose_base_env(m);
	/** Agent **/
    expose_base_agent(m);
    /** Patch **/
    expose_base_patch(m);
    /** Exceptions **/
    register_exceptions(m);
    /** mesh **/
    register_mesh(m);

}

