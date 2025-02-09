#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "constants.h"
#include <stdbool.h>
#include <unistd.h>
#include "checker.h"
#include "subGridChecker.h"
#include "colChecker.h"
#include "driver.h"

pthread_mutex_t mutex;
pthread_cond_t conditional;

/*
    Threads start here and recive the data and jobs
*/
void *threadGen(void *paramD)
{
    static int startNum = 0; // static so that it is init only once
    param *params = (param *)paramD;
    param *threadArg;
    // get job
    pthread_mutex_lock(&mutex);
    threadArg = &params[startNum];
    printf(CYAN "Thread %ld has been assigned Job: %d\n" RESET, (long)pthread_self(), threadArg->id);
    threadArg->threadID = (long)pthread_self(); // save the id to the job
    startNum++;
    sleep(params->delay); // delay for sync
    pthread_mutex_unlock(&mutex);

    if (threadArg->id <= 3) // if id 1-3 go to threadFunc1
    {
        threadFunction1(threadArg);
    }
    else
    {
        threadFunction2(threadArg);
    }
    return NULL;
}

/*
    check rows and sub grids
    executed by threads 1-3
*/

void *threadFunction1(param *arg)
{

    int i, j;
    param *params = arg;
    int validRow[3];
    int *validSub;
    validSub = malloc(3 * sizeof(int));
    int row[9];

    // check rows
    for (i = params->sRow; i < params->sRow + 3; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            row[j] = params->arr[i][j]; // create array of a row
        }
        // check row
        validRow[i % 3] = checker(row); // mod 3 so that the pos match the smaller array
        /*
            0,3,6=0
            1,4,7=1
            2,5,8=2
        */
    }

    // check Subgrids
    subChecker(params->arr, params->sRow, validSub); // validSub has the results

    // iterate over the results and modify the shared data to reflect that
    j = 0;
    for (i = params->sRow; i < params->sRow + 3; i++)
    {

        // update row critical section
        pthread_mutex_lock(&mutex);
        printf(RED "Job: %d- Thread %ld in the cs\n" RESET, params->id, (long)pthread_self());
        params->ROW[i] = validRow[j];

        pthread_mutex_unlock(&mutex);
        sleep(params->delay); // delay for sync

        // update Sub critical section
        pthread_mutex_lock(&mutex);
        printf(GREEN "Job: %d- Thread %ld in the cs\n" RESET, params->id, (long)pthread_self());
        params->SUB[i] = validSub[j];

        pthread_mutex_unlock(&mutex);
        sleep(params->delay); // delay for sync

        // increment COUNT for each valid row and grid
        if (validRow[j] == 1)
        {
            pthread_mutex_lock(&mutex);
            printf(YELLOW "Job: %d- Thread %ld in the cs\n" RESET, params->id, (long)pthread_self());
            (*params->COUNT)++;

            pthread_mutex_unlock(&mutex);
            sleep(params->delay); // delay for sync
        }
        if (validSub[j] == 1)
        {
            pthread_mutex_lock(&mutex);
            printf(YELLOW "Job: %d- Thread %ld in the cs\n" RESET, params->id, (long)pthread_self());
            (*params->COUNT)++;

            pthread_mutex_unlock(&mutex);
            sleep(params->delay); // delay for sync
        }
        j++;
    }

    /*
        increment exitNum
        if =4 this is last thread
        print out to state that
    */

    pthread_mutex_lock(&mutex);
    printf(BLUE "Job: %d- Thread %ld in the cs\n" RESET, params->id, (long)pthread_self());
    (*params->exitNum)++;
    if ((*params->exitNum) == 4)
    {
        printf(MAGENTA "Job: %d- Thread %ld is the last thread\n" RESET, params->id, (long)pthread_self());
        pthread_cond_signal(&conditional); // signal to parent that you are the last thread and have finished
    }
    pthread_mutex_unlock(&mutex);
    sleep(params->delay); // delay for sync

    free(validSub); // free memory
    return NULL;
}

