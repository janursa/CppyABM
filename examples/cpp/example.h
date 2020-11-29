#include <iostream>
#include "CPPYABM/include/ABM/bases.h"
#include "CPPYABM/include/ABM/mesh.h"
struct healingEnv;
struct myPatch;
struct Cell;

struct healingEnv: public Env {
	healingEnv():Env(){}
	virtual shared_ptr<Agent> generate_agent(std::string agent_name);
	virtual shared_ptr<Patch> generate_patch();
	virtual void update();
	void damage();
	void setup();
	void run();
	vector<shared_ptr<Cell>> agents;
	// vector<shared_ptr<myPatch>> patches;
	map<unsigned,shared_ptr<myPatch>> patches;
	unsigned clock=0;
};
struct myPatch: public Patch {
	myPatch(shared_ptr<Env> env):Patch(env){}
	bool damage_center = false;
	double tissue = 100;
	shared_ptr<Cell> agent;
};
struct Cell: public Agent {
	Cell(shared_ptr<Env> env,std::string agent_name):Agent(env,agent_name){
	}
	void update(){
		this->clock++;
	}
	unsigned clock = 12;
	shared_ptr<myPatch> patch;
};
