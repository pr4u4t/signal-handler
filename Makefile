########################################
##
## Makefile
## LINUX compilation
##
##############################################

CC = gcc
OBJ = utils.o handler.o thread.o catcher.o

# Flags
CFLAGS = -g -ggdb -Wall -pedantic -pthread
LDFLAGS = 
# Math library

LIBS = -lm -lpthread -pthread

# Includes.
INCLUDES=  -I.

# Compiling all

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDES)


all: $(OBJ)
	gcc $(LDFLAGS) handler.o utils.o -o sender $(LIBS)
	gcc $(LDFLAGS) thread.o utils.o catcher.o -o catcher $(LIBS)

# Clean obj files
clean:
	rm -f *.o; rm -f sender catcher

distclean: clean
	rm -f *.log
