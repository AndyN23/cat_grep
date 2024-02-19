#ifndef S21_GREP_
#define S21_GREP_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 10000

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
  int f;
  int c_flag;
  char str[BUFFER];
  char str_templ_o[BUFFER];
  char filename[BUFFER];
  char str_patterns[BUFFER];
} flags;

void parse_params(int argc, char *argv[], flags *flag);
void output(char *argv[], flags *flag);
void flag_f(flags *flag);

#endif