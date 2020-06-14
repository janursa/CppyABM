#include <iostream>
#include "mesh.h"
#include "tools.h"
#include "common.h"
#include "pybases.h"
#include "bases.h"


PYBIND11_MODULE(binds, m) {
	/** Envs **/
    link_env<Env,PyEnv<Env>>(m,"Env");
    
	/** Agent **/
    link_agent<Agent,PyAgent>(m,"Agent");
    /** Patch **/
    link_patch<Patch,PyPatch>(m,"Patch");
    /** Exceptions **/
    register_exceptions(m);
    /** mesh **/
    register_mesh(m);

}

