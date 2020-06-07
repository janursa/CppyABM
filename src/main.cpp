#include "iostream"
#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include "env.h"
#include "agent.h"
#include "patch.h"
#include "mesh.h"
#include "factory.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;

int main(){
	

	tools::create_directory(main_output_folder);
	/** create env **/
	
	// auto factory_ptr = make_shared<myFactory>();
	// auto env_ptr = make_shared<myEnv>(factory_ptr);
	// env_ptr->check();
	// // auto factory_ptr = make_shared<myFactory
	// /** create meshes **/
	// auto meshes = mesh_tools().grid(1,1,.1);
	
	#ifdef TEST
	
	/** create patches **/ //=> needs meshes
	// step 1: create patches from info of meshes
	map<unsigned,shared_ptr<Patch>> patches;
	for (auto & mesh_item:meshes){
		auto patch_ptr = make_shared<myPatch>(); // create patch
		patch_ptr->index = mesh_item.index;      // copy index
		patch_ptr->coords = mesh_item.coords;    // copy coords
		patch_ptr->neighbors_indices = mesh_item.neighbors_indices;  // copy neighbors indices
		patches[patch_ptr->index]= patch_ptr;
	}
	// step 2: assign neighbor patches
	for (auto &[index,patch]:patches){
		vector<shared_ptr<Patch>> neighbors;
		auto neighbors_indices = patch->neighbors_indices;
		for (auto const &neighbor_index:neighbors_indices){
			auto neighbor_patch = patches.at(neighbor_index);
			neighbors.push_back(neighbor_patch);
		}
		patch->neighbors = neighbors;
	}

	
	/** create agents **/
	unsigned agent_count = 10;
	vector<shared_ptr<Agent>> agents;
	for (unsigned i=0; i < agent_count; i++){
		agents.push_back(make_shared<MSC>());
	}
	
#endif //TEST
	
	return 0;
}




