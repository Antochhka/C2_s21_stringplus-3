#include "../s21_string.h"

int s21_sscanf(const char *str, const char *format, ...) {
  int count = 0;  // Число полей, значения которых были при­своены переменным
  char **format_specifiers = calloc(s21_strlen(format) + 2, sizeof(char *));
  int *len_codification = calloc(s21_strlen(format) + 2, sizeof(int));
  int index_f = 0;
  SscanfTokenType *array_tokens = S21_NULL;

  va_list args;  // Объявляем объект для списка аргументов
  va_start(args, format);  // Инициализируем va_list (указываем последний
                           // обязательный аргумент)

  if (sscanf_parser_flags(format, format_specifiers, &index_f,
                          len_codification)) {
    // Выделил память под массив токенов по размеру кол-ва флагов
    array_tokens = (SscanfTokenType *)calloc(index_f, sizeof(SscanfTokenType));

    // Заполнение массив длин
    for (int i = 0; i < index_f; i++) {
      array_tokens[i].format_specifier_length = len_codification[i];
    }

    free(len_codification);  // Освобождаем память под len_codification

    // Парсинг
    sscanf_parser_tokens(array_tokens, (const char **)format_specifiers,
                         index_f);
  } else {
    printf("Illigal option\n");
  }

  count = format_input(array_tokens, index_f, (char *)str, args, format);

  va_end(args);

  // Чистим память под флаги
  for (int i = 0; i < index_f; i++) {
    free(format_specifiers[i]);
  }
  free(format_specifiers);

  // Чистим память под array_tokens
  if (array_tokens != S21_NULL) {
    free(array_tokens);
  }
  return count;
}

bool sscanf_parser_flags(const char *str, char **format_specifiers,
                         int *index_f, int *len_codification) {
  regex_t regex;
  regmatch_t matches[1];  // Для спецификаторов
  bool result = false;
  const char *options =
      "%%|%\\*?([0-9]+)?([hlL]{0,2})?([cdieEfgGosuxXpn])";  // Регулярка для
                                                            // нахождения
                                                            // ширины, длины и
                                                            // спецификатора

  // Компиляция регулярного выражения
  if (regcomp(&regex, options, REG_EXTENDED | REG_NEWLINE) == 0) {
    const char *cursor = str;

    while (*cursor != '\0') {  // Пока не конец форматной строки
      // Выполняем поиск последовательности спецификатора
      if (regexec(&regex, cursor, 1, matches, 0) == 0) {
        // Длина найденного спецификатора
        int match_length = matches[0].rm_eo - matches[0].rm_so;

        len_codification[*index_f] = match_length;

        // Выделяем память, копируем и завершаем строку, хранящую спецификатор
        format_specifiers[*index_f] = calloc(match_length + 1, sizeof(char));
        s21_strncpy(format_specifiers[*index_f], cursor + matches[0].rm_so,
                    match_length);
        format_specifiers[*index_f][match_length] = '\0';

        // Увеличиваем индекс массива флагов и смещаем указатель
        (*index_f)++;
        cursor += matches[0].rm_eo;
      } else {
        break;
      }
    }

    regfree(&regex);  // Освобождаем ресурсы, связанные с регулярным выражением
  } else {
    printf("Ошибка компиляции регулярного выражения\n");
    result = false;
  }

  result =
      *index_f > 0;  // Если найден хотя бы один спецификатор, то будет true

  return result;
}

void sscanf_parser_tokens(SscanfTokenType *array_tokens,
                          const char **format_specifiers, int index_f) {
  for (int i = 0; i < index_f; i++) {
    const char *cur_specifier = format_specifiers[i];

    // Инициализируем токен
    array_tokens[i].is_skip_asterisk = cur_specifier[1] == '*';
    array_tokens[i].width = 0;
    array_tokens[i].length = ' ';
    array_tokens[i].specifier = ' ';

    int j = 1;
    while (cur_specifier[j]) {
      static char length_flags[] = "hlL";

      // Если число, то ширина
      if (isdigit(cur_specifier[j])) {
        array_tokens[i].width =
            array_tokens[i].width * 10 + (cur_specifier[j] - '0');
      }

      else if (s21_strchr(length_flags, cur_specifier[j])) {
        array_tokens[i].length = cur_specifier[j];
      }

      // Остальное - спецификаторы
      else {
        array_tokens[i].specifier = cur_specifier[j];
      }

      j++;
    }
  }
}

