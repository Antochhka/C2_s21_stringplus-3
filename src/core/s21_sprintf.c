#include "../s21_string.h"

#define CHECK_STRING_OVERFLOW(str_index, max_size)                       \
  if (*str_index >= max_size) {                                          \
    fprintf(stderr, "Error: string overflow at index %d\n", *str_index); \
    return;                                                              \
  }

void test_format_output(TokenType *tokens, int index_f, char *result,
                        const char *format, ...) {
  va_list args;
  va_start(args, format);
  format_output(tokens, index_f, result, args, format);
  va_end(args);
}
void rounding(long double fractional_part, int *len, char *string,
              int *str_index, int precision) {
  // Преобразуем дробную часть
  // Округление до указанной точности
  double factor = pow(10, precision);
  fractional_part = fractional_part * factor;  // Округляем всё число сразу
  if (fractional_part - (int)fractional_part > 0.5) {
    fractional_part = round(fractional_part);
  }
  factor /= 10;  // Уменьшаем множитель
  for (int i = 0; i < precision; i++) {
    int digit = (int)(fractional_part / factor);
    string[(*str_index)++] = '0' + digit;
    fractional_part -= digit * factor;
    factor /= 10;  // Уменьшаем множитель
    (*len)++;
  }
}
void flag_in_variable(long long int number, int len, char *str, int *str_index,
                      int width, TokenType token) {
  char ch = ' ';
  if (s21_strchr(token.flags, '0')) {
    ch = '0';
    if (number < 0 || s21_strchr(token.flags, ' ') ||
        s21_strchr(token.flags, '+')) {
      len++;
    }
    while (len < width) {
      str[(*str_index)++] = ch;
      len++;
    }
  }
  if (s21_strchr(token.flags, ' ')) {
    str[(*str_index)++] = ' ';
    len++;
  }
  if (number < 0) {
    if (s21_strchr(token.flags, ' ')) {
      str[*str_index - 1] = '-';
    } else {
      str[(*str_index)++] = '-';
      len++;
    }
  }
  if (s21_strchr(token.flags, '+') && number >= 0) {
    if (s21_strchr(token.flags, ' ')) {
      str[(*str_index) - 1] = '+';
    } else {
      str[(*str_index)++] = '+';
      len++;
    }
  }
  if (s21_strchr(token.flags, '-') == NULL) {
    while (len < width) {
      str[(*str_index)++] = ch;
      len++;
    }
    str[*str_index] = '\0';
    reverse_str(str);
  } else {
    str[*str_index] = '\0';
    reverse_str(str);
    while (len < width) {
      str[(*str_index)++] = ' ';
      len++;
    }
    str[*str_index] = '\0';
  }
}

