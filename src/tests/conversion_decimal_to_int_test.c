#include <limits.h>

#include "check_suites.h"

START_TEST(test_to_int_basic_positive) {
  s21_decimal src;
  int result;
  s21_from_int_to_decimal(123456, &src);

  int error = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result, 123456);
}
END_TEST

START_TEST(test_to_int_basic_negative) {
  s21_decimal src;
  int result;
  s21_from_int_to_decimal(-7890, &src);

  int error = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result, -7890);
}
END_TEST

START_TEST(test_to_int_zero) {
  s21_decimal src;
  int result;
  s21_from_int_to_decimal(0, &src);

  int error = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_to_int_max_positive) {
  s21_decimal src;
  int result;
  s21_from_int_to_decimal(INT_MAX, &src);

  int error = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result, INT_MAX);
}
END_TEST

START_TEST(test_to_int_min_negative) {
  s21_decimal src;
  int result;
  s21_from_int_to_decimal(INT_MIN, &src);

  int error = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result, INT_MIN);
}
END_TEST

START_TEST(test_to_int_with_fraction_truncate) {
  s21_decimal src;
  int result;
  s21_from_float_to_decimal(123.456f, &src);

  int error = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result, 123);
}
END_TEST

START_TEST(test_to_int_with_fraction_negative_truncate) {
  s21_decimal src;
  int result;
  s21_from_float_to_decimal(-78.9f, &src);

  int error = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result, -78);
}
END_TEST

START_TEST(test_to_int_overflow_negative) {
  s21_decimal src;
  int result;

  // Используем число которое точно представимо в float
  s21_from_float_to_decimal(-3000000000000.0f, &src);

  int error = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(error, 1);
}

START_TEST(test_to_int_null_pointer) {
  s21_decimal src;
  s21_from_int_to_decimal(42, &src);

  int error = s21_from_decimal_to_int(src, NULL);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_to_int_large_decimal) {
  s21_decimal src;
  int result;
  src.bits[0] = 0xFFFFFFFF;
  src.bits[1] = 0xFFFFFFFF;
  src.bits[2] = 0xFFFFFFFF;
  _set_scale(&src, 0);

  int error = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_to_int_small_fraction) {
  s21_decimal src;
  int result;
  s21_from_float_to_decimal(0.999f, &src);

  int error = s21_from_decimal_to_int(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result, 0);
}
END_TEST

Suite* decimal_to_int_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_decimal_to_int");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_to_int_basic_positive);
  tcase_add_test(tc_core, test_to_int_basic_negative);
  tcase_add_test(tc_core, test_to_int_zero);
  tcase_add_test(tc_core, test_to_int_max_positive);
  tcase_add_test(tc_core, test_to_int_min_negative);
  tcase_add_test(tc_core, test_to_int_with_fraction_truncate);
  tcase_add_test(tc_core, test_to_int_with_fraction_negative_truncate);
  tcase_add_test(tc_core, test_to_int_overflow_negative);
  tcase_add_test(tc_core, test_to_int_null_pointer);
  tcase_add_test(tc_core, test_to_int_large_decimal);
  tcase_add_test(tc_core, test_to_int_small_fraction);

  suite_add_tcase(s, tc_core);
  return s;
}