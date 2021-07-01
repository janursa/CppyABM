#include "batch_runner.h"

//!   Serial batch runner
/*!
*/



template<typename OBJECT_T>
struct SerialBatchRunner:public BatchRunner<OBJECT_T>{
    using baseClass = BatchRunner<OBJECT_T>;
    using baseClass::baseClass;

    virtual void run(){
        
        this->run_model(0,this->iter_n);

    } 
};
