#include "s21_cat.h"

void print_file(FILE *file);
void number_full_strings(FILE *file);
void number_all_strings(FILE *file);
void mark_end(FILE *file, bool flag);
void change_tabs(FILE *file, bool flag);
void squeeze_strings(FILE *file);
void non_printable(int symbol);

Flags CatFlags(int argc, char *argv[]) {
  static struct option longOptions[] = {{"number-nonblank", 0, 0, 'b'},
                                        {"number", 0, 0, 'n'},
                                        {"squeeze-blank", 0, 0, 's'},
                                        {0, 0, 0, 0}};

  int currentFlag = getopt_long(argc, argv, "benvEstT", longOptions, 0);
  Flags flags = {false, false, false, false, false, false};

  for (; currentFlag != -1;
       currentFlag = getopt_long(argc, argv, "benvEstT", longOptions, 0)) {
    switch (currentFlag) {
      case 'b':
        flags.numberNonBlank = true;
        break;
      case 'e':
        flags.markEnd = true;
        flags.nonPrintable = true;
        break;
      case 'n':
        flags.numberAll = true;
        break;
      case 's':
        flags.squeeze = true;
        break;
      case 't':
        flags.tab = true;
        flags.nonPrintable = true;
        break;
      case 'E':
        flags.markEnd = true;
        break;
      case 'v':
        flags.nonPrintable = true;
        break;
      case 'T':
        flags.tab = true;
        break;
      default:
        printf("illegal option");
        break;
    }
  }
  return flags;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    while (1) {
      int chr = getchar();
      if (chr == EOF) {
        break;
      }
      putchar(chr);
    }
  }
  Flags flags = CatFlags(argc, argv);
  for (int i = optind; i < argc; i++) {
    FILE *file = fopen(argv[i], "rb");
    if (file != NULL) {
      if (flags.numberNonBlank) {
        number_full_strings(file);
      } else if (flags.markEnd) {
        mark_end(file, flags.nonPrintable);
      } else if (flags.numberAll) {
        number_all_strings(file);
      } else if (flags.squeeze) {
        squeeze_strings(file);
      } else if (flags.tab) {
        change_tabs(file, flags.nonPrintable);
      } else {
        print_file(file);
      }
    } else {
      printf("cat: %s : No such file or directory\n", argv[i]);
    }
    fclose(file);
  }
  return 0;
}

void print_file(FILE *file) {
  int c = fgetc(file);
  while (c != EOF) {
    putc(c, stdout);
    c = fgetc(file);
  }
}

void number_full_strings(FILE *file) {
  int symbol;
  int count = 0;
  bool empty = true;
  while ((symbol = fgetc(file)) != EOF) {
    if (symbol == '\n') {
      putchar('\n');
      symbol = fgetc(file);
      if (symbol != EOF && symbol != '\n') {
        empty = false;
        count++;
        printf("%6d\t", count);
      }
      ungetc(symbol, file);
    } else {
      if (empty) {
        count++;
        printf("%6d\t", count);
        empty = false;
      }
      putchar(symbol);
    }
  }
}

void number_all_strings(FILE *file) {
  int count = 1;
  int symbol;
  printf("%6d\t", count);
  while ((symbol = fgetc(file)) != EOF) {
    if (symbol == '\n') {
      count++;
      putchar('\n');
      printf("%6d\t", count);
    } else {
      putchar(symbol);
    }
  }
}

void mark_end(FILE *file, bool flag) {
  int symbol;
  while ((symbol = fgetc(file)) != EOF) {
    if (flag == true) {
      non_printable(symbol);
    }
    if (flag == false) {
      if (symbol != '\0' && symbol != '\n') {
        putchar(symbol);
      } else {
        printf("%c%c", '$', symbol);
      }
    }
  }
}

void change_tabs(FILE *file, bool flags) {
  int symbol;
  while ((symbol = fgetc(file)) != EOF) {
    if (flags == true) {
      if (symbol != '\n' && symbol != '\t') {
        if ((symbol >= 0 && symbol <= 8) || (symbol >= 11 && symbol <= 31)) {
          printf("^%c", symbol + 64);
        } else if (symbol >= 128 && symbol <= 159) {
          printf("M-^%c", symbol - 64);
        } else if (symbol == 127) {
          printf("^?");
        } else {
          printf("%c", symbol);
        }
      } else {
        if (symbol != '\t') {
          putchar(symbol);
        } else {
          printf("%s", "^I");
        }
      }
    }
    if (flags == false) {
      if (symbol != '\t') {
        putchar(symbol);
      } else {
        printf("%s", "^I");
      }
    }
  }
}

void squeeze_strings(FILE *file) {
  int symbol;
  int count = 1;
  while ((symbol = fgetc(file)) != EOF) {
    if (symbol == '\n') {
      count++;
    } else {
      count = 0;
    }
    if (count < 3) {
      putchar(symbol);
    }
  }
}

void non_printable(int symbol) {
  if (symbol != '\n' && symbol != '\t') {
    if ((symbol >= 0 && symbol <= 8) || (symbol >= 11 && symbol <= 31)) {
      printf("^%c", symbol + 64);
    } else if (symbol >= 128 && symbol <= 159) {
      printf("M-^%c", symbol - 64);
    } else if (symbol == 127) {
      printf("^?");
    } else {
      printf("%c", symbol);
    }
  } else {
    if (symbol != '\n') {
      putchar(symbol);
    } else {
      printf("%c%c", '$', symbol);
    }
  }
}