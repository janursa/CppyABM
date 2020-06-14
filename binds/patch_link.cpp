#include "patch.cpp"
struct PyPatch : public Patch {
    using Patch::Patch;
    void step() override {
    	PYBIND11_OVERLOAD(
    		void,
    		Patch,
    		step
    		);
    }
};

