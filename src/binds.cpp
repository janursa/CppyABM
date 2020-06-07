#include <iostream>
#include <pybind11/stl.h>
#include "bind_agent.h"
#include "bind_env.h"
#include "bind_factory.h"
#include "mesh.h"
// #include "bases.h"
// #include "pybind11/pybind11.h"
// #include "agent.h"
// using std::cout;
// using std::endl;
// namespace py=pybind11;





PYBIND11_MODULE(binds, m) {
    py::class_<Agent,PyAgent,std::shared_ptr<Agent>>(m,"Agent")
    	.def(py::init<>())
    	.def_readwrite("disappear_flag",&Agent::disappear_flag);
    py::class_<Env,PyEnv,std::shared_ptr<Env>>(m,"Env")
    	.def(py::init<>())
    	.def("check",&Env::check);
    py::class_<Factory,PyFactory,std::shared_ptr<Factory>>(m,"Factory",py::dynamic_attr())
    	.def(py::init<>());
    py::class_<mesh_tools>(m,"mesh_tools")
    	.def(py::init<>())
    	.def("grid",&mesh_tools::grid);
    	// .def_readwrite("agents",&Env::agents);
 //    py::class_<Env>(m, "Env").def(py::init<>())
	//     .def("get",&Env::get)
	//     .def("get_py",&Env::get_py);
	// py::class_<AbstractFactory,PyAbstractFactory>(m,"AbstractFactory")
	// .def(py::init<>())
	// .def("create",&AbstractFactory::create);py::dynamic_attr())
		

}