void flag_in_float(long double f_num, int *len, char *str, int *str_index,
                   int *width, int *precision, TokenType token) {
  if (*precision != 0) {
    if (*precision == -1) {
      *precision = 6;
    }
    (*len)++;
  }
  char ch = ' ';
  if (f_num < 0 || s21_strchr(token.flags, ' ') ||
      s21_strchr(token.flags, '+')) {
    (*len)++;
  }

  if (!s21_strchr(token.flags, '-') && !s21_strchr(token.flags, '0')) {
    while (*len < *width - *precision) {
      str[(*str_index)++] = ch;
      (*len)++;
    }
  }
  if (s21_strchr(token.flags, ' ')) {
    str[(*str_index)++] = ' ';
  }
  if (f_num < 0) {
    if (s21_strchr(token.flags, ' ')) {
      str[*str_index - 1] = '-';
    } else {
      str[(*str_index)++] = '-';
    }
  }
  if (s21_strchr(token.flags, '+') && f_num >= 0) {
    if (s21_strchr(token.flags, ' ')) {
      str[(*str_index) - 1] = '+';
    } else {
      str[(*str_index)++] = '+';
    }
  }
  if (s21_strchr(token.flags, '0')) {
    ch = '0';
    while (*len < *width - *precision) {
      str[(*str_index)++] = ch;
      (*len)++;
    }
  }
}
void expon(char *string, int *str_index, char ch, char *temp, int exponent,
           int width, TokenType token) {
  int len = 1;
  int i = 1;
  if (s21_strchr(token.flags, '-')) {
    string[(*str_index)++] = temp[0];
    while (temp[i] != '\0') {
      if (temp[i] != ' ') {
        string[(*str_index)++] = temp[i];
        len++;
      }
      i++;
    }
  } else {
    i = 0;
    while (temp[i] != '\0') {
      string[(*str_index)++] = temp[i];
      len++;
      i++;
    }
  }

  // Добавляем экспоненциальную часть
  string[(*str_index)++] = ch;
  string[(*str_index)++] = (exponent >= 0) ? '+' : '-';

  // Модуль экспоненты
  if (exponent < 0) exponent = -exponent;

  // Экспонента в виде двух цифр
  if (exponent >= 10) {
    string[(*str_index)++] = '0' + (exponent / 10);
  } else {
    string[(*str_index)++] = '0';
  }
  string[(*str_index)++] = '0' + (exponent % 10);
  while (len < width) {
    string[(*str_index)++] = ' ';
    len++;
  }
}
int length_number(int number) {
  int count = 0;
  int sign = number;
  while (sign > 0) {
    count++;
    sign /= 10;
  }
  return count;
}

void reverse_str(char *string) {
  int i, j;
  char c;

  for (i = 0, j = s21_strlen(string) - 1; i < j; i++, j--) {
    c = string[i];
    string[i] = string[j];
    string[j] = c;
  }
}

void int_to_string(long long int number, char *string, int *str_index,
                   int width, int precision, TokenType token) {
  int i;
  long long int sign;
  i = 0;
  if ((sign = number) < 0) {  // записываем знак
    number = -number;  // делаем n положительным числом
  }
  int count;
  if (number == 0) {
    count = 1;
  } else {
    count = length_number(number);
  }
  char *str = calloc(count + precision + width + 100, sizeof(char));

  while (number > 0) {  // генерируем цифры в обратном порядке
    str[i] = number % 10 + '0';  // берем следующую цифру
    i++;
    number /= 10;
  }
  while (count < precision) {
    str[i] = '0';
    i++;
    count++;
  }
  number = sign;

  flag_in_variable(number, count, str, &i, width, token);
  if (number == 0) {
    str[i] = '0';
    i++;
  }
  i = 0;
  while (str[i]) {
    string[*str_index] = str[i];
    i++;
    (*str_index)++;
  }
  free(str);
}

void float_to_string(long double f_num, char *string, int *str_index, int width,
                     int precision, TokenType token) {
  long double temp = f_num;
  if (f_num < 0) {
    f_num = -f_num;
  }
  if (precision == -1) {
    precision = 6;
  }
  long double r_f_num =
      (round(f_num * pow(10, precision)) / pow(10, precision));
  if (r_f_num - (int)r_f_num == 0) {
    f_num = round(f_num);
  }

  if (precision == 0) {
    f_num = round(f_num);
  }

  long int whole_part = (long int)f_num;
  long double fractional_part = f_num - whole_part;
  int len = length_number(whole_part);
  char *str = calloc(precision + width + 3 + len, sizeof(char));
  int i = 0;

  flag_in_float(temp, &len, string, str_index, &width, &precision, token);

  if (whole_part == 0) {
    str[i] = '0';
    len++;
    i++;
  }

  while (whole_part > 0) {
    str[i] = whole_part % 10 + '0';
    i++;
    whole_part /= 10;
  }

  whole_part = (long int)f_num;
  reverse_str(str);
  i = 0;
  while (str[i]) {
    string[(*str_index)++] = str[i];
    i++;
  }

  if (precision != 0 || s21_strchr(token.flags, '#')) {
    string[(*str_index)++] = '.';
  }

  rounding(fractional_part, &len, string, str_index, precision);

  if (s21_strchr(token.flags, '-')) {
    while (len < width) {
      string[(*str_index)++] = ' ';
      len++;
    }
  }

  free(str);
}

