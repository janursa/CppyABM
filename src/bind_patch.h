#pragma once
#include "pybind11/pybind11.h"
#include "bases.h"
namespace py=pybind11;
using namespace std;

struct PyPatch : public Patch {
    using Patch::Patch;
    // void setup() override {
    //     PYBIND11_OVERLOAD_PURE(
    //         void, 
    //         Env,      
    //         setup         
    //     );
    // }
};
