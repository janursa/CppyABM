
#pragma once
#include <map>
#include <algorithm>
#include <random>
#include <set>
#include "common.h"
#include "mesh.h"
#include "tools.h"
using std::shared_ptr;
using std::vector;
// #define MEMORY_MONITOR
#ifdef MEMORY_MONITOR
#include<mach/mach.h>
#endif
template<class ENV, class AGENT, class PATCH>
struct Env;
template<class ENV, class AGENT, class PATCH>
struct Agent;
template<class ENV, class AGENT, class PATCH>
struct Patch;

//!   Base class for patch
/*!
  Patch class abstracts non-movable discrete elements of the simulation domain.
*/
template<class ENV, class AGENT, class PATCH>
struct Patch: public enable_shared_from_this<PATCH>{
	Patch(shared_ptr<ENV> env,MESH_ITEM mesh_item){
		this->env = env;
        this->index = mesh_item.index;      // copy index
        this->coords = mesh_item.coords;    // copy coords
        this->neighbors_indices = mesh_item.neighbors_indices;  // copy neighbors indices
        try {
            this->layer_index = mesh_item.layer_index;      // copy layer index

        }
        catch (...){

        }
        try {
            this->on_border = mesh_item.on_border;
        }
        catch (...){

        }
	}
	virtual ~Patch(){}

	virtual void step(){
		throw undefined_member("Step function is not defined inside Patch");
	}; //!< To define patch behavior
	void set_agent(shared_ptr<AGENT> agent){
		this->agent = agent;
		// this->empty = false;
		this->agent_count ++;
        agent->patch = this->shared_from_this();
        agent->has_patch = true;
	}; //!< Assigns agent to the patch
    shared_ptr<AGENT> get_agent(){
        shared_ptr<AGENT> p = this->agent.lock();
        if (p){
            return p;
        }else{
            throw invalid_pointer("poiner to agent is invalid inside patch");

        }
    }
    bool empty(){
        shared_ptr<AGENT> p = this->agent.lock();
        if (p){
            return false;
        }else{
            return true;

        }
    }
	void remove_agent(){
        this->get_agent()->has_patch = false;
        this->get_agent()->patch = nullptr;
		this->agent.reset();
		this->agent_count = 0;
	} //!< Removes agent from the patch
	shared_ptr<PATCH> empty_neighbor(bool quiet = false); //!< Returns an arbitrary adjacent patch without an agent 
	vector<shared_ptr<AGENT>> find_neighbor_agents(bool include_self = true); //!< Returns agents in one patch neighbors
	unsigned agent_count = 0; //!< Keeps the record of residing agents count
	std::weak_ptr<AGENT> agent; //!< Pointer to stores agents
	std::shared_ptr<ENV> env; //!< Pointer to the simulation environment
	// bool empty = true; //!< whether the patch host an agent
	bool on_border = false;//!< whether the patch residing on the border of the domain
    unsigned index; //!< unique index that associate patch to mesh
    unsigned layer_index; //!< the index of the layer in which the patch is residing
    vector<double> coords; //!< the coordinates of the patch
    vector<unsigned> neighbors_indices; //!< the indices of the neighbor patches
    vector<shared_ptr<PATCH>> neighbors; //!< list of neighbor patches
};

//!   Base class for Agent
/*!
  Agent class simulates movable objects
*/
template<class ENV, class AGENT, class PATCH>
struct Agent: public enable_shared_from_this<AGENT>{
	Agent(shared_ptr<ENV> env , string class_name){
		this->env = env;
		this->class_name = class_name;
	}
	virtual ~Agent(){};
	
	virtual void step(){
		throw undefined_member("Agent step function is not defined");
	}; //!< To define agent behavior
    virtual void inherit(shared_ptr<AGENT> father){
        cout<<"Inherit is not defined"<<endl;
    };//!< To define inheritage 
    virtual void update(){};  
    void move(shared_ptr<PATCH> dest, bool quiet = false); //!< Moves the agent to the given destination patch
    void order_hatch(shared_ptr<PATCH> patch = nullptr, 
            bool inherit = false, bool quiet = false, bool reset = false); //!< Orders agent to hatch. This will execute during `Env::update`.
    void order_move(shared_ptr<PATCH> patch = nullptr, 
             bool quiet = false, bool reset = false); //!< Orders agent to move. This will execute during `Env::update`.
    void order_switch(string to){
    	this->_switch = SWITCH_CONFIG(true,to);
    }; //!< Orders agent to switch to another agent. This will execute during `Env::update`.

