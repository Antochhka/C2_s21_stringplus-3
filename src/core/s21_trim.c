#include "../s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  if (trim_chars == S21_NULL || src == S21_NULL) {
    return S21_NULL;
  }
  s21_size_t len_src = s21_strlen(src);
  s21_size_t len_trim_chars = s21_strlen(trim_chars);
  char *trim_src = (char *)malloc(len_src + 1);
  s21_size_t trim_src_i = 0;
  for (s21_size_t i = 0; i < len_src; i++) {
    int is_trim_char = 0;
    for (s21_size_t j = 0; j < len_trim_chars; j++) {
      if (trim_chars[j] == src[i]) is_trim_char = 1;
    }
    if (!is_trim_char) trim_src[trim_src_i++] = src[i];
  }
  trim_src[trim_src_i] = '\0';
  return trim_src;
}