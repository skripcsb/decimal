#include "check_suites.h"

START_TEST(test_div_positive_positive) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  s21_decimal value_2 = {{25, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{4, 0, 0, 0}};

  int error = s21_div(value_1, value_2, &result);

  // Отладочная информация
  // ck_abort_msg("DEBUG test_div_positive_positive:\n"
  //              "  Input: 100 / 25\n"
  //              "  Error code: %d\n"
  //              "  Result bits: [%u][%u][%u][%u]\n"
  //              "  Expected bits: [%u][%u][%u][%u]\n"
  //              "  Result sign: %d, scale: %d\n"
  //              "  Expected sign: %d, scale: %d",
  //              error, result.bits[0], result.bits[1], result.bits[2],
  //              result.bits[3], expected.bits[0], expected.bits[1],
  //              expected.bits[2], expected.bits[3], _get_sign(&result),
  //              _get_scale(&result), _get_sign(&expected),
  //              _get_scale(&expected));

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_div_positive_negative) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  s21_decimal value_2 = {{25, 0, 0, 0}};
  _set_sign(&value_2, 1);
  s21_decimal result;
  s21_decimal expected = {{4, 0, 0, 0}};
  _set_sign(&expected, 1);

  int error = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
  ck_assert_int_eq(_get_sign(&result), 1);
}
END_TEST

START_TEST(test_div_negative_positive) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  _set_sign(&value_1, 1);
  s21_decimal value_2 = {{25, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{4, 0, 0, 0}};
  _set_sign(&expected, 1);

  int error = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
  ck_assert_int_eq(_get_sign(&result), 1);
}
END_TEST

START_TEST(test_div_negative_negative) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  _set_sign(&value_1, 1);
  s21_decimal value_2 = {{25, 0, 0, 0}};
  _set_sign(&value_2, 1);
  s21_decimal result;
  s21_decimal expected = {{4, 0, 0, 0}};

  int error = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
  ck_assert_int_eq(_get_sign(&result), 0);
}
END_TEST

START_TEST(test_div_with_fraction) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  _set_scale(&value_1, 1);

  s21_decimal value_2 = {{5, 0, 0, 0}};
  _set_scale(&value_2, 1);

  s21_decimal result;

  s21_decimal expected = {{3, 0, 0, 0}};
  _set_scale(&expected, 0);

  int error = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(error, 0);

  ck_assert_int_eq(s21_is_equal(result, expected), 1);

  ck_assert_int_eq(result.bits[0], 3);
  ck_assert_int_eq(_get_scale(&result), 0);
  ck_assert_int_eq(_get_sign(&result), 0);
}
END_TEST

START_TEST(test_div_different_scales) {
  s21_decimal value_1 = {{123400, 0, 0, 0}};  // 12.34
  _set_scale(&value_1, 4);
  s21_decimal value_2 = {{20000, 0, 0, 0}};  // 2.0
  _set_scale(&value_2, 4);
  s21_decimal result;
  s21_decimal expected = {{61700, 0, 0, 0}};  // 6.17
  _set_scale(&expected, 4);

  int error = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_div_by_zero) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result;

  int error = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(error, 3);
}
END_TEST

START_TEST(test_div_zero_by_any) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{123, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{0, 0, 0, 0}};

  int error = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_div_null_pointer) {
  s21_decimal value_1 = {{100, 0, 0, 0}};
  s21_decimal value_2 = {{25, 0, 0, 0}};

  int error = s21_div(value_1, value_2, NULL);

  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(test_div_identity) {
  s21_decimal value_1 = {{123, 0, 0, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{123, 0, 0, 0}};

  int error = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_div_large_numbers) {
  s21_decimal value_1 = {{1000000000, 0, 0, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{500000000, 0, 0, 0}};

  int error = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_div_small_numbers) {
  s21_decimal value_1 = {{100000, 0, 0, 0}};  // 0.1
  _set_scale(&value_1, 6);
  s21_decimal value_2 = {{200000, 0, 0, 0}};  // 0.2
  _set_scale(&value_2, 6);
  s21_decimal result;
  s21_decimal expected = {{500000, 0, 0, 0}};  // 0.5
  _set_scale(&expected, 6);

  int error = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

START_TEST(test_div_negative_with_fraction) {
  s21_decimal value_1 = {{10500000, 0, 0, 0}};  // 10.5
  _set_scale(&value_1, 6);
  _set_sign(&value_1, 1);
  s21_decimal value_2 = {{2000000, 0, 0, 0}};  // 2.0
  _set_scale(&value_2, 6);
  s21_decimal result;
  s21_decimal expected = {{5250000, 0, 0, 0}};  // 5.25
  _set_scale(&expected, 6);
  _set_sign(&expected, 1);

  int error = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
  ck_assert_int_eq(_get_sign(&result), 1);
}
END_TEST

START_TEST(test_div_exact_division) {
  s21_decimal value_1 = {{144, 0, 0, 0}};
  s21_decimal value_2 = {{12, 0, 0, 0}};
  s21_decimal result;
  s21_decimal expected = {{12, 0, 0, 0}};

  int error = s21_div(value_1, value_2, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(s21_is_equal(result, expected), 1);
}
END_TEST

Suite* div_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_div");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_div_positive_positive);
  tcase_add_test(tc_core, test_div_positive_negative);
  tcase_add_test(tc_core, test_div_negative_positive);
  tcase_add_test(tc_core, test_div_negative_negative);
  tcase_add_test(tc_core, test_div_with_fraction);
  tcase_add_test(tc_core, test_div_different_scales);
  tcase_add_test(tc_core, test_div_by_zero);
  tcase_add_test(tc_core, test_div_zero_by_any);
  tcase_add_test(tc_core, test_div_null_pointer);
  tcase_add_test(tc_core, test_div_identity);
  tcase_add_test(tc_core, test_div_large_numbers);
  tcase_add_test(tc_core, test_div_small_numbers);
  tcase_add_test(tc_core, test_div_negative_with_fraction);
  tcase_add_test(tc_core, test_div_exact_division);

  suite_add_tcase(s, tc_core);
  return s;
}