void str_to_string(char *string, char *str, int *str_index, int width,
                   int precision, TokenType token) {
  int count;

  if ((int)s21_strlen(str) > precision && precision != -1) {
    count = precision;
  } else {
    count = s21_strlen(str);
  }
  int i = 0;
  if (s21_strchr(token.flags, '-')) {
    for (int j = 0; j < count; j++) {
      string[*str_index] = str[j];
      (*str_index)++;
    }
    while (i < width - count) {
      string[*str_index] = ' ';
      i++;
      (*str_index)++;
    }
  } else {
    while (i < width - count) {
      string[*str_index] = ' ';
      i++;
      (*str_index)++;
    }
    for (int j = 0; j < count; j++) {
      string[*str_index] = str[j];
      (*str_index)++;
    }
  }
}

void wchar_to_string(char *string, wchar_t *str, int *str_index, int width,
                     int precision, TokenType token) {
  int count = 0;
  int i = 0;
  while (str[i]) {
    count++;
    i++;
  }

  if (count > precision && precision != -1) {
    count = precision;
  }
  i = 0;
  if (s21_strchr(token.flags, '-')) {
    for (int j = 0; j < count; j++) {
      string[*str_index] = str[j];
      (*str_index)++;
    }
    while (i < width - count) {
      string[*str_index] = ' ';
      i++;
      (*str_index)++;
    }
  } else {
    while (i < width - count) {
      string[*str_index] = ' ';
      i++;
      (*str_index)++;
    }
    for (int j = 0; j < count; j++) {
      string[*str_index] = str[j];
      (*str_index)++;
    }
  }
}

void e_to_string(long double f_num, char *string, int *str_index, char ch,
                 int width, int precision, TokenType token) {
  if (precision == -1) {
    precision = 6;
  }
  // Определение знака
  long double sign = f_num;

  if (f_num < 0) f_num = -f_num;

  // Нормализация числа и вычисление экспоненты
  int exponent = 0;
  while (f_num >= 10.0) {
    f_num /= 10.0;
    exponent++;
  }
  while (f_num > 0.0 && f_num < 1.0) {
    f_num *= 10.0;
    exponent--;
  }

  // Округление до указанной точности
  f_num *= pow(10, precision);
  if (f_num - (int)f_num > 0.5) {
    f_num = round(f_num);
  }
  f_num /= pow(10, precision);

  // Округление до указанной точности
  double factor = pow(10, precision);

  f_num = round(f_num * factor) / factor;  // Округляем всю строку (всю дробь)
  // Добавляем знак числа, если он отрицательный
  if (sign < 0) {
    f_num = -f_num;
  }

  if (precision != 0) {
    width -= 4;
  }
  char *temp = calloc(width + precision + 3 + 100, sizeof(char));
  int i = 0;
  float_to_string(f_num, temp, &i, width, precision, token);
  expon(string, str_index, ch, temp, exponent, width, token);

  free(temp);
}

