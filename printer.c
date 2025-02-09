#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "printer.h"
#include "constants.h"

// Prints out the sudoku solution
void display(int **arr)
{
    timer();
    int i, j;
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            printf(PINKH"%d "RESET_H, arr[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void timer(){
    system("clear");
    sleep(1);
    three();
    two();
    one();
}

void three(){
    printf("  3333333333333  \n");
    printf("            33  \n");
    printf("            33  \n");
    printf("            33  \n");
    printf("   3333333333   \n");
    printf("            33  \n");
    printf("            33  \n");
    printf("            33  \n");
    printf(" 3333333333333  \n");
    sleep(1);
    system("clear");
}

void two(){
    printf("  2222222222222  \n");
    printf(" 2            22 \n");
    printf("           22   \n");
    printf("         22     \n");
    printf("       22       \n");
    printf("     22         \n");
    printf("   22           \n");
    printf("  22            \n");
    printf(" 222222222222222\n");
    sleep(1);
    system("clear");
}

void one(){
    printf("   11111    \n");
    printf("      11    \n");
    printf("      11    \n");
    printf("      11    \n");
    printf("      11    \n");
    printf("      11    \n");
    printf("      11    \n");
    printf(" 1111111111  \n");
    sleep(1);
    system("clear");
}