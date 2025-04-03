#include "../s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (str == S21_NULL || src == S21_NULL) return S21_NULL;
  s21_size_t len_src = s21_strlen(src);
  s21_size_t len_str = s21_strlen(str);
  char *insert_src = malloc(len_src + len_str + 1);
  int str_i = 0;
  int src_i = 0;
  for (s21_size_t i = 0; i < len_str + len_src; i++) {
    if (i < start_index || i >= start_index + len_str)
      insert_src[i] = src[src_i++];
    else
      insert_src[i] = str[str_i++];
  }
  insert_src[len_src + len_str] = '\0';
  return insert_src;
}