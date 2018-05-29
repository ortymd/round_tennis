CFLAGS = -std=c99 -ggdb3 -Wall
LDFLAGS = -L./
LDLIBS = -l:lib_send_receive.so -l:libpthread

#.PHONY: lib_send_receive.so test

all: lib_send_receive.so test 

lib_send_receive.so: lib_send_receive.o
	$(CC) -shared -o $@ $^

lib_send_receive.o: lib_send_receive.c
	$(CC) -c $(CFLAGS) -fPIC -o $@ $^

test: test.o
  $(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm test lib_send_receive.so lib_send_receive.o test.o
