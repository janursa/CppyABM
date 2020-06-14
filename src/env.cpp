#include <iostream>
#include "bases.h"

  double Env::collect_from_patches(string tag){
    double result = 0;
    for (auto const &[index,patch]: this->patches){
        result += patch->data[tag];
    }
    return result;
}
  void Env::step_agents(){

        for (unsigned i = 0; i < this->agents.size(); i++){
            this->agents[i]->step();
        }
}
  void Env::step_patches(){

        for (unsigned i = 0; i < this->agents.size(); i++){
            this->patches[i]->step();
        }
}
  void Env::setup_agents(map<string,unsigned> config){
    for (auto const [agent_type,count]:config){
        for (unsigned i = 0; i < count; i++){
            auto agent = this->generate_agent(agent_type);
            this->place_agent_randomly(agent);
        }
        this->agent_classes.insert(agent_type);
    }
}
  void Env::update(){
    
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
            auto new_agent = this->generate_agent(this->agents[i]->class_name);
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
    /** switch **/
    for (unsigned  i = 0; i < agent_count; i++){
        auto agent = this->agents[i];
        if (!agent->_switch._flag) continue;
        auto to = agent->_switch._to;
        auto new_agent = this->generate_agent(to);
        agent->patch->remove_agent(); // get the patch empty
        this->connect_patch_agent(agent->patch,new_agent);
        agent->disappear = true;
        agent->reset_switch();

    }
    /** process disappearing **/
    int jj = 0;
    while (true) {
        if (jj >= this->agents.size()) break;
        for (int ii = jj; ii < this->agents.size(); ii++) {
            if (this->agents[ii]->disappear == true) {
                this->agents.erase(this->agents.begin() + ii);
                break;
            }
            jj++;
        };
    }
    this->update_repo(); // to remove the agents from repo
    /** update Env data **/
    // update agent counts

    this->count_agents();
    
};
  map<string,unsigned> Env::count_agents(){
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
  void Env::place_agent(shared_ptr<Patch> patch,shared_ptr<Agent> agent){
    if (!patch->empty) throw patch_availibility("Patch is not empty");
    connect_patch_agent(patch,agent);
}
  void Env::connect_patch_agent(shared_ptr<Patch> patch,shared_ptr<Agent> agent){
    patch->set_agent(agent);
    agent->set_patch(patch);
}
  void Env::place_agent_randomly(shared_ptr<Agent> agent){
    auto patch = this->find_empty_patch();
    connect_patch_agent(patch,agent);
}
  shared_ptr<Patch> Env::find_empty_patch(){
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
  void Env::setup_domain(vector<MESH_ITEM> mesh){
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
// binding class
template<typename class_name,typename py_class_name>
void link_env(py::module m, string class_name_string){
    py::class_<class_name,py_class_name,std::shared_ptr<class_name>>(m,class_name_string.c_str(),py::dynamic_attr())
        .def(py::init<>())
        .def("check",&class_name::check)
        .def("place_agent_randomly",&class_name::place_agent_randomly)
        .def("setup_domain",&class_name::setup_domain)
        .def("step_agents",&class_name::step_agents)
        .def("step_patches",&class_name::step_patches)
        .def("place_agent",&class_name::place_agent)
        .def("update",&class_name::update)
        .def("setup_agents",&class_name::setup_agents)
        .def("count_agents",&class_name::count_agents)
        .def("collect_from_patches",&class_name::collect_from_patches)
        .def_readwrite("patches",&class_name::patches)
        .def_readwrite("agents",&class_name::agents);
}