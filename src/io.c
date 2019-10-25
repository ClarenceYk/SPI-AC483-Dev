#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "private_io.h"

#define ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))

int SPI_AC483_FD = -1;

int spi_write_control_byte(const uint8_t val) {
    int retv;

    if (SPI_AC483_FD < 0)
        return -1;
    
    ac483_ctrl_msg_t msg = {
        .ctrl_h = 0x00,
        .ctrl_l = 0x00, // 0000: write HPIC
        .msg_h = val,
        .msg_l = val,
    };

    size_t size = sizeof(ac483_ctrl_msg_t);

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)(uint8_t *)&msg,
        .rx_buf = (unsigned long)NULL,
        .len = size,
    };

    retv = ioctl(SPI_AC483_FD, SPI_IOC_MESSAGE(1), &tr);
    if (retv < size)
        return -2;

    return 0;
}

int spi_read_control_byte(uint8_t *val) {
    int retv;

    if (SPI_AC483_FD < 0)
        return -1;

    if (NULL == val)
        return -2;
    
    ac483_ctrl_msg_t msg_tx = {
        .ctrl_h = 0x00,
        .ctrl_l = 0x04, // 0100: read HPIC
        .msg_h = *val,
        .msg_l = *val,
    };
    ac483_ctrl_msg_t msg_rx = {0,0,0,0};

    size_t size = sizeof(ac483_ctrl_msg_t);

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)(uint8_t *)&msg_tx,
        .rx_buf = (unsigned long)(uint8_t *)&msg_rx,
        .len = size,
    };

    retv = ioctl(SPI_AC483_FD, SPI_IOC_MESSAGE(1), &tr);
    if (retv < size)
        return -3;

    *val = msg_rx.msg_h;

    return 0;
}

int spi_write_2bytes(const uint16_t addr, const uint16_t data)
{
    int retv;

    if (SPI_AC483_FD < 0)
        return -1;

    if (addr < 0x1000 || addr > 0x17FF)
        return -2;

    // 先传地址
    ac483_ctrl_msg_t msg = {
        .ctrl_h = 0x00,
        .ctrl_l = 0x02, // 0010: write HPIA
        .msg_h = (uint8_t)(addr >> 8),
        .msg_l = (uint8_t)(addr & 0x00FF),
    };

    size_t size = sizeof(ac483_ctrl_msg_t);

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)(uint8_t *)&msg,
        .rx_buf = (unsigned long)NULL,
        .len = size,
    };

    retv = ioctl(SPI_AC483_FD, SPI_IOC_MESSAGE(1), &tr);
    if (retv < size)
        return -3;
    
    // 再传数据
    msg.ctrl_h = 0x00;
    msg.ctrl_l = 0x01; // 0001: write HPID
    msg.msg_h = (uint8_t)(data >> 8);
    msg.msg_l = (uint8_t)(data & 0x00FF);

    retv = ioctl(SPI_AC483_FD, SPI_IOC_MESSAGE(1), &tr);
    if (retv < size)
        return -4;

    return 0;
}

int spi_read_2bytes(const uint16_t addr, uint16_t *data)
{
    int retv;

    if (SPI_AC483_FD < 0)
        return -1;

    if (NULL == data)
        return -2;

    if (addr < 0x1000 || addr > 0x17FF)
        return -3;

    // 先传地址
    ac483_ctrl_msg_t msg_tx = {
        .ctrl_h = 0x00,
        .ctrl_l = 0x02, // 0010: write HPIA
        .msg_h = (uint8_t)(addr >> 8),
        .msg_l = (uint8_t)(addr & 0x00FF),
    };

    size_t size = sizeof(ac483_ctrl_msg_t);

    struct spi_ioc_transfer tr = {
        .tx_buf = (unsigned long)(uint8_t *)&msg_tx,
        .rx_buf = (unsigned long)NULL,
        .len = size,
    };

    retv = ioctl(SPI_AC483_FD, SPI_IOC_MESSAGE(1), &tr);
    if (retv < size)
        return -4;
    
    // 再读数据
    msg_tx.ctrl_h = 0x00;
    msg_tx.ctrl_l = 0x05; // 0101: read HPID
    ac483_ctrl_msg_t msg_rx = {0,0,0,0};
    tr.rx_buf = (unsigned long)(uint8_t *)&msg_rx;

    retv = ioctl(SPI_AC483_FD, SPI_IOC_MESSAGE(1), &tr);
    if (retv < size)
        return -5;
    
    *data = 0;
    *data = ((*data | msg_rx.msg_h) << 8) | msg_rx.msg_l;

    return 0;
}

