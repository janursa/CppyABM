#include "iostream"
#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include "env.h"
#include "agent.h"
#include "patch.h"

using namespace std;

struct MSC: public Agent{
	MSC(){
		this->name = "MSC";
	}
};

int test_inh_shared1(){
	auto agent_ptr = make_shared<Agent>();
	auto patch_ptr = make_shared<Patch>();
	auto env_ptr = make_shared<Env>();
	env_ptr->set();
	agent_ptr->set_env(env_ptr);
	auto env = agent_ptr->get_env();
	return env->get();
}
string test_agent_name_1(){
	auto agent_ptr = make_shared<Agent>();
	auto agent_ptr2 = agent_ptr->get_ptr();
	return agent_ptr2->name;
}
string test_agent_name_2(){
	auto agent_ptr = make_shared<MSC>();
	auto agent_ptr2 = agent_ptr->get_ptr();
	return agent_ptr2->name;
}
void test(){
	assert(test_inh_shared1() == 10);
	assert(test_agent_name_1() == "Agent");
	assert(test_agent_name_2() == "MSC");
}
int main(){
	test();
	return 0;
}




