#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define SPI_AC483_DEVICE "/dev/spidev4.0"
int     SPI_AC483_FD;

int spi_write_2bytes(const uint16_t addr, const uint16_t data)
{
    return 0;
}

int spi_read_2bytes(const uint16_t addr, const uint16_t *data)
{
    return 0;
}

int spi_init(uint8_t mode, uint8_t bits, uint32_t speed)
{
    int retv;
    int fd;
    uint8_t tmp_mode;
    uint8_t tmp_bits;
    uint32_t tmp_speed;

	fd = open(SPI_AC483_DEVICE, O_RDWR);
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

int spi_ac483_init(void)
{
    int retv;
    uint8_t mode = 0
        //| SPI_LOOP
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

    retv = spi_init(mode, bits, speed);
    if (retv < 0)
        return retv;

    SPI_AC483_FD = retv;

    return 0;
}