void expon_or_float(int exponent, long double temp, long long int sign,
                    long double f_num, char *string, int *str_index, char ch,
                    int width, int precision, TokenType token) {
  int len = 0;
  if ((exponent < -4 || exponent >= precision)) {
    temp = round(temp * pow(10, precision)) / pow(10, precision);
    temp = (temp - (int)temp) * 10;
    while ((int)temp != 0) {
      len++;
      temp -= (int)round(temp);
      temp *= 10;
    }
    if (precision > len) {
      precision = len;
    } else if (precision == len && precision != 0) {
      precision--;
    }
    if (exponent == 0 && precision == 0) {
      float_to_string(f_num, string, str_index, width, precision, token);
    } else {
      e_to_string(f_num, string, str_index, ch, width, precision, token);
    }
  } else {
    len = length_number(sign);
    if (len != 0) {
      precision -= len;
      if (precision == 0) {
        f_num = round(f_num);
      }
    } else {
      temp = f_num;
      temp *= 10;
      if (f_num == 0.0) {
        temp = 1.0;
        precision = 0;
      }
      while ((int)temp == 0) {
        precision++;
        temp -= (int)temp;
        temp *= 10;
      }
    }
    long double r_f_num =
        (round(f_num * pow(10, precision)) / pow(10, precision));
    if (r_f_num - (int)r_f_num == 0 && !s21_strchr(token.flags, '#')) {
      precision = 0;
    }
    if (s21_strchr(token.flags, '#') && len == 0 && precision == 0) {
      precision = 1;
    }
    float_to_string(f_num, string, str_index, width, precision, token);
  }
}
void g_to_string(long double f_num, char *string, int *str_index, char ch,
                 int width, int precision, TokenType token) {
  if (precision == -1) {
    precision = 6;
  }
  long double temp = f_num;
  long long int sign = (int)f_num;
  // Нормализация числа и вычисление экспоненты
  int exponent = 0;
  while (temp >= 10.0) {
    temp /= 10.0;
    exponent++;
  }
  while (temp > 0.0 && temp < 1.0) {
    temp *= 10.0;
    exponent--;
  }
  if (f_num < 0) {
    sign = (int)(-f_num);
  } else {
    sign = (int)f_num;
  }
  expon_or_float(exponent, temp, sign, f_num, string, str_index, ch, width,
                 precision, token);
}

void hex_to_string(long unsigned int num, const char *hex_chars, char *string,
                   int *str_index, int width, int precision, TokenType token) {
  int i = 0;
  int count = length_number(num);
  char *str = calloc(count + precision + width + 3 + 100, sizeof(char));
  if (num == 0) {
    str[i] = '0';
    i++;
  }

  while (num > 0) {
    str[i] = hex_chars[num % 16];
    i++;
    num /= 16;
  }
  while (i < precision) {
    str[i] = '0';
    i++;
  }

  if (s21_strchr(token.flags, '#')) {
    if (s21_strchr(hex_chars, 'f')) {
      str[i++] = 'x';
    } else {
      str[i++] = 'X';
    }
    str[i++] = '0';
    count += 2;
  }

  flag_in_variable(num, i, str, &i, width, token);

  i = 0;
  while (str[i]) {
    string[*str_index] = str[i];
    i++;
    (*str_index)++;
  }
  free(str);
}

void oct_to_string(long unsigned int num, char *string, int *str_index,
                   int width, int precision, TokenType token) {
  int count = 0;
  long unsigned int temp = num;

  if (num == 0) {
    count = 1;
  } else {
    count = length_number(num);
  }

  int i = 0;
  char *str = calloc(count + precision + width + 100, sizeof(char));
  while (num > 0) {
    str[i] = '0' + num % 8;
    i++;
    num /= 8;
  }
  while (count < precision) {
    str[i] = '0';
    i++;
    count++;
  }

  if (s21_strchr(token.flags, '#')) {
    str[i++] = '0';
    count++;
  }

  flag_in_variable(num, count, str, &i, width, token);

  i = 0;
  while (str[i]) {
    string[*str_index] = str[i];
    i++;
    (*str_index)++;
  }

  if (temp == 0) {
    string[*str_index] = '0';
    (*str_index)++;
  }
  free(str);
}

void pointer_to_string(const void *point, char *string, int *str_index,
                       int width, int precision, TokenType token) {
  uintptr_t address = (uintptr_t)point;
  char *hex_chars = "0123456789abcdef";
  int len = length_number((uintptr_t)point);
  char *str = calloc(len + precision + width + 3, sizeof(char));
  int i = 0;
  if (address == 0) {
    str[i] = '0';
    i++;
  }

  while (address > 0) {
    str[i] = hex_chars[address % 16];
    i++;
    address /= 16;
  }
  while (i < precision) {
    str[i] = '0';
    i++;
    address++;
  }
  reverse_str(str);
  string[(*str_index)++] = '0';
  string[(*str_index)++] = 'x';

  i = 0;
  while (str[i]) {
    string[*str_index] = str[i];
    i++;
    (*str_index)++;
  }
  i += 2;
  reverse_str(string);
  flag_in_variable(address, i, string, str_index, width, token);
  free(str);
}

