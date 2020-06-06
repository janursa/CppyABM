// #include <iostream>
// #include "pybind11/pybind11.h"
// using std::cout;
// using std::endl;
// namespace py=pybind11;
// using namespace std;
// struct Base{
// 	// Base(){}
// 	virtual ~Base(){}
// 	// 
//         virtual void name(){
//             cout<<"base"<<endl;
//             }

	
//     };
// struct PyBase : public Base {
//     using Base::Base;
//     void name() override {
//         PYBIND11_OVERLOAD(
//             void, 
//             Base,      
//             name         
                  
//         );
//     }
// };

// struct AbstractFactory {
// 	AbstractFactory(){}
//     virtual Base* create(){
//     	return nullptr;
//     } ;
// };


// struct PyAbstractFactory : public AbstractFactory {
//     using AbstractFactory::AbstractFactory;
//     Base* create() override {
//         PYBIND11_OVERLOAD(
//             Base*, 
//             AbstractFactory,      
//             create         
//         );
//     }
// };
// struct Env{
//         Env(){}
//         void get(AbstractFactory * AF){
//         	auto obj = AF->create();
//         	obj->name();
//         	holder = obj;
//         }
//         void get_py(py::object AF){
//         	auto obj = AF.attr("create")();
//         	obj->name();
//         	holder = obj;
//         }
//         void test(){
//             holder->name();
//         }
//         Base* holder;
        
//     };

// // #include "example.h"
// // using namespace std;
// // class PyEnv : public Env {
// // public:
// //     /* Inherit the constructors */
// //     using Env::Env;

// //     /* Trampoline (need one for each virtual function) */
// //     void run() override {
// //         PYBIND11_OVERLOAD(
// //             void, /* Return type */
// //             Env,      /* Parent class */
// //             run          /* Name of function in C++ (must match Python name) */
// //                   /* Argument(s) */
// //         );
// //     }
// // };


// PYBIND11_MODULE(binds, m) {
//     py::class_<Base, PyBase>(m,"Base",py::dynamic_attr())
//     	.def(py::init<>());
//     py::class_<Env>(m, "Env").def(py::init<>())
// 	    .def("get",&Env::get)
// 	    .def("get_py",&Env::get_py);
// 	py::class_<AbstractFactory,PyAbstractFactory>(m,"AbstractFactory")
// 	.def(py::init<>())
// 	.def("create",&AbstractFactory::create);
		

// }

