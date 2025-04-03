#include "../s21_string.h"

char *s21_strrchr(const char *str, int c) {
  // Обработка аргументов функции и краевых ситуаций
  // if (str == S21_NULL) return S21_NULL;
  s21_size_t len = s21_strlen(str);
  if (len == 0) {
    return S21_NULL;
  }

  // Явное приведение типов для избежания неопределенного поведения
  unsigned char uc = (unsigned char)c;
  const char *result = S21_NULL;

  for (s21_size_t i = 0; i <= len; i++) {
    if (str[i] == uc) {
      result = (str + i);
    }
  }

  return (char *)result;
}