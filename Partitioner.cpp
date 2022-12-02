#ifndef PARTITIONER_CPP_
#define PARTITIONER_CPP_

#include "Partitioner.h"

template <class T, typename S>
Partitioner<T,S>::Partitioner (FILE * f,uint16_t p_partitions_count,uint64_t p_partition_expected_size)
{
    // A constructor used to declare all the data members of the partitioner
    partitions_count = p_partitions_count;
    partitions = (Partition<T,S> **) calloc (partitions_count,sizeof(Partition<T,S> *));
    for ( uint16_t i = 0 ; i < partitions_count ; i++)
        partitions[i] = new Partition <T,S> (f,p_partition_expected_size,1000);
    cutpoints = (T **) calloc (partitions_count-1,sizeof(T *));
    for ( uint16_t i = 0 ; i < partitions_count-1 ; i++)
        cutpoints[i] = new T();
}
template <class T, typename S>
void Partitioner<T,S>::addCutpoint(uint16_t index,T * t)
{	
	//method used to add cut point to the partitioner
	cutpoints[index]->addToKeyValue(t->key());
}

template <class T, typename S>
void Partitioner<T,S>::calcCutpointsAverage(uint16_t p_mappers_count)
{	
	//mehtod used to calculate the cut points average of all the mappers using for loop
	for(int i=0;i<partitions_count-1 ;i++)
		cutpoints[i]->keyValueAverage(p_mappers_count);		     
}

template <class T, typename S>
void Partitioner<T,S>::addItem(T * t)
{
    /*
       *** Method used to deterine where to add this item
       *** After it determines which partition, it invokes its addItem method
       *** This mechanism is made using partition_set flag
   */
    bool partition_set = false;
    for ( uint16_t i = 0 ; i < partitions_count-1 ; i++)
    {
        if (cutpoints[i]->getKeyValue() > t->key())
        {
            partition_set = true;
            partitions[i]->addItem(t->getTeraItem());
            break;
        }
    }
    if ( !partition_set) partitions[partitions_count-1]->addItem(t->getTeraItem());

}

template <class T, typename S>
uint16_t Partitioner<T,S>::getPartitionsCount()
{
    // get the the partitions number (Thea the user entered as reducers in the terminal zrguments)
    return partitions_count;
}

template <class T, typename S>
Partition<T,S> * Partitioner<T,S>::getPartition (uint16_t index)
{
    //get a partions from the partiions array depending on its index
    if ( index < partitions_count) return partitions[index];
    else return NULL;
}

template <class T, typename S>
Partitioner<T,S>::~Partitioner ()
{
    //desrtuctor used to delete the whole pointers in the data members
    for ( uint16_t i = 0 ; i < partitions_count ; i++)
        delete(partitions[i]);
    for ( uint16_t i = 0 ; i < partitions_count-1 ; i++)
        delete(cutpoints[i]);
    free(partitions);
    free(cutpoints);
}
#endif
