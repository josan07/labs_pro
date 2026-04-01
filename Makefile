CC = gcc
CFLAGS = -Wall -O3

OBJS = main.o database.o parser.o extra.o

EXEC = eliza

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

%.o: %.c eliza.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)