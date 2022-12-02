#ifndef MAPPER_CPP
#define MAPPER_CPP

#include "Mapper.h"
#include "QuickSort.cpp"

template <class T, typename S>
void Mapper<T,S>::phase1()
{
    /*
         *** Load terasort data from the file system and load it in data structre (array called datawraper)
	 *** take a sample from the data 
	 *** create unified cut points to be used by the partitioner afterwards
    */
    dataWrapper = (T **) calloc (read_count,sizeof(T *));
    sample = (T **) calloc (sample_size,sizeof(T *));
    for ( uint64_t i = 0  ; i < read_count ; i ++){
        
	dataWrapper[i]=  new T (&data[i]); 
    }
   
	//Do your implementation here
	for (int i = 0; i < sample_size; i++){
		sample[i]=dataWrapper[rand()%read_count];
		
	}
	QuickSort <T>q(sample);
	q.quickSort(0 , sample_size-1);
	int numOfCutPoints= partitioner->getPartitionsCount()-1;
	for(int i=0;i< numOfCutPoints;i++){
		partitioner->addCutpoint(i, sample[(i+1)*(sample_size-1)/numOfCutPoints ]  );
	}
}
template <class T, typename S>
void Mapper<T,S>::phase2()
{
    /*
        *** Shuffle the data that is stored in the datawrapper among differenct partitions buffers
	*** This is done using the addItem method that is invoked against every partitioner pointer
    */
    printf ("Start Shuffling .....\n");
    for ( uint64_t i = 0 ; i  < read_count ; i ++)
        partitioner->addItem(dataWrapper[i]);    
    printf ("Finished Shuffling .....\n");
}



template <class T, typename S>
Mapper<T,S>::Mapper (FILE * f, 
                uint64_t p_start,
                uint64_t p_count,
                uint64_t p_sample_size,
                Partitioner<T,S> * p_partitioner)
{
    input = f;
    read_count = p_count; //The number of the data points that are going to be read
    read_start = p_start;// The index that the thread will start reading from
    sample_size = p_sample_size; //The size of the array that stores the sample data points
    dataWrapper = NULL;
    data = (S *) calloc (read_count,sizeof(S)); //reserving place in memory for the teraitems
    fseek(input,read_start*sizeof(S),SEEK_SET);
    uint64_t read_items = fread(data,sizeof(S),read_count,f);
    partitioner = p_partitioner;
    phase = PHASE1; //adjusting the flag to the first phase
}


template <class T, typename S>
void Mapper<T,S>::execute ()
{
   /*
       *** This function determines whether it is going to execute phase 1 or phase 2 of the thread that is invoked against
       *** It determines which phase according to a flag called phase
   */
    if ( phase == PHASE1) 
    {
        phase1();
        phase = PHASE2;
    }
    else if(phase == PHASE2) phase2();
}

template <class T, typename S>
void Mapper<T,S>::start (Mapper<T,S> * me)
{
    //starts the whole process of a mapper thread
    me->execute();
}

template <class T, typename S>
void Mapper<T,S>::setThread(std::thread * p_th)
{
    //Assing a certain thread to a certain pointer mapper
    th = p_th;
}

template <class T, typename S>
void Mapper<T,S>::waitForRunToFinish()
{
    //join the threads to make the main function wait until all the threads finish their work
    th->join();
}

template <class T, typename S>
Mapper<T,S>::~Mapper()
{
   /*
       *** Deleting the 2D array (datawrapper)
       *** Getting over the datawrapper pointers and start deleting them one by one 
       *** Then it deletes the whole array which is the pointer of the pointers (datawrapper)
   */

    if (dataWrapper != NULL)
    {
        for ( uint64_t i = 0  ; i < read_count ; i ++) delete (dataWrapper[i]);
        free(dataWrapper);
    }
    free(data);
    delete(th);
}

#endif