void format_output(TokenType *array_tokens, int index_f, char *string,
                   va_list args, const char *format) {
  char *hex_chars = "0123456789abcdef";
  int width, precision;
  int str_index = 0;
  char ch;
  void *address;
  int index = 0;
  int i = 0;
  while (format[index] != '\0') {
    while (format[index] != '%' && format[index] != '\0') {
      string[str_index] = format[index];
      str_index++;
      index++;
    }
    if (i >= index_f || format[index] == '\0') {
      break;
    }
    if (array_tokens[i].width == '*') {
      width = va_arg(args, int);
    } else {
      width = array_tokens[i].width;
    }
    if (array_tokens[i].precision == '*') {
      precision = va_arg(args, int);
    } else {
      precision = array_tokens[i].precision;
    }
    switch (array_tokens[i].specificator) {
      case 'c':
        if (!s21_strchr(array_tokens[i].flags, '-')) {
          for (int j = 0; j < width - 1; j++) {
            string[str_index++] = ' ';
          }
        }
        if (array_tokens[i].length == 'l') {
          wchar_t ch_w = va_arg(args, wchar_t);
          string[str_index++] = ch_w;
        } else {
          char ch = (char)va_arg(args, int);
          string[str_index++] = ch;
        }
        if (s21_strchr(array_tokens[i].flags, '-')) {
          for (int j = 0; j < width - 1; j++) {
            string[str_index++] = ' ';
          }
        }
        break;
      case 'i':
      case 'd':
        if (array_tokens[i].length == 'l') {
          long int num_l = va_arg(args, long int);
          int_to_string(num_l, string, &str_index, width, precision,
                        array_tokens[i]);
        } else if (array_tokens[i].length == 'h') {
          short int num_h = va_arg(args, int);
          int_to_string(num_h, string, &str_index, width, precision,
                        array_tokens[i]);
        } else {
          int num = va_arg(args, int);
          int_to_string(num, string, &str_index, width, precision,
                        array_tokens[i]);
        }
        break;
      case 'f':
        if (array_tokens[i].length == 'L') {
          long double ld_num = va_arg(args, long double);
          float_to_string(ld_num, string, &str_index, width, precision,
                          array_tokens[i]);
        } else {
          double f_num = va_arg(args, double);
          float_to_string(f_num, string, &str_index, width, precision,
                          array_tokens[i]);
        }
        break;
      case 's':
        if (array_tokens[i].length == 'l') {
          wchar_t *str = va_arg(args, wchar_t *);
          wchar_to_string(string, str, &str_index, width, precision,
                          array_tokens[i]);
        } else {
          char *str = va_arg(args, char *);
          str_to_string(string, str, &str_index, width, precision,
                        array_tokens[i]);
        }
        break;
      case 'u':
        if (array_tokens[i].length == 'l') {
          long unsigned int un_num_l = va_arg(args, long unsigned int);
          int_to_string(un_num_l, string, &str_index, width, precision,
                        array_tokens[i]);
        } else if (array_tokens[i].length == 'h') {
          short unsigned int un_num_h = va_arg(args, unsigned int);
          int_to_string(un_num_h, string, &str_index, width, precision,
                        array_tokens[i]);
        } else {
          unsigned int un_num = va_arg(args, unsigned int);
          int_to_string(un_num, string, &str_index, width, precision,
                        array_tokens[i]);
        }
        break;
      case 'e':
      case 'E':
        if (array_tokens[i].specificator == 'E') {
          ch = 'E';
        } else {
          ch = 'e';
        }
        if (array_tokens[i].length == 'L') {
          long double ld_num = va_arg(args, long double);
          e_to_string(ld_num, string, &str_index, ch, width, precision,
                      array_tokens[i]);
        } else {
          double f_num = va_arg(args, double);
          e_to_string(f_num, string, &str_index, ch, width, precision,
                      array_tokens[i]);
        }
        break;
      case 'g':
      case 'G':
        if (array_tokens[i].specificator == 'G') {
          ch = 'E';
        } else {
          ch = 'e';
        }
        if (array_tokens[i].length == 'L') {
          long double ld_num = va_arg(args, long double);
          g_to_string(ld_num, string, &str_index, ch, width, precision,
                      array_tokens[i]);
        } else {
          double f_num = va_arg(args, double);
          g_to_string(f_num, string, &str_index, ch, width, precision,
                      array_tokens[i]);
        }
        break;
      case 'x':
      case 'X':
        if (array_tokens[i].specificator == 'X') {
          hex_chars = "0123456789ABCDEF";
        }
        if (array_tokens[i].length == 'l') {
          long unsigned int un_num_l = va_arg(args, long unsigned int);
          hex_to_string(un_num_l, hex_chars, string, &str_index, width,
                        precision, array_tokens[i]);
        } else if (array_tokens[i].length == 'h') {
          short unsigned int un_num_h = va_arg(args, unsigned int);
          hex_to_string(un_num_h, hex_chars, string, &str_index, width,
                        precision, array_tokens[i]);
        } else {
          unsigned int un_num = va_arg(args, unsigned int);
          hex_to_string(un_num, hex_chars, string, &str_index, width, precision,
                        array_tokens[i]);
        }
        break;
      case 'o':
        if (array_tokens[i].length == 'l') {
          long unsigned int un_num_l = va_arg(args, long unsigned int);
          oct_to_string(un_num_l, string, &str_index, width, precision,
                        array_tokens[i]);
        } else if (array_tokens[i].length == 'h') {
          short unsigned int un_num_h = va_arg(args, unsigned int);
          oct_to_string(un_num_h, string, &str_index, width, precision,
                        array_tokens[i]);
        } else {
          unsigned int un_num = va_arg(args, unsigned int);
          oct_to_string(un_num, string, &str_index, width, precision,
                        array_tokens[i]);
        }
        break;
      case 'p': {
        address = va_arg(args, void *);
        pointer_to_string(address, string, &str_index, width, precision,
                          array_tokens[i]);
      }
    }
    index += array_tokens[i].len_specificator;
    i++;
  }

  string[str_index] = '\0';  // Добавляем нуль-терминатор
}