    void reset_hatch(){ this->_hatch = HATCH_CONFIG<ENV,AGENT,PATCH>();} 
    void reset_move(){ this->_move = MOVE_CONFIG<ENV,AGENT,PATCH>();}
    void reset_switch(){ this->_switch = SWITCH_CONFIG();}
    std::pair <bool,std::string> switch_info = std::make_pair(false,"");
    HATCH_CONFIG<ENV,AGENT,PATCH> _hatch = HATCH_CONFIG<ENV,AGENT,PATCH>();
    MOVE_CONFIG<ENV,AGENT,PATCH> _move = MOVE_CONFIG<ENV,AGENT,PATCH>();
    SWITCH_CONFIG _switch = SWITCH_CONFIG();
	std::shared_ptr<PATCH> patch; //!< Pointer to the residing patch
	std::shared_ptr<ENV> env; //!< Pointer to the simulation world
	bool disappear = false; //!< if set to true, the agent will be removed from the simulation. This will execute during `Env::update`
    string class_name; //!< Unique ID of this class
    bool has_patch = false; //!< Indicates if an agent residing on a patch
};

//!   Base class for environment
/*!
Env class stores and coordinates agents and patches.
*/
template<class ENV, class AGENT, class PATCH>
struct Env: public enable_shared_from_this<ENV>{
    Env(){};
	virtual ~Env(){};
    virtual shared_ptr<PATCH> generate_patch(MESH_ITEM) {
    	throw undefined_member("Generate patch is not defined inside Env");
    }; //!< A template class to generate patch.
	virtual	shared_ptr<AGENT> generate_agent(string class_name) {
		throw undefined_member("Generate agent is not defined inside Env");
	}; //!< A template class to generate agent.
	virtual void update_repo(){} //!< To remove the dead agents from the repo. This needs to be implemented.
    void setup_domain(vector<MESH_ITEM> mesh); //!< Sets up the domain by creating patch objects in accordance to mesh objects.
    void setup_agents(map<string,unsigned> config); //!< Creates agents and randomly distributes them in the simulation domain.
    void step_agents(); //!< Calls step function of agents
    void step_patches(); //!< Calls step function of patches
	virtual void update(); //!< Update the world. All built-in utilities such as `Agent::order_move` are executed here.
    void place_agent(shared_ptr<PATCH> patch,shared_ptr<AGENT> agent); //!< Places the given agent in the given patch. Raises an exception if the patch is not available.
    void place_agent(unsigned patch_index,shared_ptr<AGENT> agent); //!< Places the given agent in the given patch index. Raises an exception if the patch is not available.

    void place_agent_randomly(shared_ptr<AGENT> agent); //!< Places the given agent randomly in the domain. Raises exception if no patch is available.
    shared_ptr<PATCH> find_empty_patch(); //!< Finds empty patches in the entire domain
    void remove_agent(shared_ptr<AGENT> agent){
        this->agents.erase(std::remove(this->agents.begin(), this->agents.end(), agent), this->agents.end());
          // swap the one to be removed with the last element

        auto it = std::find(this->agents.begin(), this->agents.end(), agent);

        if (it != this->agents.end()) {
          using std::swap;
          // swap the one to be removed with the last element
          // and remove the item at the end of the container
          // to prevent moving all items after '5' by one
          swap(*it, this->agents.back());
          this->agents.pop_back();
        }
    }
    void process_switch(); //!< Process swtich requests
    void process_hatch(); //!< Process hatch requests
    void process_move(); //!< Process move requests
    void process_disappear(); //!< Process disappear requests 

    virtual void step() {
    	throw undefined_member("Step function is not defined inside Env");
    }; //!< Steps the simulation
    void activate_serial(){
        for (auto&agent:this->agents) agent->step();
    }; //!< Serial activation of the agents
    void activate_random(){
        auto g = tools::randomly_seeded_MT();
        std::shuffle(this->agents.begin(),this->agents.end(),g);
        for (auto&agent:this->agents) agent->step();
    }; //!< Random activation of the agents

    map<string,unsigned> count_agents(); //!< Counts the agents according to `Agent::class_name`.

