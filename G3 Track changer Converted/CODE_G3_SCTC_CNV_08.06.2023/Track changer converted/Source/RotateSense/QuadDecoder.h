#ifndef __QUAD_DECODER_H_
#define __QUAD_DECODER_H_

#include <stdbool.h>
#include <stdint.h>
#include  "fsl_ftm.h"

#define QUAD_DECODER_MODE       1

//#define SCALING_FACTOR          8

#define QUAD_DECODER_FTM               FTM1        
#define QUAD_DECODER_IRQ                FTM1_IRQn
#define QUAD_DECODER_IRQ_HANDLER        FTM1_IRQHandler
#define QUAD_DECODER_PORT               BOARD_ENCODER_PORT
#define QUAD_DECODER_GPIO               BOARD_ENCODER_GPIO
#define QUAD_DECODER_PHA_PIN            BOARD_ENCODER_MOTOR1_PIN
#define QUAD_DECODER_PHB_PIN            BOARD_ENCODER_MOTOR2_PIN
#define QUAD_DECODER_MODULO             3 //2000/4 = 500 pulses per revolution(0-3=> so 4)

typedef enum eQuadDecodeDirection_def
{
  POSITIVE_Q,
  NEGATIVE_Q,
  UNKNOWN_Q
}eQuadDecodeDirection;

bool QuadDecoderInit(void);

int32_t GetQuadDecoderCountValue(void);
void SetQuadDecoderCount(uint32_t count);
void StartQuadDecoderCount(void);
void ClearQuadDecoderCount(void);
void StopQuadDecoderCount(void);

eQuadDecodeDirection GetQuadDecodeDirection(void);

void QuadDecoderTimer_ms(void);
void CheckAndUpdateCount(void);

void QuadDecoder_Irq(void);

void DisableQuadDecoder(void);
#endif