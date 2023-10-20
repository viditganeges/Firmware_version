#include "main.h"

bool IsModeValid(uint32_t value)
{
return(Validate_i(value,mode_Min,mode_Max));
}

bool Row_Num_Valid(uint32_t value)
{
return(Validate_i(value,RowNum_Min,RowNum_Max));
}

bool IsTrackDistanceValid(uint32_t dis)
{
return(Validate_i(distance,trackDistance_Min,trackDistance_Max));
}

bool IsManualCommandValid(uint32_t value)
{
return(Validate_i(value,manualCommand_Min,manualCommand_Max));
}

bool IsPWMvalueValid(uint32_t value)
{
return(Validate_i(value,PWMvalue_Min,PWMvalue_Max));
}

bool IsRTCvalueValid(uint32_t value)
{
return(Validate_i(value,RTCvalue_Min,RTCvalue_Max));
}

bool IsMotorCurrentLimitValid(float value)
{
return(Validate_f(value,motorCurrentLimit_Min,motorCurrentLimit_Max));
}

bool IsAccelTimeValid(uint32_t accelTime)
{
  return(Validate_i(accelTime,accelTime_Min,accelTime_Max));
}

bool IsDecelTimeValid(uint32_t decelTime)
{
  return(Validate_i(decelTime,decelTime_Min,decelTime_Max));
}

bool IsDecelPulseCountDiffValid(uint32_t count)
{
  return(Validate_i(count,decelPulseCountDiff_Min,decelPulseCountDiff_Max));
}

bool IsLowBatSoCValid(uint32_t soc)
{
  return(Validate_i(soc,lowBatSoC_Min,lowBatSoC_Max));
}
bool IsPulseCountValid(uint32_t pulse)
{
  return(Validate_i(pulse,pulseCount_Min,pulseCount_Max));
}

bool IsIloadMaxConfigTimeValid(uint32_t configTime)
{
  return(Validate_i(configTime,IloadMaxConfigTime_Min,IloadMaxConfigTime_Max));
}

bool IsHeartBeatIntervalValid(uint32_t value)
{
  return(Validate_i(value,heartBeatIntervalMs_Min,heartBeatIntervalMs_Max));
}

//bool IsNoOfHeartBeatMsgValid(uint32_t value)
//{  
//  return(Validate_i(value,NoOfHeartBeatMsg_Min,NoOfHeartBeatMsg_Max));
//}

bool IsMaxReconnectTimeValid(uint32_t value)
{
return(Validate_i(value,maxReconnectTime_Min,maxReconnectTime_Max));
}
//
//bool IsAutoModeStateValid(bool value)
//{
//  if((value==0)||(value==1))
//    return true;
//  else 
//    return false;
//}
//
//bool IsManualModeStateValid(bool value)
//{
//  if((value==0)||(value==1))
//    return true;
//  else 
//    return false;
//}
//
//bool IsManualControlDirectionValid(bool value)
//{
//  if((value==0)||(value==1))
//    return true;
//  else 
//    return false;
//}
//
//bool IsBrushMotorStateValid(bool value)
//{
//  if((value==0)||(value==1))
//    return true;
//  else 
//    return false;
//}
//
//bool IsBrushMotorDirectionValid(bool value)
//{
//  if((value==0)||(value==1))
//    return true;
//  else 
//    return false;
//}
//
//bool IsEdgeSensorStateValid(bool value)
//{
//  if((value==0)||(value==1))
//    return true;
//  else 
//    return false;
//}
//
//
bool IsHeartBeatStateValid(bool value)
{
  if((value==0)||(value==1))
    return true;
  else 
    return false;
}

bool IsStateValid(bool value)
{
  if((value==0)||(value==1))
    return true;
  else 
    return false;
}
//
//bool CheckHexStrValidity(const char * hexStr, uint16_t maxLength)
//{
//  bool status;
//  uint16_t hexStrLength = strlen(hexStr);
//  uint8_t hexChar;
//  if(hexStrLength > maxLength)
//  {
//    status = false;
//  }
//  else
//  {
//    status = true;
//    for (uint16_t hexCharCount =0; hexCharCount < hexStrLength; hexCharCount++)
//    {
//      hexChar = hexStr[hexCharCount];
//      if((hexChar >= '0' && hexChar <= '9')||(hexChar >= 'A' && hexChar <= 'F')||(hexChar >= 'a' && hexChar <= 'f'))
//      {
//        
//      }
//      else
//      {
//        status = false;
//        break;
//      }
//    }
//  }
//  return status;  
//}
//
bool Validate_f (float value, float min,float max)
{
  if((value>=min)&&(value<=max))
  {
    return true;
  }
  return false;
}
//
bool Validate_i (uint32_t value,uint32_t min,uint32_t max)
{
  if((value>=min)&&(value<=max))
  {
    return true;
  }
  return false;
}
//
//bool Validate_d (double value, double min, double max)
//{
//  if((value>=min)&&(value<=max))
//  {
//    return true;
//  }
//  return false;
//}
//

bool IswheelDiaLimitValid(uint32_t value)
{
return(Validate_i(value,wheelDiaLimit_Min,wheelDiaLimit_Max));
}

bool IspulseLimitValid(uint32_t value)
{
return(Validate_i(value,pulseLimit_Min,pulseLimit_Max));
}

bool IscycleFrequencyLimitValid(uint32_t value)
{
return(Validate_i(value,cycleFrequencyLimit_Min,cycleFrequencyLimit_Max));
}

bool IsintervalLimitValid(uint32_t value)
{
return(Validate_i(value,intervalLimit_Min,intervalLimit_Max));
}

bool IsparameterLimitValid(uint32_t value)
{
return(Validate_i(value,parameterLimit_Min,parameterLimit_Max));
}

bool IsContinue1Valid(uint32_t value)
{
  return(Validate_i(value,Continue1_Min,Continue1_Max));
}

bool IsCount1Valid(uint32_t value)
{
  return(Validate_i(value,Count1_Min,Count1_Max));
}

bool IsReturn1Valid(uint32_t value)
{
  return(Validate_i(value,Return1_Min,Return1_Max));
}

bool IsBrushtimeValid(uint32_t value)
{
return(Validate_i(value,Brushtime_Min,Brushtime_Max));
}


