#include <iostream>
#include "bases.h"

 shared_ptr<Patch> Patch::empty_neighbor(bool quiet){
        auto neighbors = this->neighbors;
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(neighbors.begin(),neighbors.end(),g);
        for (auto &neighbor:neighbors){
            if (neighbor->empty){
                return neighbor;
            }
        }
        if (!quiet) throw patch_availibility("No available patch around the agent");
        return nullptr;
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