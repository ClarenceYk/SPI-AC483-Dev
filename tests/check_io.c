#include <check.h>
#include <stdint.h>
#include <stdlib.h>
#include "../src/spi_ac483.h"

START_TEST(test_io_rw)
{
    uint16_t addr = 233;
    uint16_t data = 666;
    ck_assert_int_eq(spi_write_2bytes(addr, data), 0);
    ck_assert_int_eq(spi_read_2bytes(addr, &data), 0);
}
END_TEST

START_TEST(test_spi_init)
{
    ck_assert_int_eq(spi_ac483_init(), 0);
}
END_TEST

Suite * io_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("I/O");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_spi_init);
    tcase_add_test(tc_core, test_io_rw);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = io_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
