#include "../s21_string.h"

char *s21_strchr(const char *str, int c) {
  // Обработка аргументов функции и краевых ситуаций
  // if (str == S21_NULL) return S21_NULL;

  // Явное приведение типов для избежания неопределенного поведения
  const unsigned char uc = (const unsigned char)c;
  const char *result = S21_NULL;

  while (*str != '\0') {
    if (*str == uc) {
      result = str;
      break;
    }

    str++;
  }

  if (*str == uc) {
    result = str;
  }

  return (char *)result;
}