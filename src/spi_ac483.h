#ifndef SPI_AC483_H
#define SIP_AC483_H

/**
 * Function: spi_write_2bytes
 * --------------------------
 * 向指定的 AC483 内存地址写入2字节数据
 *
 * addr: 数据写入的地址
 * data: 写入的数据
 *
 * returns: 0 成功，（其他数字含义未定）
 */
int spi_write_2bytes(const uint16_t addr, const uint16_t data);

/**
 * Function: spi_read_2bytes
 * -------------------------
 * 从指定的 AC483 内存地址读出2字节数据
 *
 * addr: 数据读取的地址
 * data: 读出的数据的存放地址
 *
 * returns: 0 成功，（其他数字含义未定）
 */
int spi_read_2bytes(const uint16_t addr, const uint16_t *data);

#endif
