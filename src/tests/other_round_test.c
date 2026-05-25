#include "check_suites.h"

START_TEST(test_round_positive_no_fraction) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(123, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 123);
}
END_TEST

START_TEST(test_round_positive_round_down) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(123.4f, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 123);
}
END_TEST

START_TEST(test_round_positive_round_up) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(123.6f, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 124);
}
END_TEST

START_TEST(test_round_negative_no_fraction) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(-456, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -456);
}
END_TEST

START_TEST(test_round_negative_round_down) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-78.4f, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -78);
}
END_TEST

START_TEST(test_round_negative_round_up) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-78.6f, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -79);
}
END_TEST

START_TEST(test_round_zero) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(0, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 0);
}
END_TEST

START_TEST(test_round_exact_half_positive_even) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(2.5f, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 3);
}
END_TEST

START_TEST(test_round_exact_half_negative_even) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-2.5f, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -3);
}
END_TEST

START_TEST(test_round_exact_half_positive_odd) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(3.5f, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 4);
}
END_TEST

START_TEST(test_round_exact_half_negative_odd) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-3.5f, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -4);
}
END_TEST

START_TEST(test_round_small_positive) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(0.4f, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 0);
}
END_TEST

START_TEST(test_round_small_negative) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-0.4f, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 0);
}
END_TEST

START_TEST(test_round_large_number) {
  s21_decimal src = {
      {0x075BCCED, 0x00000000, 0x00000000, 0x00020000}};  // 1234567.49f
  s21_decimal result;

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 1234567);
}
END_TEST

START_TEST(test_round_large_number_up) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(1234567.51f, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 1234568);
}
END_TEST

START_TEST(test_round_null_pointer) {
  s21_decimal src;
  s21_from_float_to_decimal(123.456f, &src);

  int error = s21_round(src, NULL);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_round_precision) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(99.999f, &src);

  int error = s21_round(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 100);
}
END_TEST

Suite* round_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_round");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_round_positive_no_fraction);
  tcase_add_test(tc_core, test_round_positive_round_down);
  tcase_add_test(tc_core, test_round_positive_round_up);
  tcase_add_test(tc_core, test_round_negative_no_fraction);
  tcase_add_test(tc_core, test_round_negative_round_down);
  tcase_add_test(tc_core, test_round_negative_round_up);
  tcase_add_test(tc_core, test_round_zero);
  tcase_add_test(tc_core, test_round_exact_half_positive_even);
  tcase_add_test(tc_core, test_round_exact_half_negative_even);
  tcase_add_test(tc_core, test_round_exact_half_positive_odd);
  tcase_add_test(tc_core, test_round_exact_half_negative_odd);
  tcase_add_test(tc_core, test_round_small_positive);
  tcase_add_test(tc_core, test_round_small_negative);
  tcase_add_test(tc_core, test_round_large_number);
  tcase_add_test(tc_core, test_round_large_number_up);
  tcase_add_test(tc_core, test_round_null_pointer);
  tcase_add_test(tc_core, test_round_precision);

  suite_add_tcase(s, tc_core);
  return s;
}