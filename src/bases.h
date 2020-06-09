
#pragma once
#include <map>
#include <algorithm>
#include <pybind11/stl.h>
#include "pybind11/pybind11.h"
#include "toolbox.h"
#include "common.h"
#include "mesh.h"
namespace py=pybind11;
using std::shared_ptr;
using std::vector;
using namespace tools;
struct Env;
struct Agent;
struct Patch;
using namespace std;
struct Base{
	std::string name; 
	
	/** storage **/
    vector<float> data;

    bool disappear = false;

    /** General **/

protected:

};

struct Patch: public Base{
	// Patch(){}
	Patch(shared_ptr<Env> env){
		this->env = env;
	}
	virtual ~Patch(){}
	unsigned index;
	vector<float> coords;
	vector<unsigned> neighbors_indices;
	vector<shared_ptr<Patch>> neighbors;
	/** Auxilliry funcs **/
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
	shared_ptr<Patch> empty_neighbor(bool quiet);

	bool empty = true;
	/** connectors **/
	unsigned agent_count = 0; //!< For debugging 
	std::shared_ptr<Agent> agent;
	std::shared_ptr<Env> env;
};
struct Agent: public Base,enable_shared_from_this<Agent>{
	Agent(shared_ptr<Env> env , string class_name){
		this->env = env;
		this->name = class_name;
	}
	virtual ~Agent(){};
	/** Major functions **/ 
	virtual void step()=0;
	virtual void update(){};
    /** Flags **/
    std::pair <bool,std::string> switch_info = std::make_pair(false,"");
    HATCH_CONFIG _hatch = HATCH_CONFIG();
    MOVE_CONFIG _move = MOVE_CONFIG();
    /** pure virtuals **/

    /** Auxillary funcs **/
    virtual void inherit(shared_ptr<Agent> father){};
    void set_patch(shared_ptr<Patch> patch){ this->patch = patch;}
    void move(shared_ptr<Patch> dest, bool quiet);
    std::shared_ptr<Agent> get_ptr();
    void order_hatch(shared_ptr<Patch> patch, bool inherit, bool quiet, bool reset);
    void order_move(shared_ptr<Patch> patch, bool quiet, bool reset);

    void reset_hatch(){ this->_hatch = HATCH_CONFIG();}
    void reset_move(){ this->_move = MOVE_CONFIG();}

    /** connectors **/
	std::shared_ptr<Patch> patch;
	std::shared_ptr<Env> env;

};
inline void Agent::order_hatch(shared_ptr<Patch> patch = nullptr, 
		    bool inherit = false, bool quiet = false, bool reset = false){
	this->_hatch =  HATCH_CONFIG(true,patch,inherit,quiet,reset);

}
inline void Agent::order_move(shared_ptr<Patch> patch = nullptr, 
		     bool quiet = false, bool reset = false){
	this->_move =  MOVE_CONFIG(true,patch,quiet,reset);

}

inline std::shared_ptr<Agent> Agent::get_ptr(){
    return this->shared_from_this();

}

inline shared_ptr<Patch> Patch::empty_neighbor(bool quiet = false){
    	auto neighbors = this->neighbors;
    	std::random_device rd;
		std::mt19937 g(rd());
    	std::shuffle(neighbors.begin(),neighbors.end(),g);
    	for (auto &neighbor:neighbors){
    		if (neighbor->empty){
    			return neighbor;
    		}
    	}
    	if (!quiet) throw patch_availibility("No available patch around the agent");
    	return nullptr;
}
struct Env: public Base{

	virtual ~Env(){};
    AgentsBank agents;
    PatchesBank patches;
    vector<unsigned> patches_indices;
    //** Pure virtuals **/
    virtual void setup()=0;
    virtual shared_ptr<Patch> generate_patch() = 0;
	virtual	shared_ptr<Agent> generate_agent(string agent_name) = 0;

