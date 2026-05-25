#include <stdio.h>
#include <stdlib.h>

#include "check_suites.h"

int main(void) {
  SRunner* sr = srunner_create(NULL);

  // Безопасное добавление suite'ов с проверкой
  Suite* suites[] = {add_suite(),
                     sub_suite(),
                     mul_suite(),
                     div_suite(),
                     is_less_suite(),
                     is_less_or_equal_suite(),
                     is_greater_suite(),
                     is_greater_or_equal_suite(),
                     is_equal_suite(),
                     is_not_equal_suite(),
                     float_to_decimal_suite(),
                     int_to_decimal_suite(),
                     decimal_to_int_suite(),
                     decimal_to_float_suite(),
                     floor_suite(),
                     round_suite(),
                     truncate_suite(),
                     negate_suite()};

  int num_suites = sizeof(suites) / sizeof(suites[0]);

  for (int i = 0; i < num_suites; i++) {
    if (suites[i] != NULL) {
      srunner_add_suite(sr, suites[i]);
    }
  }

  srunner_run_all(sr, CK_NORMAL);

  int number_failed = srunner_ntests_failed(sr);

  printf("\nSUMMARY:\n");
  printf("  Total Tests:  %d\n", srunner_ntests_run(sr));
  printf("  Passed:       %d\n", srunner_ntests_run(sr) - number_failed);
  printf("  Failed:       %d\n", number_failed);

  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}