NAME = cafe
INCLUDE = -Iexternal/lua/src
SRC += $(wildcard external/lua/src/*.c) $(wildcard src/lua/*.c)
MODULES = tea coffee mocha latte
CFLAGS = `sdl2-config --cflags`
LFLAGS +=`sdl2-config --libs`
LFLAGS += -lpthread -lm -ldl
export OBJ_DIR := $(PWD)/obj

export CC = clang
export CDEFS = -DCAFE_ENGINE
