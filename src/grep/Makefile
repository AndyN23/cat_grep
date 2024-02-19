CC = gcc
CFLAGS = -Wall -Wextra -Werror

.PHONY: all clean rebuild

all:  s21_grep

s21_grep: 
	$(CC) $(CFLAGS) s21_grep.c s21_grep.h -o s21_grep

rebuild:
	make clean
	make all

make clean: 
	rm -rf s21_grep