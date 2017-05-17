CC := gcc
CFLAGS := -O2 -Wall -g
.PHONY: all clean
all: wsserver 
wsserver: wsserver.c
	$(CC) -o $@ $^ -lcrypto -lssl -lpthread
clean:
	@rm -f server 
