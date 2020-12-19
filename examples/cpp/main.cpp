#include <iostream>
#include "example.cpp"

int main(){
	std::cout<<"hello world"<<endl;
	auto envObj = make_shared<healingEnv>();
	envObj->setup();
	for (unsigned i = 0; i < 100; i++){
		cout<<"iteration "<<i<<endl;
		envObj->run();
	}
	
	return 0;
}