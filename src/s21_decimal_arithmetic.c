#include <stdint.h>

#include "s21_decimal.h"
#include "s21_decimal_common.h"

static int _get_bit(s21_decimal value, int position) {
  int bit_index = position / 32;
  int bit_position = position % 32;
  return (value.bits[bit_index] >> bit_position) & 1;
}

static void _set_bit(s21_decimal *value, int position, int bit) {
  int bit_index = position / 32;
  int bit_position = position % 32;
  if (bit) {
    value->bits[bit_index] |= (1U << bit_position);
  } else {
    value->bits[bit_index] &= ~(1U << bit_position);
  }
}

static void _shift_left(s21_decimal *value) {
  int high_bit_0 = (value->bits[0] & 0x80000000) != 0;
  int high_bit_1 = (value->bits[1] & 0x80000000) != 0;

  value->bits[0] <<= 1;
  value->bits[1] <<= 1;
  value->bits[2] <<= 1;

  if (high_bit_0) value->bits[1] |= 1;
  if (high_bit_1) value->bits[2] |= 1;
}

static int _add_simple(s21_decimal value_1, s21_decimal value_2,
                       s21_decimal *result) {
  unsigned long long carry = 0;
  for (int i = 0; i < 96; i++) {
    int bit1 = _get_bit(value_1, i);
    int bit2 = _get_bit(value_2, i);
    unsigned long long sum = bit1 + bit2 + carry;
    _set_bit(result, i, sum & 1);
    carry = sum >> 1;
  }
  return carry;
}

static void _multiply_by_ten(s21_decimal *value) {
  s21_decimal temp = *value;

  for (int i = 0; i < 3; i++) {
    _shift_left(value);
  }

  s21_decimal temp2 = temp;
  _shift_left(&temp2);

  _add_simple(*value, temp2, value);
}

// Divide the 96-bit integer part of `value` by 10 (truncate remainder).
static void _normalize_scales(s21_decimal *value_1, s21_decimal *value_2) {
  int scale1 = _get_scale(value_1);
  int scale2 = _get_scale(value_2);

  while (scale1 != scale2) {
    if (scale1 < scale2) {
      _multiply_by_ten(value_1);
      scale1++;
      _set_scale(value_1, scale1);
    } else {
      _multiply_by_ten(value_2);
      scale2++;
      _set_scale(value_2, scale2);
    }
  }
}

static void _clamp_scale(int *scale) {
  if (*scale > 28) {
    *scale = 28;
  } else if (*scale < 0) {
    *scale = 0;
  }
}

static int _is_zero192(const uint32_t v[6]) {
  return v[0] == 0 && v[1] == 0 && v[2] == 0 && v[3] == 0 && v[4] == 0 &&
         v[5] == 0;
}

static int _cmp96(const uint32_t a[3], const uint32_t b[3]) {
  for (int i = 2; i >= 0; i--) {
    if (a[i] > b[i]) return 1;
    if (a[i] < b[i]) return -1;
  }
  return 0;
}

static int _cmp192_96(const uint32_t a[6], const uint32_t b[3]) {
  if (a[5] || a[4] || a[3]) return 1;
  uint32_t low[3] = {a[0], a[1], a[2]};
  return _cmp96(low, b);
}

static void _sub96(uint32_t a[3], const uint32_t b[3]) {
  uint64_t borrow = 0;
  for (int i = 0; i < 3; i++) {
    uint64_t cur = (uint64_t)a[i] - b[i] - borrow;
    a[i] = (uint32_t)cur;
    borrow = (cur >> 63) & 1U;
  }
}

static void _sub192_96(uint32_t a[6], const uint32_t b[3]) {
  uint64_t borrow = 0;
  for (int i = 0; i < 3; i++) {
    uint64_t cur = (uint64_t)a[i] - b[i] - borrow;
    a[i] = (uint32_t)cur;
    borrow = (cur >> 63) & 1U;
  }
  for (int i = 3; i < 6 && borrow; i++) {
    uint64_t cur = (uint64_t)a[i] - borrow;
    a[i] = (uint32_t)cur;
    borrow = (cur >> 63) & 1U;
  }
}

