#include "iostream"
#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
// #include "bases.h"
#include "tools.h"
#include "pybases.h"
using json = nlohmann::json;
using namespace std;
class myEnv: public Env{
	shared_ptr<Agent> generate_agent(string name){

	}
	shared_ptr<Patch> generate_patch(){

	}
};
class PyMyEnv : public PyEnv{

};


int main(){
	// test_func();
	cout<<"hell"<<endl;
	// py::module mm;
	// link_env(mm,"Env");
	// auto obj = myEnv();
	// 
	// link_env<MyEnv,PyEnv>(mm,"Env");
	return 0;
}