bool parser_flags(const char *str, char **flags, int *index_f,
                  int *len_codification) {
  regex_t regex;
  regmatch_t matches[1];  // Для спецификаторов
  int tapok = 0;

  const char *options =
      "%%?([\\+#0 "
      "-]*)?([0-9]+|\\*)?(\\.[0-9]+|\\.\\*)?([hlL]{0,2})?([cdieEfgGosuxXp]"
      ")";  // Все нужные флаги и т.п.

  // Компиляция регулярного выражения
  if (regcomp(&regex, options, REG_EXTENDED | REG_NEWLINE) != 0) {
    printf("Ошибка компиляции регулярного выражения\n");
    return false;
  }
  const char *cursor = str;
  while (*cursor != '\0') {  // Пока не конец форматной строки
    // Выполняем поиск последовательности спецификатора
    if (regexec(&regex, cursor, 1, matches, 0) == 0) {
      // Проверяем, есть ли соответствие (rm_so и rm_eo > -1)
      if (matches[0].rm_so != -1) {
        // Вычисляем длину найденного спецификатора
        int match_length = matches[0].rm_eo - matches[0].rm_so;
        len_codification[*index_f] = match_length;
        // Выделяем память для хранения найденного спецификатора
        flags[*index_f] = calloc(match_length + 1, sizeof(char));
        s21_strncpy(flags[*index_f], cursor + matches[0].rm_so, match_length);
        flags[*index_f][match_length] = '\0';  // Добавляем нуль-терминатор
        (*index_f)++;  // Увеличиваем индекс массива флагов
      }
      // Смещаем указатель на конец найденного совпадения
      cursor += matches[0].rm_eo;
    } else {
      tapok = 1;
      break;
    }
  }

  // Освобождаем ресурсы, связанные с регулярным выражением
  regfree(&regex);

  if (*index_f > 0) {
    tapok = 1;
  }

  return tapok;
}

