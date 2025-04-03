#include "../s21_string.h"

void *s21_to_lower(const char *str) {
  if (str == S21_NULL) {
    return S21_NULL;
  }
  s21_size_t len = s21_strlen(str);
  char *lower_str = malloc(len + 1);
  for (s21_size_t i = 0; i < len; i++) {
    if (str[i] >= 65 && str[i] <= 90)
      lower_str[i] = str[i] + 32;
    else
      lower_str[i] = str[i];
  }
  lower_str[len] = '\0';
  return lower_str;
}