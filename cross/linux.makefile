CFLAGS += `sdl2-config --cflags`
LFLAGS := -lpthread -lm -ldl `sdl2-config --libs`

CC := cc
AR := ar
