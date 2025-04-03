#include "../s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  // Обработка аргументов функции и краевых ситуаций
  // if (src == S21_NULL || dest == S21_NULL) return S21_NULL;
  // if (are_overlapping(dest, src, n))
  // return S21_NULL;
  // если перекрываются области памяти

  // Явное приведение типов для избежания неопределенного поведения
  unsigned char *d = (unsigned char *)dest;
  const unsigned char *s = (const unsigned char *)src;

  for (s21_size_t i = 0; i < n; i++) {
    d[i] = s[i];
  }

  return dest;
}