CC = gcc
CFLAGS = -Wall -pthread -g
OBJ = main.o setup.o validator.o printer.o driver.o checker.o bubbleSort.o subGridChecker.o colChecker.o 
EXEC = mssv

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(CFLAGS)

main.o : main.c setup.h validator.h
	$(CC) -c main.c $(CFLAGS)

setup.o : setup.c setup.h constants.h printer.h
	$(CC) -c setup.c $(CFLAGS)

validator.o: validator.c constants.h validator.h
	$(CC) -c validator.c $(CFLAGS)

printer.o : printer.c printer.h constants.h
	$(CC) -c printer.c $(CFLAGS)

driver.o : driver.c driver.h constants.h checker.h subGridChecker.h colChecker.h
	$(CC) -c driver.c $(CFLAGS)

checker.o: checker.c checker.h bubbleSort.h
	$(CC) -c checker.c $(CFLAGS)

bubbleSort.o: bubbleSort.c bubbleSort.h
	$(CC) -c bubbleSort.c $(CFLAGS)

subGridChecker.o: subGridChecker.c subGridChecker.h checker.h constants.h
	$(CC) -c subGridChecker.c $(CFLAGS)

colChecker.o: colChecker.c colChecker.h constants.h checker.h
	$(CC) -c colChecker.c $(CFLAGS)
	
clean:
	rm -f $(EXEC) $(OBJ)