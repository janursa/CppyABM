#include <iostream>
#include "bases.h"

void Agent::move(shared_ptr<Patch> dest, bool quiet){
        if (!dest->empty) {
            if (!quiet) throw patch_availibility("Given patch for move is not empty");
            else return;
        }
        this->patch->remove_agent(); // remove it from the current patch
        this->env->connect_patch_agent(dest,this->get_ptr());
    }
void Agent::order_hatch(shared_ptr<Patch> patch, bool inherit, bool quiet, bool reset)
    {
    this->_hatch =  HATCH_CONFIG(true,patch,inherit,quiet,reset);

}
void Agent::order_move(shared_ptr<Patch> patch = nullptr, 
             bool quiet = false, bool reset = false){
    this->_move =  MOVE_CONFIG(true,patch,quiet,reset);

}

std::shared_ptr<Agent> Agent::get_ptr(){
    return this->shared_from_this();

}
template<typename class_name,typename py_class_name>
void link_agent(py::module &m, string class_name_str) {
    /** Agent **/
    py::class_<class_name,py_class_name,std::shared_ptr<class_name>>(m,class_name_str.c_str(),py::dynamic_attr())
        .def(py::init<shared_ptr<Env>,string>(),"Initialize",py::arg("env"),py::arg("class_name"))
        .def("move",&class_name::move,"Move the agent to a new patch")
        .def("order_hatch",&class_name::order_hatch,"Hatch request",
            py::arg("patch")=nullptr, py::arg("inherit")=false,
            py::arg("quiet")=false, py::arg("reset")=false)
        .def("order_move",&class_name::order_move,"Move request",
            py::arg("patch")=nullptr, 
            py::arg("quiet")=false, py::arg("reset")=false)
        .def("order_switch",&class_name::order_switch,"Switch request",
            py::arg("to"))
        .def_readwrite("disappear",&class_name::disappear)
        .def_readwrite("env",&class_name::env)
        .def_readwrite("data",&class_name::data)
        .def_readwrite("patch",&class_name::patch)
        .def_readwrite("class_name",&class_name::class_name);

}