int format_input(SscanfTokenType *array_tokens, int index_f, char *string,
                 va_list args, const char *format) {
  int str_index = 0;
  int format_index = 0;
  SscanfTokenType token;
  int i = 0;
  int is_match = 1;
  int count = 0;

  // Идем до конца форматной строки или пока не найдем различие форматной строки
  // и строки, в которой производится поиск
  while (format[format_index] != '\0' && is_match) {
    // Пропускаем пробелы в строке, по которой ищем только в том случае, если в
    // форматной строке тоже стоит пробел, либо если мы в начале строки, по
    // которой ищем. Если же в форматной строке нет пробела, а в строке, по
    // которой ищем, он есть, тогда нам возможно придется считать пробел как
    // символ, не пропуская его
    if (format[format_index] == ' ' || str_index == 0) {
      while (isspace(string[str_index])) str_index++;
    }
    // А пробелы в форматной строке пропускаем всегда
    while (isspace(format[format_index])) format_index++;

    // Проверяем, форматный спецификатор это или простой символ
    if (format[format_index] == '%') {
      token = array_tokens[i];

      if (i >= index_f) {
        break;  // Проверка на выход за пределы массива спецификаторов
      }

      switch (token.specifier) {
        case 'c':
          handle_char(&token, string, &str_index, args, &count);
          break;
        case 'd':
          handle_signed_integer_dec(&token, string, &str_index, args, &count);
          break;
        case 's':
          handle_string(&token, string, &str_index, args, &count);
          break;
        case 'u':
          handle_unsigned_integer(&token, string, &str_index, args, &count);
          break;
        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
          handle_float(&token, string, &str_index, args, &count);
          break;
        case 'o':
          handle_unsigned_integer_oct(&token, string, &str_index, args, &count);
          break;
        case 'x':
        case 'X':
          handle_unsigned_integer_hex(&token, string, &str_index, args, &count);
          break;
        case 'i':
          if (s21_strncmp((string + str_index), "0x", 2) == 0 ||
              s21_strncmp((string + str_index), "0X", 2) == 0) {
            // Обработка шестнадцатеричного числа
            handle_unsigned_integer_hex(&token, string, &str_index, args,
                                        &count);
          } else if (string[str_index] == '0') {
            // Обработка восьмеричного числа
            handle_unsigned_integer_oct(&token, string, &str_index, args,
                                        &count);
          } else {
            // Обработка десятичного числа
            handle_signed_integer_dec(&token, string, &str_index, args, &count);
          }

          break;
        case 'p':
          handle_pointer(&token, string, &str_index, args, &count);
          break;
        case 'n':
          if (!(token.is_skip_asterisk)) {
            int *num = va_arg(args, int *);
            *num = str_index;
          }

          break;
        case '%':
          if (string[str_index] != format[format_index + 1]) {
            is_match = 0;
          } else {
            str_index++;
          }

          break;
      }

      format_index += token.format_specifier_length;
      i++;  // Переход к следующему спецификатору
    } else {
      // Не совпадают - останавливаемся, совпадают - продолжаем
      if (string[str_index] != format[format_index]) {
        is_match = 0;
      } else {
        str_index++;
        format_index++;
      }
    }
  }

  return count;  // Возвращаем количество успешно считанных элементов
}

double string_to_float(const char *str) {
  // Обработка знака
  int sign = 1;
  if (*str == '-') {
    sign = -1;
    str++;
  } else if (*str == '+') {
    str++;
  }

  // Чтение целой части
  unsigned long long intPart = 0;
  while (isdigit((unsigned char)*str)) {
    intPart = intPart * 10 + (*str - '0');
    str++;
  }

  // Чтение дробной части
  double fracPart = 0.0f;
  if (*str == '.') {
    str++;
    double divisor = 10.0f;
    while (isdigit((unsigned char)*str)) {
      fracPart += (*str - '0') / divisor;
      divisor *= 10.0f;
      str++;
    }
  }

  // Чтение экспоненты
  int exponent = 0;
  if (*str == 'e' || *str == 'E') {
    str++;
    int expSign = 1;
    if (*str == '-') {
      expSign = -1;
      str++;
    } else if (*str == '+') {
      str++;
    }

    while (isdigit((unsigned char)*str)) {
      exponent = exponent * 10 + (*str - '0');
      str++;
    }
    exponent *= expSign;
  }

  // Итоговое значение
  double result = sign * (intPart + fracPart);

  // Обработка экспоненты
  if (exponent != 0) {
    if (exponent > 0) {
      for (int i = 0; i < exponent; i++) {
        result *= 10.0f;
      }
    } else {
      for (int i = 0; i < -exponent; i++) {
        result /= 10.0f;
      }
    }
  }

  return result;
}

