# Mandelbrocc

## Todo
- [x] autoexposure - normalize values
- [x] decouple unecessery SDL dependencies
- [x] multithread
- [ ] create basic poc for Web. Create a build script that will render the mandelbrot in a canvas
- [ ] screenshot functionality
- [ ] "render" functionality. Create an animation based on keyframes
- [ ] use arbitary precision arithmetic
- [ ] WebGPU

## Installation
For the desktop version you need to place a static raylib library in lib/raylib. The repo contains one compiled for M1 Pro Mac. Potentially you need to replace it for your machine.

## Run it
```
./build.sh && ./build/mandel
```
