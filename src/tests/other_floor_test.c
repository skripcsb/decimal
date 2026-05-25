#include "check_suites.h"

START_TEST(test_floor_positive_no_fraction) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(123, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 123);
}
END_TEST

START_TEST(test_floor_positive_with_fraction) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(123.456f, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 123);
}
END_TEST

START_TEST(test_floor_negative_no_fraction) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(-456, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -456);
}
END_TEST

START_TEST(test_floor_negative_with_fraction) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-78.9f, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -79);
}
END_TEST

START_TEST(test_floor_zero) {
  s21_decimal src;
  s21_decimal result;
  s21_from_int_to_decimal(0, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 0);
}
END_TEST

START_TEST(test_floor_small_positive) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(0.999f, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 0);
}
END_TEST

START_TEST(test_floor_small_negative) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-0.001f, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -1);
}
END_TEST

START_TEST(test_floor_boundary_positive) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(99.999f, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 99);
}
END_TEST

START_TEST(test_floor_boundary_negative) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-100.001f, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -101);
}
END_TEST

START_TEST(test_floor_large_number) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(1234567.89f, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 1234568);
}
END_TEST

START_TEST(test_floor_large_negative) {
  s21_decimal src = {
      {0x3ADE68B1, 0x00000000, 0x00000000, 0x80020000}};  //-9876543.21f
  s21_decimal result;

  // s21_from_float_to_decimal(-9876543.21f, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -9876544);
}
END_TEST

START_TEST(test_floor_null_pointer) {
  s21_decimal src;
  s21_from_float_to_decimal(123.456f, &src);

  int error = s21_floor(src, NULL);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_floor_exact_half) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(2.5f, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, 2);
}
END_TEST

START_TEST(test_floor_negative_exact_half) {
  s21_decimal src;
  s21_decimal result;
  s21_from_float_to_decimal(-2.5f, &src);

  int error = s21_floor(src, &result);

  ck_assert_int_eq(error, 0);

  int value;
  s21_from_decimal_to_int(result, &value);
  ck_assert_int_eq(value, -3);
}
END_TEST

Suite* floor_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_floor");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_floor_positive_no_fraction);
  tcase_add_test(tc_core, test_floor_positive_with_fraction);
  tcase_add_test(tc_core, test_floor_negative_no_fraction);
  tcase_add_test(tc_core, test_floor_negative_with_fraction);
  tcase_add_test(tc_core, test_floor_zero);
  tcase_add_test(tc_core, test_floor_small_positive);
  tcase_add_test(tc_core, test_floor_small_negative);
  tcase_add_test(tc_core, test_floor_boundary_positive);
  tcase_add_test(tc_core, test_floor_boundary_negative);
  tcase_add_test(tc_core, test_floor_large_number);
  tcase_add_test(tc_core, test_floor_large_negative);
  tcase_add_test(tc_core, test_floor_null_pointer);
  tcase_add_test(tc_core, test_floor_exact_half);
  tcase_add_test(tc_core, test_floor_negative_exact_half);

  suite_add_tcase(s, tc_core);
  return s;
}