static void _shl96_add_bit(uint32_t v[3], int bit) {
  uint32_t carry = (bit != 0);
  for (int i = 0; i < 3; i++) {
    uint64_t cur = ((uint64_t)v[i] << 1) | carry;
    v[i] = (uint32_t)cur;
    carry = (uint32_t)(cur >> 32);
  }
}

static int _get_bit192(const uint32_t v[6], int pos) {
  int idx = pos / 32;
  int off = pos % 32;
  return (v[idx] >> off) & 1U;
}

static void _div192_by_96(const uint32_t dividend[6], const uint32_t divisor[3],
                          uint32_t quotient[6], uint32_t remainder[3]) {
  for (int i = 0; i < 6; i++) quotient[i] = 0;
  remainder[0] = remainder[1] = remainder[2] = 0;

  for (int bit = 191; bit >= 0; bit--) {
    _shl96_add_bit(remainder, _get_bit192(dividend, bit));
    if (_cmp96(remainder, divisor) >= 0) {
      _sub96(remainder, divisor);
      int q_idx = bit / 32;
      int q_off = bit % 32;
      quotient[q_idx] |= (1U << q_off);
    }
  }
}

static int _mul192_by_10(uint32_t v[6]) {
  uint64_t carry = 0;
  for (int i = 0; i < 6; i++) {
    uint64_t cur = (uint64_t)v[i] * 10ULL + carry;
    v[i] = (uint32_t)cur;
    carry = cur >> 32;
  }
  return carry != 0;
}

static int _mul96_by_10_add_digit(uint32_t v[3], int digit) {
  uint64_t carry = (uint64_t)digit;
  for (int i = 0; i < 3; i++) {
    uint64_t cur = (uint64_t)v[i] * 10ULL + carry;
    v[i] = (uint32_t)cur;
    carry = cur >> 32;
  }
  return carry != 0;
}

static int _cmp_double_rem_div(const uint32_t rem[6], const uint32_t div[3]) {
  uint32_t doubled[6];
  for (int i = 0; i < 6; i++) doubled[i] = rem[i];

  uint64_t carry = 0;
  for (int i = 0; i < 6; i++) {
    uint64_t cur = ((uint64_t)doubled[i] << 1) | carry;
    doubled[i] = (uint32_t)cur;
    carry = cur >> 32;
  }

  return _cmp192_96(doubled, div);
}

static int _add_one96(uint32_t v[3]) {
  uint64_t cur = (uint64_t)v[0] + 1ULL;
  v[0] = (uint32_t)cur;
  uint64_t carry = cur >> 32;
  for (int i = 1; i < 3 && carry; i++) {
    cur = (uint64_t)v[i] + carry;
    v[i] = (uint32_t)cur;
    carry = cur >> 32;
  }
  return carry != 0;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;

  if (!result) {
    error = 1;
  } else if (_is_zero_decimal(value_2)) {
    error = 3;
  } else {
    *result = s21_decimal_zero();

    int result_sign = _get_sign(&value_1) ^ _get_sign(&value_2);

    _set_sign(&value_1, 0);
    _set_sign(&value_2, 0);

    int scale1 = _get_scale(&value_1);
    int scale2 = _get_scale(&value_2);

    _set_scale(&value_1, 0);
    _set_scale(&value_2, 0);

    uint32_t dividend[6] = {
        value_1.bits[0], value_1.bits[1], value_1.bits[2], 0, 0, 0};
    uint32_t divisor[3] = {value_2.bits[0], value_2.bits[1], value_2.bits[2]};

    int result_scale = scale1 - scale2;

    if (result_scale < 0) {
      int lift = -result_scale;
      while (lift > 0 && !_mul192_by_10(dividend)) {
        result_scale++;
        lift--;
      }
    }

    uint32_t quotient192[6];
    uint32_t remainder96[3];
    _div192_by_96(dividend, divisor, quotient192, remainder96);

    if (quotient192[3] || quotient192[4] || quotient192[5]) {
      error = result_sign ? 2 : 1;
    } else {
      uint32_t q96[3] = {quotient192[0], quotient192[1], quotient192[2]};

      uint32_t rem192[6] = {
          remainder96[0], remainder96[1], remainder96[2], 0, 0, 0};

      int can_add_fractional = 1;
      while (!_is_zero192(rem192) && result_scale < 28 && !error &&
             can_add_fractional) {
        _mul192_by_10(rem192);
        int digit = 0;
        while (_cmp192_96(rem192, divisor) >= 0 && digit < 10) {
          _sub192_96(rem192, divisor);
          digit++;
        }

        uint32_t test_q[3] = {q96[0], q96[1], q96[2]};
        if (_mul96_by_10_add_digit(test_q, digit)) {
          can_add_fractional = 0;
        } else {
          q96[0] = test_q[0];
          q96[1] = test_q[1];
          q96[2] = test_q[2];
          result_scale++;
        }
      }

      if (!error && !_is_zero192(rem192)) {
        int cmp = _cmp_double_rem_div(rem192, divisor);
        if (cmp > 0 || (cmp == 0 && (q96[0] & 1))) {
          if (_add_one96(q96)) {
            error = result_sign ? 2 : 1;
          }
        }
      }

      if (!error) {
        _clamp_scale(&result_scale);
        result->bits[0] = q96[0];
        result->bits[1] = q96[1];
        result->bits[2] = q96[2];
        _set_sign(result, result_sign);
        _set_scale(result, result_scale);
      }
    }
  }

  return error;
}

