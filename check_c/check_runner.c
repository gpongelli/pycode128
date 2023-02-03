#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <check.h>

#include "pytest_suite.h"

int main(int argc, char **argv) {
    // instantiate our test suite. note this does not have to be freed!
    Suite *suite = pytest_suite();
    // create our suite runner and run all tests (CK_ENV -> set CK_VERBOSITY and
    // if not set, default to CK_NORMAL, i.e. only show failed)
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_ENV);
    // get number of failed tests and free runner
    int n_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    // succeed/fail depending on value of number of failed cases
    return (n_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
