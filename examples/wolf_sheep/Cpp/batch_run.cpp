#include "models.h"
// #include <cppyabm/batch_runner_serial.h>
#include <cppyabm/batch_runner_parallel.h>
#include <iostream>
#include <fstream>
using namespace std;


int main(){
	auto model = make_shared<WolfSheep>();
	auto runner_obj = ParallelBatchRunner<shared_ptr<WolfSheep>>(model,20);
	runner_obj.run();
	return 0;
}