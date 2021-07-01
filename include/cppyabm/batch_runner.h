#include <vector>

using namespace std;

//!   Base batch runner
/*!
This class should be specialized either to serial or parallel and should not be inherited directly.
  It receives an object of the model together with the number of replica.
  The model should have two functions of "reset" and "episode". Reset should receive an integer that indicates the iteration number.
  In its current version, user should output the results manually inside "episode" and write an script to compile them after.
*/



template<typename OBJECT_T>
struct BatchRunner{
    OBJECT_T model;
    unsigned iter_n;
    BatchRunner(OBJECT_T model, unsigned iter_n){
        this->model = model;
        this->iter_n = iter_n;
    }
    void output_cpu_time(float value){
         /* print out the cpu time */
        ofstream fd;
        string file_name = "cpu_time.csv";
        fd.open(file_name);
        fd<<value<<endl;
        fd.close();

    }
    virtual void run()=0;
        

    void run_model(int myoffset, int chunk){
        for (int eIter=myoffset; eIter<myoffset+chunk; eIter++) {
            model->reset(eIter);
            model->episode();
        }
        
    }

    
};
