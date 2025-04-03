#include "../s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
  // Обработка аргументов функции и краевых ситуаций
  // if (str == S21_NULL) return S21_NULL;

  // Явное приведение типов для избежания неопределенного поведения
  unsigned char *s = (unsigned char *)str;
  const unsigned char uc = (const unsigned char)c;

  for (s21_size_t i = 0; i < n; i++) {
    s[i] = uc;
  }

  return str;
}