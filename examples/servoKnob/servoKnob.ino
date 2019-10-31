/*
Make a custom servo that is controlled with a potentiometer.
*/

#include <ServoMotor.h>

//Required servo parameters: See example "setupParameters" for an explanation of each value.
const int POT_PIN =     	A2;
const int MOTOR_PIN1 =  	5;
const int MOTOR_PIN2 =  	3;
const int GAIN_INT =    	10;
const int GAIN_PROP =     	50;
const int GAIN_DERIV =  	100;
const int GAIN_DERIV2 = 	50;

//Creating servo class with the above parameters
ServoMotor< POT_PIN, MOTOR_PIN1, MOTOR_PIN2, 
            GAIN_INT, GAIN_PROP, GAIN_DERIV , GAIN_DERIV2
            > 
            servo;  

//Pin for potentiometer that controls servo 
const int INPUT_PIN =   A0;
            
void setup() {
  //Setting pot pin:
  pinMode( INPUT_PIN, INPUT );
  
  //Setting servo:
  servo.initialize();
}

void loop() {
  //Position the servo tries to hold: must be a value from 0 to 1024
  int targetPosition = analogRead( INPUT_PIN );

  //Update servo
  servo.setPosition( targetPosition );        
}

