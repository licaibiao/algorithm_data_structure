
TARGET = test
CC	   = g++
CFLAGS :=  -c -g
$(TARGET): SeqQueue.o main.o 
	$(CC) -o $(TARGET) SeqQueue.o main.o 
SeqQueue.o:SeqQueue.cpp SeqQueue.h 
	$(CC) $(CFLAGS) SeqQueue.cpp 
main.o:main.cpp SeqQueue.h
	$(CC) $(CFLAGS) main.cpp  
clean:
	$(RM) *.o $(TARGET)

	
