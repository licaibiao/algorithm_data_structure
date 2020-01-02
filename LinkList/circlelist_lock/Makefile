TARGET = circlelist_lock
CC	   = g++
CFLAGS := -c -g 

$(TARGET): circlelist_lock.o circlelist_lock_main.o 
	$(CC) -o $(TARGET) circlelist_lock.o circlelist_lock_main.o 

circlelist_lock.o:circlelist_lock.cpp circlelist_lock.h 
	$(CC) $(CFLAGS) circlelist_lock.cpp 

circlelist_lock_main.o:circlelist_lock_main.cpp circlelist_lock.h
	$(CC) $(CFLAGS) circlelist_lock_main.cpp  

clean:
	$(RM) *.o $(TARGET)
