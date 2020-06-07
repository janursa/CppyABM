
#pragma once
#include <map>
#include "toolbox.h"
#include <pybind11/stl.h>
#include "pybind11/pybind11.h"
namespace py=pybind11;
using std::shared_ptr;
using std::vector;
using namespace tools;
struct Env;
struct Agent;
struct Patch;
using namespace std;
template<typename T>
struct Base: public enable_shared_from_this<T>{
	std::shared_ptr<Base<T>> get_ptr();
	std::string name; 
	std::shared_ptr<Base<Agent>> agent_ptr;
	std::shared_ptr<Base<Patch>> patch_ptr;
	std::shared_ptr<Base<Env>> env_ptr;
	/** storage **/
    map<string,float> data;

    /** General **/

protected:

};
template<typename T>
inline std::shared_ptr<Base<T>> Base<T>::get_ptr(){
    return this->shared_from_this();

}

struct Agent: public Base<Agent>{
	virtual ~Agent(){};
	/** Major functions **/ 
	virtual void step()=0;
	virtual void update(){};
    /** Flags **/
    std::pair <bool,std::string> switch_info = std::make_pair(false,"");
    bool hatch_flag = false;
    bool walk_flag = false;
    bool disappear_flag = false;
    /** pure virtuals **/

};

struct Patch: public Base<Patch>{
	unsigned index;
	COORDS coords;
	vector<unsigned> neighbors_indices;
	vector<shared_ptr<Patch>> neighbors;
};
struct Factory
{	
	virtual ~Factory(){};
	virtual shared_ptr<Agent> create_agent()=0;
	virtual void update()=0;
};
struct Env: public Base<Env>{

	virtual ~Env(){};
    vector<shared_ptr<Agent>> agents;
    map<unsigned,shared_ptr<Patch>> patches;
    shared_ptr<Factory> factory_ptr;
    //** Virtuals **/
    virtual void setup()=0;

    void check(){
    	
    }
};

