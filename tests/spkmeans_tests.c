#include "munit.h"
#include "strutils.h"

static MunitResult test_strcount(const MunitParameter params[], void* data) {
    char* example_string = "The way to get started is to quit talking and begin doing. -Walt Disney";

    // Silence compiler warnings about the parameters being unused.
    (void) params;
    (void) data;

    munit_assert_size(strcount(example_string, 'z'), ==, 0);
    munit_assert_size(strcount(example_string, 'a'), ==, 5);
    munit_assert_size(strcount(example_string, 'T'), ==, 1);

    return MUNIT_OK;
}

static MunitTest test_suite_tests[] = {
    { 
        .name = (char*) "/strutils/test_strcount",
        .test = test_strcount,
        .setup = NULL,
        .tear_down = NULL,
        .options = MUNIT_TEST_OPTION_NONE,
        .parameters = NULL
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite test_suite = {
    .prefix = (char*) "",
    .tests = test_suite_tests,
    .suites = NULL,
    .iterations = 1,
    .options = MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
    return munit_suite_main(&test_suite, (void*) "µnit", argc, argv);
}