    std::map<std::string,unsigned> agents_count; //!< Keeps the record the agents according to `Agent::class_name`.
    std::set<string> agent_classes; //!< stores a list of `Agent::class_name`.
    vector<shared_ptr<AGENT>> agents; //!< Agent container
    map<unsigned,shared_ptr<PATCH>> patches; //!< Patch container
    double memory_usage(){
#ifdef MEMORY_MONITOR
    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

    
    if (KERN_SUCCESS != task_info(mach_task_self(),
                          TASK_BASIC_INFO, (task_info_t)&t_info, 
                          &t_info_count))
    {
        
    }
    return t_info.resident_size;
#else
    return 0;
#endif 
    }
};
template<class ENV, class AGENT, class PATCH>
inline void Agent<ENV,AGENT,PATCH>::move(shared_ptr<PATCH> dest, bool quiet){
        if (!dest->empty()) {
            if (!quiet) throw patch_availibility("Given patch for move is not empty");
            else return;
        }
        try{
            this->patch->remove_agent(); // remove it from the current patch
            this->env->place_agent(dest,this->shared_from_this());

        }
        catch(patch_availibility & ee){
            return;
        }
    }
template<class ENV, class AGENT, class PATCH>
inline void Agent<ENV,AGENT,PATCH>::order_hatch(shared_ptr<PATCH> patch, bool inherit, bool quiet, bool reset)
    {
    this->_hatch =  HATCH_CONFIG<ENV,AGENT,PATCH>(true,patch,inherit,quiet,reset);

}
template<class ENV, class AGENT, class PATCH>
inline void Agent<ENV,AGENT,PATCH>::order_move(shared_ptr<PATCH> patch, 
             bool quiet, bool reset ){
    this->_move =  MOVE_CONFIG<ENV,AGENT,PATCH>(true,patch,quiet,reset);

}

