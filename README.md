# Cafe

Cafe is tiny C engine with focus on portability.

Rhe engine is made of separated modules:

## Modules
- [Tea](https://github.com/cafe-engine/tea): Render module
- [Mocha](https://github.com/cafe-engine/mocha): Audio module
- [Coffee](https://github.com/cafe-engine/coffee): Lisp based language
- [Latte](https://github.com/cafe-engine/latte): Filesystem and Packaging modules

Each module can be used separately in any project.

`Cafe` uses this modules and SDL2 for input, window, OpenGL context, etc.

TODO:

- Milk: UI module

## Compile

For build, you need a C compiler, SDL2 and make.

Check the site for download SDL2 runtime binaries or the source code for [build it from yourself](https://www.libsdl.org/download-2.0.php)

### Linux

Just install SDL2 dev lib from your distro base repository (it is present in the most used distros).

On Debian based distros for example:
```
sudo apt install gcc make libsdl2-dev
```

### Windows

For compile on Windows, i recommend using [MSYS2](https://www.msys2.org), and install base-devel for make, MinGW for C compiler and the SDL2 libs:

```
pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-SDL2
```

### OSX

The C compiler and `make` you can install using `Command Line Tools` from Xcode.
I recommend install SDL2 using [Homebrew](https://brew.sh/index_pt-br):

```
brew install sdl2
```

### Emscripten

First you need to [setup the environment](https://emscripten.org/docs/getting_started/downloads.html) and do:
```
make TARGET=Web
```
