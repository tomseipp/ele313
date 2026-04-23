// Adam was actually here 22/4/26
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ch.h"
#include "hal.h"
#include "memory_protection.h"
#include <main.h>
#include "leds.h"
#include "spi_comm.h"
#include "sensors/proximity.h"
#include "motors.h"
#include "epuck1x/uart/e_uart_char.h"
#include "stdio.h"
#include "serial_comm.h"
#include "selector.h"

messagebus_t bus;
MUTEX_DECL(bus_lock);
CONDVAR_DECL(bus_condvar);



// Initialising stuff

int main(void){ 
    halInit();
    chSysInit();
    mpu_init();

    // Proximity
    messagebus_init(&bus, &bus_lock, &bus_condvar);
    proximity_start(0);
    calibrate_ir();

    //LED
    clear_leds();
    spi_comm_start();
	

    //Motors
    motors_init();

	//Bluetooth
	serial_start();

    
    // variable declarations
	int front_right;
	int front_left;
	int corner_right;
	int corner_left;
	int side_right;
	int side_left;

	int threshold = 600;

	while (1){ // infinite Main Loop!
		while (get_selector() > 1){ // when selector ==1 robot wont move
			front_right = get_calibrated_prox(0);
			front_left = get_calibrated_prox(7);
			corner_right = get_calibrated_prox(1);
			corner_left = get_calibrated_prox(6);
			side_right = get_calibrated_prox(2);
			side_left = get_calibrated_prox(5);

			if (front_right >threshold|| front_left>threshold){
				//code to turn 90 degrees
				set_led(LED1,1);
				set_led(LED3,0);
				set_led(LED7,0);
				set_rgb_led(LED2,0,0,0);
				set_rgb_led(LED8,0,0,0);
				if (side_right > threshold){
					//if something is on right side turn left
					left_motor_set_speed(-500);
					right_motor_set_speed(500); 
					chThdSleepMilliseconds(300);
				}else{
					//turn right
					left_motor_set_speed(500);
					right_motor_set_speed(-500); 
					chThdSleepMilliseconds(300);
				}
			
			} else if(corner_right >threshold|| corner_left>threshold){
				set_led(LED1,0);
				set_led(LED3,0);
				set_led(LED7,0);
				//code to turn 45 ish degrees
				if(corner_right >threshold && corner_left>threshold){
					set_rgb_led(LED2,10,0,0);
					set_rgb_led(LED8,10,0,0);
					//turn around
					left_motor_set_speed(500);
					right_motor_set_speed(-500); 
					chThdSleepMilliseconds(600);
				} else if (corner_right > threshold){
					set_rgb_led(LED2,10,0,0);
					set_rgb_led(LED8,0,0,0);
					//if something is on right side turn left
					left_motor_set_speed(200);
					right_motor_set_speed(500); 
					
				}else{
					set_rgb_led(LED2,0,0,0);
					set_rgb_led(LED8,10,0,0);
					//turn right
					left_motor_set_speed(500);
					right_motor_set_speed(200); 
				}
			
			}else if(side_right >threshold|| side_left>threshold){
				set_led(LED1,0);
				set_rgb_led(LED2,0,0,0);
				set_rgb_led(LED8,0,0,0);
				
				//code to turn 20 ish degrees
				if(side_right >threshold && side_left>threshold){
					set_led(LED3,1);
					set_led(LED7,1);
					left_motor_set_speed(500);
					right_motor_set_speed(-500); 
					chThdSleepMilliseconds(600);
				} else if (side_right > threshold){
					set_led(LED3,1);
					set_led(LED7,0);
					//if something is on right side turn left
					left_motor_set_speed(600);
					right_motor_set_speed(700); 
				}else{
					set_led(LED3,0);
					set_led(LED7,1);
					//turn right
					left_motor_set_speed(700);
					right_motor_set_speed(600);
				}
			
			}

		
		} //end of selector loop
	} // end of infinite loop
	
   
} // end of main

