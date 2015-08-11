#ifndef SENSOR_CONTROL_H_
#define SENSOR_CONTROL_H_
#include "driver_io.h"
class SensorControl {
public:
	SensorControl(DriverIO *driver_io) {
		_driver_io = driver_io;
	}
	int set_sensor_mode(int image_width, int image_height);
	int set_power_on_mode();
	int AllRegisterRead();
	int ImageRead(int image_width, int image_height);

private:
	DriverIO *_driver_io;
};

#endif // SENSOR_CONTROL_H_