long double string_to_longdouble(const char *str) {
  long double result = 0.0;

  // Обработка знака
  int sign = 1;
  if (*str == '-') {
    sign = -1;
    str++;
  } else if (*str == '+') {
    str++;
  }

  // Чтение целой части
  while (isdigit((unsigned char)*str)) {
    result = result * 10 + (*str - '0');
    str++;
  }

  // Чтение дробной части
  if (*str == '.') {
    str++;
    long double divisor = 1.0;
    while (isdigit((unsigned char)*str)) {
      divisor *= 10.0;
      result += (*str - '0') / divisor;
      str++;
    }
  }

  // Чтение экспоненты
  int exponent = 0;
  int exp_sign = 1;
  if (*str == 'e' || *str == 'E') {
    str++;
    if (*str == '-') {
      exp_sign = -1;
      str++;
    } else if (*str == '+') {
      str++;
    }

    while (isdigit((unsigned char)*str)) {
      exponent = exponent * 10 + (*str - '0');
      str++;
    }
    exponent *= exp_sign;
  }

  // Применяем знак и возвращаем результат
  result *= sign;

  // Применяем экспоненту
  while (exponent > 0) {
    result *= 10.0;
    exponent--;
  }
  while (exponent < 0) {
    result /= 10.0;
    exponent++;
  }

  return result;
}

void convert_hex_string_to_decimal(SscanfTokenType *token, const char *string,
                                   int *str_index, void *num, int *count) {
  unsigned long *ptr_long = (unsigned long *)num;
  unsigned int *ptr_int = (unsigned int *)num;
  unsigned short *ptr_short = (unsigned short *)num;

  int max_length = token->width > 0 ? token->width : INT_MAX;
  int j = 0;

  while (j++ < max_length && string[*str_index] != '\0' &&
         s21_strchr("0123456789abcdefABCDEF", string[*str_index])) {
    char c = string[*str_index];
    int value;

    if (isdigit(c)) {
      value = c - '0';
    } else if (c >= 'a' && c <= 'f') {
      value = c - 'a' + 10;
    } else {  // c >= 'A' && c <= 'F'
      value = c - 'A' + 10;
    }

    // Обработка в зависимости от типа
    if (token->length == 'l') {
      *ptr_long = (*ptr_long * 16) + value;
    } else if (token->length == 'h') {
      *ptr_short = (*ptr_short * 16) + value;
    } else {
      *ptr_int = (*ptr_int * 16) + value;
    }

    (*str_index)++;
  }

  if (j > 1) {  // Если хотя бы один символ был считан
    (*count)++;
  }
}

void handle_char(SscanfTokenType *token, const char *string, int *str_index,
                 va_list args, int *count) {
  if (token->is_skip_asterisk) {
    (*str_index)++;
  } else {
    // Убедимся, что текущий индекс не выходит за пределы строки
    if (string[*str_index] != '\0') {
      if (token->length == 'l') {
        wchar_t *wch = va_arg(args, wchar_t *);
        *wch = (wchar_t)string[*str_index];
      } else {
        char *ch = va_arg(args, char *);
        *ch = string[*str_index];
      }

      (*count)++;  // Увеличиваем счетчик только если символ считан
      (*str_index)++;
    }
  }
}

void handle_string(SscanfTokenType *token, const char *string, int *str_index,
                   va_list args, int *count) {
  if (token->is_skip_asterisk) {
    while (!isspace(string[*str_index]) && string[*str_index] != '\0')
      (*str_index)++;
  } else {
    int max_length = token->width > 0 ? token->width : INT_MAX;
    int j = 0;

    if (token->length == 'l') {
      wchar_t *str = va_arg(args, wchar_t *);

      while (j < max_length && !isspace(string[*str_index]) &&
             string[*str_index] != '\0') {
        str[j++] = string[*str_index];
        (*str_index)++;
      }
      str[j] = L'\0';
    } else {
      char *str = va_arg(args, char *);

      while (j < max_length && !isspace(string[*str_index]) &&
             string[*str_index] != '\0') {
        str[j++] = string[*str_index];
        (*str_index)++;
      }
      str[j] = '\0';
    }

    if (j > 0) {  // Если хотя бы один символ был считан
      (*count)++;
    }
  }
}

