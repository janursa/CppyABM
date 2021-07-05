#include <iostream>
#include <fstream>
#include <cppyabm/bases.h>
#include "cppyabm/mesh.h"




struct CovidModel;
struct CovidAgent;
//! Domain class to coordinate the simulation
struct Domain: public Env<Domain,Cell,Tissue> {
	using baseEnv = Env<Domain,Cell,Tissue>;
	using baseEnv::baseEnv;
	Domain(bool _output_flag):Env<Domain,Cell,Tissue>(){
		output_flag = _output_flag;
	}
	bool output_flag = false;
	virtual shared_ptr<Cell> generate_agent(std::string agent_name);
	virtual shared_ptr<Tissue> generate_patch(MESH_ITEM);
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
struct Tissue: public Patch<Domain,Cell,Tissue> {
	using basePatch = Patch<Domain,Cell,Tissue>;
	using basePatch::basePatch;
	Tissue(shared_ptr<Domain> env,MESH_ITEM mesh):basePatch(env,mesh){
		this->setup();
	}
	void setup(){
	}
	bool damage_center = false;
	double ECM = 100;

};
struct Cell: public Agent<Domain,Cell,Tissue> {
	using baseAgent = Agent<Domain,Cell,Tissue>;
	using baseAgent::baseAgent;
	Cell(shared_ptr<Domain> env,std::string agent_name):baseAgent(env,agent_name){
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
inline shared_ptr<Cell> Domain::generate_agent(std::string agent_name){
		auto agent_obj = make_shared<Cell>(this->shared_from_this(),agent_name);
		this->agents.push_back(agent_obj);
		return agent_obj;
	}
inline shared_ptr<Tissue> Domain::generate_patch(MESH_ITEM mesh){
		auto patch_obj = make_shared<Tissue>(this->shared_from_this(),mesh);
		this->patches.insert(pair<unsigned,shared_ptr<Tissue>>(patch_obj->index,patch_obj));
		return patch_obj;
	}
inline void Domain::setup(){
	auto mesh = space::grid2(1.5, 1.5, 0.015, true);
	this->setup_domain(mesh);
	std::map<std::string,unsigned> settings = {{"cell",2000}};
	this->setup_agents(settings);
	this->damage();
	this->update();
	}
inline void Domain::damage(){
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
inline void Domain::update(){
		Env<Domain,Cell,Tissue>::update();
		for (auto &agent: this->agents){
			agent->update();
		}
		int cell_count = this->agents.size();
		this->data["cell_count"].push_back(cell_count);		

	}
inline void Domain::step(){
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
inline void Cell::step(){
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
			shared_ptr<Tissue> dest;
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
inline void Domain::episode(){
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
inline void Domain::output(){
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
