#include "../s21_string.h"

s21_size_t s21_strlen(const char *str) {
  // Обработка аргументов функции и краевых ситуаций
  // if (str == S21_NULL) return 0;

  s21_size_t length = 0;

  for (; *(str + length) != '\0'; length++);

  return length;
}