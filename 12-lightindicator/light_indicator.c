// light_indicator.c
//
// Example program for bcm2835 library
// Outputs the lighting sensor data using 8 leds
//
// After installing bcm2835, you can build this 
// with something like:
// gcc -o light_indicator light_indicator.c -l bcm2835
// sudo ./light_indicator
//
// Or you can test it before installing with:
// gcc -o light_indicator -I ../../src ../../src/bcm2835.c light_indicator.c
// sudo ./light_indicator
//
// Author: Kristobal Junta
// Based on: Bart Tanghe samples

#include <bcm2835.h>
#include <stdio.h>
#include <math.h>

#define MIN(a,b) (((a)<(b))?(a):(b))

#define scale 128   // step for an output value. each led indicates <step> lm
#define refresh_time_ms 100 // time between retrieving sensor data

int main(int argc, char **argv)
{
    if (!bcm2835_init())
        return 1;
    
    // leds initialization
    int j;
    int pins[10];
    pins[0] = RPI_V2_GPIO_P1_07;
    pins[1] = RPI_V2_GPIO_P1_08;
    pins[2] = RPI_V2_GPIO_P1_18;
    pins[3] = RPI_V2_GPIO_P1_16;
    pins[4] = RPI_V2_GPIO_P1_15;
    pins[5] = RPI_V2_GPIO_P1_13;
    pins[6] = RPI_V2_GPIO_P1_12;
    pins[7] = RPI_V2_GPIO_P1_11;
    
    // turning all the leds off
    for(j=0;j<8;j++)
    {
        bcm2835_gpio_fsel(pins[j], BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(pins[j], LOW);
    }
    
    char temp[1];               //temporary values
    int ret;
    int ad[2];

    bcm2835_i2c_begin();
    bcm2835_i2c_setSlaveAddress(0x29);      // addr pin attached to ground
    bcm2835_i2c_set_baudrate(1000);         // Default

    temp[0] = 0xa0;             //select the control register
    bcm2835_i2c_write(temp,1);
    temp[0] = 0x03;             //Power up the device
    bcm2835_i2c_write(temp,1);
    bcm2835_delay(500);

    bcm2835_i2c_read(temp,1);
    
    while(1)
    {
        temp[0] = 0xac;             //Channel 0 lower byte
        bcm2835_i2c_write(temp,1);      
        bcm2835_i2c_read(temp,1);

        ad[1]= (int)temp[0];

        temp[0] = 0xad;             //channel 0 upper byte
        bcm2835_i2c_write(temp,1);
        bcm2835_i2c_read(temp,1); 

        ad[0] = (int)temp[0]; // getting the sensor value
        
        int res = ad[0]*256+ad[1]; // calculating how many pins we need, based on <scale>
        res = MIN(res / scale, 8);
        
        for(j=0;j<8;j++)
        {
            bcm2835_gpio_write(pins[j], LOW); // turning the pins off
        }
        for(j=0;j<res;j++)
        {
            bcm2835_gpio_write(pins[j], HIGH);
        }
        bcm2835_delay(refresh_time_ms);
    }
    
    bcm2835_i2c_end();
    bcm2835_close();
    
    return 0;
}