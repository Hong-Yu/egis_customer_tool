#include "driver_io.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <fcntl.h>
DriverIO::DriverIO() {
	const char *device = "/dev/esfp0";
	_device_director = open(device, O_RDWR);
	printf("Open device path: %s return: %d\n", device, _device_director);
}

int DriverIO::RegisterRead(unsigned char address, unsigned char *value) {
	int ret;
	uint8_t tx[] = {
		address
	};
	uint8_t rx[] = {
		0x0
	};
	struct egis_ioc_transfer tr;
	tr.tx_buf = (unsigned long)tx;
	tr.rx_buf = (unsigned long)rx;
	tr.len = ARRAY_SIZE(tx);
	tr.opcode = FPS_REGISTER_MASSREAD;
	ret = ioctl(_device_director, SPI_IOC_MESSAGE(1), &tr);
	*value = rx[0];
	return ret;
}

int DriverIO::RegisterWrite(unsigned char address, unsigned char value) {
	int ret;
	uint8_t tx[] = {
		address, value,
	};
	struct egis_ioc_transfer tr;
	tr.tx_buf = (unsigned long)tx;
	tr.len = ARRAY_SIZE(tx);
	tr.opcode = FPS_REGISTER_MASSWRITE;
	ret = ioctl(_device_director, SPI_IOC_MESSAGE(1), &tr);
	//printf("Write register address: %x value: %x return: %d\n", address, value, ret);
	return ret;
}


int DriverIO::ImageCapture(unsigned char* image_buffer) {
	int ret;
	uint8_t datatemp;
	uint8_t tx[] = {
		FPS_IMAGE_WIDTH,              // image width 
		FPS_IMAGE_HIGHT,              // image hight
		0x01, 0x00, 0x00, 0x00,      // TBD
	};
	//uint8_t *rx = image_buffer;
	uint8_t *rx;

	rx = (uint8_t *)malloc(FPS_IMAGE_WIDTH * FPS_IMAGE_HIGHT);
	struct egis_ioc_transfer tr;
	tr.tx_buf = (unsigned long)tx;
	tr.rx_buf = (unsigned long)rx;
	tr.len = ARRAY_SIZE(tx);
	tr.opcode = FPS_GET_ONE_IMG;
	ret = ioctl(_device_director, SPI_IOC_MESSAGE(1), &tr);
	//for (int image_index = 0; image_index < FPS_IMAGE_WIDTH * FPS_IMAGE_HIGHT; image_index++)
		//printf("%02d ", rx[image_index]);
	memcpy(image_buffer, rx, FPS_IMAGE_WIDTH * FPS_IMAGE_HIGHT);
	printf("Capture image return: %d\n", ret);
	return ret;
}