CFLAGS += `sdl2-config --cflags`
LFLAGS := -lm -mwindows -lmingw32 `sdl2-config --libs`

PREFIX = x86_64-w64-mingw32
OUT := $(NAME).exe

CC := $(PREFIX)-gcc
AR := $(PREFIX)-ar
