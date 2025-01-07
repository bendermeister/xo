CFLAGS := -g -Wall -Wextra -pedantic -std=c99
CC := clang

build: main.c xo.c xo.h
	${CC} main.c xo.c ${CFLAGS}
