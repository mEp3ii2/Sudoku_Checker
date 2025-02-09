#include <stdio.h>
#include <stdlib.h>
#include "validator.h"

/* NAME: validator
 * PURPOSE: validates the command line arguments
 * IMPORTS: argc, argv
 * EXPORTS: 1 for invalid command line params else 0
 * REFERENCES: Adapted from my UCP assignment 2 2023 SEM 2
 */

int validator(int argc, char *argv[])
{

    int result;
    result = 0;
    /* checks the amount of command line args*/
    if (argc < 3)
    {
        printf("Not enough command line args\n");
        exit(1);
    }
    else if (argc > 3)
    {
        printf("Warning! Too many command line args\n");
        exit(1);
    }
    if(checkDelay(argv[2])==1){
        exit(1);
    }
    return result;
}

//checks delay value
int checkDelay(char *delay){

    int num = atoi(delay);
    if(num <0){
        printf("Delay can't be a negative number\n");
        return 1;
    }
    else if(num ==0){//atoi returns zero on error so this serves this and also valid ints of zero for this block
        printf("Delay must be a valid integer number greater then zero\n");
        return 1;
    }
    else if(num>10){ // delay greater then allowed
        printf("Delay cannot be greater then 10\n");
        return 1;
    }
    return 0;
}