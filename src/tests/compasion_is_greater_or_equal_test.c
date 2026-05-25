#include "check_suites.h"

START_TEST(test_is_greater_or_equal_positive_positive_true) {
  s21_decimal value_1, value_2;
  s21_from_int_to_decimal(456, &value_1);
  s21_from_int_to_decimal(123, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_positive_positive_false) {
  s21_decimal value_1, value_2;
  s21_from_int_to_decimal(123, &value_1);
  s21_from_int_to_decimal(456, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_negative_negative_true) {
  s21_decimal value_1, value_2;
  s21_from_int_to_decimal(-123, &value_1);
  s21_from_int_to_decimal(-456, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_negative_negative_false) {
  s21_decimal value_1, value_2;
  s21_from_int_to_decimal(-456, &value_1);
  s21_from_int_to_decimal(-123, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_positive_negative_true) {
  s21_decimal value_1, value_2;
  s21_from_int_to_decimal(123, &value_1);
  s21_from_int_to_decimal(-456, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_negative_positive_false) {
  s21_decimal value_1, value_2;
  s21_from_int_to_decimal(-123, &value_1);
  s21_from_int_to_decimal(456, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_equal_values_true) {
  s21_decimal value_1, value_2;
  s21_from_int_to_decimal(100, &value_1);
  s21_from_int_to_decimal(100, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_with_fraction_true) {
  s21_decimal value_1, value_2;
  s21_from_float_to_decimal(12.35f, &value_1);
  s21_from_float_to_decimal(12.34f, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_with_fraction_equal) {
  s21_decimal value_1, value_2;
  s21_from_float_to_decimal(12.34f, &value_1);
  s21_from_float_to_decimal(12.34f, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_with_fraction_false) {
  s21_decimal value_1, value_2;
  s21_from_float_to_decimal(12.34f, &value_1);
  s21_from_float_to_decimal(12.35f, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_different_scales_true) {
  s21_decimal value_1, value_2;
  s21_from_float_to_decimal(1.24f, &value_1);
  s21_from_float_to_decimal(1.234f, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_different_scales_equal) {
  s21_decimal value_1, value_2;
  s21_from_float_to_decimal(1.23f, &value_1);
  s21_from_float_to_decimal(1.230f, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_different_scales_false) {
  s21_decimal value_1, value_2;
  s21_from_float_to_decimal(1.234f, &value_1);
  s21_from_float_to_decimal(1.24f, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_zero_positive_false) {
  s21_decimal value_1, value_2;
  s21_from_int_to_decimal(0, &value_1);
  s21_from_int_to_decimal(1, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_zero_zero_true) {
  s21_decimal value_1, value_2;
  s21_from_int_to_decimal(0, &value_1);
  s21_from_int_to_decimal(0, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_zero_negative_true) {
  s21_decimal value_1, value_2;
  s21_from_int_to_decimal(0, &value_1);
  s21_from_int_to_decimal(-1, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_zero_negative_false) {
  s21_decimal value_1, value_2;
  s21_from_int_to_decimal(-1, &value_1);
  s21_from_int_to_decimal(0, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(test_is_greater_or_equal_small_fractions_true) {
  s21_decimal value_1, value_2;
  s21_from_float_to_decimal(0.002f, &value_1);
  s21_from_float_to_decimal(0.001f, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_small_fractions_equal) {
  s21_decimal value_1, value_2;
  s21_from_float_to_decimal(0.001f, &value_1);
  s21_from_float_to_decimal(0.001f, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_is_greater_or_equal_small_fractions_false) {
  s21_decimal value_1, value_2;
  s21_from_float_to_decimal(0.001f, &value_1);
  s21_from_float_to_decimal(0.002f, &value_2);

  int result = s21_is_greater_or_equal(value_1, value_2);

  ck_assert_int_eq(result, 0);
}
END_TEST

Suite* is_greater_or_equal_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_is_greater_or_equal");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_is_greater_or_equal_positive_positive_true);
  tcase_add_test(tc_core, test_is_greater_or_equal_positive_positive_false);
  tcase_add_test(tc_core, test_is_greater_or_equal_negative_negative_true);
  tcase_add_test(tc_core, test_is_greater_or_equal_negative_negative_false);
  tcase_add_test(tc_core, test_is_greater_or_equal_positive_negative_true);
  tcase_add_test(tc_core, test_is_greater_or_equal_negative_positive_false);
  tcase_add_test(tc_core, test_is_greater_or_equal_equal_values_true);
  tcase_add_test(tc_core, test_is_greater_or_equal_with_fraction_true);
  tcase_add_test(tc_core, test_is_greater_or_equal_with_fraction_equal);
  tcase_add_test(tc_core, test_is_greater_or_equal_with_fraction_false);
  tcase_add_test(tc_core, test_is_greater_or_equal_different_scales_true);
  tcase_add_test(tc_core, test_is_greater_or_equal_different_scales_equal);
  tcase_add_test(tc_core, test_is_greater_or_equal_different_scales_false);
  tcase_add_test(tc_core, test_is_greater_or_equal_zero_positive_false);
  tcase_add_test(tc_core, test_is_greater_or_equal_zero_zero_true);
  tcase_add_test(tc_core, test_is_greater_or_equal_zero_negative_true);
  tcase_add_test(tc_core, test_is_greater_or_equal_zero_negative_false);
  tcase_add_test(tc_core, test_is_greater_or_equal_small_fractions_true);
  tcase_add_test(tc_core, test_is_greater_or_equal_small_fractions_equal);
  tcase_add_test(tc_core, test_is_greater_or_equal_small_fractions_false);

  suite_add_tcase(s, tc_core);
  return s;
}