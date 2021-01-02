#include "mixed_example.h"
using namespace std;

shared_ptr<Tissue> Domain::generate_patch(){
		auto patch_obj = make_shared<Tissue>(this->shared_from_this());
		return patch_obj;
	}
void Domain::setup(){
	auto mesh = grid2(1.5, 1.5, 0.015, true);
	this->setup_domain(mesh);
	std::map<std::string,unsigned> settings = {{"cell",2000}};
	this->setup_agents(settings);
	this->damage();
	this->update();
	}
void Domain::damage(){
		for (auto &[index,patch]:this->patches){
			auto x = patch->coords[0];
			auto y = patch->coords[1];
			if ((x >= 0.25 and x <=1.25) and (y>=0.25 and y<=1.25)){
				patch->damage_center = true;
				patch->ECM = 0;
				if (patch->empty == false){
					patch->agent->disappear = true;
				}
					
			}
		}
	}
void Domain::update(){
		Env<Domain,Cell,Tissue>::update();
		for (auto &agent: this->agents){
			agent->update();
		}	
	}
void Domain::episode(){
	for (unsigned i = 0; i < 336; i++){
			cout<<"iteration "<<i<<endl;
			this->step();
			// if (i%20 == 0) this->output();
		}	
}
void Domain::output(){
	ofstream file1;
	file1.open("cells.csv");
	file1 << "x,y,type,size\n";
	for (auto &agent:this->agents){
		file1<<agent->patch->coords[0]<<","<<agent->patch->coords[1]<<","<< agent->class_name<<", "<<10<<std::endl;
	}
	file1.close();
	//plot ECM density on the domain
	ofstream file2;
	file2.open("ECM.csv");
	file2<<"x,y,type,size\n";
	for (auto &[index,patch]:this->patches){
		file2<<patch->coords[0]<<","<<patch->coords[1]<<","<< patch->ECM<<", "<<10<<std::endl;
	}
	file2.close();
}
void Domain::step(){
		for (auto &cell:this->agents){
			cell->step();
		}
		this->update();
		this->clock ++;
	}	