/*
    job 4 checks the cols in the given solution
*/
void *threadFunction2(param *arg)
{
    // check cols
    int i;
    param *params = arg;

    int *validCols = colCheck(params->arr); // get column results

    for (i = 0; i < 9; i++)
    {
        params->COL[i] = validCols[i]; // no need for synch control as this is the only thread to access this

        if (validCols[i] == 1) // check if valid
        {
            // update CoUNT critical section
            pthread_mutex_lock(&mutex);
            printf(YELLOW "Job: %d- Thread %ld in the cs\n" RESET, params->id, (long)pthread_self());
            (*params->COUNT)++;
            pthread_mutex_unlock(&mutex);
            sleep(params->delay);
        }
    }

    /*
        increment exitNum
        if =4 this is last thread
        print out to state that
    */
    pthread_mutex_lock(&mutex);
    printf(BLUE "Job: %d- Thread %ld in the cs\n" RESET, params->id, (long)pthread_self());
    (*params->exitNum)++;
    if ((*params->exitNum) == 4)
    {
        printf(MAGENTA "Thread ID-%d is the last thread\n" RESET, params->id);
        pthread_cond_signal(&conditional); // signal to parent that you are the last thread
    }
    pthread_mutex_unlock(&mutex);
    sleep(params->delay); // delay for sync

    free(validCols); // free memory
    return NULL;
}

