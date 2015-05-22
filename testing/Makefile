#todo- combine makefiles if it looks nice, do separate binaries
CC     = gcc
CFLAGS = -Wall -g
INC    = -I. -I../src -I../include
DEPS   = ../src/err.h ../src/str.h ../src/file.h ../src/ipc.h ../src/shm.h ../src/sem.h ../src/shcon.h test_err.h test_file.h test_str.h
OBJS   = $(DEPS:.h=.o)
MAIN   = test-mm

all: $(MAIN)

%.o: %.c $(DEPS)
	$(CC) $(INC) -c -o $@ $< $(CFLAGS)

$(MAIN): $(OBJS) test.o
	$(CC) $(CFLAGS) $(INC) -o $(MAIN) $(OBJS) test.o

test-cli: $(OBJS) test_cli.o
	$(CC) $(CFLAGS) $(INC) -o test-cli $(OBJS) test_cli.o

clean:
	rm -f $(OBJS) test.o test_cli.o $(MAIN)
