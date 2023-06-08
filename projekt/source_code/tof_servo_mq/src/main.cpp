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

// #define VIRT

// MQ part
#include <bits/stdc++.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT	 5005
#define ADDRESS "127.0.0.1"

void write(const std::string &file_path, const std::string &text) {
    std::ofstream file;
    file.open(file_path);
    file << text << std::endl;
    file.close();
}

void export_pwm() {
    #ifndef VIRT:
        write(PWM_CHIP "/export", "0");
    #endif
}

void unexport_pwm() {
    #ifndef VIRT:
        write(PWM_CHIP "/unexport", "0");
    #endif
}

void rotate_servo(uint16_t angle) {
    uint32_t duty_cycle = MIN_DUTY_CYCLE + (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE) *
                                               (angle - MIN_DEGREE) / (MAX_DEGREE - MIN_DEGREE);
    #ifndef VIRT:
        write(PWM_CHANNEL "/period", "20000000");
        write(PWM_CHANNEL "/duty_cycle", std::to_string(duty_cycle));
        write(PWM_CHANNEL "/enable", "1");
    #endif

    usleep(250000);
    #ifndef VIRT:
        write(PWM_CHANNEL "/enable", "0");
    #endif
}


int main(int argc, char *argv[])
{
	// servo
	std::cout << "In Main!" << "server ip adress: " << argv[1] << std::endl;

    export_pwm();

    uint16_t max_angle = (MAX_DEGREE % STEP == 0) ? MAX_DEGREE : ((MAX_DEGREE / STEP) * STEP);
    uint16_t min_angle = (MIN_DEGREE % STEP == 0) ? MIN_DEGREE : (MIN_DEGREE / STEP) * STEP + STEP;

    uint16_t angle = min_angle;
    int direction = STEP;
    int no_steps = (max_angle - min_angle) / STEP;

    // tof
	int measuredDistance;
    float angles_rad = 0;

    //MQ
    int sockfd;
    struct sockaddr_in	 servaddr;


    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);


    // TOF
	// For Raspberry Pi's, the I2C channel is usually 1
	// For other boards (e.g. OrangePi) it's 0
    // arguments: i2c port 0x29 - adress where i2c is in RB4b and I2C chanel is 1
    // 
    #ifndef VIRT:
        int initRes = tofInit(1, 0x29, 1); // set long range mode (up to 2m)
        if (initRes != 1)
        {
            return -1; // problem - quit
        }
    #endif
	printf("VL53L0X device successfully opened.\n");

    std::string msg;
    //main loop

	for(int j = 0; j < no_steps; ++j) {
        rotate_servo(angle);
        #ifndef VIRT:
		    measuredDistance = tofReadDistance();
        #else:
            measuredDistance = 5;
            printf("Measured: %d\n",  measuredDistance);
        #endif

		msg = std::to_string(measuredDistance) + " " + std::to_string(angles_rad);
        char* char_msg = new char[msg.size()+1];
        strcpy(char_msg, msg.c_str());
        sendto(sockfd,  char_msg, strlen(char_msg),
               MSG_CONFIRM, (const struct sockaddr *) &servaddr,
               sizeof(servaddr));
        delete[] char_msg;
        // nowe wartości dla servo
        angle += direction;
        if (angle >= MAX_DEGREE) {
            angle = max_angle;
            direction = -direction;
        } else if (angle <= MIN_DEGREE) {
            angle = min_angle;
            direction = -direction;
        }
        angles_rad += (float) (STEP) / 57.0;
    }

return 0;
} /* main() */
