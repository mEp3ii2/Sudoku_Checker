#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.h"
#include "colChecker.h"
#include "checker.h"

int *colCheck(int **arr)
{
    int i, j;
    int **cols = colGetter(arr);          // get arrays of columns
    int *valid = malloc(9 * sizeof(int)); // array of 0 and 1 for invalid or valid columns
    for (i = 0; i < ROWS; i++)
    {
        int check[9];
        for (j = 0; j < COLS; j++)
        {
            check[j] = cols[i][j]; // get a column
        }
        valid[i] = checker(check); // check column and get result
    }

    // free memory
    for (int i = 0; i < COLS; i++)
    {
        free(cols[i]);
    }

    free(cols);
    return valid; // return results for the columns
}

/*Rotate the rows into columns*/
int **colGetter(int **arr)
{
    int i, j;
    int **cols = (int **)malloc(COLS * sizeof(int *)); // new array of columns

    // allocate memory
    for (i = 0; i < COLS; i++)
    {
        cols[i] = (int *)malloc(ROWS * sizeof(int));
    }

    // iterate over solution array
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            cols[j][i] = arr[i][j]; // assign row col from solution to col row in cols
        }
    }

    return cols;
}
