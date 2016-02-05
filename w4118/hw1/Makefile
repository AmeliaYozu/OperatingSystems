CC := gcc
LD := gcc

OBJECTS :=launchx.c
EXE := launchx

all : $(OBJECTS)
	$(CC) -o $(EXE) $(OBJECTS)

%.o: %.c
	$(CC)  -c $^

clean:
	rm -f &(EXE)

.PHONY: clean
