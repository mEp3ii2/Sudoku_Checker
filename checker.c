#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "checker.h"
#include "bubbleSort.h"

/*  Sorts array first then checks if the elemnts
    are ints from 1-9
    returns zero if invalid else 1
*/
int checker(int arr[])
{
    int i, val;
    val = 1;
    bubbleSort(arr, 9); // sort array

    for (i = 0; i < 9; i++)
    { // iterate over array
        if (arr[i] != i + 1)
        {            // check if element is the expected number
            val = 0; // return zero if not a match
            break;
        }
    }
    return val;
}
