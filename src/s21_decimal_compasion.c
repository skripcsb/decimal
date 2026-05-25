#include "s21_decimal.h"
#include "s21_decimal_common.h"

static void _copy_mantissa_u96(const s21_decimal* src, unsigned int out[3]) {
  out[0] = (unsigned int)src->bits[0];
  out[1] = (unsigned int)src->bits[1];
  out[2] = (unsigned int)src->bits[2];
}

static int _cmp_u96(const unsigned int mantissa_a[3],
                    const unsigned int mantissa_b[3]) {
  int answer = 0;
  if (mantissa_a[2] != mantissa_b[2]) {
    if (mantissa_a[2] < mantissa_b[2]) {
      answer = -1;
    } else {
      answer = 1;
    }
  } else if (mantissa_a[1] != mantissa_b[1]) {
    if (mantissa_a[1] < mantissa_b[1]) {
      answer = -1;
    } else {
      answer = 1;
    }
  } else if (mantissa_a[0] != mantissa_b[0]) {
    if (mantissa_a[0] < mantissa_b[0]) {
      answer = -1;
    } else {
      answer = 1;
    }
  }
  return answer;
}

static void _mul_u96_small(unsigned int m[3], unsigned int mul) {
  unsigned long long carry = 0;
  for (int i = 0; i < 3; ++i) {
    unsigned long long product =
        (unsigned long long)m[i] * (unsigned long long)mul;
    unsigned long long total = product + carry;
    m[i] = (unsigned int)(total & 0xFFFFFFFFULL);
    carry = total >> 32;
  }
}

static void _div_u96_small(unsigned int m[3], unsigned int divisor) {
  unsigned long long carry = 0;
  for (int i = 2; i >= 0; --i) {
    unsigned long long temp = (carry << 32) | (unsigned long long)m[i];
    m[i] = (unsigned int)(temp / divisor);
    carry = temp % divisor;
  }
}
static void _reduce_scale(unsigned int m[3], int* scale, int* target_scale) {
  while (*scale > *target_scale) {
    _div_u96_small(m, 10);
    (*scale)--;
  }
}
static int _check_overflow(const unsigned int m[3]) {
  return (m[2] > 0x19999999ULL ||
          (m[2] == 0x19999999ULL && m[1] > 0x99999999ULL));
}

static void _scale_up(unsigned int m1[3], int* scale1, unsigned int m2[3],
                      int* scale2) {
  int diff = *scale2 - *scale1;
  for (int i = 0; i < diff; ++i) {
    if (_check_overflow(m1)) {
      _reduce_scale(m2, scale2, scale1);
      break;
    }
    _mul_u96_small(m1, 10);
    (*scale1)++;
  }
}

static void _align_scales_u96(unsigned int ma[3], int* scale_a,
                              unsigned int mb[3], int* scale_b) {
  if (*scale_a != *scale_b) {
    if (*scale_a < *scale_b) {
      _scale_up(ma, scale_a, mb, scale_b);
    } else {
      _scale_up(mb, scale_b, ma, scale_a);
    }
  }
}

