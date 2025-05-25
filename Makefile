# CC=gcc
# CFLAGS=-Wall -g 
# BINARY=icsh

# all: icsh

# icsh: icsh.c
# 	$(CC) -o $(BINARY) $(CFLAGS) $<

# .PHONY: clean

# clean:
# 	rm -f $(BINARY)

CC = gcc
CFLAGS = -Wall -g
BINARY = icsh

SRCS = icsh.c built_in.c parser.c job.c globals.c executor.c command.c signal.c
OBJS = $(SRCS:.c=.o)

all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(BINARY) $(OBJS)