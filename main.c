#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include "validator.h"

int main(int argc, char *argv[])
{
    /* check if valid paramters have been passed*/
    if (validator(argc, argv) != 0)
    {
        // print out if command args not valud
        printf("\nDesired input:\n");
        printf("./mssv <filename> <delay>\n");
        printf("\nGoodbye!\n");
    }
    else
    {
        // call setup method to begin app
        setup(argv[1], atoi(argv[2]));
    }
    return 0;
}