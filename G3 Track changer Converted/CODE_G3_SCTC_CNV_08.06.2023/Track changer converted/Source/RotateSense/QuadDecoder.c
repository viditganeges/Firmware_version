#include "QuadDecoder.h"
#include "Serial_Debug.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "board.h"

#define CHECK_AND_UPDATE_COUNT_TIME             4000 //ms

static uint32_t quadDecodeTime = 0 ;
static int32_t quadDecodeCount = 0;
uint32_t rawCount = 0;
static eQuadDecodeDirection QuadDecodeDirection = UNKNOWN_Q;

static uint32_t quadDecodePositive = 0;
static uint32_t quadDecodeNegative = 0;

bool enableQuadDecode = false;

static void EnableInterrupt (void);
static void DisableInterrupt (void);

void QUAD_DECODER_IRQ_HANDLER(void)
{
  /* Clear interrupt flag.*/
  FTM_ClearStatusFlags(QUAD_DECODER_FTM, kFTM_TimeOverflowFlag);
  
  if (FTM_GetQuadDecoderFlags(QUAD_DECODER_FTM) & kFTM_QuadDecoderCountingOverflowOnTopFlag)
  {
    quadDecodePositive+=1;
  }
  else
  {
    quadDecodeNegative+=1;
  }
  //  if(quadDecodePositive>=quadDecodeNegative)
  //  {
  //  quadDecodeCount = quadDecodePositive - quadDecodeNegative;
  //  }
  //  else
  //  {
  //  quadDecodeCount = quadDecodeNegative - quadDecodePositive;
  //  }
  quadDecodeCount = quadDecodePositive - quadDecodeNegative;
}

bool QuadDecoderInit(void)
{
  
  PORT_SetPinMux(BOARD_ENCODER_PORT, QUAD_DECODER_PHA_PIN, kPORT_MuxAlt6);
  PORT_SetPinMux(BOARD_ENCODER_PORT, QUAD_DECODER_PHB_PIN, kPORT_MuxAlt6);
  ftm_config_t ftmInfo;
  ftm_phase_params_t phaseParamsConfigStruct;
  /* Initialize FTM module */
  FTM_GetDefaultConfig(&ftmInfo);
  ftmInfo.prescale = kFTM_Prescale_Divide_4;
  FTM_Init(QUAD_DECODER_FTM, &ftmInfo);
  
  /* Set the modulo values for Quad Decoder. */
  FTM_SetQuadDecoderModuloValue(QUAD_DECODER_FTM, 0U, QUAD_DECODER_MODULO);
  
  /* Enable the Quad Decoder mode. */
  phaseParamsConfigStruct.enablePhaseFilter = false;
  phaseParamsConfigStruct.phaseFilterVal = 0U;
  phaseParamsConfigStruct.phasePolarity = kFTM_QuadPhaseNormal;
  FTM_SetupQuadDecode(QUAD_DECODER_FTM, &phaseParamsConfigStruct, /* Phase A. */
                      &phaseParamsConfigStruct,                    /* Phase B. */
                      kFTM_QuadPhaseEncode);
  enableQuadDecode = true;
  FTM_EnableInterrupts(QUAD_DECODER_FTM, kFTM_TimeOverflowInterruptEnable);
  return true;
}

static void EnableInterrupt (void)
{
  EnableIRQ(QUAD_DECODER_IRQ);
}

static void DisableInterrupt (void)
{
  DisableIRQ(QUAD_DECODER_IRQ);
}

int32_t GetQuadDecoderCountValue(void)
{
  return quadDecodeCount;
}

void SetQuadDecoderCount(uint32_t count)
{
  quadDecodeCount = count;
}

void StartQuadDecoderCount(void)
{
  quadDecodeCount = 0;
  quadDecodePositive = 0;
  quadDecodeNegative = 0;
  FTM_ClearQuadDecoderCounterValue(QUAD_DECODER_FTM);
  EnableInterrupt();
}

void ClearQuadDecoderCount(void)
{
  DisableInterrupt();    
  quadDecodeCount = 0;
  quadDecodePositive = 0;
  quadDecodeNegative = 0;
  EnableInterrupt();
}

void StopQuadDecoderCount(void)
{
  DisableInterrupt();  
}

eQuadDecodeDirection GetQuadDecodeDirection(void)
{
  return QuadDecodeDirection;
}

void QuadDecoderTimer_ms(void)
{
  quadDecodeTime++; 
}


void DisableQuadDecoder(void)
{
  ClearQuadDecoderCount();
  DisableInterrupt();
  FTM_DisableInterrupts(QUAD_DECODER_FTM, kFTM_TimeOverflowInterruptEnable);
  FTM_Deinit(QUAD_DECODER_FTM);
  PORT_SetPinMux(BOARD_ENCODER_PORT, QUAD_DECODER_PHA_PIN, kPORT_PinDisabledOrAnalog);
  PORT_SetPinMux(BOARD_ENCODER_PORT, QUAD_DECODER_PHB_PIN, kPORT_PinDisabledOrAnalog);
}
