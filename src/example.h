#include "iostream"

#include <ostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;
#include "pybind11/pybind11.h"
#include <pybind11/stl.h>
#include "mesh.h"
namespace py = pybind11;
struct Agent{
    Agent(){}
    virtual void step(){
        
    }
    
};
struct Patch{
    Patch(){}
    virtual void step(){
        
    }
};
template<typename T1>
struct Env{
    Env(){}
    void setup(){};
    vector<shared_ptr<T1>> agents;
    vector<shared_ptr<Patch>> patches;
    vector<shared_ptr<Mesh>> meshes;
    // explicit Env(Agent* agent_obj){
    //     this->agent = agent_obj;
    // }
    
    void init(){
        this->setup();
        this->agents.push_back(make_shared<T1>());
    }

    virtual void step(){
        this->agents[0]->step();
        // cout<<name<<endl;
        // this->agent->step();
        // this->agent.attr("step")();
        // this->agent.step();
    }
    virtual void run(){
        this->step();
    }
};
// void Env::setup(){

// }



