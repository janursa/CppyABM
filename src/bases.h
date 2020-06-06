
#pragma once
#include <map>
#include "toolbox.h"
using std::shared_ptr;
using std::vector;
using namespace tools;
struct Env;
struct Agent;
struct Patch;
using namespace std;
template<typename T>
struct Base: public enable_shared_from_this<T>{
	std::shared_ptr<Base<T>> get_ptr();
	std::string name; 
	// void set_agent(shared_ptr<Base<T>> agent_ptr){
	// 	if (!agent_ptr) {
	// 		string message = "nullptr is given to set for agent";
	// 		throw null_ptr_error(message);
	//     }
	//     try{
	//         this->_agent_ptr  = weak_ptr<Env>(env_pt);
	//     } catch(...){
	//         string message = "Failed to convert env shared ptr to weak form";
	//     	throw weak_to_shared_error(message);
	//     }
	// };

protected:
	weak_ptr<Base<T>> _agent_ptr;                    //!< weak pointer to the patch the cell resides in

};
template<typename T>
inline std::shared_ptr<Base<T>> Base<T>::get_ptr(){
    return this->shared_from_this();

}

struct Agent: public Base<Agent>{
	
	Agent(){name = "Agent";};
	~Agent(){};
	/** Major functions **/ 
	void step(){};
	void update(){};
	/** Getters and setters **/
	shared_ptr<Patch> get_patch();                  //!< Returns _patchPtr
	shared_ptr<Env> get_env();
	void set_patch(shared_ptr<Patch> );
	void set_env(shared_ptr<Env> );

    /** Flags **/
    std::pair <bool,std::string> switch_info = std::make_pair(false,"");
    bool hatch_flag = false;
    bool walk_flag = false;
    bool disappear_flag = false;
    /** storage **/
    map<string,float> data;

    /** pure virtuals **/
    
protected:
	
    weak_ptr<Patch> _patch_ptr;                    //!< weak pointer to the patch the cell resides in
    weak_ptr<Env> _env_ptr;
};
inline void Agent::set_env(shared_ptr<Env> env_pt){
	if (!env_pt) {
		string message = "null ptr for env from inside agent";
		throw null_ptr_error(message);
    }
    try{
        this->_env_ptr  = weak_ptr<Env>(env_pt);
    } catch(...){
        string message = "Failed to convert env shared ptr to weak form";
    	throw weak_to_shared_error(message);
    }
}
inline shared_ptr<Patch> Agent::get_patch(){
    if(this->_patch_ptr.expired()){
    	throw expired_weak_ptr("Weak ptr of patch inside agent is exipred");
    }
    auto s_patchPtr = shared_ptr<Patch>(this->_patch_ptr);
    return s_patchPtr;
}

inline shared_ptr<Env> Agent::get_env(){
    if(this->_env_ptr.expired()){
        throw expired_weak_ptr("Weak ptr of env inside agent is exipred");
    }
    auto s_env_ptr = shared_ptr<Env>(this->_env_ptr);
    return s_env_ptr;
}
inline void Agent::set_patch(shared_ptr<Patch> pPtr){
    if (!pPtr) {
        string message = "null ptr for patch from inside agent";
		throw null_ptr_error(message);
    }
    try{
        this->_patch_ptr  = weak_ptr<Patch>(pPtr);
    } catch(...){
		string message = "Failed to convert patch shared ptr to weak form";
    	throw weak_to_shared_error(message);    
    }
}



struct Patch{
	// Patch(){};                  //!< Patch constructor
    // ~Patch(){};
    shared_ptr<Agent> agent_ptr;
    int test_var;
    void set(){
    	test_var = 2;
    }
};
struct Env{
    vector<shared_ptr<Agent>> agents;
    vector<shared_ptr<Patch>> patches;

    int test_var;
    void set(){
    	test_var = 10;
    }
    int get(){
    	return test_var;
    } 
};

