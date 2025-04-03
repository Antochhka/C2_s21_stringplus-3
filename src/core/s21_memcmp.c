#include "../s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  // Обработка аргументов функции и краевых ситуаций
  // if (str1 == S21_NULL && str2 == S21_NULL) return 0;  // Оба S21_NULL,
  // считаем их равными if (str1 == S21_NULL) return -1;  // str1 S21_NULL, str1
  // считается "меньше" str2 if (str2 == S21_NULL) return 1;  // str2 S21_NULL,
  // str1 считается "больше" str2

  // Явное приведение типов для избежания неопределенного поведения
  const unsigned char *s1 = (const unsigned char *)str1;
  const unsigned char *s2 = (const unsigned char *)str2;
  int result = 0;

  for (s21_size_t i = 0; i < n; i++) {
    if (s1[i] != s2[i]) {
      result = (s1[i] - s2[i]) > 0 ? 1 : -1;
      break;
    }
  }

  return result;
}