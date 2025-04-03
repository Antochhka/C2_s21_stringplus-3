#include "../s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  // Обработка аргументов функции и краевых ситуаций
  // if (delim == S21_NULL) return S21_NULL;

  static char *saved_str;
  char *token;

  if (str != S21_NULL) {
    saved_str = str;
  }

  if (saved_str != S21_NULL) {
    // Пропускаем разделители в начале строки
    while (*saved_str && s21_strchr(delim, *saved_str)) {
      saved_str++;
    }

    if (*saved_str == '\0') {
      saved_str = S21_NULL;
    }
  }

  if (saved_str != S21_NULL) {
    token = saved_str;

    // Находим конец токена
    while (*saved_str && !s21_strchr(delim, *saved_str)) {
      saved_str++;
    }

    // Проверяем, осталась исходная строка или нет
    if (*saved_str) {
      *saved_str = '\0';
      saved_str++;
    } else {
      saved_str = S21_NULL;
    }
  } else {
    token = S21_NULL;
  }

  return token;
}