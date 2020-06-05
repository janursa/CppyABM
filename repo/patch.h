
#pragma once
#include <map>
#include <string>
#include "toolbox.h"
#include "frame.h"
using namespace std;
template <unsigned dim>
class Frame;
template <unsigned dim>
class Agent;
//!  Patch class
/*!
  Keeps record of patch attributes and data.
*/
template<unsigned dim>
class Patch:public enable_shared_from_this<Patch<dim>> {
public:


    Patch();                  //!< Patch constructor
    ~Patch(){};




    static std::shared_ptr<Frame<dim>> get_modelPtr();              //!< Returns #_modelPtr
     settings_t configs(){
        return get_modelPtr()->settings["configs"];
     };
    
    void update_patch_inputs();
    void receive_patch_outputs();
    void function();

    void removeAgent();        //!< Removes cell from patch
    void initialize();             //!< initialize patch variables
    bool hasAgent() const;
    void setAgent(shared_ptr<Agent<dim>> cellPtr);
    shared_ptr<Agent<dim>> getAgent();
    vector<std::shared_ptr<Patch<dim>>> neighborPatches;
    std::map<std::string,float> data; 
    vector<float> xyzcoords; 
    unsigned patchIndex;

    py::object get_patch_model(){
        return get_modelPtr()->patch_model;
    }
    static weak_ptr<Frame<dim>>& _modelPtr(){static  weak_ptr<Frame<dim>> var{}; return var;};
    bool _hasAgent;
private:
    weak_ptr<Agent<dim>> _cellPtr;
    

};



template<unsigned dim>
inline Patch<dim>::Patch(){
    _hasAgent = false;
}
template <unsigned dim>
inline void Patch<dim>::initialize(){
    for (auto key:configs()["patch"]["attrs"].attr("keys")()) {
          string name =py::cast<string>(key);
          float value =py::cast<float>(configs()["patch"]["attrs"][key]);
          this->data.insert(std::pair<string,float>(name,value));
    }
}
template<unsigned dim>
inline void Patch<dim>::setAgent(shared_ptr<Agent<dim>> pPtr){
    if (!pPtr) {
        std:cerr<<"Agent pointer is null"<<endl;
        std::terminate();
    }
    try{
        this->_cellPtr  = weak_ptr<Agent<dim>>(pPtr);
    } catch(...){
        std::cerr<<"couldn't convert to shared ptr in set cell"<<endl;

    }

    _hasAgent = true;
}
template <unsigned dim>
void Patch<dim>::removeAgent(){
    _hasAgent = false;
    this->_cellPtr.reset();
}

template<unsigned dim>
inline shared_ptr<Agent<dim>> Patch<dim>::getAgent(){
    shared_ptr<Agent<dim>> p = _cellPtr.lock();
    if (!p) {
        throw logic_error("Weak_ptr (_cellPtr inside patch) expired");
    }
    return p;
}

template<unsigned dim>
inline std::shared_ptr<Frame<dim>> Patch<dim>::get_modelPtr(){
    shared_ptr<Frame<dim>> p = _modelPtr().lock();
    if (!p){
        throw logic_error("Weak_ptr (_modelPtr inside patch) expired)");
    }
    return p;
}

template<unsigned dim>
inline bool Patch<dim>::hasAgent() const{
    return _hasAgent;
}


template <unsigned dim>
inline void Patch<dim>::update_patch_inputs(){
    auto main_tags = get_patch_model().attr("inputs").attr("keys")();
    // for self inputs
    auto EXTRACT_SELF_INPUTS = [&](){
        for (auto tag_:get_patch_model().attr("inputs")["self"].attr("keys")()){ // collecting inputs from self
            float value;
            auto tag = py::cast<string>(tag_);
            try {value = this->data.at(tag);}
            catch (out_of_range & er) {
                cerr<<"The input '"<<tag<<"' for patch [self] is not defined but requested as input"<<endl;
                std::terminate();
            } 
            get_patch_model().attr("inputs")["self"][tag_] = value;
        }
    };
    // for patch inputs
    auto EXTRACT_AGENT_INPUTS = [&](){
        shared_ptr<Agent<dim>> agent;
        try{ agent =  this->getAgent();}
        catch(logic_error &er){return;}
        for (auto &tag_:get_patch_model().attr("inputs")["agent"].attr("keys")()){ // collecting inputs from agent
            string tag = py::cast<string>(tag_);
           
            float value = agent->data[tag];
            
            vector<float> values {value};
            get_patch_model().attr("inputs")["agent"][tag_] = value ;
        }
    };
    for (auto &tag_:main_tags){
        string tag = py::cast<string>(tag_);
        if (tag == "self") EXTRACT_SELF_INPUTS();
        else if (tag == "agent") EXTRACT_AGENT_INPUTS();
        else {
            cerr<<"Error: output key '"<<tag<<"' is neither self or agent. Correct the input formating for patch inputs. "<<endl;
            std::terminate();
        }
    }
    
   
}
template <unsigned dim>
inline void Patch<dim>::receive_patch_outputs(){
    get_patch_model().attr("forward")();  // updates the outputs in python file
    // output_t outputs; 
    auto main_tags = get_patch_model().attr("outputs").attr("keys")();
    auto RECEIVE_SELF_OUTPUTS = [&](py::dict sub_output){
        for (const auto event_key_:sub_output.attr("keys")()){
                auto event_key = py::cast<string>(event_key_);
                this->data[event_key] = py::cast<float>(sub_output[event_key_]);
                
            }
    };
    auto RECEIVE_AGENT_OUTPUTS = [&](py::dict sub_output){
        for (const auto event_key_:sub_output.attr("keys")()){
                auto event_key = py::cast<string>(event_key_);
                try {this->getAgent()->data[event_key] = py::cast<float>(sub_output[event_key_]);}
                catch(logic_error&er) {continue;};
            }
        };
    for (auto &tag_:main_tags){
        string tag = py::cast<string>(tag_);
        auto sub_output = get_patch_model().attr("outputs")[tag_];
        if (tag == "self") RECEIVE_SELF_OUTPUTS(sub_output);
        else if (tag == "patch") RECEIVE_AGENT_OUTPUTS(sub_output);
        else {
            cerr<<"Error: output key '"<<tag<<"' is neither self or agent. Correct the output formating for patch. "<<endl;
            std::terminate();
        }
    }
    
}
template <unsigned dim>
inline void Patch<dim>::function(){
    this->update_patch_inputs();
    this->receive_patch_outputs();    
}


