#include "bases.h"
// binding class
template<typename class_name,typename py_class_name>
void link_env(py::module m, string class_name_string){
    py::class_<class_name,py_class_name,std::shared_ptr<class_name>>(m,class_name_string.c_str(),py::dynamic_attr())
        .def(py::init<>())
        .def("check",&class_name::check)
        .def("place_agent_randomly",&class_name::place_agent_randomly)
        .def("setup_domain",&class_name::setup_domain)
        .def("step_agents",&class_name::step_agents)
        .def("step_patches",&class_name::step_patches)
        .def("place_agent",&class_name::place_agent)
        .def("update",&class_name::update)
        .def("setup_agents",&class_name::setup_agents)
        .def("count_agents",&class_name::count_agents)
        .def("collect_from_patches",&class_name::collect_from_patches)
        .def_readwrite("patches",&class_name::patches)
        .def_readwrite("agents",&class_name::agents);
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
template<typename class_name,typename py_class_name>
void link_patch(py::module &m, string class_name_ptr){
    py::class_<class_name,py_class_name,std::shared_ptr<class_name>>(m,class_name_ptr.c_str(),py::dynamic_attr())
        .def(py::init<shared_ptr<Env>>())
        .def("empty_neighbor", &class_name::empty_neighbor,"Return an empty patch around the patch",
            py::arg("quiet")=false)
        .def("find_neighbor_agents",&class_name::find_neighbor_agents,"Returns a vector of agents in one patch neighborhood",
            py::arg("include_self")=true)
        .def_readwrite("coords",&class_name::coords)
        .def_readwrite("agent",&class_name::agent)
        .def_readwrite("empty",&class_name::empty)
        .def_readwrite("disappear",&class_name::disappear)
        .def_readwrite("data",&class_name::data)
        .def_readwrite("neighbors",&class_name::neighbors);
        
}