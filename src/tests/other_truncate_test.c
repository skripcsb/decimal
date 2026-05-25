#include "check_suites.h"

START_TEST(test_truncate_positive_no_fraction) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(123, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 123);
}
END_TEST

START_TEST(test_truncate_positive_with_fraction) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(123.456f, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 123);
}
END_TEST

START_TEST(test_truncate_negative_no_fraction) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(-456, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -456);
}
END_TEST

START_TEST(test_truncate_negative_with_fraction) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-78.9f, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -78);
}
END_TEST

START_TEST(test_truncate_zero) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(0, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 0);
}
END_TEST

START_TEST(test_truncate_small_positive) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(0.999f, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 0);
}
END_TEST

START_TEST(test_truncate_small_negative) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-0.001f, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 0);
}
END_TEST

START_TEST(test_truncate_large_positive) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(1234567.89f, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 1234568);
}
END_TEST

START_TEST(test_truncate_large_negative) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-9876543.21f, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -9876543);
}
END_TEST

START_TEST(test_truncate_null_pointer) {
  s21_decimal src;
  s21_from_float_to_decimal(123.456f, &src);

  int error = s21_truncate(src, NULL);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_truncate_exact_half) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(2.5f, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 2);
}
END_TEST

START_TEST(test_truncate_negative_exact_half) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-2.5f, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -2);
}
END_TEST

START_TEST(test_truncate_already_integer) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(42, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 42);
}
END_TEST

START_TEST(test_truncate_preserves_sign) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-123.456f, &src);

  int error = s21_truncate(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 1);
}
END_TEST

Suite* truncate_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_truncate");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_truncate_positive_no_fraction);
  tcase_add_test(tc_core, test_truncate_positive_with_fraction);
  tcase_add_test(tc_core, test_truncate_negative_no_fraction);
  tcase_add_test(tc_core, test_truncate_negative_with_fraction);
  tcase_add_test(tc_core, test_truncate_zero);
  tcase_add_test(tc_core, test_truncate_small_positive);
  tcase_add_test(tc_core, test_truncate_small_negative);
  tcase_add_test(tc_core, test_truncate_large_positive);
  tcase_add_test(tc_core, test_truncate_large_negative);
  tcase_add_test(tc_core, test_truncate_null_pointer);
  tcase_add_test(tc_core, test_truncate_exact_half);
  tcase_add_test(tc_core, test_truncate_negative_exact_half);
  tcase_add_test(tc_core, test_truncate_already_integer);
  tcase_add_test(tc_core, test_truncate_preserves_sign);

  suite_add_tcase(s, tc_core);
  return s;
}