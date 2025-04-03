#ifndef S21_STRING_H
#define S21_STRING_H

#include <ctype.h>
#include <regex.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <stdint.h>
#include <limits.h>

#define S21_NULL ((void *)0)

typedef unsigned long s21_size_t;

typedef struct {
  char flags[6];        // флаг
  int idx_flags;        // индекс флага
  int width;          // ширина
  int precision;      // .точность
  char length;         // длина
  char specificator; // спецификатор
  int len_specificator; // длина спецификатора
} TokenType;

typedef struct {
  int is_skip_asterisk;        // пропускается ли значение
  int width;                   // ширина
  char length;                 // длина
  char specifier;              // спецификатор
  int format_specifier_length; // длина форматного спецификатора
} SscanfTokenType;

// ################################################# SPRINTF.C #################################################

int s21_sprintf(char *string, const char *format, ...);
bool parser_flags(const char *str, char **flags, int *index_f, int *len_codification);
void parser_tokens(TokenType *array_tokens, const char **flags, int index_f);
int length_number(int number);
void reverse_str(char *string);
void flag_in_variable(long long int number, int len, char *str, int *str_index, int width,
                      TokenType token);
void flag_in_float(long double f_num, int *len, char *str, int *str_index,
                   int *width, int *precision, TokenType token);
void rounding(long double fractional_part, int *len, char *string,
              int *str_index, int precision);
void int_to_string(long long int number, char *string, int *str_index,
                   int width, int precision, TokenType token);            
void float_to_string(long double f_num, char *string, int *str_index, int width,
                     int precision, TokenType token);
void str_to_string(char *string, char *str, int *str_index, int width,
                   int precision, TokenType token);
void expon(char *string, int *str_index, char ch, char *temp, int exponent,
           int width, TokenType token);
void e_to_string(long double f_num, char *string, int *str_index, char ch, int width, 
                   int precision, TokenType token);
void expon_or_float(int exponent, long double temp, long long int sign,
                    long double f_num, char *string, int *str_index, char ch,
                    int width, int precision, TokenType token);
void g_to_string(long double f_num, char *string, int *str_index, char ch,
                 int width, int precision, TokenType token);
void format_output(TokenType *array_tokens, int index_f, char *string,
                   va_list args, const char *format);
void wchar_to_string(char *string, wchar_t *str, int *str_index, int width,
                     int precision, TokenType token);                   
void hex_to_string(long unsigned int num, const char *hex_chars, char *string,
                   int *str_index, int width, int precision, TokenType token);
void oct_to_string(long unsigned int num, char *string, int *str_index, int width,
                   int precision, TokenType token);
void pointer_to_string(const void *point, char *string, int *str_index,
                       int width, int precision, TokenType token);
void test_format_output(TokenType *tokens, int index_f, char *result, const char *format, ...);

// #############################################################################################################

// ################################################# SSCANF.C ##################################################

int s21_sscanf(const char *str, const char *format, ...);
bool sscanf_parser_flags(const char *str, char **format_specifiers, int *index_f,
                  int *len_codification);
void sscanf_parser_tokens(SscanfTokenType *array_tokens, const char **format_specifiers, int index_f);
int format_input(SscanfTokenType *array_tokens, int index_f, char *string,
                   va_list args, const char *format);

double string_to_float(const char *str);
long double string_to_longdouble(const char *str);
void convert_hex_string_to_decimal(SscanfTokenType *token, const char *string, int *str_index,
                                   void *num, int *count);
void handle_char(SscanfTokenType *token, const char *string, int *str_index,
                 va_list args, int *count);
void handle_string(SscanfTokenType *token, const char *string, int *str_index,
                   va_list args, int *count);
void handle_signed_integer_dec(SscanfTokenType *token, const char *string, int *str_index,
                 va_list args, int *count);
void handle_unsigned_integer_oct(SscanfTokenType *token, const char *string,
                               int *str_index, va_list args, int *count);
void handle_unsigned_integer_hex(SscanfTokenType *token, const char *string,
                               int *str_index, va_list args, int *count);
void handle_unsigned_integer(SscanfTokenType *token, const char *string,
                             int *str_index, va_list args, int *count);
void handle_float(SscanfTokenType *token, char *string, int *str_index,
                  va_list args, int *count);
void handle_pointer(SscanfTokenType *token, char *string, int *str_index,
                    va_list args, int *count);

// #############################################################################################################

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);
void *s21_trim(const char *src, const char *trim_chars);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_to_lower(const char *str);
void *s21_to_upper(const char *str);
char *s21_strdup(const char *s);

#endif  // S21_STRING_H