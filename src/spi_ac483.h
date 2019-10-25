#ifndef SPI_AC483_H
#define SIP_AC483_H

/**
 * Function: spi_write_control_byte
 * --------------------------------
 * 向 AC483 控制寄存器写值
 *
 * val: 写入的值
 *
 * returns:  0 成功
 *          -1 SPI 设备文件未打开
 *          -2 写入值出错
 */
int spi_write_control_byte(const uint8_t val);

/**
 * Function: spi_read_control_byte
 * -------------------------------
 * 从 AC483 控制寄存器读值
 *
 * val: 读出值的存放地址
 *
 * returns:  0 成功
 *          -1 SPI 设备文件未打开
 *          -2 读取值出错
 *          -3 val 为空指针
 */
int spi_read_control_byte(uint8_t *val);

/**
 * Function: spi_write_2bytes
 * --------------------------
 * 向指定的 AC483 内存地址写入2字节数据
 *
 * addr: 数据写入的地址
 * data: 写入的数据
 *
 * returns:  0 成功
 *          -1 SPI 设备文件未打开
 *          -2 地址超出范围
 *          -3 传输地址出错
 *          -4 传输数据出错
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
 * returns:  0 成功
 *          -1 SPI 设备文件未打开
 *          -2 地址超出范围
 *          -3 传输地址出错
 *          -4 读取数据出错
 *          -5 data 为空指针
 */
int spi_read_2bytes(const uint16_t addr, uint16_t *data);

/**
 * Function: spi_ac483_init
 * ------------------------
 * 初始化与 FPGA 通信的 SPI 接口
 *
 * dev: SPI 设备文件路径
 *
 * returns:  0 成功
 *          -1 打开 SPI 设备文件出错
 *          -2 设置 SPI 模式出错
 *          -3 设置 SPI 单字位数出错
 *          -4 设置 SPI 时钟频率出错
 */
int spi_ac483_init(const char *dev);

/**
 * Function: spi_ac483_deinit
 * ------------------------
 * SPI 接口取消初始化
 *
 * returns:  0 成功
 *          -1 失败
 */
int spi_ac483_deinit(void);

#endif
