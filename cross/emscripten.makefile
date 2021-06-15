CFLAGS := -Wall -s USE_SDL=2 --embed-file main.lua --shell-file index.html
LFLAGS := -lpthread -lm -ldl `sdl2-config --libs`
OUT := $(NAME).html
CLEAR_FILES := cafe.js cafe.wasm

CC := emcc
AR := emar
