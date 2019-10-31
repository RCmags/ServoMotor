#ifndef ServoMotor_h

#define ServoMotor_h
#include "Arduino.h"

//--------------- Scale of PID gains  ----------------

#define BYTE_MAX                        255                 //Value which divides all gains
#define PWM_MAX                         255                 //Maximum input of analogWrite() function
                                                            //Scale of PID coefficients:
#define SCALAR_INT                      1E-2            
#define SCALAR_PROP                     4
#define SCALAR_DERIV                    2
#define SCALAR_DERIV2                   40

//------ Coefficients to calculate derivarives ------- 

//  See: http://www.holoborodko.com/pavel/numerical-methods/numerical-derivative/smooth-low-noise-differentiators/ 

#define N_SAMPLES                       7                   //Number of filtered sensor readings that are stored - Must equal number of coefficients
#define COEFFICIENT_DERIVATIVE          1,4,5,0,-5,-4,-1       
#define COEFFICIENT_DERIVATIVE_2ND      1,2,-1,-4,-1,2,1        

//-------- Default Low pass filter settings ---------- [ No characters after backlash! ]

#define TEMPLATE_TYPE_DEFAULT_INPUTS                                                                                    \
          uint32_t  INTERVAL            = 5,                /*  PID refresh interval                            */      \
          uint8_t   N_FILT              = 3,                /*  Number of cascaded filters                      */      \
          uint8_t   FILTER_GAIN         = 100,              /*  Filter decay rate - MUST be value from 0 to 255 */      \
          uint8_t   SENSOR_DEADBAND     = 2                 /*  Minimum error of PID proportional and integral  */  

//--------------- Template Parameters ---------------- [ No characters after backlash! ]
          
#define TEMPLATE_TYPE_REQUIRED_INPUTS               \
          uint8_t   POT_PIN,                        \
          uint8_t   MOTOR_PIN1,                     \
          uint8_t   MOTOR_PIN2,                     \
          int16_t   GAIN_INT,                       \
          int16_t   GAIN_PROP,                      \
          int16_t   GAIN_DERIV,                     \
          int16_t   GAIN_DERIV2                     \
          
#define TEMPLATE_TYPE_INPUTS                        \
                    TEMPLATE_TYPE_REQUIRED_INPUTS,  \
          uint32_t  INTERVAL,                       \
          uint8_t   N_FILT,                         \
          uint8_t   FILTER_GAIN,                    \
          uint8_t   SENSOR_DEADBAND  

#define TEMPLATE_INPUTS                             \
                    POT_PIN,                        \
                    MOTOR_PIN1,                     \
                    MOTOR_PIN2,                     \
                    GAIN_INT,                       \
                    GAIN_PROP,                      \
                    GAIN_DERIV,                     \
                    GAIN_DERIV2,                    \
                    INTERVAL,                       \
                    N_FILT,                         \
                    FILTER_GAIN,                    \
                    SENSOR_DEADBAND  

//---------------- Class definition ------------------ 
                    
template < TEMPLATE_TYPE_REQUIRED_INPUTS, TEMPLATE_TYPE_DEFAULT_INPUTS >
class ServoMotor {
  
  private:
    
    //---- Variables ----
    uint32_t    last_time; 
    float       integral;
    float       filter[N_FILT];
    int         sensor_value[N_SAMPLES];
    
    //---- Constants -----
    static const int8_t COEFF_DERIV[N_SAMPLES] PROGMEM; 
    static const int8_t COEFF_DERIV2[N_SAMPLES] PROGMEM;
 
    //---- Functions ----

    //Miscellaneous:
    void setDirection(int);
    static int deadband( int, int );

    //Sensor signal:
    void filterSensor(void);
    void storeSensorValue( void );
    int firFilter( const int8_t* );
    
    //Calculus functions of sensor signal:
    int derivative2( void );
    int derivarive( void );
    int rectIntegral( int );
    int pidOutput( int );

 public:
 
    //Set pins and arrays
    void initialize(void);
    
    //Servo control:
    int readSensor(void);
    void setPosition(int);

}; 

//---------- Adding function definitions ------------- 

#include "ServoMotor.tpp"

//----------------- Clearing labels ------------------  

#undef BYTE_MAX   
#undef PWM_MAX                  
#undef SCALAR_INT                   
#undef SCALAR_PROP              
#undef SCALAR_DERIV                 
#undef SCALAR_DERIV2
              
#undef N_SAMPLES
#undef COEFFICIENT_DERIVATIVE
#undef COEFFICIENT_DERIVATIVE_2ND
    
#undef TEMPLATE_TYPE_REQUIRED_INPUTS
#undef TEMPLATE_TYPE_DEFAULT_INPUTS 
#undef TEMPLATE_TYPE_INPUTS
#undef TEMPLATE_INPUTS

//----------------------------------------------------  

#endif
