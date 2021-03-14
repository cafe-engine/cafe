NAME = cafe
INCLUDE = -Iexternal/lua/src
SRC += $(wildcard external/lua/src/*.c)
MODULES = tea coffee mocha latte
LFLAGS = -lSDL2 -lm -ldl
export OBJ_DIR := $(PWD)/obj
