#include "check_suites.h"

START_TEST(test_from_int_basic_positive) {
  s21_decimal result;
  int src = 123456;

  int error = s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);

  int back_convert;
  s21_from_decimal_to_int(result, &back_convert);

  ck_assert_int_eq(back_convert, src);
}
END_TEST

START_TEST(test_from_int_basic_negative) {
  s21_decimal result;
  int src = -7890;

  int error = s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 1);

  int back_convert;
  s21_from_decimal_to_int(result, &back_convert);
  ck_assert_int_eq(back_convert, src);
}
END_TEST

START_TEST(test_from_int_zero) {
  s21_decimal result;
  int src = 0;

  int error = s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 0);

  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_from_int_max_positive) {
  s21_decimal result;
  int src = INT_MAX;

  int error = s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 0);

  int back_convert;
  s21_from_decimal_to_int(result, &back_convert);
  ck_assert_int_eq(back_convert, src);
}
END_TEST

START_TEST(test_from_int_min_negative) {
  s21_decimal result;
  int src = INT_MIN;

  int error = s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 1);

  int back_convert;
  s21_from_decimal_to_int(result, &back_convert);
  ck_assert_int_eq(back_convert, src);
}
END_TEST

START_TEST(test_from_int_one) {
  s21_decimal result;
  int src = 1;

  int error = s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);

  int back_convert;
  s21_from_decimal_to_int(result, &back_convert);
  ck_assert_int_eq(back_convert, src);
}
END_TEST

START_TEST(test_from_int_negative_one) {
  s21_decimal result;
  int src = -1;

  int error = s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 1);

  int back_convert;
  s21_from_decimal_to_int(result, &back_convert);
  ck_assert_int_eq(back_convert, src);
}
END_TEST

START_TEST(test_from_int_scale_zero) {
  s21_decimal result;
  int src = 42;

  int error = s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_scale(&result), 0);
}
END_TEST

START_TEST(test_from_int_null_pointer) {
  int src = 123;

  int error = s21_from_int_to_decimal(src, NULL);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_from_int_medium_positive) {
  s21_decimal result;
  int src = 65536;

  int error = s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);

  int back_convert;
  s21_from_decimal_to_int(result, &back_convert);
  ck_assert_int_eq(back_convert, src);
}
END_TEST

START_TEST(test_from_int_medium_negative) {
  s21_decimal result;
  int src = -32768;

  int error = s21_from_int_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 1);

  int back_convert;
  s21_from_decimal_to_int(result, &back_convert);
  ck_assert_int_eq(back_convert, src);
}
END_TEST

Suite* int_to_decimal_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_int_to_decimal");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_from_int_basic_positive);
  tcase_add_test(tc_core, test_from_int_basic_negative);
  tcase_add_test(tc_core, test_from_int_zero);
  tcase_add_test(tc_core, test_from_int_max_positive);
  tcase_add_test(tc_core, test_from_int_min_negative);
  tcase_add_test(tc_core, test_from_int_one);
  tcase_add_test(tc_core, test_from_int_negative_one);
  tcase_add_test(tc_core, test_from_int_scale_zero);
  tcase_add_test(tc_core, test_from_int_null_pointer);
  tcase_add_test(tc_core, test_from_int_medium_positive);
  tcase_add_test(tc_core, test_from_int_medium_negative);

  suite_add_tcase(s, tc_core);
  return s;
}