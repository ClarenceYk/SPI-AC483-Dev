#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "spi_ac483.h"
#include "private_io.h"

int SPI_AC483_FD = -1;

int spi_write_control_byte(uint8_t val)
{
    int retv;

    if (SPI_AC483_FD < 0)
        return -1;
    
    ac483_ctrl_msg_t ctrl_msg = {
        .ctrl_h = 0x00,
        .ctrl_l = 0x00, // 0000: write HPIC
    };
    uint8_t buff[2] = { val, val };

    struct spi_ioc_transfer tr[2] = {
        {
            .tx_buf = (unsigned long)(uint8_t *)&ctrl_msg,
            .rx_buf = (unsigned long)NULL,
            .len = AC483_CTRL_MSG_LEN(),
        },
        {
            .tx_buf = (unsigned long)buff,
            .rx_buf = (unsigned long)NULL,
            .len = ARRAY_SIZE(buff),
        }
    };

    retv = ioctl(SPI_AC483_FD, SPI_IOC_MESSAGE(2), tr);
    if (retv < AC483_CTRL_MSG_LEN() + ARRAY_SIZE(buff))
        return -2;

    return 0;
}

int spi_read_control_byte(uint8_t *val)
{
    int retv;

    if (SPI_AC483_FD < 0)
        return -1;

    if (NULL == val)
        return -2;
    
    ac483_ctrl_msg_t ctrl_msg = {
        .ctrl_h = 0x00,
        .ctrl_l = 0x04, // 0100: read HPIC
    };
    uint8_t buff[2] = { 0,  };

    struct spi_ioc_transfer tr[2] = {
        {
            .tx_buf = (unsigned long)(uint8_t *)&ctrl_msg,
            .rx_buf = (unsigned long)NULL,
            .len = AC483_CTRL_MSG_LEN(),
        },
        {
            .tx_buf = (unsigned long)NULL,
            .rx_buf = (unsigned long)buff,
            .len = ARRAY_SIZE(buff),
        }
    };

    retv = ioctl(SPI_AC483_FD, SPI_IOC_MESSAGE(2), tr);
    if (retv < AC483_CTRL_MSG_LEN() + ARRAY_SIZE(buff))
        return -3;

    *val = buff[0];

    return 0;
}

int spi_write_2bytes(uint16_t addr, uint16_t data)
{
    uint8_t buf[2] = {
        (uint8_t)(data >> 8),
        (uint8_t)(data & 0x00FF)
    };

    return spi_io_operate(AC483_OP_WRITE, addr, buf, ARRAY_SIZE(buf));
}

int spi_read_2bytes(uint16_t addr, uint16_t *data)
{
    int retv;
    uint8_t buf[2] = { 0, };

    if (NULL == data) {
        retv = -2;
        goto END;
    }

    retv = spi_io_operate(AC483_OP_READ, addr, buf, ARRAY_SIZE(buf));
    if (retv < 0)
        goto END;

    *data = 0;
    *data = ((*data | buf[0]) << 8) | buf[1];

END:
    return retv;
}

int spi_write_block(uint16_t addr, const uint8_t *blck, size_t size)
{
    uint8_t *t_blck = (uint8_t *)blck;
    return spi_io_operate(AC483_OP_WRITE, addr, t_blck, size);
}

int spi_read_block(uint16_t addr, uint8_t *blck, size_t size)
{
    return spi_io_operate(AC483_OP_READ, addr, blck, size);
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

