TARGET = circlelist
CC	   = gcc
CFLAGS :=  -c -g
$(TARGET): circlelist.o circlelist_main.o 
	$(CC) -o $(TARGET) circlelist.o circlelist_main.o 
circlelist.o:circlelist.c circlelist.h 
	$(CC) $(CFLAGS) circlelist.c 
circlelist_main.o:circlelist_main.c circlelist.h
	$(CC) $(CFLAGS) circlelist_main.c  
clean:
	$(RM) *.o $(TARGET)
