#include <iostream>
#include "ABM/mesh.h"
#include "ABM/bind_tools.h"
#include "ABM/common.h"
#include "ABM/pybases.h"
#include "ABM/bases.h"
struct expEnv;
struct expPatch;
struct expAgent;
struct expEnv: public baseEnv<expEnv,expAgent,expPatch>{

};
struct expAgent: public baseAgent<expEnv,expAgent,expPatch>{
    expAgent(shared_ptr<expEnv> env , string class_name):baseAgent<expEnv,expAgent,expPatch>(env,class_name){
        
    }
};
struct expPatch: public basePatch<expEnv,expAgent,expPatch>{
    expPatch(shared_ptr<expEnv> env):basePatch<expEnv,expAgent,expPatch>(env){}
};

EXPOSE_AGENT_CONTAINER(expAgent);
EXPOSE_PATCH_CONTAINER(expPatch);

PYBIND11_MODULE(cppyabm, m) {
    binds_tools::expose_defaults<expEnv,expAgent,expPatch>(m);
    binds_tools::expose_env<expEnv,expAgent,expPatch,PyEnv<expEnv,expAgent,expPatch>>(m,"Env");
    binds_tools::expose_agent<expEnv,expAgent,expPatch,PyAgent<expEnv,expAgent,expPatch>>(m,"Agent");
    binds_tools::expose_patch<expEnv,expAgent,expPatch,PyPatch<expEnv,expAgent,expPatch>>(m,"Patch");
}

