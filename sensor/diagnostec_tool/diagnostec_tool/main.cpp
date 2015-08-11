#include <stdio.h>
#include <stdlib.h>

#include "driver_io.h"
#include "image_manager.h"
#include "sensor_control.h"

int main(int argc, char *argv[]) {

	printf("love is an open door. 2015.8.11 11:49\n");
	DriverIO driver_io;
	SensorControl sensor_control(&driver_io);
	//image 
	const int k_image_width = 0x80;
	const int k_image_height = 0x80;
	sensor_control.set_power_on_mode();
	sensor_control.set_sensor_mode(k_image_width, k_image_height);
	sensor_control.ImageRead(k_image_width, k_image_height);
	sensor_control.AllRegisterRead();


}