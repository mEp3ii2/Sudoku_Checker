#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include "constants.h"
#include "printer.h"
#include "driver.h"

/* NAME: setup
 * PURPOSE: setups the array from the provided file
 * IMPORTS: fileName, delay
 * EXPORTS: NONE
 * ASSERTIONS: validator function returned 0
 * REFERENCES: Adapted from UCP Assignment 2 2023 SEM 2
 */

void setup(char fileName[], int delay)
{
    int i;
    // assign memory for the sudoku sol
    int **sudokuArr = (int **)malloc(ROWS * sizeof(int *));

    for (int i = 0; i < ROWS; i++)
    {
        (sudokuArr)[i] = (int *)malloc(COLS * sizeof(int));
    }

    FILE *f = fopen(fileName, "r"); // open file

    if (f == NULL)
    {
        perror("Error: could not open file\n");
        return;
    }
    else
    { // read file
        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (fscanf(f, "%d", &sudokuArr[i][j]) != 1) // assign values to array
                {
                    fprintf(stderr, "Error reading file\n");
                    fclose(f);
                    return;
                }
            }
        }
        fclose(f); // file reading finished
    }

    display(sudokuArr);        // print sudoku sol
    driverM(sudokuArr, delay); // thread section

    // free memory
    for (i = 0; i < ROWS; i++)
    {
        free(sudokuArr[i]);
    }
    free(sudokuArr);
}