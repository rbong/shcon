# COMPILER VARIABLES
CC     = gcc
# FLAG VARIABLES
INC    = -I./src -I./test -I./include
CFLAGS = -Wall -g $(INC)
# FILE VARIABLES
DEPS   = $(wildcard src/*.c) $(wildcard test/*.c)
OBJS   = $(DEPS:.c=.o)
# BINARY VARIABLES
BINS   = test/test-mm src/mm

all: $(BINS)

%.o: %.c $(DEPS)
	$(CC) $(INC) -c -o $@ $< $(CFLAGS)

$(BINS): $(OBJS)
	$(CC) $(CFLAGS) $(INC) $(MAIN) $(OBJS) -o $@

clean:
	rm -f $(OBJS) src/mm test/test-mm
