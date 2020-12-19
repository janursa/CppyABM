#include <iostream>
#include "example.h"
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include "CPPYABM/include/ABM/bind_tools.h"

using CellsBank = vector<shared_ptr<Cell>>;
using myPatchesBank = map<unsigned,shared_ptr<myPatch>>;
PYBIND11_MAKE_OPAQUE(CellsBank);
PYBIND11_MAKE_OPAQUE(myPatchesBank);
// struct PyhealingEnv : public healingEnv {
//     using healingEnv::healingEnv;
//     shared_ptr<Cell> generate_agent(string agent_name) override {
//         PYBIND11_OVERLOAD_PURE(
//             shared_ptr<Cell>, 
//             healingEnv,      
//             generate_agent,
//             agent_name         
//         );
//     }
// };
template<class derivedEnv,class derivedAgent,class derivedPatch>
class PyhealingEnv: public PyEnv<derivedEnv,derivedAgent,derivedPatch>{
    using PyEnv<derivedEnv,derivedAgent,derivedPatch>::PyEnv;
    shared_ptr<derivedAgent> generate_agent(string agent_name) override {
        PYBIND11_OVERLOAD(
            shared_ptr<derivedAgent>, 
            derivedEnv,      
            generate_agent,
            agent_name         
        );
    }
    void update() override {
        PYBIND11_OVERLOAD(
            void, 
            derivedEnv,      
            update
                     
        );
    }
};
struct PyCell : public Cell {
    using Cell::Cell;
    void step() override {
        PYBIND11_OVERLOAD(
            void, 
            Cell,      
            step
                     
        );
    }
};
PYBIND11_MODULE(myBinds, m) {
    expose_defaults<healingEnv,Cell,myPatch>(m);
    // expose_base_env<healingEnv,Cell,myPatch>(m,"baseEnv");
    // expose_base_agent<healingEnv,Cell,myPatch>(m,"baseAgent");
    // expose_base_patch<healingEnv,Cell,myPatch>(m,"basePatch");
    auto env_obj = expose_env<healingEnv,Cell,myPatch,PyhealingEnv<healingEnv,Cell,myPatch>>(m,"healingEnv");
    env_obj.def("setup",&healingEnv::setup);
    env_obj.def("step",&healingEnv::step);
    expose_agent<healingEnv,Cell,myPatch,PyCell>(m,"Cell");
}

