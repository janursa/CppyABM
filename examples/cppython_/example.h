#include <iostream>
#include "CPPYABM/include/ABM/bases.h"
#include "CPPYABM/include/ABM/mesh.h"
struct healingEnv;
struct myPatch;
struct Cell;
struct healingEnv {
	virtual shared_ptr<Cell> generate_agent(string name){cout<<"in base"<<endl;};
	void setup(){
		this->generate_agent("some name");
	};
};
struct Cell {
	// Cell(shared_ptr<healingEnv> env,string name):baseAgent<healingEnv,Cell,myPatch>(env,name){}
};
struct myPatch {
};