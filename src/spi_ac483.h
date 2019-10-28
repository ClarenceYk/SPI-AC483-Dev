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
int spi_write_control_byte(uint8_t val);

/**
 * Function: spi_read_control_byte
 * -------------------------------
 * 从 AC483 控制寄存器读值
 *
 * val: 读出值的存放地址
 *
 * returns:  0 成功
 *          -1 SPI 设备文件未打开
 *          -2 val 为空指针
 *          -3 读取值出错
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
int spi_write_2bytes(uint16_t addr, uint16_t data);

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
 *          -2 data 为空指针
 *          -3 地址超出范围
 *          -4 传输地址出错
 *          -5 读取数据出错
 */
int spi_read_2bytes(uint16_t addr, uint16_t *data);

/**
 * Function: spi_write_block
 * --------------------------
 * 向指定的 AC483 内存地址写入整块数据
 *
 * addr: 数据写入的起始地址
 * blck: 数据缓存起始地址
 * size: 数据缓存大小
 *
 * returns: >0 实际写入字节数
 *          -1 SPI 设备文件未打开
 *          -2 blck 为空指针
 *          -3 地址超出范围
 *          -4 传输地址出错
 *          -5 传输数据出错
 */
int spi_write_block(uint16_t addr, const uint8_t *blck, size_t size);

/**
 * Function: spi_read_block
 * -------------------------
 * 从指定的 AC483 内存地址读出整块数据
 *
 * addr: 数据读取的起始地址
 * blck: 数据缓存起始地址
 * size: 数据缓存大小
 *
 * returns: >0 实际读取字节数
 *          -1 SPI 设备文件未打开
 *          -2 blck 为空指针
 *          -3 地址超出范围
 *          -4 传输地址出错
 *          -5 读取数据出错
 */
int spi_read_block(uint16_t addr, uint8_t *blck, size_t size);

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