void parser_tokens(TokenType *array_tokens, const char **flags, int index_f) {
  for (int i = 0; i < index_f; i++) {
    const char *flag = flags[i];
    // Инициализируем токен
    array_tokens[i].flags[0] = '\0';
    array_tokens[i].idx_flags = 0;
    array_tokens[i].width = 0;
    array_tokens[i].precision = -1;  // -1 означает, что точность не задана
    array_tokens[i].length = ' ';
    array_tokens[i].specificator = ' ';

    int j = 1;
    bool is_accuracy = false;
    bool is_digit = false;
    while (flag[j]) {
      static char flagss[] = " -+#0";
      static char length_flags[] = "hlL";

      if (s21_strchr(flagss, flag[j])) {
        if (flag[j] == '0' && is_digit) {
        } else {
          array_tokens[i].flags[(array_tokens[i].idx_flags)++] = flag[j];
        }
      }

      // Если число без ., то ширина
      if (isdigit(flag[j]) && !is_accuracy) {
        if (flag[j] != '0') {
          is_digit = true;
        }
        array_tokens[i].width = array_tokens[i].width * 10 + (flag[j] - '0');
      } else if (flag[j] == '.') {
        is_accuracy = true;
      }

      // Если число после . то точность
      if (isdigit(flag[j]) && is_accuracy) {
        is_digit = true;
        // Сбрасываем precision с -1 на 0 перед накоплением
        array_tokens[i].precision =
            (array_tokens[i].precision == -1 ? 0 : array_tokens[i].precision);
        array_tokens[i].precision =
            array_tokens[i].precision * 10 + (flag[j] - '0');
      } else if (s21_strchr(length_flags, flag[j])) {
        array_tokens[i].length = flag[j];
      }
      // Остальное токены
      else {
        array_tokens[i].specificator = flag[j];
      }
      j++;
    }
    array_tokens[i].flags[array_tokens[i].idx_flags++] = '\0';
  }
}

int s21_sprintf(char *string, const char *format, ...) {
  int res_lenght = 0;  // длина записанной строки
  char **flags = calloc(s21_strlen(format) + 2, sizeof(char *));
  int *len_codification = calloc(s21_strlen(format) + 2, sizeof(int));
  int index_f = 0;
  TokenType *array_tokens = S21_NULL;
  va_list args;  // Объявляем объект для списка аргументов
  va_start(args, format);  // Инициализируем va_list (указываем последний
                           // обязательный аргумент)

  if (parser_flags(format, flags, &index_f, len_codification)) {
    // Выделил память под массив токенов по размеру кол-ва флагов
    array_tokens = (TokenType *)calloc(index_f, sizeof(TokenType));

    // Вывод флагов для отладки + заполнение массив длин
    for (int i = 0; i < index_f; i++) {
      array_tokens[i].len_specificator = len_codification[i];
    }

    free(len_codification);  // Освобождаем память под len_codification
    // Парсинг
    parser_tokens(array_tokens, (const char **)flags, index_f);
  } else {
    printf("Illegal option");
  }

  format_output(array_tokens, index_f, string, args, format);

  // Чистим память под флаги
  for (int i = 0; i < index_f; i++) {
    free(flags[i]);
  }
  free(flags);

  // Чистим память под array_tokens
  if (array_tokens != S21_NULL) {
    free(array_tokens);
  }
  return res_lenght;
}
