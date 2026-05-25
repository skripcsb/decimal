#include <float.h>

#include "check_suites.h"

START_TEST(test_to_float_basic_positive) {
  s21_decimal src;
  float result;
  s21_from_float_to_decimal(123.456f, &src);

  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_float_eq_tol(result, 123.456f, 1e-5f);
}
END_TEST

START_TEST(test_to_float_basic_negative) {
  s21_decimal src;
  float result;
  s21_from_float_to_decimal(-78.9f, &src);

  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_float_eq_tol(result, -78.9f, 1e-5f);
}
END_TEST

START_TEST(test_to_float_zero) {
  s21_decimal src;
  float result;
  s21_from_int_to_decimal(0, &src);

  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_float_eq(result, 0.0f);
}
END_TEST

START_TEST(test_to_float_small_value) {
  s21_decimal src;
  float result;
  s21_from_float_to_decimal(0.000001f, &src);

  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_float_eq_tol(result, 0.000001f, 1e-7f);
}
END_TEST

START_TEST(test_to_float_large_value) {
  s21_decimal src;
  float result;
  s21_from_float_to_decimal(1e7f, &src);

  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_float_eq_tol(result, 1e7f, 1e-1f);
}
END_TEST

START_TEST(test_to_float_precision) {
  s21_decimal src;
  float result;
  s21_from_float_to_decimal(3.14159f, &src);

  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_float_eq_tol(result, 3.14159f, 1e-5f);
}
END_TEST

START_TEST(test_to_float_negative_zero) {
  s21_decimal src;
  float result;
  s21_from_int_to_decimal(0, &src);
  _set_sign(&src, 1);

  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(test_to_float_overflow) {
  s21_decimal src;
  float result;
  src.bits[0] = 0xFFFFFFFF;
  src.bits[1] = 0xFFFFFFFF;
  src.bits[2] = 0xFFFFFFFF;
  _set_scale(&src, 0);

  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(test_to_float_negative_overflow) {
  s21_decimal src;
  float result;
  src.bits[0] = 0xFFFFFFFF;
  src.bits[1] = 0xFFFFFFFF;
  src.bits[2] = 0xFFFFFFFF;
  _set_scale(&src, 0);
  _set_sign(&src, 1);

  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(test_to_float_null_pointer) {
  s21_decimal src;
  s21_from_float_to_decimal(123.456f, &src);

  int error = s21_from_decimal_to_float(src, NULL);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_to_float_very_small) {
  s21_decimal src;
  float result;
  s21_from_float_to_decimal(1e-20f, &src);

  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(test_to_float_rounding) {
  s21_decimal src;
  float result;
  s21_from_float_to_decimal(12.3456789f, &src);

  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_float_eq_tol(result, 12.3456789f, 1e-6f);
}
END_TEST

START_TEST(test_to_float_integer) {
  s21_decimal src;
  float result;
  s21_from_int_to_decimal(42, &src);

  int error = s21_from_decimal_to_float(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_float_eq(result, 42.0f);
}
END_TEST

Suite* decimal_to_float_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_decimal_to_float");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_to_float_basic_positive);
  tcase_add_test(tc_core, test_to_float_basic_negative);
  tcase_add_test(tc_core, test_to_float_zero);
  tcase_add_test(tc_core, test_to_float_small_value);
  tcase_add_test(tc_core, test_to_float_large_value);
  tcase_add_test(tc_core, test_to_float_precision);
  tcase_add_test(tc_core, test_to_float_negative_zero);
  tcase_add_test(tc_core, test_to_float_overflow);
  tcase_add_test(tc_core, test_to_float_negative_overflow);
  tcase_add_test(tc_core, test_to_float_null_pointer);
  tcase_add_test(tc_core, test_to_float_very_small);
  tcase_add_test(tc_core, test_to_float_rounding);
  tcase_add_test(tc_core, test_to_float_integer);

  suite_add_tcase(s, tc_core);
  return s;
}