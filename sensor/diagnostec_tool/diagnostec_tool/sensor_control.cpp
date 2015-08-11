#include "sensor_control.h"

#include <stdio.h>
#include <stdlib.h>

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
	_driver_io->RegisterWrite(0x09, 0x00);
	//	Leave suspend
	_driver_io->RegisterWrite(0x08, 0x01); //Detect mode power down
	_driver_io->RegisterWrite(0x0B, 0x00); //Normal mode
	// Over temperature control
	_driver_io->RegisterWrite(0x05, 0x02); // Power down
	//
	_driver_io->RegisterWrite(0x10, 0x00);
	_driver_io->RegisterWrite(0x11, image_height - 1);
	_driver_io->RegisterWrite(0x12, 0x00);
	_driver_io->RegisterWrite(0x13, image_width - 1);
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

int SensorControl::ImageRead(int image_width, int image_height) {
	_driver_io->RegisterWrite(0x0D, 0x01); //Timing generation on
	unsigned char* image_buffer = (unsigned char*)malloc(image_width * image_height);
	_driver_io->ImageCapture(image_buffer);
	ImageManager image_manager(image_width, image_height);
	image_manager.set_image_buffer(image_buffer);
	image_manager.SaveImageToBmp("egis_image.bmp");
	_driver_io->RegisterWrite(0x0D, 0x00); //Timing generation off
}