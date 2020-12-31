#include <iostream>
#include <fstream>
#include "CPPYABM/include/ABM/bases.h"
#include "CPPYABM/include/ABM/mesh.h"
struct Domain;
struct Tissue;
struct Cell;

struct Domain: public Env<Domain,Cell,Tissue> {
	Domain():Env<Domain,Cell,Tissue>(){}
	virtual shared_ptr<Cell> generate_agent(std::string agent_name);
	virtual shared_ptr<Tissue> generate_patch();
	virtual void update();
	void damage();
	void setup();
	void step();
	void episode();
	void output();
	unsigned clock=0;
};
struct Tissue: public Patch<Domain,Cell,Tissue> {
	Tissue(shared_ptr<Domain> env):Patch<Domain,Cell,Tissue>(env){}
	bool damage_center = false;
	double ECM = 100;
};
struct Cell: public Agent<Domain,Cell,Tissue> {
	Cell(shared_ptr<Domain> env,std::string agent_name):Agent<Domain,Cell,Tissue>(env,agent_name){
	}
	void update(){
		this->clock++;
	}
	void step();
	unsigned clock = 12;
};
