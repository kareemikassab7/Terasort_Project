#ifndef QUICKSORT_CPP_
#define QUICKSORT_CPP_


#include "QuickSort.h"
#include"TeraSortItem.h"
#include<iostream>
#include<cmath> //This is included because we use absolute value function when we call insertion sort to make sure that the parameter is positive number

/* 
 
            *********         *********    
                   *** ALERT ***
            *********         *********  

*/


/*
 template<class T>
void QuickSort<T>::insertionSort ( int64_t list_size)
{
if ( list_size <= 1 ) return;
for ( int i = 1 ; i < list_size ; i ++)
{
TeraSortItem *tmp = teraSortItem[i];
int j = i;
for ( ; j > 0 && tmp < teraSortItem[j-1] ; j -- )
	teraSortItem[j] = teraSortItem[j-1];
teraSortItem[j]= tmp;
}
}
*/


template <class T>
QuickSort<T>::QuickSort(T ** p_teraSortItem)
{   
    //Quicksort constructor that is used to assing the teraSortItem array data member
    teraSortItem = p_teraSortItem;
}

/*
template <class T>
int64_t QuickSort<T>::partition( int64_t l, int64_t r)
{
    int i = l;
    int j = r;
    int pivot = teraSortItem[(i + j) / 2];
    int temp;

    while (i <= j)
    {
        while (teraSortItem[i] < pivot)
            i++;
        while (teraSortItem[j] > pivot)
            j--;
        if (i <= j)
        {
            temp = teraSortItem[i];
            teraSortItem[i] = teraSortItem[j];
            teraSortItem[j] = temp;
            i++;
            j--;
        }
    }
    if (j > l)
        quickSort( l, j);
    if (i < r)
        quickSort( i, r);
}	
	
	{
        int64_t pivot = teraSortItem[r]->key(); // pivot 
        int64_t i = (l - 1); // Index of smaller element 

        for (int64_t j = l; j <= r- 1; j++)
        {
                // If current element is smaller than or 
                // equal to pivot 
                if (teraSortItem[j]->key() <= pivot)
                {
                        i++; // increment index of smaller element 
                        teraSortItem[i]->swap(teraSortItem[j]);
                }
        }
        teraSortItem[i+1]->swap(teraSortItem[r]);
        return (i + 1);
}


	 {
	//Do your implementation here
	
	uint64_t center =(l+r)/2;
	if ( teraSortItem[center] < teraSortItem[l] )
	teraSortItem[center]->swap(teraSortItem[l]);
	if ( teraSortItem[r] < teraSortItem[l] )
	teraSortItem[r]->swap(teraSortItem[l]);
	if ( teraSortItem[r] < teraSortItem[center] )
	teraSortItem[r]->swap(teraSortItem[center]);
	teraSortItem[r-1]->swap(teraSortItem[center]);
	return teraSortItem[r-1]->key();
}
*/
template <class T>
void QuickSort<T>::quickSort( int64_t l, int64_t r)
{

    /*
       *** Assinging the most left and the most right elemtns in the array to i and j
       *** Getting the the average to take the pivot
       *** Making a loop to make group 1 on the left and group on the right
       *** Calling the quickSort recursively again to give another two arrays and so on
       *** The stopping case is when the left beocme more than the right
    */
    int64_t i = l;
    int64_t j = r;
    uint128_t pivot = teraSortItem[(i + j) / 2]->key();
    

    while (i <= j)
    {
        while (teraSortItem[i]->key() < pivot)
            i++;
        while (teraSortItem[j]->key() > pivot)
            j--;
        if (i <= j)
        {
             teraSortItem[i]->swap(teraSortItem[j]);
            i++;
            j--;
        }
    }
    if (j > l)
        quickSort( l, j);
    if (i < r)
        quickSort( i, r);
}

template <class T>
QuickSort<T>::~QuickSort()
{
    //Empty destructor    
}

#endif
