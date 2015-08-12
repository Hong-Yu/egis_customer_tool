#include "sensor_control.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "image_manager.h"

int SensorControl::set_power_on_mode() {
	// Power up
	_driver_io->RegisterWrite(0x04, 0x00); // ADC control
	_driver_io->RegisterWrite(0x07, 0x00); // Bandgap and V2I
	_driver_io->RegisterWrite(0x09, 0x00); // PGA
}

int SensorControl::set_sensor_mode(int image_width, int image_height) {
	// DC and Gain
	_driver_io->RegisterWrite(0x03, 0x80); //Dc offset cancellation
	_driver_io->RegisterWrite(0x09, 0x00); //Gain
	//	Leave suspend
	_driver_io->RegisterWrite(0x08, 0x01); //Detect mode power down
	_driver_io->RegisterWrite(0x0B, 0x00); //Normal mode
	// Over temperature control
	_driver_io->RegisterWrite(0x05, 0x02); // Power down
	// Capture range
	_driver_io->RegisterWrite(0x10, 0x00);
	_driver_io->RegisterWrite(0x11, image_height - 1);
	_driver_io->RegisterWrite(0x12, 0x00);
	_driver_io->RegisterWrite(0x13, image_width - 1);
}

int SensorControl::set_detect_mode() {
	_driver_io->RegisterWrite(0x0B, 0x01); // Suspen mode
	_driver_io->RegisterWrite(0x07, 0x00); // Detect mode power up
	_driver_io->RegisterWrite(0x02, 0x02); // Finger on event interrupt enable
	// Detect range
	_driver_io->RegisterWrite(0x10, 0x05);
	_driver_io->RegisterWrite(0x11, 0x25);
	_driver_io->RegisterWrite(0x12, 0x40);
	_driver_io->RegisterWrite(0x13, 0x4f);
}

int SensorControl::AllRegisterRead() {
	unsigned char address;
	unsigned char value;
	for (address = 0; address < 0x14; address++) {
		_driver_io->RegisterRead(address, &value);
		printf("read: %x value: %x\n", address, value);
	}
	return 0;
}

int SensorControl::RegisterRead(unsigned char address, unsigned char *value) {
	int ret;
	ret = _driver_io->RegisterRead(address, value);
	printf("Read register address: %x value: %x return: %d\n", address, *value, ret);
}

int SensorControl::RegisterWrite(unsigned char address, unsigned char value) {
	int ret;
	ret = _driver_io->RegisterWrite(address, value);
	printf("Write register address: %x value: %x return: %d\n", address, value, ret);
}

int SensorControl::ImageRead(int image_width, int image_height) {
	_driver_io->RegisterWrite(0x0D, 0x01); //Timing generation on
	unsigned char* image_buffer = (unsigned char*)malloc(image_width * image_height);
	_driver_io->ImageCapture(image_buffer);
	ImageManager image_manager(image_width, image_height);
	image_manager.set_image_buffer(image_buffer);
	image_manager.SaveImageToBmp("egis_image.bmp");
	printf("Save image to: egis_image.bmp\n");
	_driver_io->RegisterWrite(0x0D, 0x00); //Timing generation off
}

int SensorControl::InterruptCheck(unsigned char dc_offset, unsigned char threshold) {
	unsigned char value;
	_driver_io->RegisterWrite(0x09, 0x00); // Gain
	_driver_io->RegisterWrite(0x03, dc_offset << 1); // Dc offset
	_driver_io->RegisterWrite(0x08, threshold << 1); // Threshold
	_driver_io->RegisterRead(0x08, &value);
	_driver_io->RegisterWrite(0x08, threshold << 1);
	usleep(1000 * 20);
	const int time_max = 100;
	int event_count = 0;
	unsigned int microseconds = 1000 * 10;
	for (int time_index = 0; time_index < time_max; time_index++) {
		usleep(microseconds);
		_driver_io->RegisterRead(0x01, &value);
		printf("%x ", value);
		if (value & 0x02) event_count++;
	}
	printf("\n");
	if ((float)event_count / (float)time_max > 0.2)
		printf("Interrupt has been received.\n");
	else
		printf("No interrupt received.\n");

}