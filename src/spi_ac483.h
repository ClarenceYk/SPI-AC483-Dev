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

/**
 * Function: spi_ac483_init
 * ------------------------
 * 初始化与 FPGA 通信的 SPI 接口
 *
 * return:  0 成功
 *         -1 打开 SPI 设备文件出错
 *         -2 设置 SPI 模式出错
 *         -3 设置 SPI 单字位数出错
 *         -4 设置 SPI 时钟频率出错
 */
int spi_ac483_init(void);

#endif
