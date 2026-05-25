#include "check_suites.h"

START_TEST(test_mul_positive_positive) {
  s21_decimal value_1 = {{12, 0, 0, 0}};
  s21_decimal value_2 = {{34, 0, 0, 0}};
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{408, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_positive_negative) {
  s21_decimal value_1 = {{25, 0, 0, 0}};
  s21_decimal value_2 = {{4, 0, 0, 0x80000000}};
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{100, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
  ck_assert_int_eq(_get_sign(&result), 1);
}
END_TEST

START_TEST(test_mul_negative_positive) {
  s21_decimal value_1 = {{8, 0, 0, 0x80000000}};
  s21_decimal value_2 = {{7, 0, 0, 0}};
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{56, 0, 0, 0x80000000}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
  ck_assert_int_eq(_get_sign(&result), 1);
}
END_TEST

START_TEST(test_mul_negative_negative) {
  s21_decimal value_1 = {{9, 0, 0, 0x80000000}};
  s21_decimal value_2 = {{6, 0, 0, 0x80000000}};
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{54, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
  ck_assert_int_eq(_get_sign(&result), 0);
}
END_TEST

START_TEST(test_mul_zero_any) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{123, 0, 0, 0}};
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_any_zero) {
  s21_decimal value_1 = {{456, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_with_fraction) {
  s21_decimal value_1 = {{15, 0, 0, 0x00010000}};  // 1.5
  s21_decimal value_2 = {{25, 0, 0, 0x00010000}};  // 2.5
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{375, 0, 0, 0x00020000}};  // 3.75
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_different_scales) {
  s21_decimal value_1 = {{123, 0, 0, 0x00020000}};  // 1.23
  s21_decimal value_2 = {{456, 0, 0, 0x00020000}};  // 4.56
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{56088, 0, 0, 0x00040000}};  // 5.6088
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_overflow_positive) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};  // MAX
  s21_decimal value_2 = {{2, 0, 0, 0}};
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_mul_null_pointer) {
  s21_decimal value_1 = {{12, 0, 0, 0}};
  s21_decimal value_2 = {{34, 0, 0, 0}};

  int error = s21_mul(value_1, value_2, NULL);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_mul_commutative) {
  s21_decimal value_1 = {{7, 0, 0, 0}};
  s21_decimal value_2 = {{8, 0, 0, 0}};
  s21_decimal result1, result2;

  int error1 = s21_mul(value_1, value_2, &result1);
  int error2 = s21_mul(value_2, value_1, &result2);

  ck_assert_int_eq(error1, 0);
  ck_assert_int_eq(error2, 0);

  ck_assert_int_eq(s21_is_equal(result1, result2), 1);
}
END_TEST

START_TEST(test_mul_identity) {
  s21_decimal value_1 = {{123, 0, 0, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  ck_assert_int_eq(s21_is_equal(result, value_1), 1);
}
END_TEST

START_TEST(test_mul_large_numbers) {
  s21_decimal value_1 = {{1000000, 0, 0, 0}};
  s21_decimal value_2 = {{2000000, 0, 0, 0}};
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{0xA94A2000, 0x1D1, 0, 0}};  // 2000000000000
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_mul_negative_with_fraction) {
  s21_decimal value_1 = {{25, 0, 0, 0x80010000}};  // -2.5
  s21_decimal value_2 = {{15, 0, 0, 0x80010000}};  // -1.5
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{375, 0, 0, 0x00020000}};  // 3.75
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
  ck_assert_int_eq(_get_sign(&result), 0);
}
END_TEST

START_TEST(test_mul_small_fractions) {
  s21_decimal value_1 = {{1, 0, 0, 0x00010000}};  // 0.1
  s21_decimal value_2 = {{2, 0, 0, 0x00010000}};  // 0.2
  s21_decimal result;

  int error = s21_mul(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);

  s21_decimal expected = {{2, 0, 0, 0x00020000}};  // 0.02
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

Suite *mul_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_mul");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_mul_positive_positive);
  tcase_add_test(tc_core, test_mul_positive_negative);
  tcase_add_test(tc_core, test_mul_negative_positive);
  tcase_add_test(tc_core, test_mul_negative_negative);
  tcase_add_test(tc_core, test_mul_zero_any);
  tcase_add_test(tc_core, test_mul_any_zero);
  tcase_add_test(tc_core, test_mul_with_fraction);
  tcase_add_test(tc_core, test_mul_different_scales);
  tcase_add_test(tc_core, test_mul_overflow_positive);
  tcase_add_test(tc_core, test_mul_null_pointer);
  tcase_add_test(tc_core, test_mul_commutative);
  tcase_add_test(tc_core, test_mul_identity);
  tcase_add_test(tc_core, test_mul_large_numbers);
  tcase_add_test(tc_core, test_mul_negative_with_fraction);
  tcase_add_test(tc_core, test_mul_small_fractions);

  suite_add_tcase(s, tc_core);
  return s;
}