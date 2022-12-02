#ifndef TERASORT_CPP
#define TERASORT_CPP

#include "TeraSort.h"
#include<cmath>

template <class T, typename S>
TeraSort<T,S>::TeraSort (char * p_input_file_name,char * p_output_file_name,
                            uint16_t p_mappers, uint16_t p_reducers,uint64_t p_sample_size)
{
    /*
        *** TeraSort Constructor
	*** Open the input and the output file tha is pased in the argument
	*** Setting the data members to zeros
	*** Setting the pointers of the mmebrs to NULLs
	*** If the input file is opened successfully, calculate the size fo th efile
	*** Making mappers threads adn make them read from the file by dividing the records among the threads
	** Make a new partioner pointer
    */
    input = fopen (p_input_file_name,"rb");
    output = fopen (p_output_file_name,"wb");
    file_byte_size = 0 ;
    file_record_size = 0 ;
    mappers = p_mappers;
    reducers = p_reducers;
    sample_size = p_sample_size;
    map_engines = NULL;
    partitioner=NULL;
    if ( input != NULL && output != NULL)
    {
	fseek(input, 0 , SEEK_END);
	file_byte_size = ftell(input);
	rewind(input);
	file_record_size = file_byte_size / 100.0;
	map_engines = (Mapper <T,S> **) calloc(mappers,sizeof(Mapper <T,S> *));
	reduce_engines= (Reducer <T,S> **)calloc(reducers, sizeof(Reducer <T,S> *));
	int small_mappers = mappers - 1;
	partitioner = new Partitioner<T,S>(output, reducers , file_record_size/reducers);
       for (int i = 0; i < mappers-1; i++){
           map_engines[i] = new Mapper<TeraSortItem, teraitem_r> (input, i * floor(file_record_size * 1.0/mappers *1.0), floor(file_record_size * 1.0 / mappers * 1.0), sample_size, partitioner);
       }
       map_engines[small_mappers] = new Mapper<TeraSortItem, teraitem_r> (input, (small_mappers) * floor(file_record_size * 1.0 /mappers * 1.0),file_record_size * 1.0 - ((small_mappers)*floor(file_record_size * 1.0/mappers * 1.0)), sample_size, partitioner);
    }
}

template <class T, typename S>
void TeraSort<T,S>::execute()
{
    /*
        *** Method execute that starts the whole program
	*** First: Making the mappers threads to start their phase 1
	*** Second: Joining the threads 
	*** Third: Calculate the average of the cut points using those mappers
        *** Fourth: Making new mapper threads to start phase 2 	
	*** Fifth: Joining those thread to mek the main function wait for them
	*** Sixth: Calling the dump mehtod to write the data from the memory buffers to the files
    */
    printf ("Terasort Start Execute\n");
    for ( uint16_t i = 0 ; i  < mappers; i++)
    {
	cout << "start making a new thread number: " << i + 1 << endl;
        std::thread * th = new std::thread(Mapper<T,S>::start, map_engines[i]);    
        map_engines[i]->setThread(th);
    }
    for( uint16_t i=0 ;i < mappers; i++){
    	map_engines[i]->waitForRunToFinish();
    }
    partitioner->calcCutpointsAverage(mappers);
    for( uint16_t i=0;i< mappers; i++){
	std::thread * th = new std::thread(Mapper<T,S>::start, map_engines[i]);
	map_engines[i]->setThread(th);

    }
    for( uint16_t i=0 ;i < mappers; i++){
        map_engines[i]->waitForRunToFinish();

    }

    for ( uint16_t i = 0 ; i  < reducers; i++)
    {	reduce_engines[i]= new Reducer<T,S>(partitioner->getPartition(i));
        std::thread * th = new std::thread(Reducer<T,S>::start, reduce_engines[i]);
        reduce_engines[i]->setThread(th);
    }
    for( uint16_t i=0 ;i < reducers; i++){
        reduce_engines[i]->waitForRunToFinish();

    }

    for ( uint16_t i = 0 ; i  < reducers; i++)
        partitioner->getPartition(i)->dump();

}
template <class T, typename S>
TeraSort<T,S>::~TeraSort()
{
    //Destructor used to delete all the pointers in the data members
    if ( input != NULL) fclose (input);
    if ( output != NULL) fclose (output);
    if ( map_engines != NULL)
    {
        for ( uint16_t i = 0 ; i  < mappers; i++)
            delete (map_engines[i]);
        free(map_engines);
    }
    if ( partitioner != NULL ) delete(partitioner);
}

#endif
