#include "s21_string.h"

char *s21_strdup(const char *str) {
  if (str == S21_NULL) {
    return S21_NULL;
  }

  s21_size_t len = s21_strlen(str) + 1;
  char *dup = (char *)malloc(len);

  if (dup != S21_NULL) {
    s21_strncpy(dup, str, len);
    dup[len - 1] = '\0';
  }

  return dup;
}
