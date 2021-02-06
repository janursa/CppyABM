#include <iostream>
#include "cpp_example.h"
#include <chrono>
using namespace std;
int main(int argc, char** argv){
	if (argc>2){
		cerr<<"More than one argument is given"<<endl;
		exit(2);
	}
	bool output_flag;
	
	output_flag = argv[1];
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	auto envObj = make_shared<Domain>(output_flag);
	envObj->setup();
	envObj->episode();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();	
	std::cout << "Simulation took " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()/1000000 << " s" << std::endl;

	
	return 0;
}