#include "s21_grep.h"

int main(int argc, char *argv[]) {
  flags flag = {0};
  parse_params(argc, argv, &flag);
  while (optind < argc) {
    output(argv, &flag);
    optind++;
  }
  return 0;
}

void parse_params(int argc, char *argv[], flags *flag) {
  int opt;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0)) != -1) {
    switch (opt) {
      case 'e':
        flag->e = 1;
        strcat(flag->str_patterns, optarg);
        strcat(flag->str_patterns, "|");
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'o':
        flag->o = 1;
        break;
      case 'f':
        flag->f = 1;
        strcpy(flag->filename, optarg);
        flag_f(flag);
        break;
      default:
        break;
    }
    if (flag->v && flag->o) {
      flag->o = 0;
    }
  }
  if (!flag->e && !flag->f) {
    if (argc > optind) {
      strcat(flag->str_patterns, argv[optind]);
    }
    optind++;
  }
  if (flag->e || flag->f) {
    flag->str_patterns[strlen(flag->str_patterns) - 1] = '\0';
  }
  if (argc - optind > 1) flag->c_flag = 1;
}

void output(char *argv[], flags *flag) {
  FILE *file;
  regex_t reg;
  regmatch_t start;
  int counter = 0;
  int str_number = 0;
  int flag_i = REG_EXTENDED;
  if (flag->i) {
    flag_i = REG_EXTENDED | REG_ICASE;
  }
  regcomp(&reg, flag->str_patterns, flag_i);
  file = fopen(argv[optind], "rb");
  if (file != NULL) {
    while (fgets(flag->str, BUFFER, file) != NULL) {
      int match = regexec(&reg, flag->str, 1, &start, 0);
      str_number++;
      if (flag->o) strcpy(flag->str_templ_o, flag->str);
      if ((!match || flag->v) && flag->c_flag && !flag->l && !flag->h &&
          !flag->c && !flag->f)
        printf("%s:", argv[optind]);
      if (!match) counter++;
      if (flag->v) match = !match;
      if (!match && !flag->l && !flag->c && !flag->n && !flag->o) {
        printf("%s", flag->str);
        if (flag->str[strlen(flag->str) - 1] != '\n') printf("\n");
      }
      if (!match && flag->n && !flag->c && !flag->l) {
        if (flag->o) {
          printf("%d:", str_number);
        } else
          printf("%d:%s", str_number, flag->str);
      }
      if (!match && flag->o && !flag->l && !flag->c) {
        char *pointer = flag->str_templ_o;
        while ((regexec(&reg, pointer, 1, &start, 0) == 0)) {
          printf("%.*s\n", (int)(start.rm_eo - start.rm_so),
                 pointer + start.rm_so);
          pointer += start.rm_eo;
        }
      }
    }
    regfree(&reg);
    if (flag->l && counter < 1 && flag->v) {
      printf("%s\n", argv[optind]);
    }
    if (flag->l && counter > 0 && !flag->c) printf("%s\n", argv[optind]);
    if (flag->c && flag->c_flag && !flag->h) printf("%s:", argv[optind]);
    if (flag->c && !flag->l && !flag->v) printf("%d\n", counter);
    if (flag->c && !flag->l && flag->v) printf("%d\n", str_number = counter);
    if (flag->c && flag->l) {
      if (counter > 0) {
        counter = 1;
        printf("%d\n%s\n", counter, argv[optind]);
      } else
        printf("%d\n", counter);
    }
    fclose(file);
  } else {
    regfree(&reg);
    if (!flag->s)
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
  }
}
void flag_f(flags *flag) {
  FILE *f;
  f = fopen(flag->filename, "rb");
  if (f != NULL) {
    while (!feof(f)) {
      if (fgets(flag->str, 1000, f) != NULL) {
        if (flag->str[strlen(flag->str) - 1] == '\n' &&
            strlen(flag->str) - 1 != 0)
          flag->str[strlen(flag->str) - 1] = '\0';
        strcat(flag->str_patterns, flag->str);
        strcat(flag->str_patterns, "|");
      }
    }
    fclose(f);
  }
}