void handle_signed_integer_dec(SscanfTokenType *token, const char *string,
                               int *str_index, va_list args, int *count) {
  int sign = 1;  // Знак числа
  int has_sign = 0;  // Есть ли явный знак перед числом (+/-)

  // Проверяем наличие знака
  if (string[*str_index] == '-') {
    sign = -1;
    has_sign = 1;
    (*str_index)++;
  } else if (string[*str_index] == '+') {
    has_sign = 1;
    (*str_index)++;
  }

  if (token->is_skip_asterisk) {
    while (isdigit(string[*str_index])) (*str_index)++;
  } else {
    int max_length = token->width > 0 ? token->width : INT_MAX;
    int j = 0;

    if (token->length == 'l') {
      long *num = va_arg(args, long *);
      *num = 0;

      while ((j++ < (max_length - has_sign)) && isdigit(string[*str_index])) {
        *num = *num * 10 + (string[*str_index] - '0');
        (*str_index)++;
      }
      *num *= sign;  // Учитываем знак
    } else if (token->length == 'h') {
      short *num = va_arg(args, short *);
      *num = 0;

      while ((j++ < (max_length - has_sign)) && isdigit(string[*str_index])) {
        *num = *num * 10 + (string[*str_index] - '0');
        (*str_index)++;
      }
      *num *= sign;  // Учитываем знак
    } else {
      int *num = va_arg(args, int *);
      *num = 0;

      while ((j++ < (max_length - has_sign)) && isdigit(string[*str_index])) {
        *num = *num * 10 + (string[*str_index] - '0');
        (*str_index)++;
      }
      *num *= sign;  // Учитываем знак
    }

    if (j > 1) {  // Если хотя бы один символ был считан
      (*count)++;
    }
  }
}

void handle_unsigned_integer_oct(SscanfTokenType *token, const char *string,
                                 int *str_index, va_list args, int *count) {
  if (token->is_skip_asterisk) {
    while ((string[*str_index] >= '0' && string[*str_index] <= '7')) {
      (*str_index)++;
    }
  } else {
    int max_length = token->width > 0 ? token->width : INT_MAX;
    int j = 0;

    if (token->length == 'l') {
      unsigned long *num = va_arg(args, unsigned long *);
      *num = 0;

      while (j++ < max_length && string[*str_index] >= '0' &&
             string[*str_index] <= '7') {
        *num = *num * 8 + (string[*str_index] - '0');
        (*str_index)++;
      }
    } else if (token->length == 'h') {
      unsigned short *num = va_arg(args, unsigned short *);
      *num = 0;

      while (j++ < max_length && string[*str_index] >= '0' &&
             string[*str_index] <= '7') {
        *num = *num * 8 + (string[*str_index] - '0');
        (*str_index)++;
      }
    } else {
      unsigned int *num = va_arg(args, unsigned int *);
      *num = 0;

      while (j++ < max_length && string[*str_index] >= '0' &&
             string[*str_index] <= '7') {
        *num = *num * 8 + (string[*str_index] - '0');
        (*str_index)++;
      }
    }

    if (j > 1) {  // Если хотя бы один символ был считан
      (*count)++;
    }
  }
}

void handle_unsigned_integer_hex(SscanfTokenType *token, const char *string,
                                 int *str_index, va_list args, int *count) {
  // Проверка префикса "0x" или "0X"
  if (string[*str_index] == '0' &&
      (string[*str_index + 1] == 'x' || string[*str_index + 1] == 'X')) {
    (*str_index) += 2;  // Пропускаем префикс
  }

  if (token->is_skip_asterisk) {
    while (string[*str_index] != '\0' &&
           s21_strchr("0123456789abcdefABCDEF", string[*str_index])) {
      (*str_index)++;
    }
  } else {
    if (token->length == 'l') {
      unsigned long *num = va_arg(args, unsigned long *);
      *num = 0;
      convert_hex_string_to_decimal(token, string, str_index, num, count);
    } else if (token->length == 'h') {
      unsigned short *num = va_arg(args, unsigned short *);
      *num = 0;
      convert_hex_string_to_decimal(token, string, str_index, num, count);
    } else {
      unsigned int *num = va_arg(args, unsigned int *);
      *num = 0;
      convert_hex_string_to_decimal(token, string, str_index, num, count);
    }
  }
}

