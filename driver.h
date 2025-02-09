#ifndef DRIVER_H
#define DRIVER_H
void *threadGen(void* paramD);
void *threadFunction1(param* arg);
void *threadFunction2(param* arg);
void driverM(int ** Sol,int delay);
void prepThreadType1(int id, int *row,int *sub,int start,long threadID);
void displayThreadResultType1(int id, int* row, int rowLen,int* grid,int gridLen,long threadID);
void displayThreadResultType2(int id, int* col, int colLen,long threadID);
void invalid();
#endif