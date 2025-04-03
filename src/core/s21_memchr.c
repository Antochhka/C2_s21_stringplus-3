#include "../s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  // Обработка аргументов функции и краевых ситуаций
  // if (str == S21_NULL) return S21_NULL;

  // Явное приведение типов для избежания неопределенного поведения
  const unsigned char *s = (const unsigned char *)str;
  unsigned char uc = (unsigned char)c;
  void *result = S21_NULL;

  for (s21_size_t i = 0; i < n; i++) {
    if (s[i] == uc) {
      result = (void *)(s + i);
      break;
    }
  }

  return result;
}