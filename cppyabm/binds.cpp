#include <iostream>
#include "cppyabm/mesh.h"
#include "cppyabm/bind_tools.h"
#include "cppyabm/common.h"
#include "cppyabm/bases.h"
struct expEnv;
struct expPatch;
struct expAgent;
struct expMultiPatch;

// these classes are just to enable a connection between different classes 
struct expEnv: public Env<expEnv,expAgent,expPatch>{};
struct expAgent: public Agent<expEnv,expAgent,expPatch>{
    expAgent(shared_ptr<expEnv> env , string class_name):Agent<expEnv,expAgent,expPatch>(env,class_name){}
};
struct expPatch: public Patch<expEnv,expAgent,expPatch>{
    expPatch(shared_ptr<expEnv> env, MESH_ITEM mesh_item):Patch<expEnv,expAgent,expPatch>(env,mesh_item){}
};


EXPOSE_AGENT_CONTAINER(expAgent);
EXPOSE_PATCH_CONTAINER(expPatch);

PYBIND11_MODULE(binds, m) {
    using tramEnv = bind_tools::tramEnv<expEnv,expAgent,expPatch>;
    using tramAgent = bind_tools::tramAgent<expEnv,expAgent,expPatch>;
    using tramPatch = bind_tools::tramPatch<expEnv,expAgent,expPatch>;

    bind_tools::expose_defaults<expEnv,expAgent,expPatch>(m);
    bind_tools::expose_env<expEnv,expAgent,expPatch,tramEnv>(m,"Env");
    bind_tools::expose_agent<expEnv,expAgent,expPatch,tramAgent>(m,"Agent");
    patch_obj = bind_tools::expose_patch<expEnv,expAgent,expPatch,tramPatch>(m,"Patch");
}

