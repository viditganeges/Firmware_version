#ifndef _VALUESVALIDATE_H    /* Guard against multiple inclusion */
#define _VALUESVALIDATE_H

#include "main.h"

static const uint32_t accelTime_Min              = 0;
static const uint32_t accelTime_Max              = 2147483647; //4294967295;
static const uint32_t decelTime_Min              = 0;
static const uint32_t decelTime_Max              = 2147483647; //4294967295;
static const uint32_t decelPulseCountDiff_Min    = 0;
static const uint32_t decelPulseCountDiff_Max    = 65535;
static const uint32_t PWMvalue_Min               = 0;
static const uint32_t PWMvalue_Max               = 100;
static const uint32_t RTCvalue_Min               = 0;
static const uint32_t RTCvalue_Max               = 1000000;
static const uint32_t lowBatSoC_Min              = 0;
static const uint32_t lowBatSoC_Max              = 100;
static const uint32_t pulseCount_Min             = 0;
static const uint32_t pulseCount_Max             = 65535;
static const float motorCurrentLimit_Min         = 0;
static const float motorCurrentLimit_Max         = 10;
static const uint32_t manualCommand_Min          = 1;
static const uint32_t manualCommand_Max          = 7;
static const uint32_t IloadMaxConfigTime_Min     = 0;
static const uint32_t IloadMaxConfigTime_Max     = 2147483647;
static const uint32_t heartBeatIntervalMs_Min    = 100;
static const uint32_t heartBeatIntervalMs_Max    = 100000;
static const uint32_t NoOfHeartBeatMsg_Min       = 1;
static const uint32_t NoOfHeartBeatMsg_Max       = 500;
static const uint32_t maxReconnectTime_Min       = 1;
static const uint32_t maxReconnectTime_Max       = (15)*(60)*(1000); //10 minutes

static const float wheelDiaLimit_Min         = 0;
static const float wheelDiaLimit_Max         = 500;
static const float pulseLimit_Min            = 1;
static const float pulseLimit_Max            = 4;
static const float cycleFrequencyLimit_Min   = 1;
static const float cycleFrequencyLimit_Max   = 19;
static const uint32_t intervalLimit_Min      = 1;
static const uint32_t intervalLimit_Max      = 60;
static const uint32_t parameterLimit_Min     = 0;
static const uint32_t parameterLimit_Max     = 7;
static const uint32_t Continue1_Min          = 0;
static const uint32_t Continue1_Max          = 5;
static const uint32_t Count1_Min             = 1;
static const uint32_t Count1_Max             = 10;
static const uint32_t Return1_Min            = 1;
static const uint32_t Return1_Max            = 10;
static const uint32_t Brushtime_Min          = 0;
static const uint32_t Brushtime_Max          = 1000;

bool IsPWMvalueValid(uint32_t value);
bool IsRTCvalueValid(uint32_t value);
bool IsMotorCurrentLimitValid(float value);
bool IsAccelTimeValid(uint32_t accelTime);
bool IsDecelTimeValid(uint32_t decelTime);
bool IsDecelPulseCountDiffValid(uint32_t count);
bool IsLowBatSoCValid(uint32_t soc);
bool IsPulseCountValid(uint32_t pulse);
bool IsManualCommandValid(uint32_t value);
bool IsIloadMaxConfigTimeValid(uint32_t configTime);
bool IsHeartBeatIntervalValid(uint32_t value);
bool IsNoOfHeartBeatMsgValid(uint32_t value);
bool IsMaxReconnectTimeValid(uint32_t value);
bool IsHeartBeatStateValid(bool value);
bool IsStateValid(bool value);
bool Validate_f (float value, float min,float max);
bool Validate_i (uint32_t value,uint32_t min,uint32_t max);
bool IswheelDiaLimitValid(float value);
bool IspulseLimitValid(float value);
bool IscycleFrequencyLimitValid(float value);
bool IsintervalLimitValid(uint32_t value);
bool IsparameterLimitValid(uint32_t value);
bool IsContinue1Valid(uint32_t value);
bool IsCount1Valid(uint32_t value);
bool IsReturn1Valid(uint32_t value);
bool IsBrushtimeValid(uint32_t value);

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
