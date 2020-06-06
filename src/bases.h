
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
struct Base{

protected:
	/** Getters and setters **/
	shared_ptr<Base> get_patch();                  //!< Returns _patchPtr
	shared_ptr<Env> get_env();
	std::shared_ptr<Agent> get_ptr();
	void set_patch(shared_ptr<Patch> );
	void set_env(shared_ptr<Env> );

    weak_ptr<Base> _patch_ptr;                    //!< weak pointer to the patch the cell resides in
    weak_ptr<Base> _env_ptr;
    weak_ptr<Base> _agent_ptr;
}

struct Agent:public enable_shared_from_this<Agent>{
	Agent() {};
	virtual ~Agent();
	/** Major functions **/ 
	void step(){};
	void update(){};

	std::string name; 
    /** Flags **/
    std::pair <bool,std::string> switch_info = std::make_pair(false,"");
    bool hatch_flag = false;
    bool walk_flag = false;
    bool disappear_flag = false;
    /** storage **/
    map<string,float> data;

    /** pure virtuals **/
    

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
inline std::shared_ptr<Agent> Agent::get_ptr(){
    return this->shared_from_this();
}



struct Patch{
	Patch();                  //!< Patch constructor
    ~Patch(){};
    shared_ptr<Agent> agent_ptr;
    };
struct Env{
    vector<shared_ptr<Agent>> agents;
    vector<shared_ptr<Patch>> patches;
    };

