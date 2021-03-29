NAME = cafe
INCLUDE = -Iexternal/lua/src
SRC += $(wildcard external/lua/src/*.c) $(wildcard src/lua/*.c)
MODULES = tea coffee mocha latte
CFLAGS = `sdl2-config --cflags`
ifeq ($(OS),Window_NT)
    LFLAGS += -Lexternal
    LFLAGS += -pthread -lm -mwindow -opengl
else
    LFLAGS +=`sdl2-config --libs`
    LFLAGS += -lpthread -lm -ldl -lGL
endif
export TEA_BACKEND = sdl
export OBJ_DIR := $(PWD)/obj

export CDEFS = -DCAFE_ENGINE
