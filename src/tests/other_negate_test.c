#include "check_suites.h"

START_TEST(test_negate_positive) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(123, &src);

  int error = s21_negate(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 1);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -123);
}
END_TEST

START_TEST(test_negate_negative) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(-456, &src);

  int error = s21_negate(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 456);
}
END_TEST

START_TEST(test_negate_zero) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(0, &src);

  int error = s21_negate(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 1);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 0);
}
END_TEST

START_TEST(test_negate_negative_zero) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(0, &src);
  _set_sign(&src, 1);

  int error = s21_negate(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 0);
}
END_TEST

START_TEST(test_negate_with_fraction) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(123.456f, &src);

  int error = s21_negate(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 1);

  float value;
  s21_from_decimal_to_float(result, &value);
  ck_assert_float_eq_tol(value, -123.456f, 1e-5f);
}
END_TEST

START_TEST(test_negate_negative_with_fraction) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-78.9f, &src);

  int error = s21_negate(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 0);

  float value;
  s21_from_decimal_to_float(result, &value);
  ck_assert_float_eq_tol(value, 78.9f, 1e-5f);
}
END_TEST

START_TEST(test_negate_preserves_scale) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(123.456f, &src);
  int original_scale = _get_scale(&src);

  int error = s21_negate(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_scale(&result), original_scale);
}
END_TEST

START_TEST(test_negate_preserves_value) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(123.456f, &src);

  int error = s21_negate(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(result.bits[0], src.bits[0]);
  ck_assert_int_eq(result.bits[1], src.bits[1]);
  ck_assert_int_eq(result.bits[2], src.bits[2]);
}
END_TEST

START_TEST(test_negate_null_pointer) {
  s21_decimal src;
  s21_from_int_to_decimal(42, &src);

  int error = s21_negate(src, NULL);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_negate_large_number) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(2147483647, &src);

  int error = s21_negate(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 1);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -2147483647);
}
END_TEST

Suite* negate_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_negate");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_negate_positive);
  tcase_add_test(tc_core, test_negate_negative);
  tcase_add_test(tc_core, test_negate_zero);
  tcase_add_test(tc_core, test_negate_negative_zero);
  tcase_add_test(tc_core, test_negate_with_fraction);
  tcase_add_test(tc_core, test_negate_negative_with_fraction);
  tcase_add_test(tc_core, test_negate_preserves_scale);
  tcase_add_test(tc_core, test_negate_preserves_value);
  tcase_add_test(tc_core, test_negate_null_pointer);
  tcase_add_test(tc_core, test_negate_large_number);

  suite_add_tcase(s, tc_core);
  return s;
}