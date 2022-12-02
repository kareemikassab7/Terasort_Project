#ifndef PARTITION_CPP_
#define PARTITION_CPP_

#include "Partition.h"
#include "QuickSort.cpp"
#include <iostream>
using namespace std;

template <class T, typename S>
bool Partition<T,S>::resize ()
{
    /*
       *** A resize method to increase the partition size in the memory if it is full 
       *** It indicates whehter it is full or not using current variable
       *** Then if it needs to increase its size it uses realloc    
    */
    if ( current == partition_size)
    {
        partition_size += increment;
        data = (S *) realloc (data,partition_size*sizeof(S));
        dataWrapper = (T **) realloc (dataWrapper,partition_size*sizeof(T *));
        memset (((char*) data)+(current*sizeof(S)),0,increment*sizeof(S));
        memset (((char*) dataWrapper)+(current*sizeof(T*)),0,increment*sizeof(T*));
        return true;
    }
    return false;
}
template <class T, typename S>
Partition<T,S>::Partition (FILE * f,uint64_t p_partition_size,uint64_t p_increment)
{
    //A partition constructor the us used to set all the data members to zeros and reserve place in memory for the pointers
    partition_size = p_partition_size;
    increment = p_increment;
    current = 0;
    output = f;
    data = (S *) calloc (partition_size,sizeof(S));
    dataWrapper = (T **) calloc (partition_size,sizeof(T *));
}
template <class T, typename S>
void Partition<T,S>::addItem(S * s){
	/*
	   *** addItem method used to add item in the data wrapper and the data arrays
	   *** Then it increments the current index whenever it adds an item
	   *** We need to lock and unlick using mutex so that all the threads can work in parallel without any conflicts
	   *** We resize the array sizes when needed
	*/
	mtx.lock();
	resize();
	data[current] = *s;
	dataWrapper[current]= new  T(&data[current]);
	current++;
	mtx.unlock();
}
template <class T, typename S>
void Partition<T,S>::sort()
{
    //This method is used to invoke quick sort method to sort all the partiions after populating them
    for ( uint64_t i = 0  ; i < partition_size ; i ++)
        dataWrapper[i] = new T (&data[i]);
    QuickSort <T> quickSort (dataWrapper); //creating a quick sort object
    quickSort.quickSort(0,current-1); //invoking the sort method against
}
template <class T, typename S>
void Partition<T,S>::dump()
{
    //Dump method to write the data from the memory to a file
    fwrite(data,sizeof(S),current,output);
}
template <class T, typename S>
Partition<T,S>::~Partition ()
{
   //Destructor used too free all the pointers used in the data members
    if ( data != NULL ) free(data);
    for ( uint64_t i = 0  ; i < partition_size ; i ++) delete (dataWrapper[i]);
    free(dataWrapper);
}
#endif
