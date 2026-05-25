#ifndef S21_DECIMAL_COMMON_H
#define S21_DECIMAL_COMMON_H

#include "s21_decimal.h"

int _get_sign(const s21_decimal* dec);
void _set_sign(s21_decimal* dec, int sign);
float _fabs(float x);
int _is_nan(float x);
int _is_inf(float x);
int _is_zero(float x);
int _get_scale(const s21_decimal* dec);
void _set_scale(s21_decimal* dec, int scale);
int _is_zero_decimal(s21_decimal value);
s21_decimal s21_decimal_zero(void);

#endif
