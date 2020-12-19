#include <iostream>
#include "ABM/mesh.h"
#include "ABM/bind_tools.h"
#include "ABM/common.h"
#include "ABM/pybases.h"
#include "ABM/bases.h"

using AgentsBank = vector<shared_ptr<expAgent>>;
using PatchesBank = map<unsigned,shared_ptr<expPatch>>;
PYBIND11_MAKE_OPAQUE(AgentsBank);
PYBIND11_MAKE_OPAQUE(PatchesBank);

PYBIND11_MODULE(cppyabm, m) {
	/** Envs **/
    expose_defaults<expEnv,expAgent,expPatch>(m);

    expose_base_env<expEnv,expAgent,expPatch>(m,"baseEnv");
    expose_base_agent<expEnv,expAgent,expPatch>(m,"baseAgent");
    expose_base_patch<expEnv,expAgent,expPatch>(m,"basePatch");
    expose_env<expEnv,expAgent,expPatch,PyEnv<expEnv,expAgent,expPatch>>(m,"Env");
    expose_agent<expEnv,expAgent,expPatch,PyAgent<expEnv,expAgent,expPatch>>(m,"Agent");
    expose_patch<expEnv,expAgent,expPatch,PyPatch<expEnv,expAgent,expPatch>>(m,"Patch");
}

