#include "../s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  // Обработка аргументов функции и краевых ситуаций
  // if (haystack == S21_NULL || needle == S21_NULL) return S21_NULL;

  int needle_len = s21_strlen(needle);
  int matches = 0;
  const char *result = S21_NULL;

  for (int i = 0; haystack[i] != '\0'; i++) {
    for (int j = 0; j < needle_len; j++) {
      if (haystack[i + j] == needle[j]) {
        matches++;
      } else {
        matches = 0;
        break;
      }
    }

    if (matches == needle_len) {
      result = (haystack + i);
      break;
    }
  }

  return (char *)result;
}