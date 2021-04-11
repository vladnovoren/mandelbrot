TARGET=bin/mandelbrot.out
CC=g++ -O3 -mavx2
SDL=`sdl2-config --cflags --libs`
CFLAGS=# `sdl2-config --flags` 
LIBS=libs/mandelbrot.o $(SDL)

mandelbrot_o: src/mandelbrot.cpp include/mandelbrot.hpp
	$(CC) -c src/mandelbrot.cpp -o libs/mandelbrot.o $(SDL)

main_o: src/main.cpp
	$(CC) -c src/main.cpp -o libs/main.o $(SDL)

avx2: mandelbrot_o main_o
	$(CC) libs/main.o -o $(TARGET) $(LIBS)
