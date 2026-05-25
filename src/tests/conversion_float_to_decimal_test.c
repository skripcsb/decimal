#include "check_suites.h"

START_TEST(test_from_float_basic_positive) {
  s21_decimal result;
  float src = 123.456f;

  int error = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);

  // Проверяем обратную конвертацию
  float back_convert;
  s21_from_decimal_to_float(result, &back_convert);

  ck_assert_float_eq_tol(back_convert, src, 1e-7f);
}
END_TEST

START_TEST(test_from_float_basic_negative) {
  s21_decimal result;
  float src = -78.9f;

  int error = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 1);  // Должен быть отрицательный знак

  float back_convert;
  s21_from_decimal_to_float(result, &back_convert);
  ck_assert_float_eq_tol(back_convert, src, 1e-7f);
}
END_TEST

START_TEST(test_from_float_zero) {
  s21_decimal result;
  float src = 0.0f;

  int error = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);
  ck_assert_int_eq(_get_sign(&result), 0);  // Ноль без знака

  // Проверяем что все биты нули (кроме возможно scale)
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(result.bits[1], 0);
  ck_assert_int_eq(result.bits[2], 0);
}
END_TEST

START_TEST(test_from_float_small_value) {
  s21_decimal result;
  float src = 0.000001f;  // Маленькое число

  int error = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);

  float back_convert;
  s21_from_decimal_to_float(result, &back_convert);
  ck_assert_float_eq_tol(back_convert, src, 1e-7f);
}
END_TEST

START_TEST(test_from_float_large_value) {
  s21_decimal result;
  float src = 1e7f;  // Большое число

  int error = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);

  float back_convert;
  s21_from_decimal_to_float(result, &back_convert);
  ck_assert_float_eq_tol(back_convert, src,
                         1e-1f);  // Большая погрешность для больших чисел
}
END_TEST

START_TEST(test_from_float_precision) {
  s21_decimal result;
  float src = 3.14159f;  // Известное число

  int error = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);

  float back_convert;
  s21_from_decimal_to_float(result, &back_convert);
  ck_assert_float_eq_tol(back_convert, src, 1e-7f);
}
END_TEST

START_TEST(test_from_float_negative_zero) {
  s21_decimal result;
  float src = -0.0f;

  int error = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);
  // Ноль должен остаться без знака или с отрицательным знаком в зависимости от
  // реализации
}
END_TEST

START_TEST(test_from_float_nan) {
  s21_decimal result;
  float src = 0.0f / 0.0f;  // NaN

  int error = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(error, 1);  // Должна быть ошибка для NaN
}
END_TEST

START_TEST(test_from_float_infinity) {
  s21_decimal result;
  float src = 1.0f / 0.0f;  // Infinity

  int error = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(error, 1);  // Должна быть ошибка для Infinity
}
END_TEST

START_TEST(test_from_float_negative_infinity) {
  s21_decimal result;
  float src = -1.0f / 0.0f;  // -Infinity

  int error = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(error, 1);  // Должна быть ошибка для -Infinity
}
END_TEST

START_TEST(test_from_float_null_pointer) {
  // Передаем NULL указатель
  float src = 123.456f;

  int error = s21_from_float_to_decimal(src, NULL);

  ck_assert_int_eq(error, 1);  // Должна быть ошибка для NULL
}
END_TEST

START_TEST(test_from_float_very_small) {
  s21_decimal result;
  float src = 1e-20f;  // Очень маленькое число (денормализованное)

  int error = s21_from_float_to_decimal(src, &result);

  // Может вернуть ошибку или ноль в зависимости от реализации
  if (error == 0) {
    float back_convert;
    s21_from_decimal_to_float(result, &back_convert);
    ck_assert_float_eq_tol(back_convert, 0.0f, 1e-10f);
  }
}
END_TEST

START_TEST(test_from_float_rounding) {
  s21_decimal result;
  float src = 12.3456789f;  // Проверка округления

  int error = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(error, 0);

  float back_convert;
  s21_from_decimal_to_float(result, &back_convert);
  ck_assert_float_eq_tol(back_convert, src, 1e-6f);
}
END_TEST

Suite* float_to_decimal_suite(void) {
  Suite* s;
  TCase* tc_core;

  s = suite_create("s21_decimal_conversion");
  tc_core = tcase_create("Core");

  // Добавляем тесты
  tcase_add_test(tc_core, test_from_float_basic_positive);
  tcase_add_test(tc_core, test_from_float_basic_negative);
  tcase_add_test(tc_core, test_from_float_zero);
  tcase_add_test(tc_core, test_from_float_small_value);
  tcase_add_test(tc_core, test_from_float_large_value);
  tcase_add_test(tc_core, test_from_float_precision);
  tcase_add_test(tc_core, test_from_float_negative_zero);
  tcase_add_test(tc_core, test_from_float_nan);
  tcase_add_test(tc_core, test_from_float_infinity);
  tcase_add_test(tc_core, test_from_float_negative_infinity);
  tcase_add_test(tc_core, test_from_float_null_pointer);
  tcase_add_test(tc_core, test_from_float_very_small);
  tcase_add_test(tc_core, test_from_float_rounding);

  suite_add_tcase(s, tc_core);
  return s;
}