int s21_is_less(s21_decimal a, s21_decimal b) {
  // возвращает 1 если а < b
  int result = 0;
  int a_zero = _is_zero_decimal(a);
  int b_zero = _is_zero_decimal(b);

  if (!(a_zero && b_zero)) {
    int sign_a = _get_sign(&a);
    int sign_b = _get_sign(&b);
    if (sign_a != sign_b) {
      result = (sign_a == 1 && sign_b == 0);
    } else {
      unsigned int ma[3], mb[3];
      _copy_mantissa_u96(&a, ma);
      _copy_mantissa_u96(&b, mb);
      int scale_a = _get_scale(&a);
      int scale_b = _get_scale(&b);
      _align_scales_u96(ma, &scale_a, mb, &scale_b);
      int cmp = _cmp_u96(ma, mb);
      if (cmp != 0) {
        result = (sign_a == 1) ? (cmp > 0) : (cmp < 0);
      }
    }
  }
  return result;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  // // возвращает 1 если а <= b
  int result = 0;
  int a_zero = _is_zero_decimal(a);
  int b_zero = _is_zero_decimal(b);

  if (a_zero && b_zero) {
    result = 1;
  } else {
    int sign_a = _get_sign(&a);
    int sign_b = _get_sign(&b);
    if (sign_a != sign_b) {
      result = (sign_a == 1 && sign_b == 0);
    } else {
      unsigned int ma[3], mb[3];
      _copy_mantissa_u96(&a, ma);
      _copy_mantissa_u96(&b, mb);
      int scale_a = _get_scale(&a);
      int scale_b = _get_scale(&b);
      _align_scales_u96(ma, &scale_a, mb, &scale_b);
      int cmp = _cmp_u96(ma, mb);
      if (cmp == 0) {
        result = 1;
      } else if (sign_a == 1) {
        result = (cmp > 0);
      } else {
        result = (cmp < 0);
      }
    }
  }
  return result;
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  // возвращает 1 если а > b
  int result = 0;
  int a_zero = _is_zero_decimal(a);
  int b_zero = _is_zero_decimal(b);

  if (!(a_zero && b_zero)) {
    int sign_a = _get_sign(&a);
    int sign_b = _get_sign(&b);
    if (sign_a != sign_b) {
      result = (sign_a == 0 && sign_b == 1);
    } else {
      unsigned int ma[3], mb[3];
      _copy_mantissa_u96(&a, ma);
      _copy_mantissa_u96(&b, mb);
      int scale_a = _get_scale(&a);
      int scale_b = _get_scale(&b);
      _align_scales_u96(ma, &scale_a, mb, &scale_b);
      int cmp = _cmp_u96(ma, mb);
      if (cmp != 0) {
        result = (sign_a) ? (cmp < 0) : (cmp > 0);
      }
    }
  }
  return result;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  // возвращает 1 если а >= b
  int result = 0;
  int a_zero = _is_zero_decimal(a);
  int b_zero = _is_zero_decimal(b);

  if (a_zero && b_zero) {
    result = 1;
  } else {
    int sign_a = _get_sign(&a);
    int sign_b = _get_sign(&b);
    if (sign_a != sign_b) {
      result = (sign_a == 0 && sign_b == 1);
    } else {
      unsigned int ma[3], mb[3];
      _copy_mantissa_u96(&a, ma);
      _copy_mantissa_u96(&b, mb);
      int scale_a = _get_scale(&a);
      int scale_b = _get_scale(&b);
      _align_scales_u96(ma, &scale_a, mb, &scale_b);
      int cmp = _cmp_u96(ma, mb);
      if (cmp == 0) {
        result = 1;
      } else if (sign_a == 1) {
        result = (cmp < 0);
      } else {
        result = (cmp > 0);
      }
    }
  }
  return result;
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
  // возвращает 1 если а == b
  int result = 0;
  int a_zero = _is_zero_decimal(a);
  int b_zero = _is_zero_decimal(b);

  if (a_zero && b_zero) {
    result = 1;
  } else {
    int sign_a = _get_sign(&a);
    int sign_b = _get_sign(&b);
    if (sign_a != sign_b) {
      result = 0;
    } else {
      unsigned int ma[3], mb[3];
      _copy_mantissa_u96(&a, ma);
      _copy_mantissa_u96(&b, mb);
      int scale_a = _get_scale(&a);
      int scale_b = _get_scale(&b);
      _align_scales_u96(ma, &scale_a, mb, &scale_b);
      int cmp = _cmp_u96(ma, mb);
      if (cmp == 0) {
        result = 1;
      } else {
        result = 0;
      }
    }
  }
  return result;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  // возвращает 1 если а != b
  int result = 0;
  int a_zero = _is_zero_decimal(a);
  int b_zero = _is_zero_decimal(b);

  if (a_zero && b_zero) {
    result = 0;
  } else {
    int sign_a = _get_sign(&a);
    int sign_b = _get_sign(&b);
    if (sign_a != sign_b) {
      result = 1;
    } else {
      unsigned int ma[3], mb[3];
      _copy_mantissa_u96(&a, ma);
      _copy_mantissa_u96(&b, mb);
      int scale_a = _get_scale(&a);
      int scale_b = _get_scale(&b);
      _align_scales_u96(ma, &scale_a, mb, &scale_b);
      int cmp = _cmp_u96(ma, mb);
      if (cmp == 0) {
        result = 0;
      } else {
        result = 1;
      }
    }
  }
  return result;
}
