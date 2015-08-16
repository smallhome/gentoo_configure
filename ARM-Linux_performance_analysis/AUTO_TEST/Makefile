CC = arm-angstrom-linux-gnueabi-gcc

default: all

all:
	$(CC) auto.c -o auto 
	cd CPU-bound; $(CC) main.c -o matrix_mul
	cd IO-bound; $(CC) main.c -o io
	cd lmbench-next/AUTO_TEST/gnuplot_CTX; $(CC) get_data.c -o get_data
	cd line_size; $(CC) test_line.c -o test_line

clean:
	rm auto
	cd CPU-bound; rm matrix_mul
	cd IO-bound; rm io
	cd lmbench-next/AUTO_TEST/gnuplot_CTX; rm get_data
	cd line_size; rm test_line

