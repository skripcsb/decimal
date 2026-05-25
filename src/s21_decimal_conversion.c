#include <stdio.h>

#include "s21_decimal.h"
#include "s21_decimal_common.h"

static double _pow_double(double x, int exp) {
  double result = 1.0;
  for (int i = 0; i < exp; i++) {
    result *= x;
  }
  return result;
}

int s21_from_int_to_decimal(int src, s21_decimal* dst) {
  int error = 0;
  if (!dst) {
    error = 1;
  } else {
    *dst = s21_decimal_zero();
    if (src < 0) {
      _set_sign(dst, 1);

      if (src == INT_MIN) {
        dst->bits[0] = 2147483648U;
      } else {
        dst->bits[0] = -src;
      }
    } else {
      dst->bits[0] = src;
    }

    _set_scale(dst, 0);
  }

  return error;
}
static int _initial_float_checks(float src, s21_decimal* dst, int* pass) {
  int error = 0;
  *pass = 0;

  if (!dst || _is_nan(src) || _is_inf(src)) {
    error = 1;
  } else {
    *dst = s21_decimal_zero();

    if (_is_zero(src)) {
      *pass = 1;
    } else if (_fabs(src) < 1e-28f && _fabs(src) > 0) {
      *dst = s21_decimal_zero();
      // *pass = 1;
      error = 1;
    }
  }

  return error;
}

static void _extract_sign(float* src, int* is_negative) {
  *is_negative = 0;
  if (*src < 0) {
    *is_negative = 1;
    *src = -*src;
  }
}

static void _decimal_part(float src, int* scale, int* pow_scale,
                          unsigned int* mantissa) {
  char str[100];
  sprintf(str, "%.7g", src);
  char mantissa_str[100] = "";
  char pow_str[5] = "";
  int count_scale = 0, pow_sign = 0;
  int j = 0, k = 0;  // mantissa&pow iterator
  for (int i = 0; str[i] != '\0'; i++) {
    if (count_scale && str[i] != 'e') (*scale)++;
    if (str[i] != '.' && str[i] != 'e' && pow_sign == 0) {
      mantissa_str[j++] = str[i];
    } else if (str[i] == '.') {
      count_scale = 1;
    } else if (str[i] == 'e') {
      pow_sign = (str[++i] == '+') ? 1 : -1;
      count_scale = 0;
    } else if (pow_sign != 0) {
      pow_str[k++] = str[i];
    }
  }
  mantissa_str[j] = '\0';
  pow_str[k] = '\0';

  sscanf(pow_str, "%d", pow_scale);
  *pow_scale *= pow_sign;
  sscanf(mantissa_str, "%u", mantissa);
}

int s21_from_float_to_decimal(float src, s21_decimal* dst) {
  int error = 0, pass = 0, is_negative = 0, scale = 0, pow_scale = 0;
  error = _initial_float_checks(src, dst, &pass);
  if (!error && !pass) {
    _extract_sign(&src, &is_negative);
    unsigned int mantissa = 0;
    _decimal_part(src, &scale, &pow_scale, &mantissa);
    if (pow_scale == 28) {
      float temp_mantissa = mantissa;
      int temp_scale = scale;
      for (int i = 0; i < temp_scale; i++) temp_mantissa /= 10;
      if (_fabs(temp_mantissa) > 7.922f) error = 1;
    }
    scale *= -1;
    scale += pow_scale;
    if (scale > 28 || pow_scale > 28 || pow_scale < -28 || error) {
      error = 1;
      dst->bits[0] = 0;
    } else {
      if (scale < -28) {
        int last_digit = 0;
        for (int i = 0; i < -(scale + 28); i++) {
          last_digit = mantissa % 10;
          mantissa /= 10;
        }
        if (last_digit >= 5) mantissa++;
        scale = -28;
      }
      dst->bits[0] = mantissa;
    }
    dst->bits[1] = 0, dst->bits[2] = 0, dst->bits[3] = 0;
    while (scale > 0 && !error) {
      unsigned long long tmp = 0;
      unsigned long long carry = 0;
      tmp = (unsigned long long)dst->bits[0] * 10ULL + carry;
      dst->bits[0] = (unsigned int)(tmp & 0xFFFFFFFF);
      carry = tmp >> 32;
      tmp = (unsigned long long)dst->bits[1] * 10ULL + carry;
      dst->bits[1] = (unsigned int)(tmp & 0xFFFFFFFF);
      carry = tmp >> 32;
      tmp = (unsigned long long)dst->bits[2] * 10ULL + carry;
      dst->bits[2] = (unsigned int)(tmp & 0xFFFFFFFF);
      scale--;
    }
    if (!error) {
      if (scale < 0) _set_scale(dst, -scale);
      _set_sign(dst, is_negative);
    }
  }
  return error;
}

int s21_from_decimal_to_int(s21_decimal src, int* dst) {
  s21_decimal temp;
  int error = 0;
  if (!dst)
    error = 1;
  else {
    error = s21_truncate(src, &temp);
    if (temp.bits[2] || temp.bits[1]) error = 1;
  }
  if (!error) {
    int sign = _get_sign(&src);
    *dst = temp.bits[0];
    if (sign) (*dst) *= -1;
  }
  return error;
}

int s21_from_decimal_to_float(s21_decimal src, float* dst) {
  int error = 0;

  if (!dst) {
    error = 1;

  } else {
    *dst = 0.0f;

    int scale = _get_scale(&src);
    int sign = _get_sign(&src);

    double result = 0.0;

    if (src.bits[2] != 0) {
      result = (double)src.bits[2];
      result = result * 4294967296.0;  // сдвиг на 32 бита
      result = result * 4294967296.0;  // еще 32 бита
    }

    if (src.bits[1] != 0) {
      result += (double)src.bits[1] * 4294967296.0;
    }

    result += (double)(unsigned int)src.bits[0];

    if (scale > 0) {
      double divisor = _pow_double(10.0, scale);
      result /= divisor;
    }

    if (sign) {
      result = -result;
    }

    if (result > FLT_MAX || result < -FLT_MAX) {
      error = 1;
    } else {
      *dst = (float)result;
    }
  }

  return error;
}
