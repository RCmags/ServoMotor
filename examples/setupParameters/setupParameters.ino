/*
Make a servo that holds a fixed position.

This sketch explains the results of each parameter required by the servoMotor class. 
It also shows how to adjust the PID refresh interval and the sensor low pass filter.
*/

#include <ServoMotor.h>

//Required parameters - these must be defined for each servo:

    //- Arduino Pins:

    //Sensor pin: MUST be an analog pin. It reads the signal of the sensor attached to the servo. Generally a potentiometer.
constexpr int POT_PIN =      A2;    
    
    //H-bridge pins: BOTH MUST be capable of pwm output. These control the DC motor. Swap them to reverse the direction of rotation.
constexpr int MOTOR_PIN1 =   5;
constexpr int MOTOR_PIN2 =   3;

    //- PID gains: Controls the behaviour of the servo. All values are scaled from 0 to 255. They can exceed 255 and can also be negative. 
    
    //Integral of error: Builds up pressure until servo moves to the desired position. Easily induces wobble. Damped by the proportional term. 
constexpr float GAIN_INT =     0.5;

    //Proportional term: Adds a centering force that tries to align the servo with the desired position. Acts like a spring. 
constexpr float GAIN_PROP =    0.5;

    //First deriative of measurement: Adds a resistive force that slows down the servo. Acts like a viscous damper. Damps the proportional term. 
constexpr float GAIN_DERIV =   0.05;
 
// /*Creating a servo with required parameters:        [Comment out this block of code to adjust optional parameters] 
ServoMotor< POT_PIN, MOTOR_PIN1, MOTOR_PIN2, 
            &GAIN_INT, &GAIN_PROP, &GAIN_DERIV
          > servo; 
// */

//Remove multi-line comments to adjust optional parameters
/*
    //Decay of filter:  Controls the cutoff frequency of the filter. A lower value means a lower cutoff frequency. 
    //                  This increases signal smoothness but also increases delay.
    //                  MUST BE GREATEAR THAN 0 AND SMALLER THAN 1 or it becomes unstable.
constexpr float FILTER_GAIN =  0.1;

    //Deadband of error used for PID loop:  Supresses noise at the cost of less precise motion.
    //                                      MUST BE GREATEAR OR EQUAL 0
constexpr int DEADBAND =     2;

//Creating a servo with optional parameters:
ServoMotor< POT_PIN, MOTOR_PIN1, MOTOR_PIN2, 
            &GAIN_INT, &GAIN_PROP, &GAIN_DERIV,
            &FILTER_GAIN, DEADBAND
            > servo;  
*/

void setup() {
  //Each servo must be initialized before use
  servo.initialize();
}

void loop() {
  //Target position must be a value from 0 to 1024
  servo.setPosition( 500 );        
}
