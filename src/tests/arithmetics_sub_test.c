#include "check_suites.h"

START_TEST(test_sub_positive_positive) {
  s21_decimal value_1 = {{456, 0, 0, 0}};
  s21_decimal value_2 = {{123, 0, 0, 0}};
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{333, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_positive_negative) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  s21_decimal value_2 = {{50, 0, 0, 0x80000000}};
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{150, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_negative_positive) {
  s21_decimal value_1 = {{100, 0, 0, 0x80000000}};
  s21_decimal value_2 = {{50, 0, 0, 0}};
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{150, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_negative_negative) {
  s21_decimal value_1 = {{100, 0, 0, 0x80000000}};
  s21_decimal value_2 = {{50, 0, 0, 0x80000000}};
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{50, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_zero_zero) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_equal) {
  s21_decimal value_1 = {{456, 0, 0, 0}};
  s21_decimal value_2 = {{456, 0, 0, 0}};
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_with_fraction) {
  s21_decimal value_1 = {{5678, 0, 0, 0x00020000}};  // 56.78
  s21_decimal value_2 = {{1234, 0, 0, 0x00020000}};  // 12.34
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{4444, 0, 0, 0x00020000}};  // 44.44
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_different_scales) {
  s21_decimal value_1 = {{4567, 0, 0, 0x00030000}};  // 4.567
  s21_decimal value_2 = {{123, 0, 0, 0x00020000}};   // 1.23
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{3337, 0, 0, 0x00030000}};  // 3.337
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_overflow_positive) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};  // MAX
  s21_decimal value_2 = {{1, 0, 0, 0x80000000}};                    // -1
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_sub_overflow_negative) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  _set_sign(&value_1, 1);

  s21_decimal value_2 = {{1, 0, 0, 0}};
  _set_sign(&value_2, 0);

  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 2);
}
END_TEST

START_TEST(test_sub_null_pointer) {
  s21_decimal value_1 = {{456, 0, 0, 0}};
  s21_decimal value_2 = {{123, 0, 0, 0}};

  int error = s21_sub(value_1, value_2, NULL);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_sub_negative_result) {
  s21_decimal value_1 = {{123, 0, 0, 0}};
  s21_decimal value_2 = {{456, 0, 0, 0}};
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  s21_decimal expected = {{333, 0, 0, 0}};
  _set_sign(&expected, 1);

  ck_assert_msg(error == 0, "Error code: %d", error);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_identity) {
  s21_decimal value_1 = {{123, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  ck_assert_int_eq(s21_is_equal(result, value_1), 1);
}
END_TEST

START_TEST(test_sub_from_zero) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{123, 0, 0, 0}};
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{123, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_large_numbers) {
  s21_decimal value_1 = {{3000000000U, 0, 0, 0}};
  s21_decimal value_2 = {{1000000000U, 0, 0, 0}};
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{2000000000U, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_sub_negative_with_fraction) {
  s21_decimal value_1 = {{1234, 0, 0, 0x80020000}};  // -12.34
  s21_decimal value_2 = {{5678, 0, 0, 0x80020000}};  // -56.78
  s21_decimal result;

  int error = s21_sub(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{4444, 0, 0, 0x00020000}};  // 44.44
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

Suite* sub_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_sub");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_sub_positive_positive);
  tcase_add_test(tc_core, test_sub_positive_negative);
  tcase_add_test(tc_core, test_sub_negative_positive);
  tcase_add_test(tc_core, test_sub_negative_negative);
  tcase_add_test(tc_core, test_sub_zero_zero);
  tcase_add_test(tc_core, test_sub_equal);
  tcase_add_test(tc_core, test_sub_with_fraction);
  tcase_add_test(tc_core, test_sub_different_scales);
  tcase_add_test(tc_core, test_sub_overflow_positive);
  tcase_add_test(tc_core, test_sub_overflow_negative);
  tcase_add_test(tc_core, test_sub_null_pointer);
  tcase_add_test(tc_core, test_sub_negative_result);
  tcase_add_test(tc_core, test_sub_identity);
  tcase_add_test(tc_core, test_sub_from_zero);
  tcase_add_test(tc_core, test_sub_large_numbers);
  tcase_add_test(tc_core, test_sub_negative_with_fraction);

  suite_add_tcase(s, tc_core);
  return s;
}