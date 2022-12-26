/*
Make a custom servo that is controlled with a potentiometer.
*/

#include <ServoMotor.h>

//Required servo parameters: See example "setupParameters" for an explanation of each value.
constexpr int   POT_PIN    =  A2;
constexpr int   MOTOR_PIN1 =  5;
constexpr int   MOTOR_PIN2 =  3;
constexpr float GAIN_INT   =  0.5;
constexpr float GAIN_PROP  =  0.5;
constexpr float GAIN_DERIV =  0.05;

//Creating servo class with the above parameters
ServoMotor< POT_PIN, MOTOR_PIN1, MOTOR_PIN2, 
            &GAIN_INT, &GAIN_PROP, &GAIN_DERIV
          > servo;  

//Pin for potentiometer that controls servo 
constexpr int INPUT_PIN    =  A0;
            
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

