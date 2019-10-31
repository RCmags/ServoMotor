#include <ServoMotor.h>

//Required parameters - these must be defined for each servo

	//- Pins:

	//Pin of potentiometer attached to servo
const int POT_PIN =		 A2;	
	
	//H-bridge pins - Swap to reverse motor
const int MOTOR_PIN1 =	 5;
const int MOTOR_PIN2 =	 3;

	//- PID gains: 
	
	//Integral of error: Builds up pressure until servo moves to desired position 
const int GAIN_INT =	 10;

	//Proportional term: Adds a spring to the servo.
const int GAIN_PROP =	 50;

	//First deriative of error: Adds damping to the servo. 
const int GAIN_DERIV =	 100;

	//Second derivative of error: helps removing wobble caused the derivative term. Especially noticesable with large refresh intervals
const int GAIN_DERIV2 =	 50;

//Comment out this block of code to adjust optional parameters
 
ServoMotor< POT_PIN, MOTOR_PIN1, MOTOR_PIN2, 
			GAIN_INT, GAIN_PROP, GAIN_DERIV , GAIN_DERIV2
			> 
			servo; 

//Remove multi-line comments to adjust optional parameters
/*
	//Refresh interval of PID loop: Longer values smoothen the derivative terms but add a delay to the reaction time of the servo.
const int INTERVAL =	 10;	  

	//- Sensor low pass filter: This removes noise in the signal of the position sensor

	//Number of poles: More poles make the output signal smoother at the cost of greater delay - Use the smallest number possible
const int N_FILT =		 2;

	//Decay of filter: Controls the cutoff frequency of the filter. MUST BE GREATEAR THAN 0 and CANNOT EXCEED 255 or it becomes unstable.
	//				   A lower value means a lower cutoff frequency. This increases signal smoothness but also increases delay. 
const int FILTER_GAIN =	 200;

	//Deadband of error used for PID loop. Supresses noise at the cost of less precise motion.
const int DEADBAND =	 1;

ServoMotor< POT_PIN, MOTOR_PIN1, MOTOR_PIN2, 
			GAIN_INT, GAIN_PROP, GAIN_DERIV , GAIN_DERIV2,
			INTERVAL, N_FILT, FILTER_GAIN, DEADBAND
			> 
			servo;	
*/

void setup() {
  //Each servo must be initialized before use
  servo.initialize();
}

void loop() {
  //Target position must be a value from 0 to 1024
  servo.setPosition( 500 );		   
}

