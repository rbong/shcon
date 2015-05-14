CC     = gcc
CFLAGS = -Wall -O3
INC    = -I. -I../include
DEPS   = shm.c str.c util.c err.c
SRCS   = $(DEPS:.h=.c) main.c
OBJS   = $(SRCS:.c=.o)
MAIN   = mm

all: $(MAIN)

%.o: %.c $(DEPS)
	$(CC) $(INC) -c -o $@ $< $(CFLAGS)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INC) -o $(MAIN) $(OBJS)

clean:
	rm -f $(OBJS) $(MAIN)
