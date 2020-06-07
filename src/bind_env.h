#pragma once
#include "pybind11/pybind11.h"
#include "bases.h"
namespace py=pybind11;
using namespace std;

struct PyEnv : public Env {
    using Env::Env;
    void setup() override {
        PYBIND11_OVERLOAD_PURE(
            void, 
            Env,      
            setup         
        );
    }
};
