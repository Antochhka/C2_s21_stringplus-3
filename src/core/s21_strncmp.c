#include "../s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  // Обработка аргументов функции и краевых ситуаций
  // if (str1 == S21_NULL && str2 == S21_NULL) return 0;  // Оба S21_NULL,
  // считаем их равными if (str1 == S21_NULL) return -1;  // str1 S21_NULL, str1
  // считается "меньше" str2 if (str2 == S21_NULL) return 1;  // str2 S21_NULL,
  // str1 считается "больше" str2

  int result = 0;

  for (s21_size_t i = 0; i < n && (str1[i] != '\0' || str2[i] != '\0'); i++) {
    if (str1[i] != str2[i]) {
      result = (str1[i] - str2[i]) /* > 0 ? 1 : -1*/;
      break;
    }
  }

  return result;
}