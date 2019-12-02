#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "eecs388_lib.h"
#define SERVO_PULSE_MAX 2400    /* 2400 us */
    #define SERVO_PULSE_MIN 544     /* 544 us */
    #define SERVO_PERIOD    20000   /* 20000 us (20ms) */
 void servo(int gpio, int pos)
    {
    int pulse = SERVO_PULSE_MIN + ((SERVO_PULSE_MAX-SERVO_PULSE_MIN)/180)*pos;
        gpio_write(gpio, ON);
       delay_usec(pulse); 
       gpio_write(gpio, OFF);
       delay_usec(SERVO_PERIOD-pulse);
    }
int main()
{   //initialize RGB LEDS
    int gpio1 = GREEN_LED;
    int gpio2 = RED_LED;
    int gpio3 = BLUE_LED;
    gpio_mode(gpio1, OUTPUT);
    gpio_mode(gpio2, OUTPUT);
    gpio_mode(gpio3, OUTPUT);
    gpio_mode(PIN_19, OUTPUT);
    gpio_write(gpio1,ON);
    gpio_write(gpio2,ON);
    gpio_write(gpio3,ON);
    // initialize UART channels
    ser_setup(0); // uart0 (LIDAR)
    ser_setup(1); // uart1 (raspberry pi)

    printf("Setup completed.\n");
    printf("Begin the main loop.\n");

    int prev_dist = 0;
    int prev_angle = 0; 
    int dist = 0;
    float data = 0;
    while (1) {
        if ('Y' == ser_read(0) && 'Y' == ser_read(0)) {
            int dist1 = ser_read(0);
            int dist2 =  ser_read(0);
            int dist3 = dist2<<8;
            dist = dist1 | dist3; 
           // printf("dist = %d\n", dist);  //print stmnt for dist.
        }
          if(dist < 50){
                gpio_write(gpio1,OFF);
                gpio_write(gpio2,ON);
                gpio_write(gpio3,OFF);
            }
          else if(dist > 50){
              gpio_write(gpio1,ON);

              gpio_write(gpio3,ON);
          }
        if(ser_isready(1)){
            float data = '0' + ser_read(1);
           // printf("angle = %f\n", data); //print stmnt for angle.
            servo(PIN_19, data);

            if(data < -30){
                gpio_write(gpio1,ON);
                gpio_write(gpio2,OFF);
                gpio_write(gpio3,OFF);
            }
            else if(data > 30){
                gpio_write(gpio1,OFF);
                gpio_write(gpio2,OFF);
                gpio_write(gpio3,ON);
            }
            else{
                gpio_write(gpio1,ON);
                gpio_write(gpio2,ON);
                gpio_write(gpio3,ON);
            }
          
            
        }
    }
  
}
 