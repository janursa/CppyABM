#include "example.h"
using namespace std;
template<typename T1>
class PyEnv : public Env<T1> {
public:
    /* Inherit the constructors */
    using Env<T1>::Env;

    /* Trampoline (need one for each virtual function) */
    void run() override {
        PYBIND11_OVERLOAD(
            void, /* Return type */
            Env<T1>,      /* Parent class */
            run          /* Name of function in C++ (must match Python name) */
                  /* Argument(s) */
        );
    }
};
class PyAgent : public Agent {
public:
    /* Inherit the constructors */
    using Agent::Agent;

    /* Trampoline (need one for each virtual function) */
    void step() override {
        PYBIND11_OVERLOAD(
            void, /* Return type */
            Agent,      /* Parent class */
            step          /* Name of function in C++ (must match Python name) */
                  /* Argument(s) */
        );
    }
};
PYBIND11_MODULE(example, m) {
    py::class_<Agent,shared_ptr<Agent>, PyAgent>(m,"Agent",py::dynamic_attr())
    	.def(py::init<>());
    py::class_<Env<Agent>, PyEnv<Agent>>(m, "Env").def(py::init<>())
    	.def_readwrite("agents",&Env<Agent>::agents)
	    .def("init",&Env<Agent>::init)
	    .def("step",&Env<Agent>::step)
	    .def("run",&Env<Agent>::run);
}

