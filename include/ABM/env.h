#pragma once
#include <map>
#include <algorithm>
#include <random>
#include "common.h"
#include "mesh.h"
#include "tools.h"
#include "patch.h"
#include "agent.h"
using std::shared_ptr;
using std::vector;
using namespace std;

//!   Base clase for environments
/*!
  
*/
struct Env:enable_shared_from_this<Env>{
	virtual ~Env(){};
    AgentsBank agents;
    PatchesBank patches;
    vector<unsigned> patches_indices;
    //** Pure virtuals **/
    virtual shared_ptr<Patch> generate_patch() {
    	throw undefined_member("Generate patch is not defined inside Env");
    };
	virtual	shared_ptr<Agent> generate_agent(string class_name) {
		throw undefined_member("Generate agent is not defined inside Env");
	};
	// virtual void update_repo(){throw undefined_member("Update repository is not implemented but called");};
	virtual void update_repo(){}
	//** main functions **/
    void setup_domain(vector<MESH_ITEM> mesh);
    void step_agents();
    void step_patches();
    
	virtual void update();
    //** Place agents **/
    virtual void setup_agents(map<string,unsigned> config);
    void place_agent(shared_ptr<Patch> patch,shared_ptr<Agent> agent);
    void place_agent_randomly(shared_ptr<Agent> agent);
    shared_ptr<Patch> find_empty_patch(); //!< Finds empty patches in the entire domain
    void connect_patch_agent(shared_ptr<Patch> patch,shared_ptr<Agent> agent);

    virtual void step() {
    	throw undefined_member("Step function is not defined inside Env");
    };
    //** aux functions **//
    std::shared_ptr<Env> get_ptr(){
    	return this->shared_from_this();
    };
    map<string,unsigned> count_agents();

    /** Env data **/
    std::map<std::string,unsigned> agents_count;
    std::set<string> agent_classes;
    string class_name;
    virtual void set_data(string tag, double value) {throw undefined_member("Set data is used before implementation");};
    virtual double get_data(string tag) {throw undefined_member("Get data is used before implementation");};


};
