echo 'Building...'
rm -rf build/
mkdir build
clang -c src/complex.c -o build/complex.o `sdl2-config --cflags --libs`
clang -c src/application.c -o build/application.o `sdl2-config --cflags --libs`
clang -c src/main.c -o build/main.o `sdl2-config --cflags --libs`
clang build/complex.o build/application.o build/main.o -o build/mandel `sdl2-config --cflags --libs`
echo '\n\n'
time ./build/mandel
