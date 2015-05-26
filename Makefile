# COMPILER VARIABLES
CC     = gcc
# FLAG VARIABLES
INC    = -I./src -I./test -I./include
CFLAGS = -Wall -g $(INC)
# FILE VARIABLES
DEPS   = $(wildcard src/*.c) $(wildcard test/*.c)
OBJS   = $(SRCS:.h=.o)
OBJS   = $(DEPS:.c=.o)

all: mm test-mm

%.o: %.c $(DEPS)
	$(CC) $(INC) -c -o $@ $< $(CFLAGS)

test-mm: $(DEPS)
	$(CC) $(CFLAGS) $(INC) -o testing/test-mm $(MAIN) $(OBJS)

mm: $(DEPS)
	$(CC) $(CFLAGS) $(INC) -o src/mm $(MAIN) $(OBJS)

clean:
	rm -f $(OBJS) src/mm test/test-mm
