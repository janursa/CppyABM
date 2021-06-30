#include <iostream>
#include <fstream>
#include <cppyabm/bases.h>
#include "cppyabm/mesh.h"
#include <algorithm>




struct WolfSheep;
struct GrassPatch;
struct Animal;
struct PARAMS{
	static unsigned steps;
	static float height;
	static float width;
	static unsigned initial_sheep;
	static unsigned initial_wolves;
	// sheep_reproduce=0.04,
	static float sheep_reproduce;
	static float wolf_reproduce;
	static unsigned wolf_gain_from_food;
	static unsigned grass_regrowth_time;
	static unsigned sheep_gain_from_food;
	//unsigned  sheep_gain_from_food=10 
};
unsigned PARAMS::steps = 1000;
 float PARAMS::height=100;
 float PARAMS::width=100;
 unsigned PARAMS::initial_sheep=2000;
 unsigned PARAMS::initial_wolves=500;
// sheep_reproduce=0.04,
 float PARAMS::sheep_reproduce=0.08;
 float PARAMS::wolf_reproduce=0.05;
 unsigned PARAMS::wolf_gain_from_food=40;
 unsigned PARAMS::grass_regrowth_time=30;
 unsigned PARAMS::sheep_gain_from_food=4;

//! WolfSheep class to coordinate the simulation
struct WolfSheep: public Env<WolfSheep,Animal,GrassPatch>{
	using baseEnv = Env<WolfSheep,Animal,GrassPatch>;
	// using baseEnv::baseEnv;
	WolfSheep(bool _output_flag):Env<WolfSheep,Animal,GrassPatch>(){
		output_flag = _output_flag;
		cout<<output_flag<<endl;
		auto mesh = space::grid2(PARAMS::height, PARAMS::width,1, true);
		this->setup_domain(mesh);
	}
	bool output_flag = false;
	virtual shared_ptr<Animal> generate_agent(std::string agent_name);
	virtual shared_ptr<GrassPatch> generate_patch(MESH_ITEM);
	virtual void update();
	void damage();
	void setup();
	virtual void step();
	void episode();
	void output();
	std::map<std::string,std::vector<int>> data= {{"cell_count",{}}};
	int tick=0;
#ifdef MEMORY_MONITOR
	struct task_basic_info t_info;
	mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
#endif //MEMORY_MONITOR
	double memory_usage_max;
};
struct GrassPatch: public Patch<WolfSheep,Animal,GrassPatch> {
	using basePatch = Patch<WolfSheep,Animal,GrassPatch>;
	using basePatch::basePatch;
	GrassPatch(shared_ptr<WolfSheep> env,MESH_ITEM mesh):basePatch(env,mesh){
		this->setup();
	}
	void setup(){
	}
	bool damage_center = false;
	double ECM = 100;

};
struct Animal: public Agent<WolfSheep,Animal,GrassPatch> {
	using baseAgent = Agent<WolfSheep,Animal,GrassPatch>;
	using baseAgent::baseAgent;
	Animal(shared_ptr<WolfSheep> env,std::string agent_name):baseAgent(env,agent_name){
		this->setup();
	}
	void update(){
		this->clock++;
	}
	virtual void step();
	virtual void setup(){
		this->cycle_t = 12;
	}
	int cycle_t;
	int clock = 0;
};
inline shared_ptr<Animal> WolfSheep::generate_agent(std::string agent_name){
		auto agent_obj = make_shared<Animal>(this->shared_from_this(),agent_name);
		this->agents.push_back(agent_obj);
		return agent_obj;
	}
inline shared_ptr<GrassPatch> WolfSheep::generate_patch(MESH_ITEM mesh){
		auto patch_obj = make_shared<GrassPatch>(this->shared_from_this(),mesh);
		this->patches.insert(pair<unsigned,shared_ptr<GrassPatch>>(patch_obj->index,patch_obj));
		return patch_obj;
	}
inline void WolfSheep::setup(){
	auto mesh = space::grid2(1.5, 1.5, 0.015, true);
	this->setup_domain(mesh);
	std::map<std::string,unsigned> settings = {{"cell",2000}};
	this->setup_agents(settings);
	this->damage();
	this->update();
	}
inline void WolfSheep::damage(){
	for (auto &[index,patch]:this->patches){
		auto x = patch->coords[0];
		auto y = patch->coords[1];
		if ((x >= 0.25 and x <=1.25) and (y>=0.25 and y<=1.25)){
			patch->damage_center = true;
			patch->ECM = 0;
			if (patch->empty() == false){
				auto agents = patch->get_agents();
				agents[0]->disappear = true;
			}
				
		}
	}
}
inline void WolfSheep::update(){
		Env<WolfSheep,Animal,GrassPatch>::update();
		for (auto &agent: this->agents){
			agent->update();
		}
		int cell_count = this->agents.size();
		this->data["cell_count"].push_back(cell_count);		

	}
inline void WolfSheep::step(){
	auto usage = this->memory_usage();
	if (usage > memory_usage_max){
		memory_usage_max = usage;
	}
	unsigned ii = 0;
	for (auto&[index,patch]:this->patches){
		if (patch->empty()) ii++;
	}
	cout<<"number of available patches "<<ii<<endl;

	for (unsigned i = 0; i<this->agents.size(); i++){

		this->agents[i]->step();
	}
	this->update();
	this->tick ++;
}	
inline void Animal::step(){
	this->order_move(nullptr,true,false);
	// { // for moving
	// 	try {
	// 		auto dest = this->get_patch()->empty_neighbor();
	// 		this->move(dest,true);
			  
	// 	}catch(patch_availibility &ee){
			
	// 	}
		
	// }
	
	auto neighbor_cell_count = this->get_patch()->find_neighbor_agents().size();
	if (this->get_patch()->damage_center and this->clock >= this->cycle_t){
		if (neighbor_cell_count <= 6){
			shared_ptr<GrassPatch> dest;
			try{
				dest = this->get_patch()->empty_neighbor();
				auto new_agent = this->env->generate_agent("cell");
				this->env->place_agent(dest,new_agent);
				// this->order_hatch(nullptr,false,true,false);
				this->clock = 0 ;
			}
			catch(patch_availibility & ee){
				
			}
			
		}
	}
	if (this->get_patch()->ECM < 100) {
		this->get_patch()->ECM += 1;	
	}
		
	if (neighbor_cell_count >7){
		this->disappear = true;
	}
}
inline void WolfSheep::episode(){
	for (unsigned i = 0; i < 336; i++){
			cout<<"iteration "<<i<<" agents "<<this->agents.size()<<endl;
			this->step();
			if (this->output_flag) this->output();
		}	
	ofstream file1;
	file1.open("memory_usage.csv");
	file1<<memory_usage_max<<"\n";
	file1.close();
	cout<<"Memory usage "<<memory_usage_max<<endl;
}
inline void WolfSheep::output(){
	// plot agents on the domain
	ofstream file1;
	file1.open("cells.csv");
	file1 << "x,y,type,size\n";
	for (auto &agent:this->agents){
		file1<<agent->get_patch()->coords[0]<<","<<agent->get_patch()->coords[1]<<","<< agent->class_name<<", "<<10<<std::endl;
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
	// cell counts
	ofstream file3;
	file3.open("cell_count.csv");
	file3<<"cell_count\n";
	for (unsigned i=0;i<this->data["cell_count"].size(); i++){
		file3<<i<<","<<this->data["cell_count"][i]<<std::endl;
	}
	file3.close();

}
