TARGET=bin/mandelbrot.out
CC=g++ -O3
SDL=`sdl2-config --cflags --libs`
LIBS=libs/mandelbrot.o $(SDL)

all: no_sse

libs_dir:
	mkdir -p libs

bin_dir:
	mkdir -p bin

mandelbrot_o: libs_dir src/mandelbrot.cpp include/mandelbrot.hpp
	$(CC) -c src/mandelbrot.cpp -o libs/mandelbrot.o $(SDL)

main_o: libs_dir src/main.cpp
	$(CC) -c src/main.cpp -o libs/main.o $(SDL)

no_sse: bin_dir mandelbrot_o main_o
	$(CC) libs/main.o -o $(TARGET) $(LIBS)

clean:
	rm ./bin/*.out
	rm ./libs/*.o
	cd ..
