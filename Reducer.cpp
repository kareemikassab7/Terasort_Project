#ifndef REDUCER_CPP_
#define REDUCER_CPP_

#include "Reducer.h"

template <class T, typename S>
Reducer<T,S>::Reducer (Partition<T,S> * p_partition)
{
    // Declaring the partition pointer data member
    partition = p_partition;
}
template <class T, typename S>
void  Reducer<T,S>::execute ()
{
    //This function is get invoked against each thread working on a partition to sort the items after adjusting the elements inside according to the cut points
    printf ("Launching Reducer Sort Engine...\n");
    	partition->sort();
    printf ("Reducer Sort Engine Terminated...\n");
}
template <class T, typename S>
void Reducer<T,S>::start (Reducer<T,S> * me)
{
    //This method starts the whole process for every reducer thread
    me->execute();
}

template <class T, typename S>
void Reducer<T,S>::setThread(std::thread * p_th)
{
    //Declaring the thread pointer of the reducer
    th = p_th;
}

template <class T, typename S>
void Reducer<T,S>::waitForRunToFinish()
{
    //Joining the thread to make the main process wait and not to finsih until the whole threads finish their work
    th->join();
}
template <class T, typename S>
Reducer<T,S>::~Reducer ()
{
    //Destructor used to free the thread pointer of the data members
    delete(th);
}

#endif
