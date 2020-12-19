#include "example.h"
using namespace std;

shared_ptr<myPatch> healingEnv::generate_patch(){
		auto patch_obj = make_shared<myPatch>(this->shared_from_this());
		return patch_obj;
	}
void healingEnv::setup(){
	auto mesh = grid2(1.5, 1.5, 0.015, true);
	this->setup_domain(mesh);
	std::map<std::string,unsigned> settings = {{"cell",2000}};
	this->setup_agents(settings);
	this->damage();
	this->update();
	}
void healingEnv::damage(){
		for (auto &[index,patch]:this->patches){
			auto x = patch->coords[0];
			auto y = patch->coords[1];
			if ((x >= 0.25 and x <=1.25) and (y>=0.25 and y<=1.25)){
				patch->damage_center = true;
				patch->tissue = 0;
				if (patch->empty == false){
					patch->agent->disappear = true;
				}
					
			}
		}
	}
void healingEnv::update(){
		baseEnv<healingEnv,Cell,myPatch>::update();
		for (auto &agent: this->agents){
			agent->update();
		}
			
	}
void healingEnv::step(){
		cout<<"Step "<<this->clock<<endl;
		for (auto &cell:this->agents){
			cell->step();
		}
		this->update();
		this->clock ++;
	}	