#include "check_suites.h"

START_TEST(test_add_positive_positive) {
  s21_decimal value_1 = {{123, 0, 0, 0}};
  s21_decimal value_2 = {{456, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{579, 0, 0, 0}};

  int error = s21_add(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_positive_negative) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  s21_decimal value_2 = {{50, 0, 0, 0}};
  _set_sign(&value_2, 1);
  s21_decimal result;
  s21_decimal expected = {{50, 0, 0, 0}};

  int error = s21_add(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_negative_positive) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  _set_sign(&value_1, 1);
  s21_decimal value_2 = {{50, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{50, 0, 0, 0}};
  _set_sign(&expected, 1);

  int error = s21_add(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_negative_negative) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  _set_sign(&value_1, 1);
  s21_decimal value_2 = {{50, 0, 0, 0}};
  _set_sign(&value_2, 1);
  s21_decimal result;
  s21_decimal expected = {{150, 0, 0, 0}};
  _set_sign(&expected, 1);

  int error = s21_add(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_zero_zero) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{0, 0, 0, 0}};

  int error = s21_add(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_with_fraction) {
  s21_decimal value_1 = {{1234, 0, 0, 0}};  // 12.34
  _set_scale(&value_1, 2);
  s21_decimal value_2 = {{5678, 0, 0, 0}};  // 56.78
  _set_scale(&value_2, 2);
  s21_decimal result;
  s21_decimal expected = {{6912, 0, 0, 0}};  // 69.12
  _set_scale(&expected, 2);

  int error = s21_add(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_different_scales) {
  s21_decimal value_1 = {{12300, 0, 0, 0}};  // 1.23
  _set_scale(&value_1, 4);
  s21_decimal value_2 = {{4567, 0, 0, 0}};  // 4.567
  _set_scale(&value_2, 3);
  s21_decimal result;
  s21_decimal expected = {{5797, 0, 0, 0}};  // 5.797
  _set_scale(&expected, 3);

  int error = s21_add(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_overflow_positive) {
  s21_decimal value_1 = {
      {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};  // Почти максимальное
  s21_decimal value_2 = {{1, 0, 0, 0}};
  s21_decimal result;

  int error = s21_add(value_1, value_2, &result);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_add_overflow_negative) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  _set_sign(&value_1, 1);
  s21_decimal value_2 = {{1, 0, 0, 0}};
  _set_sign(&value_2, 1);
  s21_decimal result;

  int error = s21_add(value_1, value_2, &result);

  ck_assert_int_eq(error, 2);
}
END_TEST

START_TEST(test_add_null_pointer) {
  s21_decimal value_1 = {{123, 0, 0, 0}};
  s21_decimal value_2 = {{456, 0, 0, 0}};

  int error = s21_add(value_1, value_2, NULL);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_add_commutative) {
  s21_decimal value_1 = {{123, 0, 0, 0}};
  s21_decimal value_2 = {{456, 0, 0, 0}};
  s21_decimal result1, result2;

  int error1 = s21_add(value_1, value_2, &result1);
  int error2 = s21_add(value_2, value_1, &result2);

  ck_assert_int_eq(error1, 0);
  ck_assert_int_eq(error2, 0);
  ck_assert_int_eq(s21_is_equal(result1, result2), 1);
}
END_TEST

START_TEST(test_add_identity) {
  s21_decimal value_1 = {{123, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0}};

  int error = s21_add(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_large_numbers) {
  s21_decimal value_1 = {{1000000000, 0, 0, 0}};
  s21_decimal value_2 = {{2000000000, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{3000000000U, 0, 0, 0}};

  int error = s21_add(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_add_negative_with_fraction) {
  s21_decimal value_1 = {{1234, 0, 0, 0}};  // 12.34
  _set_scale(&value_1, 2);
  _set_sign(&value_1, 1);
  s21_decimal value_2 = {{5678, 0, 0, 0}};  // 56.78
  _set_scale(&value_2, 2);
  _set_sign(&value_2, 1);
  s21_decimal result;
  s21_decimal expected = {{6912, 0, 0, 0}};  // 69.12
  _set_scale(&expected, 2);
  _set_sign(&expected, 1);

  int error = s21_add(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

Suite* add_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_add");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_add_positive_positive);
  tcase_add_test(tc_core, test_add_positive_negative);
  tcase_add_test(tc_core, test_add_negative_positive);
  tcase_add_test(tc_core, test_add_negative_negative);
  tcase_add_test(tc_core, test_add_zero_zero);
  tcase_add_test(tc_core, test_add_with_fraction);
  tcase_add_test(tc_core, test_add_different_scales);
  tcase_add_test(tc_core, test_add_overflow_positive);
  tcase_add_test(tc_core, test_add_overflow_negative);
  tcase_add_test(tc_core, test_add_null_pointer);
  tcase_add_test(tc_core, test_add_commutative);
  tcase_add_test(tc_core, test_add_identity);
  tcase_add_test(tc_core, test_add_large_numbers);
  tcase_add_test(tc_core, test_add_negative_with_fraction);

  suite_add_tcase(s, tc_core);
  return s;
}