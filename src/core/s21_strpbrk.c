#include "../s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  // Обработка аргументов функции и краевых ситуаций
  // if (str1 == S21_NULL || str2 == S21_NULL) return S21_NULL;

  const char *result = S21_NULL;
  int found = 0;

  for (int i = 0; *(str1 + i) != '\0'; i++) {
    for (int j = 0; str2[j] != '\0'; j++) {
      if (str1[i] == str2[j]) {
        result = (str1 + i);
        found = 1;
        break;
      }
    }

    if (found) {
      break;
    }
  }

  return (char *)result;
}