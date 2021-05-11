# Chip8

**Important: This is work in progress and code will change very frequently. Don't expect it yet to work at all!**

## What is it?
Chip8 like the name suggests is a Chip8 interpreter / emulator.

## How to run it?

```bash
# Linux
git clone git@github.com:patrickwilmes/Chip8.git Chip8/
cd Chip8/
git submodule update --init --recursive
./vcpkg/vcpkg bootstrap-vcpkg.sh
./vcpkg/vcpkg install sdl2 sdl2-ttf sdl2-image
./vcpkg/vcpkg integrate install
mkdir build
cd build
cmake ..
make
./Interpreter/Chip8 <ROM>
```