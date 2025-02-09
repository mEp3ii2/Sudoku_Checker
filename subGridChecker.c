#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.h"
#include "checker.h"
#include "subGridChecker.h"

/*  Creates arrays for subgrids
    Passes to checker method to valid grids
*/
void subChecker(int **arr, int sRow, int *validGrid)
{
    int i, j, g1, g2, g3;

    // arrays fro grids
    int grid1[ROWS];
    int grid2[ROWS];
    int grid3[ROWS];

    g1 = g2 = g3 = 0; // positions for each grid

    for (i = sRow; i < sRow + 3; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if (j < 3)
            { // col value less then 3 is the left grid
                grid1[g1] = arr[i][j];
                g1++;
            }
            else if (j < 6)
            { ////col value less then 6 is the middle grid
                grid2[g2] = arr[i][j];
                g2++;
            }
            else
            { // remainder is the right grid
                grid3[g3] = arr[i][j];
                g3++;
            }
        }
    }

    // check grids and assign results
    validGrid[0] = checker(grid1);
    validGrid[1] = checker(grid2);
    validGrid[2] = checker(grid3);
}