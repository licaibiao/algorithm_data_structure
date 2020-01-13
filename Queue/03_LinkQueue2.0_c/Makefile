
TARGET = test
CC	   = gcc
CFLAGS :=  -c -g
$(TARGET): LinkQueue.o main.o 
	$(CC) -o $(TARGET) LinkQueue.o main.o 
SeqQueue.o:LinkQueue.c LinkQueue.h 
	$(CC) $(CFLAGS) LinkQueue.c 
main.o:main.c LinkQueue.h
	$(CC) $(CFLAGS) main.c  
clean:
	$(RM) *.o $(TARGET)

	
