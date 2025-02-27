#include <stdbool.h>
#include <stdio.h>

// Adapted from geeksforgeeks.org

/*Swaps 2 ints*/
void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

/*Bubble Sort algorithm*/
void bubbleSort(int *arr, int n)
{
    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++)
    {
        swapped = false;
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false)
        {
            break;
        }
    }
}