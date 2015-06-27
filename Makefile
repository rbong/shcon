# COMPILER VARIABLES
CC     = gcc
# FLAG VARIABLES
INC    = -I./src -I./include
DEF    = -D MM_HDR_VER=3
CFLAGS = -Wall -g $(INC) $(DEF)
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
