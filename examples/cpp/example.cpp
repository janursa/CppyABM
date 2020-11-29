#include "example.h"
using namespace std;
shared_ptr<Agent> healingEnv::generate_agent(std::string agent_name){
		auto agent_obj = make_shared<Cell>(this->shared_from_this(),agent_name);
		this->agents.push_back(agent_obj);
		return agent_obj;
	}
shared_ptr<Patch> healingEnv::generate_patch(){
		auto patch_obj = make_shared<Patch>(this->shared_from_this());
		return patch_obj;
	}
void healingEnv::setup(){
	auto cell = this->generate_agent("cell");
	cell->clock;
		// auto mesh = grid2(1.5, 1.5, 0.015, true);
		// this->setup_domain(mesh);
		// std::map<std::string,unsigned> settings = {{"cell",2000}};
		// this->setup_agents(settings);
		// this->damage();
		// this->update();
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
		Env::update();
		for (auto &agent: this->agents){
			agent->update();
		}
			
	}
void healingEnv::run(){
		for (auto &cell:this->agents){
			// auto dest_patch = cell->patch->empty_neighbor(true);
			// cell->order_move(nullptr,true,false);
			// auto neighbor_cell_count = cell->patch->find_neighbor_agents().size();
			// if (cell->patch->damage_center and cell->clock >= 12){
			// 	if (neighbor_cell_count <= 6){
			// 		cell->order_hatch(nullptr,true,false,false);
			// 		cell->clock = 0 ;
			// 	}
			// }
			// if (cell->patch->tissue < 100) {
			// 	cell->patch->tissue += 1;	
			// }
				
			// if (neighbor_cell_count >7){
			// 	cell->disappear = true;
			// }

		}
		// this->update();
		// this->clock ++;
	}	