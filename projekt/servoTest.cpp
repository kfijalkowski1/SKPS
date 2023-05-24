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


int main() {
    std::cout << "Hello, Main!" << std::endl;

    export_pwm();

    uint16_t max_angle = MAX_DEGREE % STEP == 0 ? MAX_DEGREE : (MAX_DEGREE / STEP);
    uint16_t min_angle = MIN_DEGREE % STEP == 0 ? MIN_DEGREE : (MIN_DEGREE / STEP) * STEP + STEP;

    uint16_t angle = min_angle;
    int direction = STEP;

    while (true) {
        rotate_servo(angle);
        sleep(1);

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
}
