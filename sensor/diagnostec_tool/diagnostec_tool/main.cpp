
#include <stdio.h>
#include <stdlib.h>


#include "driver_io.h"
#include "image_manager.h"
#include "sensor_control.h"

enum {
	k_read_register_table = 1,
	k_read_register,
	K_write_register,
	k_capture_image,
	k_check_interrupt,
	k_clean_data,
	k_exit
};

int Menu() {

	printf("%d. All register read\n", k_read_register_table);
	printf("%d. Register read\n", k_read_register);
	printf("%d. Register write\n", K_write_register);
	printf("%d. Image capture\n", k_capture_image);
	printf("%d. Interrupt check (software)\n", k_check_interrupt);
	printf("%d. Data clean\n", k_clean_data);
	printf("%d. Exit\n", k_exit);
	int menu_num;
	scanf("%d", &menu_num);
	return menu_num;
}

int main(int argc, char *argv[]) {
	printf("ET300. 2015.8.13 11:06\n");
	DriverIO driver_io;
	SensorControl sensor_control(&driver_io);
	// Image begin 
	const int k_image_width = 0x80;
	const int k_image_height = 0x80;
	const char *filename = "/data/data/egistec.fingerauth.host.service/calibration_data";
	int return_code;
	int address, value;
	unsigned char value_char;
	char buffer_char[255];
	int menu_num;
	int dc_offset, threshold, interval, times;
	bool is_continue = true;
	do {
		menu_num = Menu();
		printf("The function %d\n", menu_num);
		switch (menu_num) {
		case k_read_register_table:
			sensor_control.AllRegisterRead();
			break;
		case k_read_register:
			printf("Enter register address: ");
			scanf("%x", &address);
			sensor_control.RegisterRead((unsigned char)address, &value_char);
			break;
		case K_write_register:
			printf("Enter register address: ");
			scanf("%x", &address);
			printf("Enter register value: ");
			scanf("%x", &value);
			sensor_control.RegisterWrite((unsigned char)address, (unsigned char)value);
			break;
		case k_capture_image:
			sensor_control.set_power_on_mode();
			sensor_control.set_sensor_mode(k_image_width, k_image_height);
			sensor_control.ImageRead(k_image_width, k_image_height);
			break;
		case k_check_interrupt:
			sensor_control.set_power_on_mode();
			sensor_control.set_detect_mode();
			printf("Enter DC offset: (default: 0x80)");
			//fflush(stdin);
			//fgets(buffer_char, sizeof(buffer_char), stdin);
			scanf("%x", &dc_offset);
			printf("Enter threshold: (default: 0x04)");
			scanf("%x", &threshold);
			printf("Enter time interval: (default: 5 ms)");
			scanf("%d", &interval);
			printf("Enter the number of : (default: 100)");
			scanf("%d", &times);
			sensor_control.InterruptCheck((unsigned char)dc_offset, 
				(unsigned char)threshold, interval, times);
			break;
		case k_clean_data:
			remove(filename);
			printf("Remove file: %s\n", filename);
			break;
		case k_exit:
			is_continue = false;
			break;
		}
		if (is_continue) {
			printf("Press \"Enter\" key to menu.\n");
			fflush(stdout); // Will now print everything in the stdout buffer
			system("read");
		}
	} while (is_continue);



}