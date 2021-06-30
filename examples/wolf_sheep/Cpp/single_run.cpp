#include "models.h"
#include <iostream>
#include <fstream>
using namespace std;

void output(DataType results){
	ofstream fd;
	fd.open("single_run_results.csv");
	vector<string> keys;
	vector<vector<float>> stack_results;
	for (auto & [key,value]:results){
		keys.push_back(key);
		stack_results.push_back(value);
	}
	// write the header
	for (auto &key:keys){
		fd<<","<<key;
	}
	fd<<endl;
	// write the rows
	unsigned len = 0;
	if (stack_results.size()>0) len = stack_results[0].size();
	for (unsigned i = 0; i<len; i++){
		fd<<i;
		for (unsigned j=0; j<keys.size(); j++){
			fd<<","<<stack_results[j][i];
		}	
		fd<<endl;
	}
	fd.close();
}
int main(){
	auto runner_obj = make_shared<WolfSheep>();
	auto results = runner_obj->episode();
	output(results);
	return 0;
}