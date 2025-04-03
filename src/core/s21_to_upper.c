#include "../s21_string.h"

void *s21_to_upper(const char *str) {
  if (str == S21_NULL) {
    return S21_NULL;
  }
  s21_size_t len = s21_strlen(str);
  char *upper_str = malloc(len + 1);
  for (s21_size_t i = 0; i < len; i++) {
    if (str[i] >= 97 && str[i] <= 122)
      upper_str[i] = str[i] - 32;
    else
      upper_str[i] = str[i];
  }
  upper_str[len] = '\0';
  return upper_str;
}