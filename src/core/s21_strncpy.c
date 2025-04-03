#include "../s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  // Обработка аргументов функции и краевых ситуаций
  // if (src == S21_NULL || dest == S21_NULL) return S21_NULL;
  // if (are_overlapping(dest, src, n))
  // return S21_NULL;  // если перекрываются области памяти

  char *dest_start = dest;

  while (n > 0 && *src != '\0') {
    *dest++ = *src++;
    n--;
  }

  while (n > 0) {
    *dest++ = '\0';
    n--;
  }

  return dest_start;
}