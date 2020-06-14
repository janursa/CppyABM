#include <iostream>
#include "bases.h"

void Agent::move(shared_ptr<Patch> dest, bool quiet){
        if (!dest->empty) {
            if (!quiet) throw patch_availibility("Given patch for move is not empty");
            else return;
        }
        this->patch->remove_agent(); // remove it from the current patch
        this->env->connect_patch_agent(dest,this->get_ptr());
    }
void Agent::order_hatch(shared_ptr<Patch> patch, bool inherit, bool quiet, bool reset)
    {
    this->_hatch =  HATCH_CONFIG(true,patch,inherit,quiet,reset);

}
void Agent::order_move(shared_ptr<Patch> patch = nullptr, 
             bool quiet = false, bool reset = false){
    this->_move =  MOVE_CONFIG(true,patch,quiet,reset);

}

std::shared_ptr<Agent> Agent::get_ptr(){
    return this->shared_from_this();

}