/*
    main controller of the application
    creates threads and then calls print methods to display results
*/
void driverM(int **sudoku, int time)
{
    int i;
    pthread_t threads[4]; // array of threads

    // shared data
    int **Sol = sudoku;
    int *Row = malloc(9 * sizeof(int));
    int *Sub = malloc(9 * sizeof(int));
    int *Col = malloc(9 * sizeof(int));
    int Counter = 0;
    int exitNum = 0;
    int delay = time;

    param params[4] = {{1, 0, 0, Sol, Row, Col, Sub, &Counter, delay, &exitNum},
                       {2, 3, 0, Sol, Row, Col, Sub, &Counter, delay, &exitNum},
                       {3, 6, 0, Sol, Row, Col, Sub, &Counter, delay, &exitNum},
                       {4, 0, 0, Sol, Row, Col, Sub, &Counter, delay, &exitNum}};
    pthread_mutex_init(&mutex, NULL);

    // create threads here
    for (i = 0; i < 4; i++)
    {
        // go to the generic thread method to start
        pthread_create(&threads[i], NULL, threadGen, (void *)params);
    }

    // wait for threads to fin
    pthread_mutex_lock(&mutex);
    while (exitNum != 4)
    {
        pthread_cond_wait(&conditional, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    printf("\n");

    // display results
    prepThreadType1(1, Row, Sub, 0, params[0].threadID);
    prepThreadType1(2, Row, Sub, 3, params[1].threadID);
    prepThreadType1(3, Row, Sub, 6, params[2].threadID);
    displayThreadResultType2(4, Col, 9, params[3].threadID);
    printf("There are in total %d valid rows, columns, and sub-grids, and the solution is", Counter);
    if (Counter == 27)
    {
        printf(GREENH " valid\n" RESET_H);
    }
    else
    {
        printf(REDH " invalid\n" RESET_H);
        invalid();
    }

    // cleanup
    free(Row);
    free(Col);
    free(Sub);

    for (i = 0; i < 4; i++)
    {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&conditional);
}

/*
    gets the invalid rows and grids and passed to the display method
*/
void prepThreadType1(int id, int *row, int *sub, int start, long threadID)
{
    int i, j;
    int countR = 0;
    int countS = 0;

    /*  see how many invalid rows there are so we can create an array
        of the correct length
    */
    for (i = start; i < start + 3; i++)
    {
        if (row[i] == 0)
        {
            countR++;
        }
    }
    int *invalidRow = (int *)malloc(countR * sizeof(int)); // create array for the invalid rows
    j = 0;
    for (i = start; i < start + 3; i++)
    {
        if (row[i] == 0)
        {
            invalidRow[j] = i; // assign the position of the invalid rows
            j++;
        }
    }

    /*
        see how many invalid grids there are so we can create an array
        of the correct length
    */
    for (i = start; i < start + 3; i++)
    {
        if (sub[i] == 0)
        {
            countS++;
        }
    }
    int *invalidSub = (int *)malloc(countS * sizeof(int)); // create array for the invalid grids
    j = 0;
    for (i = start; i < start + 3; i++)
    {
        if (sub[i] == 0)
        {

            invalidSub[j] = i; // assign the position of the invalid grids
            j++;
        }
    }

    displayThreadResultType1(id, invalidRow, countR, invalidSub, countS, threadID); // pass created invailid arrays to display method

    // free memorys
    free(invalidRow);
    free(invalidSub);
}

void displayThreadResultType1(int id, int *row, int rowLen, int *grid, int gridLen, long threadID)
{
    int i, j, count;
    count = 0;
    printf("JOB %d Thread ID-%ld: ", id, threadID);

    /*
        if the length of the invalid row and sub grid array are zero
        it means that the thread section is all valid
    */
    if (rowLen == 0 && gridLen == 0)
    {
        printf(GREEN "valid\n" RESET);
    }
    else
    { // invalid in either row or grid
        if (row != NULL)
        {
            for (i = 0; i < rowLen; i++)
            {
                if (count > 0)
                {
                    printf(", "); // comm to seperate for each invalid instance after the first
                }
                printf(RED "row %d " RESET, row[i] + 1); // print row index plus one to match indexing in sudoku
                count++;
            }
        }
        if (grid != NULL)
        {
            for (j = 0; j < gridLen; j++)
            {
                if (count > 0)
                {
                    printf(", "); // comm to seperate for each invalid instance after the first
                }
                printf(RED "sub-grid %d " RESET, grid[j] + 1); // print grid index plus one to match indexing in sudoku
                count++;
            }
        }
        if (i + j > 0)
        { // total invalid results greater then 1 ='are'
            printf(RED "are invalid\n" RESET);
        }
        else
        { // only one invalid entry then ='is'
            printf(RED "is invalid\n" RESET);
        }
    }
}

/*
    Iterate over column results
    finds index of invalid results
    and displays
*/
void displayThreadResultType2(int id, int *col, int colLen, long threadID)
{
    int i, count;
    count = 0;
    printf("JOB %d Thread ID-%ld: ", id, threadID);

    for (i = 0; i < colLen; i++)
    {
        if (col[i] == 0)
        {
            if (count > 0)
            {
                printf(", "); // comm to seperate for each invalid instance after the first
            }
            printf(RED "column %d " RESET, i + 1); // print col index plus one to match indexing in sudoku
            count++;
        }
    }

    if (count == 0)
    { // no invalid results
        printf(GREEN "valid\n" RESET);
    }
    else if (count == 1)
    { // 1 invalid result ='is'
        printf(RED "is invalid\n" RESET);
    }
    else
    { // multiple invalid results ='are'
        printf(RED "are invalid\n" RESET);
    }
}

void invalid()
{
    printf(GREEN "\n");
    printf("⢀⡴⠑⡄⠀⠀⠀⠀⠀⠀⠀⣀⣀⣤⣤⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n");
    printf("⠸⡇⠀⠿⡀⠀⠀⠀⣀⡴⢿⣿⣿⣿⣿⣿⣿⣷⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n");
    printf("⠀⠀⠀⠀⠑⢄⣠⠾⠁⣀⣄⡈⠙⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀ \n");
    printf("⠀⠀⠀⠀⢀⡀⠁⠀⠀⠈⠙⠛⠂⠈⣿⣿⣿⣿⣿⠿⡿⢿⣆⠀⠀⠀⠀⠀⠀⠀ \n");
    printf("⠀⠀⠀⢀⡾⣁⣀⠀⠴⠂⠙⣗⡀⠀⢻⣿⣿⠭⢤⣴⣦⣤⣹⠀⠀⠀⢀⢴⣶⣆ \n");
    printf("⠀⠀⢀⣾⣿⣿⣿⣷⣮⣽⣾⣿⣥⣴⣿⣿⡿⢂⠔⢚⡿⢿⣿⣦⣴⣾⠁⠸⣼⡿ \n");
    printf("⠀⢀⡞⠁⠙⠻⠿⠟⠉⠀⠛⢹⣿⣿⣿⣿⣿⣌⢤⣼⣿⣾⣿⡟⠉⠀⠀⠀⠀⠀ \n");
    printf("⠀⣾⣷⣶⠇⠀⠀⣤⣄⣀⡀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ \n");
    printf("⠀⠉⠈⠉⠀⠀⢦⡈⢻⣿⣿⣿⣶⣶⣶⣶⣤⣽⡹⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ \n");
    printf("⠀⠀⠀⠀⠀⠀⠉⠲⣽⡻⢿⣿⣿⣿⣿⣿⣿⣷⣜⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ \n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣷⣶⣮⣭⣽⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀ \n");
    printf("⠀⠀⠀⠀⠀⠀⣀⣀⣈⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀ \n");
    printf("⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀ \n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀ \n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⠿⠿⠿⠿⠛⠉\n" RESET);
    // Source for Asci art https://www.twitchquotes.com/copypastas/2781
}