template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::step_agents(){
    for (unsigned i = 0; i < this->agents.size(); i++){
        this->agents[i]->step();
    }
}
template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::step_patches(){

    for (auto &[index,patch]:this->patches){
        patch->step();
    }
}
template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::setup_agents(map<string,unsigned> config){
    for (auto const [agent_type,count]:config){
        for (unsigned i = 0; i < count; i++){
            auto patch = this->find_empty_patch();
            auto agent = this->generate_agent(agent_type);
    		this->place_agent(patch,agent);
        }
        this->agent_classes.insert(agent_type);
    }
}
template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::update(){
    auto g = tools::randomly_seeded_MT();
    std::shuffle(this->agents.begin(),this->agents.end(),g);
    /// move 
    this->process_move();
    /// hatch 
    this->process_hatch();
    /// switch 
    this->process_switch();
    /// process disappearing
    this->process_disappear(); 
    this->update_repo(); // to remove the agents from repo
    // update agent counts
    this->count_agents();
};
template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::process_move(){
    unsigned agent_count = this->agents.size();
    for (unsigned  i = 0; i < agent_count; i++){
        if (!this->agents[i]->_move._flag) continue;
        auto dest = this->agents[i]->_move._patch;
        if (dest == nullptr){ // find a random patch
                try{
                    dest = this->agents[i]->patch->empty_neighbor();
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
            if (!dest->empty()){ 
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

            this->agents[i]->move(dest,this->agents[i]->_move._quiet);
            this->agents[i]->reset_move();
    }
}
template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::process_hatch(){
    auto agent_count = this->agents.size();
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
                        break;
                    }
                }
                
            }
            // check if another agent hasn't already taken it 
            if (!patch->empty()){ 
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
            
            auto new_agent = this->generate_agent(this->agents[i]->class_name);
            if (this->agents[i]->_hatch._inherit){
                new_agent->inherit(this->agents[i]);
            }
            this->place_agent(patch,new_agent);
            this->agents[i]->reset_hatch();
        
            
        };
    }
}
template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::process_disappear(){
    unsigned size = this->agents.size();
    auto counter = 0;
    unsigned i = 0;
    while(i<(size-counter)){
        if(this->agents[i]->disappear == true){
            swap(this->agents[i], this->agents[size-counter-1]); // we swap the agent location in the vector with the last agent
            counter++;
        }else{
            i++;
        }
    }
    if (counter>0){//if anything there to erase
        this->agents.erase(this->agents.end()-counter,this->agents.end());
    }
}
template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::process_switch(){
    auto agent_count = this->agents.size();
    for (unsigned  i = 0; i < agent_count; i++){
        auto agent = this->agents[i];
        if (!agent->_switch._flag) continue;
        auto to = agent->_switch._to;
        // auto dest = agent->patch;
        // dest->remove_agent(); // get the patch empty
        // auto new_agent = this->generate_agent(to);
        // this->place_agent(dest,new_agent);
        // agent->disappear = true;
        agent->class_name = to;
        agent->reset_switch();
    }
}
template<class ENV, class AGENT, class PATCH>
inline map<string,unsigned> Env<ENV,AGENT,PATCH>::count_agents(){
    map<string,unsigned> agents_count;
    for (unsigned i = 0; i < this->agents.size(); i++) {
        auto agent =  this->agents[i];
        agents_count[agent->class_name]++;
        
        agent_classes.insert(agent->class_name);
        
    }
    // to add those agents that were present at setup time but disappeared
    for (auto const & agent_class:this->agent_classes){
        if (agents_count.find(agent_class) == agents_count.end()){
            agents_count[agent_class] = 0;
        }
    }
    this->agents_count = agents_count;
    return agents_count;
}
template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::place_agent(shared_ptr<PATCH> patch,shared_ptr<AGENT> agent){
    if (!patch->empty()) {
        if (patch->get_agent() == agent) return;
        else throw patch_availibility("Patch is not empty");
    }
    else{
       if (agent->has_patch){
            agent->patch->remove_agent();
        }
        patch->set_agent(agent); 
    }
    
}
template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::place_agent(unsigned patch_index,shared_ptr<AGENT> agent){
    for (auto &[index,patch]:this->patches){
        if (index == patch_index){
            if (!patch->empty()) {
                if (patch->get_agent() == agent) return;
                else throw patch_availibility("Patch is not empty");
            }
            if (agent->has_patch){
                agent->patch->remove_agent();
            }
            patch->set_agent(agent);
            return;
        }
    }
    throw patch_availibility("The given index for patch is not defined.");
    
    
}
template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::place_agent_randomly(shared_ptr<AGENT> agent){
    auto patch = this->find_empty_patch();
    place_agent(patch,agent);
}
template<class ENV, class AGENT, class PATCH>
inline shared_ptr<PATCH> Env<ENV,AGENT,PATCH>::find_empty_patch(){
    std::vector<int> patch_keys;
    for(auto const& patch: this->patches)
        patch_keys.push_back(patch.first);
    
    auto g = tools::randomly_seeded_MT();

    std::shuffle(patch_keys.begin(), patch_keys.end(), g);
    
    for (auto const&i:patch_keys){
        auto potential_patch = this->patches.at(i);
        if (potential_patch->empty()){
            return potential_patch;
        }
    }
    throw patch_availibility("All patches are occupied.");
}
template<class ENV, class AGENT, class PATCH>
 inline void Env<ENV,AGENT,PATCH>::setup_domain(vector<MESH_ITEM> mesh){
        // step 1: create patches from info of meshes
        for (auto & mesh_item:mesh){
            this->generate_patch(mesh_item); // create patch

        }
        // step 2: assign neighbor patches
        for (auto &[index,patch]:patches){
            vector<shared_ptr<PATCH>> neighbors;
            auto neighbors_indices = patch->neighbors_indices;
            for (auto const &neighbor_index:neighbors_indices){
                auto neighbor_patch = patches.at(neighbor_index);
                neighbors.push_back(neighbor_patch);
            }
            patch->neighbors = neighbors;
        }
        // log_mesh(mesh,"mesh.txt");
    }
template<class ENV, class AGENT, class PATCH>
shared_ptr<PATCH> Patch<ENV,AGENT,PATCH>::empty_neighbor(bool quiet){
        auto neighbors = this->neighbors;
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(neighbors.begin(),neighbors.end(),g);
        for (auto &neighbor:neighbors){
            if (neighbor->empty()){
                return neighbor;
            }
        }
        if (!quiet) throw patch_availibility("No available patch around the agent");
        return nullptr;
}
template<class ENV, class AGENT, class PATCH>
vector<shared_ptr<AGENT>> Patch<ENV,AGENT,PATCH>::find_neighbor_agents(bool include_self){
        vector<shared_ptr<AGENT>> neighbor_agents;
        if (!this->empty() & include_self) neighbor_agents.push_back(this->get_agent());
        for (auto const &patch:this->neighbors){
            if (!patch->empty()) neighbor_agents.push_back(patch->get_agent());
        }
        return neighbor_agents;
    }

