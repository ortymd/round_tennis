CFLAGS = -ggdb3 -Wall

test.so:    test.o
	$(CC) -shared -o $@ $^

test.o:     test.c
	$(CC) -c $(CFLAGS) -fPIC -o $@ $^

clean:
	rm test.o test.so
