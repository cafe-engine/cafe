NAME = cafe
INCLUDE = -Iexternal/lua/src
SRC += $(wildcard external/lua/src/*.c) $(wildcard src/lua/*.c)
MODULES = tea coffee mocha latte
CFLAGS = `sdl2-config --cflags`
ifeq ($(OS),Windows_NT)
    export CC = x86_64-w64-mingw32-gcc
    export AR = x86_64-w64-mingw32-ar
    LFLAGS += -Lexternal
    LFLAGS += -pthread -lm -mwindows -opengl -lSDL2
else
    LFLAGS +=`sdl2-config --libs`
    LFLAGS += -lpthread -lm -ldl -lGL
endif
export TEA_BACKEND = sdl
export OBJ_DIR := $(PWD)/obj

export CDEFS = -DCAFE_ENGINE
