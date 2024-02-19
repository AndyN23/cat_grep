#ifndef S21_CAT_H_
#define S21_CAT_H_

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  bool numberNonBlank;
  bool markEnd;
  bool numberAll;
  bool squeeze;
  bool tab;
  bool nonPrintable;
} Flags;

#endif  // S21_CAT_H_