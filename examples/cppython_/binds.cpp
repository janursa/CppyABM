#include <iostream>
#include "example.h"
#include "CPPYABM/include/ABM/bind_tools.h"

class PyhealingEnv: public healingEnv{
    using healingEnv::healingEnv;
    shared_ptr<Cell> generate_agent(string agent_name) override {
        PYBIND11_OVERLOAD(
            shared_ptr<Cell>, 
            healingEnv,      
            generate_agent,
            agent_name         
        );
    }
};

PYBIND11_MODULE(myBinds, m) {
    py::class_<healingEnv,PyhealingEnv,std::shared_ptr<healingEnv>> (m,"healingEnv",py::dynamic_attr())
        .def(py::init<>())
        .def("setup",&healingEnv::setup);
    // py::class_<Cell,std::shared_ptr<Cell>> (m,"Cell",py::dynamic_attr())
    //     .def(py::init<shared_ptr<healingEnv>,string>());
 
}

