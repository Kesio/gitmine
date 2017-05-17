CC := gcc
CFLAGS := -O2 -Wall -g
.PHONY: all clean
all: server 
server: server.c
	$(CC) -o $@ $^ -lcrypto -lssl -lpthread
clean:
	@rm -f server 
