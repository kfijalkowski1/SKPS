// TOF PART - https://github.com/bitbank2/VL53L0X
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "tof.h" // time of flight sensor library

// servo part - 
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <random>
#include <string>

#define MAX_DEGREE 180
#define MIN_DEGREE 0
#define STEP 5

#define MIN_DUTY_CYCLE 1'000'000
#define MAX_DUTY_CYCLE 2'000'000

#define PWM_CHIP "/sys/class/pwm/pwmchip0"
#define PWM_CHANNEL "/sys/class/pwm/pwmchip0/pwm0"

void write(const std::string &file_path, const std::string &text) {
    std::ofstream file;
    file.open(file_path);
    file << text << std::endl;
    file.close();
}

void export_pwm() {
    write(PWM_CHIP "/export", "0");
}

void unexport_pwm() {
    write(PWM_CHIP "/unexport", "0");
}

void rotate_servo(uint16_t angle) {
    uint32_t duty_cycle = MIN_DUTY_CYCLE + (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE) *
                                               (angle - MIN_DEGREE) / (MAX_DEGREE - MIN_DEGREE);
    write(PWM_CHANNEL "/period", "20000000");
    write(PWM_CHANNEL "/duty_cycle", std::to_string(duty_cycle));
    write(PWM_CHANNEL "/enable", "1");

    usleep(250000);
    write(PWM_CHANNEL "/enable", "0");
}



int main(int argc, char *argv[])
{
	// servo
	std::cout << "Hello, Main!" << std::endl;

    export_pwm();

    uint16_t max_angle = MAX_DEGREE % STEP == 0 ? MAX_DEGREE : (MAX_DEGREE / STEP);
    uint16_t min_angle = MIN_DEGREE % STEP == 0 ? MIN_DEGREE : (MIN_DEGREE / STEP) * STEP + STEP;

    uint16_t angle = min_angle;
    int direction = STEP;
    int no_steps = max_angle / STEP;


	int iDistance;
	int model, revision;

    // TOF
	// For Raspberry Pi's, the I2C channel is usually 1
	// For other boards (e.g. OrangePi) it's 0
	int initRes = tofInit(0, 0x29, 1); // set long range mode (up to 2m)
	if (initRes != 1)
	{
		return -1; // problem - quit
	}
	printf("VL53L0X device successfully opened.\n");
	initRes = tofGetModel(&model, &revision);
	printf("Model ID - %d\n", model);
	printf("Revision ID - %d\n", revision);

	for(int j = 0; j < no_steps; ++j) {
        rotate_servo(angle);

		iDistance = tofReadDistance();
		printf("Distance = %dmm\n", iDistance);
		usleep(50000); // 50ms

        angle += direction;
        if (angle >= MAX_DEGREE) {
            angle = max_angle;
            direction = -direction;
        } else if (angle <= MIN_DEGREE) {
            angle = min_angle;
            direction = -direction;
        }
    }

return 0;
} /* main() */
