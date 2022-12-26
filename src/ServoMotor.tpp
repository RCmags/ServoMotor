//------------------ Initialization ------------------ 

    //- Set Pins and variables
template <TEMPLATE_TYPE_INPUTS>
void ServoMotor<TEMPLATE_INPUTS>::initialize( void ) {  
  //Set pins
  pinMode( POT_PIN, INPUT );
  pinMode( MOTOR_PIN1, OUTPUT );
  pinMode( MOTOR_PIN2, OUTPUT ); 

  digitalWrite( MOTOR_PIN1, LOW );
  digitalWrite( MOTOR_PIN2, LOW );
  
  //Default values
  last_time = millis();
  
  x[0] = 0;     // set to zero error
  x[1] = x[0];
  
  sxdt = 0;     // assume zero initial conditions
  dx_dt = 0;
}

//----------

template <TEMPLATE_TYPE_INPUTS>
void ServoMotor<TEMPLATE_INPUTS>::calculus(float input) {
  // 0. timer
  uint32_t curr_time = micros();
  float dt  = float(curr_time - last_time)*1e-6;
  last_time = curr_time; 
  
  // 1. smoother and derivative: alpha-beta filter 
  constexpr float BETA = (*ALPHA) * (*ALPHA) * 0.25; 
    // error
  float xi = readSensor() - input;
  xi = deadband(xi, SENSOR_DEADBAND);
  
  float dx = 0.5*( xi - x[1] );
    // position
  float xnew = x[0];
  xnew += dx_dt*dt;  
  xnew += (*ALPHA) * dx; 
    // velocity
  dx_dt += BETA * dx/dt;

    // store past position
  x[1] = x[0];
  x[0] = xnew;

  // 2. trapezoidal integral:
  float xdt = 0.5 * ( x[0] + x[1] ) * dt;
  sxdt += (*GAIN_INT) * xdt;
  sxdt = constrain(sxdt, -PWM_MAX, PWM_MAX);    
}

//------------------ PID control --------------------- 

    //- Calculate PID signal to reach a target value
template <TEMPLATE_TYPE_INPUTS>
float ServoMotor<TEMPLATE_INPUTS>::PIDcontroller( int input ) {   
  // smoothen, derivative, integral
  calculus(input); 
  float output;
  // sum terms
  output = (*GAIN_PROP)*x[0] + sxdt + (*GAIN_DERIV)*dx_dt;      
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
  float output = PIDcontroller(input);
  setDirection(output);
}
