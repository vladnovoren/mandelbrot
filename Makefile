TARGET=bin/mandelbrot.out
CC=g++ -O3 -mavx2
SDL=`sdl2-config --cflags --libs`
LIBS=libs/mandelbrot.o $(SDL)

all: avx2

mandelbrot_o: src/mandelbrot.cpp include/mandelbrot.hpp
	$(CC) -c src/mandelbrot.cpp -o libs/mandelbrot.o $(SDL)

main_o: src/main.cpp
	$(CC) -c src/main.cpp -o libs/main.o $(SDL)

avx2: mandelbrot_o main_o
	mkdir -p bin
	$(CC) libs/main.o -o $(TARGET) $(LIBS)
