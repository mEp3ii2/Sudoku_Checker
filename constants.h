#ifndef CONSTANTS_H
#define CONSTANTS_H
#define ROWS 9
#define COLS 9

#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define YELLOW  "\x1B[33m"
#define BLUE    "\x1B[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET   "\x1B[0m"

#define REDH "\033[1;31m"
#define GREENH "\033[1;32m" 
#define PINKH "\033[1;35m"
#define RESET_H "\033[0m"

//struct of data that is given to the thread
typedef struct {
    int id;
    int sRow;
    int sCol;
    int **arr;
    int *ROW; // shared data
    int *COL; // shared data
    int *SUB; // shared data
    int *COUNT; // shared data
    int delay; // shared data
    int *exitNum; // shared data
    long threadID;
} param;

#endif