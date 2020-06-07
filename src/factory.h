#pragma once
#include "bases.h"
#include "agent.h"
using namespace std;
struct myFactory:public Factory
{	
	virtual shared_ptr<Agent> create_agent(){
		return make_shared<MSC>();
	};
	virtual void update(){
		cout<<"no update"<<endl;
		return;
	};
};