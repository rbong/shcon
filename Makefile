# COMPILER VARIABLES
CC     = gcc
# FLAG VARIABLES
INC    = -I./src -I./include
CFLAGS = -Wall -g $(INC)
# FILE VARIABLES
DEPS   = $(wildcard src/*.c)
OBJS   = $(DEPS:.c=.o)
# BINARY VARIABLES
BINS   = src/mm

all: $(BINS)

%.o: %.c $(DEPS)
	$(CC) $(INC) -c -o $@ $< $(CFLAGS)

$(BINS): $(OBJS)
	$(CC) $(CFLAGS) $(INC) $(MAIN) $(OBJS) -o $@

clean:
	rm -f $(OBJS) src/mm
