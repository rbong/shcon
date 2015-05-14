CC     = gcc
CFLAGS = -Wall -O3
INC    = -I. -I../src -I../include
DEPS   = ../src/err.h ../src/str.h test_error.h test_str.h
SRCS   = $(DEPS:.h=.c) test.c
OBJS   = $(SRCS:.c=.o)
MAIN   = test-mm

all: $(MAIN)

%.o: %.c $(DEPS)
	$(CC) $(INC) -c -o $@ $< $(CFLAGS)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INC) -o $(MAIN) $(OBJS)

clean:
	rm -f $(OBJS) $(MAIN)
