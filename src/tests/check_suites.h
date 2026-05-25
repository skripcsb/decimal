#ifndef CHECK_SUITS_H
#define CHECK_SUITS_H

#include <check.h>
#include <stdio.h>

#include "../s21_decimal.h"
#include "../s21_decimal_common.h"

// Arithmetics
Suite* add_suite(void);
Suite* sub_suite(void);
Suite* mul_suite(void);
Suite* div_suite(void);

// Compasion
Suite* is_less_suite(void);
Suite* is_less_or_equal_suite(void);
Suite* is_greater_suite(void);
Suite* is_greater_or_equal_suite(void);
Suite* is_equal_suite(void);
Suite* is_not_equal_suite(void);

// Conversion
Suite* float_to_decimal_suite(void);
Suite* int_to_decimal_suite(void);
Suite* decimal_to_int_suite(void);
Suite* decimal_to_float_suite(void);

// Other
Suite* floor_suite(void);
Suite* round_suite(void);
Suite* truncate_suite(void);
Suite* negate_suite(void);

// EXTRA
Suite* s21_float_to_decimal_suite(void);

#endif
