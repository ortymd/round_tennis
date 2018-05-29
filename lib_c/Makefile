CFLAGS = -std=c99 -ggdb3 -Wall

all: lib_send_receive.so

lib_send_receive.so: lib_send_receive.o
	$(CC) -shared -o $@ $^

lib_send_receive.o: lib_send_receive.c
	$(CC) -c $(CFLAGS) -fPIC -o $@ $^

clean:
	rm test lib_send_receive.so lib_send_receive.o  
