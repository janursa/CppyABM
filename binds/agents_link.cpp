#include "agents.cpp"
struct PyAgent : public Agent {
    using Agent::Agent;
    void step() override {
        PYBIND11_OVERLOAD_PURE(
            void, 
            Agent,      
            step         
        );
    }

    void inherit(shared_ptr<Agent> father) override{
    	PYBIND11_OVERLOAD(
            void, 
            Agent,      
            inherit,
            father         
        );
    };
};

void link_agent(py::module &m) {
	/** Agent **/
    py::class_<Agent,PyAgent,std::shared_ptr<Agent>>(m,"Agent",py::dynamic_attr())
    	.def(py::init<shared_ptr<Env>,string>(),"Initialize",py::arg("env"),py::arg("class_name"))
    	.def("move",&Agent::move,"Move the agent to a new patch")
    	.def("order_hatch",&Agent::order_hatch,"Hatch request",
    		py::arg("patch")=nullptr, py::arg("inherit")=false,
    		py::arg("quiet")=false, py::arg("reset")=false)
    	.def("order_move",&Agent::order_move,"Move request",
    		py::arg("patch")=nullptr, 
    		py::arg("quiet")=false, py::arg("reset")=false)
    	.def("order_switch",&Agent::order_switch,"Switch request",
    		py::arg("to"))
    	.def_readwrite("disappear",&Agent::disappear)
    	.def_readwrite("env",&Agent::env)
    	.def_readwrite("data",&Agent::data)
    	.def_readwrite("patch",&Agent::patch)
    	.def_readwrite("class_name",&Agent::class_name);

}

