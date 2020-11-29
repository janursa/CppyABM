#include <iostream>
#include "example.cpp"

int main(){
	std::cout<<"hello world"<<endl;
	auto envObj = make_shared<healingEnv>();
	envObj->setup();
	envObj->run();
	return 0;
}