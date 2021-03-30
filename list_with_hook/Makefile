
EXE := list
%.o: %.c
	$(CC) -c $< -o $@
SOURCE := $(wildcard *.c)
OBJS   := $(patsubst %.c,%.o,$(SOURCE))
$(EXE):$(OBJS)
	$(CC) -g -o ./$(EXE) $(OBJS)
	$(RM) -f *.o 
clean:
	$(RM) -f *.o $(EXE)



