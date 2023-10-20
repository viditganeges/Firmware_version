#ifndef __DEFAULT_VALUES_CONFIG_H_
#define __DEFAULT_VALUES_CONFIG_H_

//enum eHwVersion{HVER_1_0,HVER_1_1};

#define HVER_1_0                0
#define HVER_1_1                1

#define HARDWARE_VERSION                                   HVER_1_0 //HVER_1_1


#define DEVICE_FIRMWARE_VERSION                             "FV:3.0.0"  //"D2.0V"        //"D1.1.6V" RT old            //  D1.1.2V old                

#define DEFAULT_DEV_ID                                     "FFFFF"
#define DEFAULT_DEV_HW_REV                                  "CHV:3.0 " //"1.1V"   //should be changed according to HARDWARE_VERSION
#define DEFAULT_DEV_SW_REV                                 DEVICE_FIRMWARE_VERSION
#define DEFAULT_DEV_SERIAL_NO                              "CSL000"

#define DEFAULT_DEVICE_WELCOME_MSG_HEADER                  "CONV TRACK CHANGER "
#define DEFAULT_DEVICE_WELCOME_MSG_FOOTER                  " **"
  
#define DEFAULT_DEVICE_WELCOME_MSG                         DEFAULT_DEVICE_WELCOME_MSG_HEADER DEFAULT_DEV_SW_REV //DEFAULT_DEVICE_WELCOME_MSG_FOOTER

#define DEFAULT_AUTO_PWM_DUTY                              20000U
#define DEFAULT_MANUAL_PWM_DUTY                            20000U
#define DEFAULT_EMERGENCY_PWM_DUTY                         20000U
#define DEFAULT_APPROACH_PWM_DUTY                          7000U

#define DEFAULT_DECEL_START_COUNT_DIFF                      (5)
#define DEFAULT_PWM_ACCEL_TIME                              2000U
#define DEFAULT_PWM_DECEL_TIME                              1000U

#define DEFAULT_MIN_BATT_VOLTAGE_MOTOR                      (22.2)
#define DEFAULT_MAX_MOTOR_CURRENT_MOTOR                     6U
#define DEFAULT_ILOAD_MAX_NOISE_TIME                        500U
#define DEFAULT_ILOAD_MAX_FREQ_TIME                         15000U
#define DEFAULT_PAUSE_DELAY_TIME                            3000U

//#define DEFAULT_MAX_POS_COUNT_FOR_PV_SETUP                  25  //20000
//#define DEFAULT_MAX_NEG_COUNT_FOR_PV_SETUP                  25  //20000

#define DEFAULT_MAX_TEMP_SENSOR_ONE                         100
#define DEFAULT_MAX_TEMP_SENSOR_TWO                         100

#define DEFAULT_BRUSH_MOT_DIRECTION                         0
#define DEFAULT_MOT_1_DIRECTION                             0
#define DEFAULT_MOT_2_DIRECTION                             0

#define DEFAULT_AUTO_SCHEDULED_TIME_HOUR                    00
#define DEFAULT_AUTO_SCHEDULED_TIME_MINUTE                  01

#define DEFAULT_ILOAD_MAX_REPEAT_COUNT                      1U
#define DEFAULT_ILOAD_MAX_NOISE_TIME                        500U
#define DEFAULT_ILOAD_MAX_FREQ_TIME                         15000U
#define DEFAULT_PAUSE_DELAY_TIME                            3000U

#define DEFAULT_ILOAD1_MAX_VALUE                            (2.0f)
#define DEFAULT_ILOAD2_MAX_VALUE                            (2.0f)
#define DEFAULT_ILOAD3_MAX_VALUE                            (2.0f)

#define DEFAULT_MIN_BATT_SOC                                (50)
#define DEFAULT_BRUSH_ENABLED_STATE                         true
#define DEFAULT_EDGE_SENSOR_ENABLED_STATE                   true

#define DEFAULT_TRACK_DISTANCE                              0
#define DEFAULT_TRACK_COUNT                                 0
#define DEFAULT_RFID_DATA                                   0

#define DEFAULT_FORWARD_COUNT                               0
#define DEFAULT_REVERSE_COUNT                               0

#define DEFAULT_ENCODER_PULSE_TOLERANCE                     25

#define MAX_ZIGBEE_RECONNECT_TIME_DEFAULT                   (1)*(60)*(1000) //1 minute
#define HEARTBEAT_INTERVAL_MS_DEFAULT                       3000U

//07D
//07D
#define DEFAULT_MAX_POS_COUNT_FOR_PV_SETUP                  21  //20000
#define DEFAULT_MAX_NEG_COUNT_FOR_PV_SETUP                  21  //20000
#define DEFAULT_POSITIVE_DISTANCE                          5000U  //20000
#define DEFAULT_NEGATIVE_DISTANCE                          5000U  //20000
#define DEFAULT_WHEEL_DIA                                  150U
#define DEFAULT_PULSE                                      2U
#define DEFAULT_INTERVAL                                   (1.0f)
#define DEFAULT_CYCLE_FREQUENCY                            5  
#define DEFAULT_COUNT                                      5U
#define DEFAULT_COUNTINUE                                  0

#endif