#pragma once
#include "pybind11/pybind11.h"
#include "bases.h"
namespace py=pybind11;
using namespace std;

struct PyAgent : public Agent {
    using Agent::Agent;
    void step() override {
        PYBIND11_OVERLOAD_PURE(
            void, 
            Agent,      
            step         
        );
    }
};