	//** main functions **/
    void setup_domain(vector<MESH_ITEM> mesh);
    virtual void update();
    //** Place agents **/
    void place_agent(shared_ptr<Patch> patch,shared_ptr<Agent> agent);
    void place_agent_randomly(shared_ptr<Agent> agent);
    shared_ptr<Patch> find_empty_patch(); //!< Finds empty patches in the entire domain
    void connect_patch_agent(shared_ptr<Patch> patch,shared_ptr<Agent> agent);
    void step_agents();
    void check(){
    	
    }


};
inline void Env::step_agents(){

    	for (unsigned i = 0; i < this->agents.size(); i++){
    		this->agents[i]->step();
    	}
}
inline void Env::update(){
	
	/** hatch **/
	unsigned agent_count = this->agents.size();
	for (unsigned  i = 0; i < agent_count; i++){

		if (this->agents[i]->_hatch._flag){
			auto inherit = this->agents[i]->_hatch._inherit;
			auto patch = this->agents[i]->_hatch._patch;

			if (patch == nullptr){ // find a random patch
				try{
					patch = this->agents[i]->patch->empty_neighbor();
				}
				// failure: manage how to respond
				catch(patch_availibility&ee){
					
					if (!this->agents[i]->_hatch._quiet){ // only throw when it's forced
						if (this->agents[i]->_hatch._reset){ // the try failed so reset the order
							this->agents[i]->reset_hatch();
						}
						throw ee;
					} 
					else{ // go to the next agent
						if (this->agents[i]->_hatch._reset){ // the try failed so reset the order
							this->agents[i]->reset_hatch();
						}
						continue;
					}
				}
				
			}
			// check if another agent hasn't already taken it 
			if (!patch->empty){ 
				// failure: manage how to respond
				
				if (this->agents[i]->_hatch._quiet){ // only throw when it's forced
					if (this->agents[i]->_hatch._reset){ // the try failed so reset the order
						this->agents[i]->reset_hatch();
					}	
					continue;
				}
				else{
					if (this->agents[i]->_hatch._reset){ // the try failed so reset the order
						this->agents[i]->reset_hatch();
					}
					throw patch_availibility("No patch for hatching. If you want to silent this error, pass argumen quiet as true");
				}
			}
			auto new_agent = this->generate_agent(this->agents[i]->name);
			if (this->agents[i]->_hatch._inherit){
				new_agent->inherit(this->agents[i]);
			}
			this->connect_patch_agent(patch,new_agent);
			this->agents[i]->reset_hatch();
		};
	}
	/** move **/
	for (unsigned  i = 0; i < agent_count; i++){
		if (!this->agents[i]->_move._flag) continue;
		auto patch = this->agents[i]->_move._patch;
		if (patch == nullptr){ // find a random patch
				try{
					patch = this->agents[i]->patch->empty_neighbor();
				}
				// failure: manage how to respond
				catch(patch_availibility&ee){
					
					if (!this->agents[i]->_move._quiet){ // only throw when it's forced
						if (this->agents[i]->_move._reset){ // the try failed so reset the order
							this->agents[i]->reset_move();
						}
						throw ee;
					} 
					else{ // go to the next agent
						if (this->agents[i]->_move._reset){ // the try failed so reset the order
							this->agents[i]->reset_move();
						}
						continue;
					}
				}
				
			}
			// check if another agent hasn't already taken it 
			if (!patch->empty){ 
				// failure: manage how to respond
				
				if (this->agents[i]->_move._quiet){ // only throw when it's forced
					if (this->agents[i]->_move._reset){ // the try failed so reset the order
						this->agents[i]->reset_move();
					}	
					continue;
				}
				else{
					if (this->agents[i]->_move._reset){ // the try failed so reset the order
						this->agents[i]->reset_move();
					}
					throw patch_availibility("No patch for moving. If you want to silent this error, pass argumen quiet as true");
				}
			}

			this->agents[i]->move(patch,this->agents[i]->_move._quiet);

			this->agents[i]->reset_move();

	}
	
};
inline void Env::place_agent(shared_ptr<Patch> patch,shared_ptr<Agent> agent){
	if (!patch->empty) throw patch_availibility("Patch is not empty");
	connect_patch_agent(patch,agent);
}
inline void Env::connect_patch_agent(shared_ptr<Patch> patch,shared_ptr<Agent> agent){
	patch->set_agent(agent);
	agent->set_patch(patch);
}
inline void Env::place_agent_randomly(shared_ptr<Agent> agent){
	auto patch = this->find_empty_patch();
	connect_patch_agent(patch,agent);
}
inline shared_ptr<Patch> Env::find_empty_patch(){
	/**
	 * Finds the first empty patch. If none found, throws an exeption.
	 */
	auto patches_indices_copy = this->patches_indices;
	auto patch_count = this->patches_indices.size();

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(patches_indices_copy.begin(), patches_indices_copy.end(), g);
	
	for (auto const&i:patches_indices_copy){
		auto potential_patch = this->patches.at(i);
		if (potential_patch->empty){
			return potential_patch;
		}
	}
	throw patch_availibility("All patches are occupied.");
}
inline void Env::setup_domain(vector<MESH_ITEM> mesh){
    	/** create patches **/ 
		// step 1: create patches from info of meshes
		
		for (auto & mesh_item:mesh){
			auto patch = this->generate_patch(); // create patch
			patch->index = mesh_item.index;      // copy index
			patch->coords = mesh_item.coords;    // copy coords
			patch->neighbors_indices = mesh_item.neighbors_indices;  // copy neighbors indices
			patches[patch->index]= patch;
			this->patches_indices.push_back(patch->index);
		}
		// step 2: assign neighbor patches
		for (auto &[index,patch]:patches){
			vector<shared_ptr<Patch>> neighbors;
			auto neighbors_indices = patch->neighbors_indices;
			for (auto const &neighbor_index:neighbors_indices){
				auto neighbor_patch = patches.at(neighbor_index);
				neighbors.push_back(neighbor_patch);
			}
			patch->neighbors = neighbors;
		}
    }
void Agent::move(shared_ptr<Patch> dest, bool quiet = false){
    	if (!dest->empty) {
    		if (!quiet) throw patch_availibility("Given patch for move is not empty");
    		else return;
    	}
    	this->patch->remove_agent(); // remove it from the current patch
    	this->env->connect_patch_agent(dest,this->get_ptr());
    }