#include "iostream"
#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include "env.h"
#include "agent.h"
#include "patch.h"
#include "mesh.h"
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
	
	

	
	/** create agents **/
	unsigned agent_count = 10;
	vector<shared_ptr<Agent>> agents;
	for (unsigned i=0; i < agent_count; i++){
		agents.push_back(make_shared<MSC>());
	}
	
#endif //TEST
	
	return 0;
}




