#include <Arduino.h>

#ifndef ServoMotor_h
#define ServoMotor_h

//--------------- Scale of PID gains  ----------------

#define PWM_MAX                          255                 //Maximum input of analogWrite() function

//-------- Default Low pass filter settings ---------- [ No characters after backlash! ]

constexpr float DEFAULT_ALPHA          = 0.20;

#define TEMPLATE_TYPE_DEFAULT_INPUTS                                                                                   \
          const float* ALPHA           = &DEFAULT_ALPHA,   /*  Filter decay rate - MUST be value from 0 to 255 */      \
          uint8_t      SENSOR_DEADBAND = 4                 /*  Minimum error of PID proportional and integral  */  

//--------------- Template Parameters ---------------- [ No characters after backlash! ]
          
#define TEMPLATE_TYPE_REQUIRED_INPUTS               \
          uint8_t        POT_PIN,                   \
          uint8_t        MOTOR_PIN1,                \
          uint8_t        MOTOR_PIN2,                \
          const float*   GAIN_INT,                  \
          const float*   GAIN_PROP,                 \
          const float*   GAIN_DERIV                 \
          
#define TEMPLATE_TYPE_INPUTS                        \
                    TEMPLATE_TYPE_REQUIRED_INPUTS,  \
          const float*   ALPHA,                     \
          uint8_t        SENSOR_DEADBAND  

#define TEMPLATE_INPUTS                             \
                    POT_PIN,                        \
                    MOTOR_PIN1,                     \
                    MOTOR_PIN2,                     \
                    GAIN_INT,                       \
                    GAIN_PROP,                      \
                    GAIN_DERIV,                     \
                    ALPHA,                          \
                    SENSOR_DEADBAND  

//---------------- Class definition ------------------ 
                    
template <TEMPLATE_TYPE_REQUIRED_INPUTS, TEMPLATE_TYPE_DEFAULT_INPUTS>
class ServoMotor {
  
  private:
    //---- Variables ----
    uint32_t last_time; 
    float    x[2], dx_dt, sxdt;
 
    //---- Functions ----

    //Miscellaneous:
    void setDirection(int);
    static float deadband( float, float );

    //Calculus functions of position error:
    void calculus(float);
    float PIDcontroller(int);

 public:
    //Set pins and arrays
    void initialize();
    
    //Servo control:
    int readSensor();
    void setPosition(int);

}; 

//---------- Adding function definitions ------------- 

#include "ServoMotor.tpp"

//----------------- Clearing labels ------------------  
    
#undef TEMPLATE_TYPE_REQUIRED_INPUTS
#undef TEMPLATE_TYPE_DEFAULT_INPUTS 
#undef TEMPLATE_TYPE_INPUTS
#undef TEMPLATE_INPUTS

//----------------------------------------------------  

#endif
