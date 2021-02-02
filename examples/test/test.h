#include "CppyABM/include/ABM/bases.h"
// declare the derived classes
struct myEnv;
struct myAgent;
struct myPatch;
// define myEnv
struct myEnv: public Env<myEnv,myAgent,myPatch> /*Env is inherited using template arguments*/ {
	myEnv():Env<myEnv,myAgent,myPatch>() /*a call to the base class's instructor*/{ 
	}
	shared_ptr<myAgent> generate_agent(std::string agent_name){
		auto agent_obj = make_shared<myAgent>(this->shared_from_this(),agent_name);
		this->agents.push_back(agent_obj);
		return agent_obj;
	};
	shared_ptr<myPatch> generate_patch(MESH_ITEM mesh){
		auto patch_obj = make_shared<myPatch>(this->shared_from_this(),mesh);
		this->patches.insert(pair<unsigned,shared_ptr<myPatch>>(patch_obj->index,patch_obj));
		return patch_obj;
	};
};
struct myPatch: public Patch<myEnv,myAgent,myPatch> {
	myPatch(shared_ptr<myEnv> env,MESH_ITEM mesh):Patch<myEnv,myAgent,myPatch>(env,mesh)/*a call to the base class's instructor*/{

	}
};
struct myAgent: public Agent<myEnv,myAgent,myPatch> {
	myAgent(shared_ptr<myEnv> env,std::string agent_name):Agent<myEnv,myAgent,myPatch>(env,agent_name)/*a call to the base class's instructor*/{
	}
};