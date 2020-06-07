#pragma once
#include "pybind11/pybind11.h"
#include "bases.h"
namespace py=pybind11;
using namespace std;
struct PyFactory : public Factory {
    using Factory::Factory;
    shared_ptr<Agent> create_agent() override {
        PYBIND11_OVERLOAD_PURE(
            shared_ptr<Agent>, 
            Factory,      
            create_agent         
        );
    };
    void update() override {
        PYBIND11_OVERLOAD_PURE(
            void, 
            Factory,      
            update         
        );
    }
};
