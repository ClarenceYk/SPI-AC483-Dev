#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "private_io.h"

extern int SPI_AC483_FD;

int spi_io_operate(uint8_t rw, uint16_t addr, uint8_t *blck, size_t size)
{
    if (SPI_AC483_FD < 0)
        return -1;

    if (NULL == blck)
        return -2;

    if (addr < AC483_AVAIL_MEM_STA || addr > AC483_AVAIL_MEM_END)
        return -3;

    // 先传地址
    ac483_ctrl_msg_t ctrl_msg = {
        .ctrl_h = 0x00,
        .ctrl_l = 0x02, // 0010: write HPIA
    };
    uint8_t buff[2] = {
        (uint8_t)(addr >> 8),
        (uint8_t)(addr & 0x00FF)
    };

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

    int retv = ioctl(SPI_AC483_FD, SPI_IOC_MESSAGE(2), tr);
    if (retv < AC483_CTRL_MSG_LEN() + ARRAY_SIZE(buff))
        return -4;
    
    // 再操作数据
    ctrl_msg.ctrl_h = 0x00;
    ctrl_msg.ctrl_l = rw; // 0101: read HPID; 0001: write HPID

    if (AC483_OP_READ == rw) {
        tr[1].tx_buf = (unsigned long)NULL;
        tr[1].rx_buf = (unsigned long)blck;
    } else {
        tr[1].tx_buf = (unsigned long)blck;
        tr[1].rx_buf = (unsigned long)NULL;
    }

    if (size/2-1 + addr > AC483_AVAIL_MEM_END)
        size = 2 * (AC483_AVAIL_MEM_END - addr + 1);
    else
        size = 2 * (size / 2);
    tr[1].len = size;

    retv = ioctl(SPI_AC483_FD, SPI_IOC_MESSAGE(2), tr);
    if (retv < AC483_CTRL_MSG_LEN() + size)
        return -5;

    return size;
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

