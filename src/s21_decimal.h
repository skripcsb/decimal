#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <float.h>
#include <limits.h>
// #define FLT_MAX 3.4e38f
#define DECIMAL_SCALE_MASK \
  0b00000000111111110000000000000000  // 0x00FF0000  // Маска для извлечения
                                      // масштаба
#define DECIMAL_SIGN_MASK \
  0b10000000000000000000000000000000  // 0x80000000  // Маска для знака (бит 31)
#define DECIMAL_TEN_MULT_MASK 0b11111000000000000000000000000000
#define DECIMAL_SCALE_SHIFT 16  // Сдвиг для масштаба

typedef struct {
  unsigned int bits[4];
} s21_decimal;

// Arithmetics
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal* result);

// Compasion
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

// Conversion
int s21_from_int_to_decimal(int src, s21_decimal* dst);
int s21_from_float_to_decimal(float src, s21_decimal* dst);
int s21_from_decimal_to_int(s21_decimal src, int* dst);
int s21_from_decimal_to_float(s21_decimal src, float* dst);

// Other
int s21_floor(s21_decimal value, s21_decimal* result);
int s21_round(s21_decimal value, s21_decimal* result);
int s21_truncate(s21_decimal value, s21_decimal* result);
int s21_negate(s21_decimal value, s21_decimal* result);

#endif