#!/bin/sh

set -xe
rm -rf build
mkdir build

# List of source files
# SOURCES="complex.c event_loop.c frontend.c mandelbrot.c rendering.c sdl_frontend.c util.c"
SOURCES="complex.c frontend.c mandelbrot.c rendering.c util.c main.c"
OBJECTS=$(echo $SOURCES | sed 's/\.c/\.o/g')

# Compile each source file
for SOURCE in $SOURCES; do
    echo "Compiling $SOURCE"
    time clang -c "src/$SOURCE" -o $(echo $SOURCE | sed 's/\.c/\.o/')
done

# clang -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL src/main.c src/raylib/src/libraylib.a -o build/mandel

# Link all object files into a single executable
echo "Linking..."
clang lib/raylib/libraylib.a $OBJECTS -o build/mandel -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

# Clean up the object files
echo "Cleaning up..."
rm -f $OBJECTS
