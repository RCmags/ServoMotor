//------------------ Initialization ------------------ 

    //- Set Pins, variables and fill arrays
template <TEMPLATE_TYPE_INPUTS>
void ServoMotor<TEMPLATE_INPUTS>::initialize( void ) {  
        //Set pins
  pinMode( POT_PIN, INPUT );
  pinMode( MOTOR_PIN1, OUTPUT );
  pinMode( MOTOR_PIN2, OUTPUT ); 

  digitalWrite( MOTOR_PIN1, LOW );
  digitalWrite( MOTOR_PIN2, LOW );
  
        //Default values
  integral = 0;
  last_time = millis();
  
  x[0] = readSensor();
  x[1] = x[0];
}

//----------

template <TEMPLATE_TYPE_INPUTS>
float ServoMotor<TEMPLATE_INPUTS>::updateTimer() {
  uint32_t curr_time = micros();
  float dt  = float(curr_time - last_time)*1e-6;
  last_time = curr_time; 
  return dt;	
}

template <TEMPLATE_TYPE_INPUTS>
void ServoMotor<TEMPLATE_INPUTS>::filterSensor(float dt) {
  // alpha-beta filter:
  constexpr float BETA = (*ALPHA) * (*ALPHA) * 0.25; 
    // error
  float xi = readSensor();
  float dx = 0.5*( xi - x[1] );
    // position
  float xnew = x[0];
  xnew += v*dt;  
  xnew += (*ALPHA) * dx; 
    // velocity
  v += BETA * dx/dt;

  // store past position
  x[1] = x[0];
  x[0] = xnew;
}

//------------------ PID control --------------------- 

    //- Rectangular integral of a given input
template <TEMPLATE_TYPE_INPUTS>
float ServoMotor<TEMPLATE_INPUTS>::rectIntegral( float input, float dt ) {   
        //Integrate                     
  integral += (*GAIN_INT) * input * dt;
        //Prevent integral windup
  integral = constrain( integral, -PWM_MAX, PWM_MAX );
  
  return integral;
}


    //- Calculate PID signal to reach a target value
template <TEMPLATE_TYPE_INPUTS>
float ServoMotor<TEMPLATE_INPUTS>::pidOutput( int input ) {   
		// timer
  float dt = updateTimer();
  filterSensor(dt);
    
  float output = x[0] - float(input);
        //Filter error
  output = deadband( output, SENSOR_DEADBAND );
        //Combine PID terms
  output = (*GAIN_PROP)*output + (*GAIN_DERIV)*v + rectIntegral(output, dt);
        //Account for analogWrite Saturation
  output = constrain( output, -PWM_MAX, PWM_MAX );

  return output; 
}

//------------------ Miscellaneous  ----------------
  
    //- H-Bridge control function: Given a speed and direction, send PWM signal to appropriate pin
template <TEMPLATE_TYPE_INPUTS>
void ServoMotor<TEMPLATE_INPUTS>::setDirection( int input ) { 
  if( input >= 0 ) { 
    digitalWrite( MOTOR_PIN2, LOW );  
    analogWrite( MOTOR_PIN1, input ); 
  } else if(input < 0 ) {
    digitalWrite( MOTOR_PIN1, LOW );
    analogWrite( MOTOR_PIN2, -input ); 
  } 
}


    //- Set values to zero if within the interval [-band, band]
template <TEMPLATE_TYPE_INPUTS>
float ServoMotor<TEMPLATE_INPUTS>::deadband( float input, float band ) {
  if( input >= band ) {
    return input - band;
  } else if ( input <= -band ) {
    return input + band;
  } else {
    return 0;
  }
}

//------------------ Position control ------------------ 

    //- Return sensor reading
template <TEMPLATE_TYPE_INPUTS>
int ServoMotor<TEMPLATE_INPUTS>::readSensor(void) {
  return analogRead(POT_PIN);
}

    //- Update the sensor filters then set motor direction and speed
template <TEMPLATE_TYPE_INPUTS>
void ServoMotor<TEMPLATE_INPUTS>::setPosition( int input ) {   
  float target = pidOutput(input);
  setDirection(target);
}
