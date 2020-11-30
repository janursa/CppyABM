#pragma once
#include <map>
#include <algorithm>
#include <random>
#include "common.h"
#include "mesh.h"
#include "tools.h"

using std::shared_ptr;
using std::vector;
using namespace std;


//!   Base clase for Agents
/*!
  
*/
struct Agent: enable_shared_from_this<Agent>{
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

	bool disappear = false;
	string class_name;
    virtual void set_data(string tag, double value) {throw undefined_member("Set data is used before implementation");};
    virtual double get_data(string tag) {throw undefined_member("Get data is used before implementation");};


};
