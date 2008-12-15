all: pipes threads

pipes: pipes.c
	gcc $? -o $@

threads: threads.c
	gcc -static $? -o $@ -L -ISDL -lm -lpthread

clean:
	rm threads pipes
