#pragma once
#include <map>
#include <algorithm>
#include <random>
#include "common.h"
#include "mesh.h"
#include "tools.h"
#include "env.h"
using std::shared_ptr;
using std::vector;
using namespace std;

struct Patch{
	Patch(shared_ptr<Env> env){
		this->env = env;
	}
	virtual ~Patch(){}
	unsigned index;
	unsigned layer_index;
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
		if (!this->empty & include_self) neighbor_agents.push_back(this->agent);
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
	bool on_border = false;
	string class_name;
    virtual void set_data(string tag, double value) {throw undefined_member("Set data is used before implementation");};
    virtual double get_data(string tag) {throw undefined_member("Get data is used before implementation");};


};