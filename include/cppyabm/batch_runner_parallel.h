#include "batch_runner.h"
#include <mpi.h>

//!   Parallel Batch Runner
/*!
 To run this, execute:
 - mpirun -n 4 ./program
 */


struct MPI_struct{
    static int numProcs;
    static int procID;
    static const int master_p=0; //master processor id
};
inline int MPI_struct::numProcs = 0;
inline int MPI_struct::procID = 0;

template<typename OBJECT_T>
struct ParallelBatchRunner: public BatchRunner<OBJECT_T>{
    using baseClass = BatchRunner<OBJECT_T>;
    using baseClass::baseClass;

    void run(){
        
        MPI_Init(NULL,NULL);
        MPI_Comm_size(MPI_COMM_WORLD,&MPI_struct::numProcs);
        MPI_Comm_rank(MPI_COMM_WORLD,&MPI_struct::procID);
        int chunksize,offset, tag2, tag1,dest,source,remainder_jobs,cpu_time;
        MPI_Status status;
        tag2 = 1;
        tag1 = 2;
        /* tasks conducted by the master processor */
        if (MPI_struct::procID == MPI_struct::master_p){
            auto start = chrono::high_resolution_clock::now();

            /* send each task its portion of the array-master keeps the 1st portion */
            chunksize = this->iter_n/MPI_struct::numProcs;        //partion is the allocated job for each processor
            remainder_jobs = this->iter_n%MPI_struct::numProcs;
            offset = chunksize+remainder_jobs;
            for (dest =1; dest<MPI_struct::numProcs; dest++){
                MPI_Send(&this->iter_n,1,MPI_INT, dest, tag1, MPI_COMM_WORLD); //!<send  _total_iteration_n
                MPI_Send(&chunksize,1,MPI_INT, dest, tag1, MPI_COMM_WORLD); //!<send chunksize
                MPI_Send(&offset,1,MPI_INT, dest, tag1, MPI_COMM_WORLD);
                offset += chunksize;
            }
            /* master does its part of the work */
            offset = 0;
            this->run_model(offset,chunksize+remainder_jobs);  // task1 also does carry out the jobs which are left over
            /* wait to receive results from other processes */
            for (source =1;source<MPI_struct::numProcs; source++){
                MPI_Recv(&offset,1,MPI_INT,source,tag1,MPI_COMM_WORLD,&status);
            }
            auto end = chrono::high_resolution_clock::now();
            auto iter_dur = chrono::duration_cast<chrono::milliseconds>(end-start);
            cout<<"Simulation for "<<this->iter_n<<" took "<<iter_dur.count()<<" milliseconds"<<endl;
  
        }
        /* tasks for the other processors */
        if (MPI_struct::procID != MPI_struct::master_p){
            source = MPI_struct::master_p;
            MPI_Recv(&this->iter_n,1,MPI_INT, source, tag1, MPI_COMM_WORLD,&status);
            MPI_Recv(&chunksize,1,MPI_INT, source, tag1, MPI_COMM_WORLD,&status);
            MPI_Recv(&offset,1,MPI_INT, source, tag1, MPI_COMM_WORLD,&status);
            cout<<"proc:"<<MPI_struct::procID<<" chunksize:"<<chunksize<<" offset:"<<offset<<endl;
            this->run_model(offset,chunksize);
            dest = MPI_struct::master_p;
            MPI_Send(&offset, 1, MPI_INT, dest, tag1, MPI_COMM_WORLD);
        }
        MPI_Finalize();
    
    }
        
};
