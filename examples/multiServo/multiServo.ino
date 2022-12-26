/*
Make two custom servos that are controlled by a single potentiometer. 
*/

#include <ServoMotor.h>

//- Parameters: See example "setupParameters" for an explanation of each value.

//Servo 1:
constexpr int   SERVO_POT_PIN     =  A2;
constexpr int   SERVO_MOTOR_PIN1  =  5;
constexpr int   SERVO_MOTOR_PIN2  =  3;
constexpr float SERVO_GAIN_INT    =  0.0;
constexpr float SERVO_GAIN_PROP   =  0.5;
constexpr float SERVO_GAIN_DERIV  =  0.05;

//Servo 2:
constexpr int   SERVO2_POT_PIN    =  A1;
constexpr int   SERVO2_MOTOR_PIN1 =  6;
constexpr int   SERVO2_MOTOR_PIN2 =  9;
constexpr float SERVO2_GAIN_INT   =  0.5;
constexpr float SERVO2_GAIN_PROP  =  0.5;
constexpr float SERVO2_GAIN_DERIV =  0.05;

//Each servo class is created via a seperate template:

//Servo 1:
ServoMotor< SERVO_POT_PIN, SERVO_MOTOR_PIN1, SERVO_MOTOR_PIN2, 
            &SERVO_GAIN_INT, &SERVO_GAIN_PROP, &SERVO_GAIN_DERIV
          > servo;  

//Servo 2:
ServoMotor< SERVO2_POT_PIN, SERVO2_MOTOR_PIN1, SERVO2_MOTOR_PIN2, 
            &SERVO2_GAIN_INT, &SERVO2_GAIN_PROP, &SERVO2_GAIN_DERIV
          > servo2; 

//Pin for potentiometer that controls servo 
constexpr int INPUT_PIN = 	A0;

void setup() {
  //Setting pot pin:
  pinMode( INPUT_PIN, INPUT );

  //Setting up servos:
  servo.initialize();
  servo2.initialize();
}

void loop() {
  //Position the servos try to hold: must be a value from 0 to 1024
  int targetPosition = analogRead( INPUT_PIN );
  
  //Updating servos
  servo.setPosition( targetPosition );    
  servo2.setPosition( targetPosition );    
}

