#ifndef PRIVATE_IO_H
#define PRIVATE_IO_H

#define ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))
#define AC483_CTRL_MSG_LEN() (sizeof(ac483_ctrl_msg_t))

#define AC483_AVAIL_MEM_STA 0x1000
#define AC483_AVAIL_MEM_END 0x17FF
#define AC483_OP_READ  0x05
#define AC483_OP_WRITE 0x01

typedef struct AC483_Ctrl_Msg {
    uint8_t ctrl_h;
    uint8_t ctrl_l;
} ac483_ctrl_msg_t;

/**
 * Function: spi_io_operate
 * ------------------------
 * 封装 SPI 设备操作
 *
 */
int spi_io_operate(uint8_t rw, uint16_t addr, uint8_t *blck, size_t size);

#endif
