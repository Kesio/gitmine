CC := gcc
CFLAGS := -O2 -Wall -g
.PHONY: all clean
all: chat_server chat_client demo
demo: demo.c
	$(CC) -o $@ $^ `pkg-config --cflags --libs gtk+-2.0`  
chat_server: chat_server.c
	$(CC) -o $@ -D_REENTRANT $^ -lpthread
chat_client: chat_client.c
	$(CC) -o $@ -D_REENTRANT $^ -lpthread
clean:
	@rm -f chat_server chat_client demo
