#include "../s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  // Обработка аргументов функции и краевых ситуаций
  // if (dest == S21_NULL || src == S21_NULL) return S21_NULL;

  char *dest_start = dest;

  // Ищем конец строки, чтобы начать запись с него
  while (*dest != '\0') {
    dest++;
  }

  // Копируем данные до конца src, но не более n байт
  while (n-- > 0 && *src != '\0') {
    *dest++ = *src++;
  }

  *dest = '\0';

  return dest_start;
}