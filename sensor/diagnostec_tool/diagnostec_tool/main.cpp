
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
	printf("%d. Interrupt check\n", k_check_interrupt);
	printf("%d. Data clean\n", k_clean_data);
	printf("%d. Exit\n", k_exit);
	int menu_num;
	scanf("%d", &menu_num);
	return menu_num;
}

int main(int argc, char *argv[]) {
	printf("love is an open door. 2015.8.12 02:17\n");
	DriverIO driver_io;
	SensorControl sensor_control(&driver_io);
	// Image begin 
	const int k_image_width = 0x80;
	const int k_image_height = 0x80;
	const char *filename = "/data/data/egistec.fingerauth.host.service/calibration_data";
	int return_code;
	int address, value;
	unsigned char value_char;
	int menu_num;
	int dc_offset, threshold;
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
			scanf("%d", &address);
			sensor_control.RegisterRead((unsigned char)address, &value_char);
			break;
		case K_write_register:
			printf("Enter register address: ");
			scanf("%d", &address);
			printf("Enter register value: ");
			scanf("%d", &value);
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
			printf("Enter DC offset: ");
			scanf("%d", &dc_offset);
			printf("Enter threshold: ");
			scanf("%d", &threshold);
			sensor_control.InterruptCheck((unsigned char)dc_offset, (unsigned char)threshold);
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