// SPDX-FileCopyrightText: 2023 Gabriele Pongelli
//
// SPDX-License-Identifier: MIT

#define PY_SSIZE_T_CLEAN
#include "Python.h"

#include <stdio.h>
#include <check.h>
#include "pytest_suite.h"

START_TEST(print_stuff)
{
    // initialize interpreter, load pytest main and run (relies on pytest.ini)
    Py_Initialize();
    PyRun_SimpleString("from pytest import main\nmain()\n");
    // required finalization function; if something went wrong, exit immediately
    if (Py_FinalizeEx() < 0) {
        // __func__ only defined in C99+
        fprintf(stderr, "error: %s: Py_FinalizeEx error\n", __func__);
        exit(120);
    }
}
END_TEST

Suite *pytest_suite() {
    // create suite called pytest_suite + add test case named core
    Suite *suite = suite_create("pytest_suite");
    TCase *tc_core = tcase_create("core");
    // register case together with test func, add to suite, and return suite
    tcase_add_test(tc_core, print_stuff);
    suite_add_tcase(suite, tc_core);
    return suite;
}
