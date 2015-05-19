CC     = gcc
CFLAGS = -Wall -O3
INC    = -I. -I../include
DEPS   = util.h str.h file.h mm.h shm.h err.h
SRCS   = $(DEPS:.h=.c)
OBJS   = $(SRCS:.c=.o)
MAIN   = mm

all: $(MAIN)

%.o: %.c $(DEPS)
	$(CC) $(INC) -c -o $@ $< $(CFLAGS)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INC) -o $(MAIN) $(OBJS)

clean:
	rm -f $(OBJS) $(MAIN)