static void add_96_to_192(uint32_t a[3], uint32_t b[3], uint32_t res[6]) {
  uint64_t carry = 0;
  for (int i = 0; i < 3; i++) {
    uint64_t sum = (uint64_t)a[i] + b[i] + carry;
    res[i] = (uint32_t)sum;
    carry = sum >> 32;
  }
  for (int i = 3; i < 6; i++) {
    res[i] = 0;
  }
  res[3] = (uint32_t)carry;
}

static void sub_96_from_96(uint32_t a[3], uint32_t b[3], uint32_t res[3]) {
  uint64_t borrow = 0;
  for (int i = 0; i < 3; i++) {
    uint64_t diff = (uint64_t)a[i] - b[i] - borrow;
    res[i] = (uint32_t)diff;
    borrow = (diff >> 63) & 1U;
  }
}

static int cmp_96(const uint32_t a[3], const uint32_t b[3]) {
  for (int i = 2; i >= 0; i--) {
    if (a[i] > b[i]) return 1;
    if (a[i] < b[i]) return -1;
  }
  return 0;
}

static int fit_192_to_96_add(uint32_t src[6], uint32_t dst[3], int *scale,
                             int sign) {
  int error = 0;

  while ((src[3] || src[4] || src[5] || *scale > 28) && *scale > 0) {
    uint64_t rem = 0;
    for (int i = 5; i >= 0; i--) {
      uint64_t cur = (rem << 32) | src[i];
      src[i] = (uint32_t)(cur / 10);
      rem = cur % 10;
    }

    if (rem > 5 || (rem == 5 && (src[0] & 1))) {
      uint64_t carry = 1;
      for (int i = 0; i < 6; i++) {
        uint64_t s = (uint64_t)src[i] + carry;
        src[i] = (uint32_t)s;
        carry = s >> 32;
        if (!carry) i = 6;  // Skip remaining iterations
      }
    }

    (*scale)--;
  }

  if (src[3] || src[4] || src[5]) {
    error = sign ? 2 : 1;
  } else {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
  }

  return error;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;

  if (!result) {
    error = 1;
  } else {
    *result = s21_decimal_zero();

    int sign1 = _get_sign(&value_1);
    int sign2 = _get_sign(&value_2);

    _normalize_scales(&value_1, &value_2);
    int result_scale = _get_scale(&value_1);

    _set_sign(&value_1, 0);
    _set_sign(&value_2, 0);

    uint32_t a[3] = {value_1.bits[0], value_1.bits[1], value_1.bits[2]};
    uint32_t b[3] = {value_2.bits[0], value_2.bits[1], value_2.bits[2]};

    if (sign1 == sign2) {
      // Same sign: add magnitudes
      uint32_t wide[6];
      add_96_to_192(a, b, wide);

      uint32_t res96[3];
      error = fit_192_to_96_add(wide, res96, &result_scale, sign1);

      if (!error) {
        result->bits[0] = res96[0];
        result->bits[1] = res96[1];
        result->bits[2] = res96[2];
        _set_sign(result, sign1);
        _set_scale(result, result_scale);
      }
    } else {
      // Different signs: subtract smaller from larger
      int cmp = cmp_96(a, b);
      int result_sign = sign1;

      uint32_t res96[3];
      if (cmp >= 0) {
        sub_96_from_96(a, b, res96);
      } else {
        sub_96_from_96(b, a, res96);
        result_sign = sign2;
      }

      result->bits[0] = res96[0];
      result->bits[1] = res96[1];
      result->bits[2] = res96[2];
      _set_sign(result, result_sign);
      _set_scale(result, result_scale);
    }
  }

  return error;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;

  if (!result) {
    error = 1;
  } else {
    *result = s21_decimal_zero();

    int sign1 = _get_sign(&value_1);
    int sign2 = _get_sign(&value_2);

    _normalize_scales(&value_1, &value_2);
    int result_scale = _get_scale(&value_1);

    _set_sign(&value_1, 0);
    _set_sign(&value_2, 0);

    uint32_t a[3] = {value_1.bits[0], value_1.bits[1], value_1.bits[2]};
    uint32_t b[3] = {value_2.bits[0], value_2.bits[1], value_2.bits[2]};

    if (sign1 != sign2) {
      // Different signs: add magnitudes
      uint32_t wide[6];
      add_96_to_192(a, b, wide);

      uint32_t res96[3];
      error = fit_192_to_96_add(wide, res96, &result_scale, sign1);

      if (!error) {
        result->bits[0] = res96[0];
        result->bits[1] = res96[1];
        result->bits[2] = res96[2];
        _set_sign(result, sign1);
        _set_scale(result, result_scale);
      }
    } else {
      // Same sign: subtract smaller from larger
      int cmp = cmp_96(a, b);
      int result_sign = sign1;

      uint32_t res96[3];
      if (cmp >= 0) {
        sub_96_from_96(a, b, res96);
      } else {
        sub_96_from_96(b, a, res96);
        result_sign = !sign1;
      }

      result->bits[0] = res96[0];
      result->bits[1] = res96[1];
      result->bits[2] = res96[2];
      _set_sign(result, result_sign);
      _set_scale(result, result_scale);
    }
  }

  return error;
}

