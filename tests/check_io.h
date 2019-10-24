#include "../src/spi_ac483.h"

/**
 * Function: spi_loop_test
 * -----------------------
 * 用于 SPI 设备回环测试
 * 注意: 测试时需要将 MISO 和 MOSI 短接
 * 
 * returns:   0 成功
 *          !=0 失败
 */
int spi_loop_test(void);