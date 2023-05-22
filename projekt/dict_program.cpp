#include <stdlib.h>
#include <stdio.h>
#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "bcm2835.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define VERSION_REQUIRED_MAJOR 1
#define VERSION_REQUIRED_MINOR 0
#define VERSION_REQUIRED_BUILD 1
#define SERVO_PIN 18


float measureDist(VL53L0X_Dev_t *pMyDevice)
{
    VL53L0X_RangingMeasurementData_t    RangingMeasurementData;

    VL53L0X_PerformSingleRangingMeasurement(pMyDevice,
            &RangingMeasurementData);

    return RangingMeasurementData.RangeMilliMeter;
}

void initDistFinder(VL53L0X_Dev_t *pMyDevice)
{

    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    int i;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    uint8_t VhvSettings;
    uint8_t PhaseCal;

    if(Status == VL53L0X_ERROR_NONE) { Status = VL53L0X_StaticInit(pMyDevice);}

    if(Status == VL53L0X_ERROR_NONE) { Status = VL53L0X_PerformRefCalibration(pMyDevice, &VhvSettings, &PhaseCal); }

    if(Status == VL53L0X_ERROR_NONE) { Status = VL53L0X_PerformRefSpadManagement(pMyDevice, &refSpadCount, &isApertureSpads);}

    if(Status == VL53L0X_ERROR_NONE) { Status = VL53L0X_SetDeviceMode(pMyDevice, VL53L0X_DEVICEMODE_SINGLE_RANGING); }

    if (Status == VL53L0X_ERROR_NONE) { Status = VL53L0X_SetLimitCheckEnable(pMyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1);}

    if (Status == VL53L0X_ERROR_NONE) { Status = VL53L0X_SetLimitCheckEnable(pMyDevice, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1);}

    if (Status == VL53L0X_ERROR_NONE) { Status = VL53L0X_SetLimitCheckValue(pMyDevice,VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.1*65536));}

    if (Status == VL53L0X_ERROR_NONE) { Status = VL53L0X_SetLimitCheckValue(pMyDevice, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(60*65536));}

    if (Status == VL53L0X_ERROR_NONE) { Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(pMyDevice, 33000);}

    if (Status == VL53L0X_ERROR_NONE) { Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);}

    if (Status == VL53L0X_ERROR_NONE) { Status = VL53L0X_SetVcselPulsePeriod(pMyDevice, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);}

}


// servo part: https://www.circuitden.com/blog/16

void move(int angle, int prev_angle)
{
    int j;
    float hightime = 10.3*(float)angle + 546;
    char c;
    int pulse = (int)abs(((angle - prev_angle)*33)/180) + 10;
    for(j=0; j<pulse; j++)//exucting pulses
    {
        bcm2835_gpio_set(SERVO_PIN);
        bcm2835_delayMicroseconds((int)hightime);
        bcm2835_gpio_clr(SERVO_PIN);
        bcm2835_delayMicroseconds(20000 - (int)hightime);//each pulse is 20ms
    }
}


int main(int argc, char **argv)
{
    int max_angle = atoi(argv[1]);
    int angle_inc = atoi(argv[2]);
    // distance reader part
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;
    VL53L0X_Dev_t MyDevice;
    VL53L0X_Dev_t *pMyDevice = &MyDevice;
    float dist;

    // Initialize dist
    pMyDevice->I2cDevAddr      = 0x29;

    pMyDevice->fd = VL53L0X_i2c_init("/dev/i2c-1", pMyDevice->I2cDevAddr);//choose between i2c-0 and i2c-1; On the raspberry pi zero, i2c-1 are pins 2 and 3
    if (MyDevice.fd<0) {
        Status = VL53L0X_ERROR_CONTROL_INTERFACE;
        printf ("Failed to init\n");
    }

    if(Status == VL53L0X_ERROR_NONE)
    {
        printf ("Call of VL53L0X_DataInit\n");
        Status = VL53L0X_DataInit(&MyDevice); // Data initialization
    }

    // servo part
    if(!bcm2835_init()) return 1;


    bcm2835_gpio_fsel(SERVO_PIN, BCM2835_GPIO_FSEL_OUTP); //set pin 18 as output
    int prev_angle = 0;
    int cur_angle = 0;

    for(;cur_angle < max_angle; cur_angle+=angle_inc)
    {
        move(cur_angle, prev_angle);
        prev_angle = cur_angle; //keeping track of previous angle to later calculate number of pulses
        dist = measureDist(pMyDevice);
        printf("Dist: %f", dist);
    }

    printf ("Close Comms\n");
    VL53L0X_i2c_close();
    return (0);
}