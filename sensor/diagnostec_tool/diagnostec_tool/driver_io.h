#ifndef DRIVER_IO_H_
#define DRIVER_IO_H_

#include <asm/ioctl.h>
#include <linux/types.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define SPI_IOC_MAGIC			'k'
#define SPI_MSGSIZE(N) \
	((((N)*(sizeof (struct egis_ioc_transfer))) < (1 << _IOC_SIZEBITS)) \
	? ((N)*(sizeof (struct egis_ioc_transfer))) : 0)
#define SPI_IOC_MESSAGE(N) _IOW(SPI_IOC_MAGIC, 0, char[SPI_MSGSIZE(N)])

#define  FPS_IMAGE_WIDTH          0x80
#define  FPS_IMAGE_HIGHT          0x80
#define FPS_REGISTER_MASSREAD             0x01
#define FPS_REGISTER_MASSWRITE            0x02
#define FPS_GET_ONE_IMG                   0x03


struct egis_ioc_transfer {
	__u64		tx_buf;
	__u64		rx_buf;

	__u32		len;
	__u32		speed_hz;

	__u16		delay_usecs;
	__u8		bits_per_word;
	__u8		cs_change;
	__u8		opcode;
	__u8		pad[3];
	//  __u32		pad;

	/* If the contents of 'struct spi_ioc_transfer' ever change
	* incompatibly, then the ioctl number (currently 0) must change;
	* ioctls with constant size fields get a bit more in the way of
	* error checking than ones (like this) where that field varies.
	*
	* NOTE: struct layout is the same in 64bit and 32bit userspace.
	*/
};

class DriverIO {
public:
	DriverIO();
	int RegisterRead(unsigned char address, unsigned char *value);
	int RegisterWrite(unsigned char address, unsigned char value);
	int ImageCapture(unsigned char* image_buffer);
private:
	int _device_director;
};

#endif // DRIVER_IO_H_