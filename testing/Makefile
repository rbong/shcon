#todo- combine makefiles if it looks nice
CC     = gcc
CFLAGS = -Wall -g
INC    = -I. -I../src -I../include
DEPS   = ../src/err.h ../src/str.h ../src/file.h *.h
SRCS   = $(DEPS:.h=.c)
OBJS   = $(SRCS:.c=.o)
MAIN   = test-mm

all: $(MAIN)

%.o: %.c $(DEPS)
	$(CC) $(INC) -c -o $@ $< $(CFLAGS)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INC) -o $(MAIN) $(OBJS)

clean:
	rm -f $(OBJS) $(MAIN)
