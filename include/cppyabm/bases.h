
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

//!   Single patch class
/*!
  Specilization of the base patch for single agent occupancy
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
    }; //!< To define patch behavior #TODO: see if this can go
    
    vector<shared_ptr<AGENT>> find_neighbor_agents(bool include_self = true); //!< Returns agents in one patch neighbors
    shared_ptr<PATCH> empty_neighbor(bool quiet = false); //!< Returns an arbitrary adjacent patch without an agent 

   
    unsigned agent_count = 0; //!< Keeps the record of residing agents count
    std::shared_ptr<ENV> env; //!< Pointer to the simulation environment
    // bool empty = true; //!< whether the patch host an agent
    bool on_border = false;//!< whether the patch residing on the border of the domain
    unsigned index; //!< unique index that associate patch to mesh
    unsigned layer_index; //!< the index of the layer in which the patch is residing
    vector<double> coords; //!< the coordinates of the patch
    vector<unsigned> neighbors_indices; //!< the indices of the neighbor patches
    vector<shared_ptr<PATCH>> neighbors; //!< list of neighbor patches

	
	void remove_agents(){
        auto valid_agents = this->get_agents();
        for (unsigned i = 0; i < valid_agents.size(); i++){
            valid_agents[i]->remove_patch();
        }
        this->agents.erase(this->agents.begin(),this->agents.end());
		this->agent_count = 0;

	} //!< Removes all agents from the patch
    void remove_agent(shared_ptr<AGENT> agent){
        agent->remove_patch();
        // agent.reset();
        unsigned index;
        bool flag = false;
        for (unsigned i = 0; i < this->agents.size(); i++){
            shared_ptr<AGENT> p = this->agents[i].lock();
            if (p){
                if (p == agent){
                    flag = true;
                    index = i;
        //             break;      
                }
            }else{
        //         // cerr<<"poiner to agent is invalid inside patch"<<endl;
        //         // throw invalid_pointer("poiner to agent is invalid inside patch");
            }
        }
        if (flag){

            this->agents.erase(this->agents.begin()+index);
            this->agent_count --; 
        }
        

        
    } //!< Removes the given agent from the patch
    virtual void add_agent(shared_ptr<AGENT> agent){
        this->agents.push_back(agent);
        this->agent_count ++;
    }; //!< Assigns agent to the patch

    vector<shared_ptr<AGENT>> get_agents(){
        vector<shared_ptr<AGENT>> _agents;
        vector<unsigned> invalid_agents_indices;
        for (unsigned i = 0; i < this->agents.size(); i++){
            shared_ptr<AGENT> p = this->agents[i].lock();
            if (p) _agents.push_back(p);
            else invalid_agents_indices.push_back(i);      
        }
        // here erase those invalid ones
        if (invalid_agents_indices.size()>0){
            unsigned size = this->agents.size();
            auto counter = 0;
            for (auto &i:invalid_agents_indices ){
                swap(this->agents[i], this->agents[size-counter-1]); // we swap the agent location in the vector with the last agent
                counter++;
            }
            this->agents.erase(this->agents.end()-counter,this->agents.end());
        }
        
        return _agents;
        
    }
    bool empty(){
        for (auto&agent:this->agents){
            shared_ptr<AGENT> p = agent.lock();
            if (p){
                return false;
            }
        }
        return true;
    }
    
	vector<std::weak_ptr<AGENT>> agents; //!< Pointer to stores agents
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
	std::shared_ptr<PATCH> _patch; //!< Pointer to the residing patch
	std::shared_ptr<ENV> env; //!< Pointer to the simulation world
    std::shared_ptr<PATCH> get_patch(){
        if (!this->has_patch){
            cerr<<"The patch of the agent is not allocated but is called"<<endl;
            throw invalid_pointer("The agent doenst have a patch");
        }
        return this->_patch;
    }
    void set_patch(std::shared_ptr<PATCH> patch_ptr){
        this->has_patch= true;
        this->_patch = patch_ptr;
    }
    void remove_patch(){
        this->has_patch = false;
        this->_patch = nullptr;
    }
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
    	throw undefined_member("Generate patch is either not defined inside Env or not exposed in tram class for python development");
    }; //!< A template class to generate patch.
	virtual	shared_ptr<AGENT> generate_agent(string class_name) {
		throw undefined_member("Generate agent is either not defined inside Env or not exposed in tram class for python development");
	}; //!< A template class to generate agent.
	virtual void update_repo(){} //!< To remove the dead agents from the repo. This needs to be implemented.
    void setup_domain(vector<MESH_ITEM> mesh); //!< Sets up the domain by creating patch objects in accordance to mesh objects.
    void setup_agents(map<string,unsigned> config); //!< Creates agents and randomly distributes them in the simulation domain.
    void step_agents(); //!< Calls step function of agents
    void step_patches(); //!< Calls step function of patches
	virtual void update(); //!< Update the world. All built-in utilities such as `Agent::order_move` are executed here.
    void place_agent(shared_ptr<PATCH> patch,shared_ptr<AGENT> agent, bool quiet = false); //!< Places the given agent in the given patch. Raises an exception if the patch is not available.
    void place_agent(unsigned patch_index,shared_ptr<AGENT> agent, bool quiet = false); //!< Places the given agent in the given patch index. Raises an exception if the patch is not available.

    void place_agent_randomly(shared_ptr<AGENT> agent,bool quiet = false); //!< Places the given agent randomly in the domain. Raises exception if no patch is available.
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
        for (unsigned i =0; i < this->agents.size(); i++) this->agents[i]->step();
    }; //!< Serial activation of the agents
    void activate_random(){
        auto g = tools::randomly_seeded_MT();
        std::shuffle(this->agents.begin(),this->agents.end(),g);
        this->activate_serial();
    }; //!< Random activation of the agents

    map<string,unsigned> count_agents(); //!< Counts the agents according to `Agent::class_name`.

    std::map<std::string,unsigned> agents_count; //!< Keeps the record the agents according to `Agent::class_name`.
    std::set<string> agent_classes; //!< stores a list of `Agent::class_name`.
    vector<shared_ptr<AGENT>> agents; //!< Agent container
    map<unsigned,shared_ptr<PATCH>> patches; //!< Patch container
    vector<int> patches_keys; //!< Patch container
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
        this->get_patch()->remove_agent(this->shared_from_this()); // remove it from the current patch
        this->env->place_agent(dest,this->shared_from_this(),quiet); 
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
                    dest = this->agents[i]->get_patch()->empty_neighbor();
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
                    patch = this->agents[i]->get_patch()->empty_neighbor();
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
inline void Env<ENV,AGENT,PATCH>::place_agent(shared_ptr<PATCH> patch,shared_ptr<AGENT> agent, bool quiet ){
    if (!patch->empty() && !quiet){

        cerr<<"placing agent in a patch which has already an agent"<<endl;
        throw patch_availibility("placing agent in a patch which has already an agent");
    }
    if (agent->has_patch){
        string message = "The agent has already a patch so cannot be assigend to a new patch";
        cerr<<message<<endl;
        throw illegal_action(message);

    }
    patch->add_agent(agent); 
    agent->set_patch(patch);

}
template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::place_agent(unsigned patch_index,shared_ptr<AGENT> agent, bool quiet){
    for (auto &[index,patch]:this->patches){
        if (index == patch_index){
            this->place_agent(patch,agent);
            break;
        }
    }
    throw patch_availibility("The given index for patch is not defined.");
    
    
}
template<class ENV, class AGENT, class PATCH>
inline void Env<ENV,AGENT,PATCH>::place_agent_randomly(shared_ptr<AGENT> agent, bool quiet){
    auto patch = this->find_empty_patch();
    place_agent(patch,agent,quiet);
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
            this->patches_keys.push_back(mesh_item.index);
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
        if (!this->empty() & include_self){
            auto agents = this->get_agents();
            neighbor_agents.insert(neighbor_agents.end(),agents.begin(),agents.end());
        } 

        for (auto const &patch:this->neighbors){
            if (!patch->empty()) {
                auto agents =patch->get_agents();
                neighbor_agents.insert(neighbor_agents.end(), agents.begin(),agents.end());
            }
        }
        return neighbor_agents;
    }