void handle_unsigned_integer(SscanfTokenType *token, const char *string,
                             int *str_index, va_list args, int *count) {
  if (token->is_skip_asterisk) {
    while (isdigit(string[*str_index])) (*str_index)++;
  } else {
    int max_length = token->width > 0 ? token->width : INT_MAX;
    int j = 0;

    if (token->length == 'l') {
      unsigned long *num = va_arg(args, unsigned long *);
      *num = 0;

      while (j++ < max_length && isdigit(string[*str_index])) {
        *num = *num * 10 + (string[*str_index] - '0');
        (*str_index)++;
      }
    } else if (token->length == 'h') {
      unsigned short *num = va_arg(args, unsigned short *);
      *num = 0;

      while (j++ < max_length && isdigit(string[*str_index])) {
        *num = *num * 10 + (string[*str_index] - '0');
        (*str_index)++;
      }
    } else {
      unsigned int *num = va_arg(args, unsigned int *);
      *num = 0;

      while (j++ < max_length && isdigit(string[*str_index])) {
        *num = *num * 10 + (string[*str_index] - '0');
        (*str_index)++;
      }
    }

    if (j > 1) {  // Если хотя бы один символ был считан
      (*count)++;
    }
  }
}

void handle_float(SscanfTokenType *token, char *string, int *str_index,
                  va_list args, int *count) {
  int max_length = token->width > 0 ? token->width : INT_MAX;
  int length = 0;

  // Ищем конец числа
  char *end_ptr = (string + (*str_index));
  while ((length < max_length) &&
         (isdigit(*end_ptr) || *end_ptr == '.' || *end_ptr == 'e' ||
          *end_ptr == 'E' || *end_ptr == '+' || *end_ptr == '-')) {
    end_ptr++;
    length++;
  }

  // Создаем подстроку для преобразования в float/long double
  char *number_str = (char *)malloc(length + 1);
  s21_strncpy(number_str, (string + (*str_index)), length);
  number_str[length] = '\0';

  if (!(token->is_skip_asterisk)) {
    if (token->length == 'L') {
      long double *num = va_arg(args, long double *);
      *num = string_to_longdouble(number_str);
    } else {
      float *num = va_arg(args, float *);
      *num = string_to_float(number_str);
    }

    (*count)++;
  }

  (*str_index) += length;
  free(number_str);
}

void handle_pointer(SscanfTokenType *token, char *string, int *str_index,
                    va_list args, int *count) {
  (*str_index) += 2;  // Пропускаем префикс

  if (token->is_skip_asterisk) {
    while (string[*str_index] != '\0' &&
           s21_strchr("0123456789abcdefABCDEF", string[*str_index])) {
      (*str_index)++;
    }
  } else {
    void **ptr = va_arg(args, void **);
    int error = 0;

    // Переменная для хранения результата
    unsigned long address = 0;

    // Преобразуем шестнадцатеричное представление в число
    while (!isspace(string[*str_index]) && string[*str_index] != '\0') {
      char c = string[*str_index];
      unsigned long value;

      // Преобразуем символ в соответствующее значение
      if (isdigit(c)) {
        value = c - '0';  // '0' -> 0, '1' -> 1, ..., '9' -> 9
      } else if (c >= 'a' && c <= 'f') {
        value = 10 + (c - 'a');  // 'a' -> 10, ..., 'f' -> 15
      } else if (c >= 'A' && c <= 'F') {
        value = 10 + (c - 'A');  // 'A' -> 10, ..., 'F' -> 15
      } else {
        error = 1;
        break;
      }

      // Обновляем адрес. Сдвигаем на 4 бита и добавляем новое значение
      address = (address << 4) | value;

      (*str_index)++;
    }

    if (!error) {
      *ptr = (void *)address;
      (*count)++;
    }
  }
}
