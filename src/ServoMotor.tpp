//------------------ Initialization ------------------ 

    //- Set arrays of coefficients:

        //1st Derivative
template <TEMPLATE_TYPE_INPUTS>
const int8_t ServoMotor<TEMPLATE_INPUTS>::COEFF_DERIV[] = {COEFFICIENT_DERIVATIVE};
    
        //2nd derivarive
template <TEMPLATE_TYPE_INPUTS>
const int8_t ServoMotor<TEMPLATE_INPUTS>::COEFF_DERIV2[] = {COEFFICIENT_DERIVATIVE_2ND};


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
  
        //Fill arrays with sensor readings
  for( uint8_t index = 0; index < N_FILT; index += 1 ) {
    filter[index] = readSensor();
  }
  
  for( uint8_t index =0; index < N_SAMPLES; index += 1 ) {
    sensor_value[index] = readSensor();
  }
}

//------------------ Sensor filtering ----------------  

    //- Cascaded single pole low pass filters: applied to sensor reading
template <TEMPLATE_TYPE_INPUTS>
void ServoMotor<TEMPLATE_INPUTS>::filterSensor(void) {
    
        //Filter gain - Sets cutoff frequency   
  constexpr float DECAY = float(FILTER_GAIN)    /   float(BYTE_MAX);

        //First stage filters raw value
  filter[0] += ( float( readSensor() ) - filter[0] )*DECAY;
        //Following stages filter the previous stage
  for( uint8_t index = 1; index < N_FILT; index += 1 ) {
     filter[index] += ( filter[index-1] - filter[index] )*DECAY;
  }
}


    //- Store filtered sensor readings in an array
template <TEMPLATE_TYPE_INPUTS>
void ServoMotor<TEMPLATE_INPUTS>::storeSensorValue( void ) {       

        //Shift every element in array to the right - Deletes last element
  for( uint8_t index = N_SAMPLES - 1; index > 0; index -= 1 ) {
    sensor_value[index] = sensor_value[index-1];
  }
        //Store filtered reading - trucation suppresses noise
  sensor_value[0] = int( filter[N_FILT-1] );
}


    //- Finite impulse response filter: applied to sensor readings
template <TEMPLATE_TYPE_INPUTS>
int ServoMotor<TEMPLATE_INPUTS>::firFilter( const int8_t coeff[] ) {

  int output = 0;
    
        //Multiply tap by corresponding coefficient, then accumulate result
  for( uint8_t index = 0; index < N_SAMPLES; index += 1 ) {
    output += int( int8_t( pgm_read_byte(coeff + index) ) )* sensor_value[index];
  }

  return output;
}       

//------------------ PID control --------------------- 

    //- Second derivative of filtered sensor signal
template <TEMPLATE_TYPE_INPUTS>
int ServoMotor<TEMPLATE_INPUTS>::derivative2( void ) {
  return firFilter( COEFF_DERIV2 );
}

    //- First derivative of filtered sensor signal
template <TEMPLATE_TYPE_INPUTS>
int ServoMotor<TEMPLATE_INPUTS>::derivarive( void ) {
  return firFilter( COEFF_DERIV );
}

    
    //- Rectangular integral of a given input
template <TEMPLATE_TYPE_INPUTS>
int ServoMotor<TEMPLATE_INPUTS>::rectIntegral( int input ) {   

        //Interger coefficient:
  constexpr float KI    = float(GAIN_INT)*float(INTERVAL)*float(SCALAR_INT)    
                            /   float(BYTE_MAX);  

        //Integrate                     
  integral += float(input)*KI;
        //Prevent integral windup
  integral = constrain( integral, -PWM_MAX, PWM_MAX );
  
  return int(integral);
}


    //- Calculate PID signal to reach a target value
template <TEMPLATE_TYPE_INPUTS>
int ServoMotor<TEMPLATE_INPUTS>::pidOutput( int input ) {   

        //Define coefficients:
  constexpr float KP    = float(GAIN_PROP)*float(SCALAR_PROP)       
                            /   float(BYTE_MAX);
  
  constexpr float KD    = float(GAIN_DERIV)*float(SCALAR_DERIV)     
                            /   ( float(BYTE_MAX)*float(INTERVAL) );
  
  constexpr float KD2   = float(GAIN_DERIV2)*float(SCALAR_DERIV2)   
                            /   ( float(BYTE_MAX)*float(INTERVAL*INTERVAL) );
    
  int output;
        //Filter error
  output = deadband( sensor_value[0] - input, SENSOR_DEADBAND );
        //Combine PID terms
  output = output*KP + derivarive()*KD + derivative2()*KD2 + rectIntegral(output);
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
int ServoMotor<TEMPLATE_INPUTS>::deadband( int input, int band ) {
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
  
        //Filter sensor as fast as polling permits
  filterSensor();
  
        //Sample filtered value at a regular interval
  if( (millis() - last_time) >= INTERVAL ) {
     last_time = millis();
     storeSensorValue();
     setDirection( pidOutput(input) );
  }
}

