#include <iostream>
#include "cpp_example.cpp"
#include <chrono>

int main(){

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	auto envObj = make_shared<Domain>();
	envObj->setup();
	envObj->episode();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();	
	std::cout << "Simulation took " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/1000000 << " s" << std::endl;

	
	return 0;
}