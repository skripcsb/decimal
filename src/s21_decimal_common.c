#include "s21_decimal_common.h"

#include "s21_decimal.h"

void _set_sign(s21_decimal* dec, int sign) {
  if (sign)
    dec->bits[3] |= DECIMAL_SIGN_MASK;
  else
    dec->bits[3] &= ~DECIMAL_SIGN_MASK;
}

int _get_sign(const s21_decimal* dec) {
  return (dec->bits[3] & DECIMAL_SIGN_MASK) ? 1 : 0;
}

// для float
float _fabs(float x) {
  // float abs
  return (x < 0) ? -x : x;
}
int _is_nan(float x) {
  // NaN не равен самому себе
  return (x != x);
}

int _is_inf(float x) {
  // Бесконечность - очень большое число
  return (_fabs(x) > 1e29f && x == x);  // NaN не пройдет
}

int _is_zero(float x) { return (_fabs(x) < 1e-29f); }

int _get_scale(const s21_decimal* dec) {
  return (dec->bits[3] & DECIMAL_SCALE_MASK) >> DECIMAL_SCALE_SHIFT;
}

void _set_scale(s21_decimal* dec, int scale) {
  dec->bits[3] = (dec->bits[3] & ~DECIMAL_SCALE_MASK) |
                 ((scale << DECIMAL_SCALE_SHIFT) & DECIMAL_SCALE_MASK);
}

int _is_zero_decimal(s21_decimal value) {
  return value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0;
}

s21_decimal s21_decimal_zero(void) {
  s21_decimal output = {{0, 0, 0, 0}};
  return output;
}
