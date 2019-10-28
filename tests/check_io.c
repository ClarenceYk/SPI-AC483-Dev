#include <check.h>
#include <stdint.h>
#include <stdlib.h>
#include "check_io.h"

START_TEST(test_spi_init)
{
    ck_assert_int_eq(spi_ac483_init(""), -1);
    ck_assert_int_eq(spi_ac483_init("jd7ergax6"), -1);
    ck_assert_int_eq(spi_ac483_init("/dev/spidev1.1"), -1);
    ck_assert_int_eq(spi_ac483_init("/dev/spidev4.0"), 0);
    ck_assert_int_eq(spi_ac483_deinit(), 0);
}
END_TEST

START_TEST(test_spi_loop)
{
    spi_ac483_init("/dev/spidev4.0");
    ck_assert_int_eq(spi_loop_test(), 0);
    spi_ac483_deinit();
}
END_TEST

START_TEST(test_io_write)
{
    uint16_t addr;
    uint16_t data = 666;
    ck_assert_int_eq(spi_write_2bytes(addr, data), -1);
    spi_ac483_init("/dev/spidev4.0");
    addr = 0x1ff;
    ck_assert_int_eq(spi_write_2bytes(addr, data), -2);
    addr = 0x2000;
    ck_assert_int_eq(spi_write_2bytes(addr, data), -2);
    addr = 0x1234;
    ck_assert_int_eq(spi_write_2bytes(addr, data), 0);
    spi_ac483_deinit();
}
END_TEST

START_TEST(test_io_read)
{
    uint16_t addr;
    uint16_t data;
    ck_assert_int_eq(spi_read_2bytes(addr, &data), -1);
    spi_ac483_init("/dev/spidev4.0");
    addr = 0x1ff;
    ck_assert_int_eq(spi_read_2bytes(addr, &data), -3);
    addr = 0x2000;
    ck_assert_int_eq(spi_read_2bytes(addr, &data), -3);
    addr = 0x1234;
    ck_assert_int_eq(spi_read_2bytes(addr, (void *)0), -2);
    ck_assert_int_eq(spi_read_2bytes(addr, &data), 0);
    ck_assert_int_eq(data, 0);
    spi_ac483_deinit();
}
END_TEST

START_TEST(test_ctrl_reg_write)
{
    uint8_t data = 0xab;
    ck_assert_int_eq(spi_write_control_byte(data), -1);
    spi_ac483_init("/dev/spidev4.0");
    ck_assert_int_eq(spi_write_control_byte(data), 0);
    spi_ac483_deinit();
}
END_TEST

START_TEST(test_ctrl_reg_read)
{
    uint8_t data = 0xab;
    ck_assert_int_eq(spi_read_control_byte(&data), -1);
    spi_ac483_init("/dev/spidev4.0");
    ck_assert_int_eq(spi_read_control_byte((void *)0), -2);
    ck_assert_int_eq(spi_read_control_byte(&data), 0);
    ck_assert_int_eq(data, 0);
    spi_ac483_deinit();
}
END_TEST

Suite * io_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("I/O");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_spi_init);
    tcase_add_test(tc_core, test_spi_loop);
    tcase_add_test(tc_core, test_io_write);
    tcase_add_test(tc_core, test_io_read);
    tcase_add_test(tc_core, test_ctrl_reg_write);
    tcase_add_test(tc_core, test_ctrl_reg_read);

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
