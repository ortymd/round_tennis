CFLAGS = -std=c99 -ggdb3 -Wall -I./
LDFLAGS = -L./
LDLIBS = -lpthread -l:lib_send_receive.o

all: lib_send_receive.so test tests

lib_send_receive.so: lib_send_receive.o
	$(CC) -shared -o $@ $^

lib_send_receive.o: lib_send_receive.c
	$(CC) -c $(CFLAGS) -fPIC -o $@ $^

test: test.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

tests: 
	$(MAKE)	-C tests/

clean:
	rm test lib_send_receive.so lib_send_receive.o test.o
