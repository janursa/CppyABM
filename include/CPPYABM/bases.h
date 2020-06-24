
#pragma once
#include <map>
#include <algorithm>
#include <random>
#include "pybind11/pybind11.h"
// #include <nlohmann/json.hpp>
// using json = nlohmann::json
#include "common.h"
#include "mesh.h"
#include "tools.h"
namespace py=pybind11;
using std::shared_ptr;
using std::vector;
struct Env;
struct Agent;
struct Patch;
using namespace std;
//!  Base class
/*!
  All classes inherit this
*/
struct Base{
	string class_name;
    bool disappear = false;
    virtual void set_data(string tag) {throw undefined_member("Set data is used before implementation");};
    virtual double get_data(string tag) {throw undefined_member("Get data is used before implementation");};

};
//!   Base clase for patches
/*!
  
*/
struct Patch: public Base{
	Patch(shared_ptr<Env> env){
		this->env = env;
	}
	virtual ~Patch(){}
	unsigned index;
	vector<double> coords;
	vector<unsigned> neighbors_indices;
	vector<shared_ptr<Patch>> neighbors;
	/** main funcs **/
	virtual void step(){
		throw undefined_member("Step function is not defined inside Patch");
	};
	/** Auxs funcs **/
	void set_agent(shared_ptr<Agent> agent){
		this->agent = agent;
		this->empty = false;
		this->agent_count ++;
		if (this->agent_count>1){
			cout<<"Patch holds more than an agent"<<endl;
		}
	}
	void remove_agent(){
		this->agent = nullptr;
		this->empty = true;
		this->agent_count = 0;
	}
	shared_ptr<Patch> empty_neighbor(bool quiet = false);
	vector<shared_ptr<Agent>> find_neighbor_agents(bool include_self = true){
		vector<shared_ptr<Agent>> neighbor_agents;
		if (!this->empty and include_self) neighbor_agents.push_back(this->agent);
		for (auto const &patch:this->neighbors){
			if (!patch->empty) neighbor_agents.push_back(patch->agent);
		}
		return neighbor_agents;
	}
	/** connectors **/
	unsigned agent_count = 0; //!< For debugging 
	std::shared_ptr<Agent> agent;
	std::shared_ptr<Env> env;
	/** patch data **/
	bool empty = true;

};

//!   Base clase for Agents
/*!
  
*/
struct Agent: public Base,enable_shared_from_this<Agent>{
	Agent(shared_ptr<Env> env , string class_name){
		this->env = env;
		this->class_name = class_name;
	}
	virtual ~Agent(){};
	/** Major functions **/ 
	virtual void step(){
		throw undefined_member("Agent step function is not defined");
	};
    /** Flags **/
    std::pair <bool,std::string> switch_info = std::make_pair(false,"");
    HATCH_CONFIG _hatch = HATCH_CONFIG();
    MOVE_CONFIG _move = MOVE_CONFIG();
    SWITCH_CONFIG _switch = SWITCH_CONFIG();
    /** pure virtuals **/

    /** Auxillary funcs **/
    virtual void inherit(shared_ptr<Agent> father){cout<<"Inherit is not defined"<<endl;};
    virtual void update(){};
    void set_patch(shared_ptr<Patch> patch){ this->patch = patch;}
    void move(shared_ptr<Patch> dest, bool quiet = false);
    std::shared_ptr<Agent> get_ptr();
    void order_hatch(shared_ptr<Patch> patch = nullptr, 
            bool inherit = false, bool quiet = false, bool reset = false);
    void order_move(shared_ptr<Patch> patch, bool quiet, bool reset);
    void order_switch(string to){
    	this->_switch = SWITCH_CONFIG(true,to);
    };

    void reset_hatch(){ this->_hatch = HATCH_CONFIG();}
    void reset_move(){ this->_move = MOVE_CONFIG();}
    void reset_switch(){ this->_switch = SWITCH_CONFIG();}

    /** connectors **/
	std::shared_ptr<Patch> patch;
	std::shared_ptr<Env> env;

};


//!   Base clase for environments
/*!
  
*/
struct Env: public Base,enable_shared_from_this<Env>{
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
	virtual void update_repo(){throw undefined_member("Update repository is not implemented but called");};
	//** main functions **/
    void setup_domain(vector<MESH_ITEM> mesh);
    void step_agents();
    void step_patches();
    virtual void update();
    //** Place agents **/
    void setup_agents(map<string,unsigned> config);
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

};

