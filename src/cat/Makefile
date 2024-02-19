CC = gcc
CFLAGS = -Wall -Wextra -Werror

.PHONY: all clean rebuild

all: s21_cat

s21_cat: s21_cat.c s21_cat.h
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

rebuild:
	make clean
	make all
	
clean: 
	rm -rf s21_cat
	rm -rf s21_cat.log
	rm -rf cat.log
