#include <iostream>
#include "ABM/mesh.h"
#include "ABM/bind_tools.h"
#include "ABM/common.h"
#include "ABM/bases.h"
struct expEnv;
struct expPatch;
struct expAgent;
struct expEnv: public Env<expEnv,expAgent,expPatch>{

};
struct expAgent: public Agent<expEnv,expAgent,expPatch>{
    expAgent(shared_ptr<expEnv> env , string class_name):Agent<expEnv,expAgent,expPatch>(env,class_name){
        
    }
};
struct expPatch: public Patch<expEnv,expAgent,expPatch>{
    expPatch(shared_ptr<expEnv> env):Patch<expEnv,expAgent,expPatch>(env){}
};

EXPOSE_AGENT_CONTAINER(expAgent);
EXPOSE_PATCH_CONTAINER(expPatch);

PYBIND11_MODULE(cppyabm, m) {
    bind_tools::expose_defaults<expEnv,expAgent,expPatch>(m);
    bind_tools::expose_env<expEnv,expAgent,expPatch,bind_tools::tramEnv<expEnv,expAgent,expPatch>>(m,"Env");
    bind_tools::expose_agent<expEnv,expAgent,expPatch,bind_tools::tramAgent<expEnv,expAgent,expPatch>>(m,"Agent");
    bind_tools::expose_patch<expEnv,expAgent,expPatch,bind_tools::tramPatch<expEnv,expAgent,expPatch>>(m,"Patch");
}

