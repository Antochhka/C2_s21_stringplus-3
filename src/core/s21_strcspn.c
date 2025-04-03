#include "../s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  // Обработка аргументов функции и краевых ситуаций
  // if (str1 == S21_NULL || str2 == S21_NULL) return 0;

  s21_size_t length = 0;
  int found = 0;

  for (; *(str1 + length) != '\0'; length++) {
    for (s21_size_t i = 0; str2[i] != '\0'; i++) {
      if (str1[length] == str2[i]) {
        found = 1;
        break;
      }
    }

    if (found) {
      break;
    }
  }

  return (found ? length : s21_strlen(str1));
}