int spi_init(const char *dev, uint8_t mode, uint8_t bits, uint32_t speed)
{
    int retv;
    int fd;
    uint8_t tmp_mode;
    uint8_t tmp_bits;
    uint32_t tmp_speed;

    fd = open(dev, O_RDWR);
    if (fd < 0)
        return fd;
    
    // spi mode
    retv = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (retv == -1)
        return -2;
    
    retv = ioctl(fd, SPI_IOC_RD_MODE, &tmp_mode);
    if (retv == -1)
        return -2;
    
    if (tmp_mode != mode)
        return -2;
    
    // bits per word
    retv = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (retv == -1)
        return -3;
    
    retv = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &tmp_bits);
    if (retv == -1)
        return -3;
    
    if (tmp_bits != bits)
        return -3;
    
    // max speed hz
    retv = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (retv == -1)
        return -4;
    
    retv = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &tmp_speed);
    if (retv == -1)
        return -4;
    
    if (tmp_speed != speed)
        return -4;
    
    return fd;
}

int spi_ac483_init(const char *dev)
{
    int retv;
    uint8_t mode = 0
#ifdef ENABLEGCOV
        | SPI_LOOP
#endif
        | SPI_CPHA
        | SPI_CPOL
        //| SPI_LSB_FIRST
        //| SPI_CS_HIGH
        //| SPI_3WIRE
        //| SPI_NO_CS
        //| SPI_READY
        ;
    uint8_t bits = 8;
    uint32_t speed = 500000;

    retv = spi_init(dev, mode, bits, speed);
    if (retv < 0)
        return retv;

    SPI_AC483_FD = retv;

    return 0;
}

int spi_ac483_deinit(void)
{
    if (SPI_AC483_FD < 0)
        return 0;
    
    return close(SPI_AC483_FD);
}

// Only for testing
int spi_loop_test(void)
{
    int retv;
    uint8_t tx[] = {
        0x53, 0x69, 0x78, 0x20, 0x6a, 0x61, 0x76, 0x65,
        0x6c, 0x69, 0x6e, 0x73, 0x20, 0x74, 0x68, 0x72,
        0x6f, 0x77, 0x6e, 0x20, 0x62, 0x79, 0x20, 0x74,
        0x68, 0x65, 0x20, 0x71, 0x75, 0x69, 0x63, 0x6b,
        0x20, 0x73, 0x61, 0x76, 0x61, 0x67, 0x65, 0x73,
        0x20, 0x77, 0x68, 0x69, 0x7a, 0x7a, 0x65, 0x64,
        0x20, 0x66, 0x6f, 0x72, 0x74, 0x79, 0x20, 0x70,
        0x61, 0x63, 0x65, 0x73, 0x20, 0x62, 0x65, 0x79,
    };
    uint8_t rx[ARRAY_SIZE(tx)] = {0, };

    struct spi_ioc_transfer trs[] = {
        {
            .tx_buf = (unsigned long)tx,
            .rx_buf = (unsigned long)rx,
            .len = ARRAY_SIZE(tx),
        },
    };

    retv = ioctl(SPI_AC483_FD, SPI_IOC_MESSAGE(1), trs);
    if (retv < ARRAY_SIZE(tx))
        goto END;

    int idx;
    for (idx = 0; idx < ARRAY_SIZE(tx); idx ++) {
        if (tx[idx] == rx[idx])
            retv --;
    }

END:
    return retv;
}
