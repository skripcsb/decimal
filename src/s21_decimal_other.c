#include "s21_decimal.h"
#include "s21_decimal_common.h"

int s21_floor(s21_decimal value, s21_decimal* result) {
  int error = 0;

  if (!result) {
    error = 1;
  } else {
    int sign = _get_sign(&value);
    s21_decimal fractional;
    s21_decimal value_unsigned_truncated;

    s21_decimal value_unsigned = value;
    _set_sign(&value_unsigned, 0);

    s21_truncate(value_unsigned, &value_unsigned_truncated);

    s21_sub(value_unsigned, value_unsigned_truncated, &fractional);

    int fractional_is_positive = 0;

    if (fractional.bits[0] != 0 || fractional.bits[1] != 0 ||
        fractional.bits[2] != 0) {
      fractional_is_positive = 1;
    }

    if (sign == 1 && fractional_is_positive) {
      s21_decimal one = {{1, 0, 0, 0}};
      s21_add(value_unsigned_truncated, one, &value_unsigned_truncated);
    }

    *result = value_unsigned_truncated;
    _set_sign(result, sign);
  }

  return error;
}

int s21_round(s21_decimal value, s21_decimal* result) {
  int error = 0;

  if (!result) {
    error = 1;
  } else {
    int sign = _get_sign(&value);
    s21_decimal fractional;
    s21_decimal value_unsigned_truncated;

    s21_decimal value_unsigned = value;
    _set_sign(&value_unsigned, 0);

    s21_truncate(value_unsigned, &value_unsigned_truncated);

    s21_sub(value_unsigned, value_unsigned_truncated, &fractional);

    s21_decimal two = {{2, 0, 0, 0}};
    s21_decimal doubled_fractional;
    s21_mul(fractional, two, &doubled_fractional);

    s21_decimal one = {{1, 0, 0, 0}};

    if (s21_is_greater_or_equal(doubled_fractional, one)) {
      s21_add(value_unsigned_truncated, one, &value_unsigned_truncated);
    }

    *result = value_unsigned_truncated;
    _set_sign(result, sign);
  }

  return error;
}

int s21_truncate(s21_decimal value, s21_decimal* result) {
  int error = 0;
  if (!result)
    error = 1;
  else {
    *result = value;

    int scale = _get_scale(&value);

    for (int i = 0; i < scale && scale != 0; i++) {
      unsigned long long remainder = 0;
      for (int j = 2; j >= 0; j--) {
        unsigned long long cur =
            (remainder << 32) | (unsigned int)result->bits[j];
        result->bits[j] = (unsigned int)(cur / 10);
        remainder = cur % 10;
      }
    }

    _set_scale(result, 0);
  }

  return error;
}

int s21_negate(s21_decimal value, s21_decimal* result) {
  int error = 0;
  if (!result)
    error = 1;
  else {
    *result = value;
    result->bits[3] ^= (1U << 31);
  }

  return error;
}
