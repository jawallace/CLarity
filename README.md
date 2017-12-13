# CLarity
Project for EN.605.417.81 - Introduction to GPU Programming

# Building

## Dependencies
* cmake
* Qt5 (for demo) (sudo apt-get install qt5-default)

## Build Instructions
```
git submodule init
git submodule update
mkdir _build
cd _build
cmake ..
make
``` 

## Running Unit Tests
```
cd _build/test
./clarity_test_suite
```

# Running CLarity

## GUI
```
cd _build/demo
./clarity_demo
```
Hit 'Generate Terrain' and an image should appear.
The controls on the bottom can be used to look around the scene.

## Command Line Interface
```
cd _build/cli
./clarity_cli help
```
The usage instructions can be seen by running 
```
./clarity_cli help
```

Typical usage:
```
# generate a terrain map
./clarity_cli terrain 30.0 3 50 terrain.bin

# execute range mapping on the CPU
./clarity_cli range CPU terrain.bin 90.0 4000 15000.0 15000.0 3000.0 0.0 90.0 range.img.cpu.bin

# execute range mapping on an OpenCL platform
./clarity_cli range OpenCL terrain.bin 90.0 4000 15000.0 15000.0 3000.0 0.0 90.0 range.img.cpu.bin
```