static void mul_96_to_192(s21_decimal a, s21_decimal b, uint32_t res[6]) {
  for (int i = 0; i < 6; i++) res[i] = 0;

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      uint64_t cur = (uint64_t)a.bits[i] * b.bits[j];
      int k = i + j;

      uint64_t sum = (uint64_t)res[k] + cur;
      res[k] = (uint32_t)sum;
      uint64_t carry = sum >> 32;

      k++;
      while (carry && k < 6) {
        uint64_t s = (uint64_t)res[k] + carry;
        res[k] = (uint32_t)s;
        carry = s >> 32;
        k++;
      }
    }
  }
}

static int fit_192_to_96(uint32_t src[6], s21_decimal *dst, int *scale,
                         int sign) {
  int error = 0;

  while ((src[3] || src[4] || src[5] || *scale > 28) && *scale > 0) {
    uint64_t rem = 0;
    for (int i = 5; i >= 0; i--) {
      uint64_t cur = (rem << 32) | src[i];
      src[i] = (uint32_t)(cur / 10);
      rem = cur % 10;
    }

    if (rem > 5 || (rem == 5 && (src[0] & 1))) {
      uint64_t carry = 1;
      for (int i = 0; i < 6 && carry; i++) {
        uint64_t s = (uint64_t)src[i] + carry;
        src[i] = (uint32_t)s;
        carry = s >> 32;
      }
    }

    (*scale)--;
  }

  if (src[3] || src[4] || src[5]) {
    error = sign ? 2 : 1;
  } else {
    dst->bits[0] = src[0];
    dst->bits[1] = src[1];
    dst->bits[2] = src[2];
  }

  return error;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;

  if (!result) {
    error = 1;
  } else {
    *result = s21_decimal_zero();

    int sign = _get_sign(&value_1) ^ _get_sign(&value_2);
    int scale = _get_scale(&value_1) + _get_scale(&value_2);

    _set_sign(&value_1, 0);
    _set_sign(&value_2, 0);
    _set_scale(&value_1, 0);
    _set_scale(&value_2, 0);

    uint32_t wide[6];
    mul_96_to_192(value_1, value_2, wide);

    error = fit_192_to_96(wide, result, &scale, sign);

    if (!error) {
      _set_sign(result, sign);
      _set_scale(result, scale);
    }
  }

  return error;
}
