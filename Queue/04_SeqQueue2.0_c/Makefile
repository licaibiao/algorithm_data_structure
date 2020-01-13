
TARGET = test
CC	   = gcc
CFLAGS :=  -c -g
$(TARGET): SeqQueue.o main.o 
	$(CC) -o $(TARGET) SeqQueue.o main.o 
SeqQueue.o:SeqQueue.c SeqQueue.h 
	$(CC) $(CFLAGS) SeqQueue.c 
main.o:main.c SeqQueue.h
	$(CC) $(CFLAGS) main.c  
clean:
	$(RM) *.o $(TARGET)

	
