/*******************************************************************************
  PIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_pio.h

  Summary:
    PIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (PIO) module.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

#ifndef PLIB_PIO_H
#define PLIB_PIO_H

#include "device.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for GPIO_PD2_MotorRelayCtrl2 pin ***/
#define GPIO_PD2_MotorRelayCtrl2_Set()               (PIOD_REGS->PIO_SODR = (1<<2))
#define GPIO_PD2_MotorRelayCtrl2_Clear()             (PIOD_REGS->PIO_CODR = (1<<2))
#define GPIO_PD2_MotorRelayCtrl2_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<2))
#define GPIO_PD2_MotorRelayCtrl2_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<2))
#define GPIO_PD2_MotorRelayCtrl2_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<2))
#define GPIO_PD2_MotorRelayCtrl2_Get()               ((PIOD_REGS->PIO_PDSR >> 2) & 0x1)
#define GPIO_PD2_MotorRelayCtrl2_PIN                  PIO_PIN_PD2

/*** Macros for GPIO_PA29_MotorRelayCtrl1 pin ***/
#define GPIO_PA29_MotorRelayCtrl1_Set()               (PIOA_REGS->PIO_SODR = (1<<29))
#define GPIO_PA29_MotorRelayCtrl1_Clear()             (PIOA_REGS->PIO_CODR = (1<<29))
#define GPIO_PA29_MotorRelayCtrl1_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<29))
#define GPIO_PA29_MotorRelayCtrl1_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<29))
#define GPIO_PA29_MotorRelayCtrl1_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<29))
#define GPIO_PA29_MotorRelayCtrl1_Get()               ((PIOA_REGS->PIO_PDSR >> 29) & 0x1)
#define GPIO_PA29_MotorRelayCtrl1_PIN                  PIO_PIN_PA29

/*** Macros for MOTORC_IN2 pin ***/
#define MOTORC_IN2_Get()               ((PIOC_REGS->PIO_PDSR >> 22) & 0x1)
#define MOTORC_IN2_PIN                  PIO_PIN_PC22

/*** Macros for MOTORA_PC21_lowfault pin ***/
#define MOTORA_PC21_lowfault_Set()               (PIOC_REGS->PIO_SODR = (1<<21))
#define MOTORA_PC21_lowfault_Clear()             (PIOC_REGS->PIO_CODR = (1<<21))
#define MOTORA_PC21_lowfault_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<21))
#define MOTORA_PC21_lowfault_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<21))
#define MOTORA_PC21_lowfault_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<21))
#define MOTORA_PC21_lowfault_Get()               ((PIOC_REGS->PIO_PDSR >> 21) & 0x1)
#define MOTORA_PC21_lowfault_PIN                  PIO_PIN_PC21

/*** Macros for STEPPERMOTOR1_DIR pin ***/
#define STEPPERMOTOR1_DIR_Set()               (PIOD_REGS->PIO_SODR = (1<<7))
#define STEPPERMOTOR1_DIR_Clear()             (PIOD_REGS->PIO_CODR = (1<<7))
#define STEPPERMOTOR1_DIR_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<7))
#define STEPPERMOTOR1_DIR_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<7))
#define STEPPERMOTOR1_DIR_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<7))
#define STEPPERMOTOR1_DIR_Get()               ((PIOD_REGS->PIO_PDSR >> 7) & 0x1)
#define STEPPERMOTOR1_DIR_PIN                  PIO_PIN_PD7

/*** Macros for GPIO_PD8_IO6 pin ***/
#define GPIO_PD8_IO6_Set()               (PIOD_REGS->PIO_SODR = (1<<8))
#define GPIO_PD8_IO6_Clear()             (PIOD_REGS->PIO_CODR = (1<<8))
#define GPIO_PD8_IO6_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<8))
#define GPIO_PD8_IO6_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<8))
#define GPIO_PD8_IO6_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<8))
#define GPIO_PD8_IO6_Get()               ((PIOD_REGS->PIO_PDSR >> 8) & 0x1)
#define GPIO_PD8_IO6_PIN                  PIO_PIN_PD8

/*** Macros for GPIO_PD9_IO7 pin ***/
#define GPIO_PD9_IO7_Set()               (PIOD_REGS->PIO_SODR = (1<<9))
#define GPIO_PD9_IO7_Clear()             (PIOD_REGS->PIO_CODR = (1<<9))
#define GPIO_PD9_IO7_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<9))
#define GPIO_PD9_IO7_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<9))
#define GPIO_PD9_IO7_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<9))
#define GPIO_PD9_IO7_Get()               ((PIOD_REGS->PIO_PDSR >> 9) & 0x1)
#define GPIO_PD9_IO7_PIN                  PIO_PIN_PD9

/*** Macros for STEPPERMOTOR1_EN pin ***/
#define STEPPERMOTOR1_EN_Set()               (PIOD_REGS->PIO_SODR = (1<<0))
#define STEPPERMOTOR1_EN_Clear()             (PIOD_REGS->PIO_CODR = (1<<0))
#define STEPPERMOTOR1_EN_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<0))
#define STEPPERMOTOR1_EN_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<0))
#define STEPPERMOTOR1_EN_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<0))
#define STEPPERMOTOR1_EN_Get()               ((PIOD_REGS->PIO_PDSR >> 0) & 0x1)
#define STEPPERMOTOR1_EN_PIN                  PIO_PIN_PD0

/*** Macros for MOTORA_PD1_toff pin ***/
#define MOTORA_PD1_toff_Set()               (PIOD_REGS->PIO_SODR = (1<<1))
#define MOTORA_PD1_toff_Clear()             (PIOD_REGS->PIO_CODR = (1<<1))
#define MOTORA_PD1_toff_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<1))
#define MOTORA_PD1_toff_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<1))
#define MOTORA_PD1_toff_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<1))
#define MOTORA_PD1_toff_Get()               ((PIOD_REGS->PIO_PDSR >> 1) & 0x1)
#define MOTORA_PD1_toff_PIN                  PIO_PIN_PD1

/*** Macros for MOTORA_PD3_decaylow pin ***/
#define MOTORA_PD3_decaylow_Set()               (PIOD_REGS->PIO_SODR = (1<<3))
#define MOTORA_PD3_decaylow_Clear()             (PIOD_REGS->PIO_CODR = (1<<3))
#define MOTORA_PD3_decaylow_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<3))
#define MOTORA_PD3_decaylow_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<3))
#define MOTORA_PD3_decaylow_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<3))
#define MOTORA_PD3_decaylow_Get()               ((PIOD_REGS->PIO_PDSR >> 3) & 0x1)
#define MOTORA_PD3_decaylow_PIN                  PIO_PIN_PD3

/*** Macros for GPIO_PC18_IO3 pin ***/
#define GPIO_PC18_IO3_Set()               (PIOC_REGS->PIO_SODR = (1<<18))
#define GPIO_PC18_IO3_Clear()             (PIOC_REGS->PIO_CODR = (1<<18))
#define GPIO_PC18_IO3_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<18))
#define GPIO_PC18_IO3_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<18))
#define GPIO_PC18_IO3_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<18))
#define GPIO_PC18_IO3_Get()               ((PIOC_REGS->PIO_PDSR >> 18) & 0x1)
#define GPIO_PC18_IO3_PIN                  PIO_PIN_PC18

/*** Macros for GPIO_PD29_IO8 pin ***/
#define GPIO_PD29_IO8_Set()               (PIOD_REGS->PIO_SODR = (1<<29))
#define GPIO_PD29_IO8_Clear()             (PIOD_REGS->PIO_CODR = (1<<29))
#define GPIO_PD29_IO8_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<29))
#define GPIO_PD29_IO8_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<29))
#define GPIO_PD29_IO8_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<29))
#define GPIO_PD29_IO8_Get()               ((PIOD_REGS->PIO_PDSR >> 29) & 0x1)
#define GPIO_PD29_IO8_PIN                  PIO_PIN_PD29

/*** Macros for HOLD pin ***/
#define HOLD_Set()               (PIOD_REGS->PIO_SODR = (1<<31))
#define HOLD_Clear()             (PIOD_REGS->PIO_CODR = (1<<31))
#define HOLD_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<31))
#define HOLD_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<31))
#define HOLD_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<31))
#define HOLD_Get()               ((PIOD_REGS->PIO_PDSR >> 31) & 0x1)
#define HOLD_PIN                  PIO_PIN_PD31

/*** Macros for GPIO_PC25_decay pin ***/
#define GPIO_PC25_decay_Set()               (PIOC_REGS->PIO_SODR = (1<<25))
#define GPIO_PC25_decay_Clear()             (PIOC_REGS->PIO_CODR = (1<<25))
#define GPIO_PC25_decay_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<25))
#define GPIO_PC25_decay_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<25))
#define GPIO_PC25_decay_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<25))
#define GPIO_PC25_decay_Get()               ((PIOC_REGS->PIO_PDSR >> 25) & 0x1)
#define GPIO_PC25_decay_PIN                  PIO_PIN_PC25

/*** Macros for GPIO_PC23_IO4 pin ***/
#define GPIO_PC23_IO4_Set()               (PIOC_REGS->PIO_SODR = (1<<23))
#define GPIO_PC23_IO4_Clear()             (PIOC_REGS->PIO_CODR = (1<<23))
#define GPIO_PC23_IO4_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<23))
#define GPIO_PC23_IO4_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<23))
#define GPIO_PC23_IO4_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<23))
#define GPIO_PC23_IO4_Get()               ((PIOC_REGS->PIO_PDSR >> 23) & 0x1)
#define GPIO_PC23_IO4_PIN                  PIO_PIN_PC23

/*** Macros for GPIO_PD4_IO5 pin ***/
#define GPIO_PD4_IO5_Set()               (PIOD_REGS->PIO_SODR = (1<<4))
#define GPIO_PD4_IO5_Clear()             (PIOD_REGS->PIO_CODR = (1<<4))
#define GPIO_PD4_IO5_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<4))
#define GPIO_PD4_IO5_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<4))
#define GPIO_PD4_IO5_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<4))
#define GPIO_PD4_IO5_Get()               ((PIOD_REGS->PIO_PDSR >> 4) & 0x1)
#define GPIO_PD4_IO5_PIN                  PIO_PIN_PD4

/*** Macros for GPIO_PC20_IO1 pin ***/
#define GPIO_PC20_IO1_Set()               (PIOC_REGS->PIO_SODR = (1<<20))
#define GPIO_PC20_IO1_Clear()             (PIOC_REGS->PIO_CODR = (1<<20))
#define GPIO_PC20_IO1_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<20))
#define GPIO_PC20_IO1_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<20))
#define GPIO_PC20_IO1_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<20))
#define GPIO_PC20_IO1_Get()               ((PIOC_REGS->PIO_PDSR >> 20) & 0x1)
#define GPIO_PC20_IO1_PIN                  PIO_PIN_PC20

/*** Macros for MOTORA_PE2_tofflow pin ***/
#define MOTORA_PE2_tofflow_Set()               (PIOE_REGS->PIO_SODR = (1<<2))
#define MOTORA_PE2_tofflow_Clear()             (PIOE_REGS->PIO_CODR = (1<<2))
#define MOTORA_PE2_tofflow_Toggle()            (PIOE_REGS->PIO_ODSR ^= (1<<2))
#define MOTORA_PE2_tofflow_OutputEnable()      (PIOE_REGS->PIO_OER = (1<<2))
#define MOTORA_PE2_tofflow_InputEnable()       (PIOE_REGS->PIO_ODR = (1<<2))
#define MOTORA_PE2_tofflow_Get()               ((PIOE_REGS->PIO_PDSR >> 2) & 0x1)
#define MOTORA_PE2_tofflow_PIN                  PIO_PIN_PE2

/*** Macros for MOTORB_PC17_decaylow pin ***/
#define MOTORB_PC17_decaylow_Set()               (PIOC_REGS->PIO_SODR = (1<<17))
#define MOTORB_PC17_decaylow_Clear()             (PIOC_REGS->PIO_CODR = (1<<17))
#define MOTORB_PC17_decaylow_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<17))
#define MOTORB_PC17_decaylow_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<17))
#define MOTORB_PC17_decaylow_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<17))
#define MOTORB_PC17_decaylow_Get()               ((PIOC_REGS->PIO_PDSR >> 17) & 0x1)
#define MOTORB_PC17_decaylow_PIN                  PIO_PIN_PC17

/*** Macros for MOTORB_IN1 pin ***/
#define MOTORB_IN1_Set()               (PIOA_REGS->PIO_SODR = (1<<0))
#define MOTORB_IN1_Clear()             (PIOA_REGS->PIO_CODR = (1<<0))
#define MOTORB_IN1_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<0))
#define MOTORB_IN1_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<0))
#define MOTORB_IN1_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<0))
#define MOTORB_IN1_Get()               ((PIOA_REGS->PIO_PDSR >> 0) & 0x1)
#define MOTORB_IN1_PIN                  PIO_PIN_PA0

/*** Macros for MOTORB_PC16_lowfault pin ***/
#define MOTORB_PC16_lowfault_Set()               (PIOC_REGS->PIO_SODR = (1<<16))
#define MOTORB_PC16_lowfault_Clear()             (PIOC_REGS->PIO_CODR = (1<<16))
#define MOTORB_PC16_lowfault_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<16))
#define MOTORB_PC16_lowfault_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<16))
#define MOTORB_PC16_lowfault_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<16))
#define MOTORB_PC16_lowfault_Get()               ((PIOC_REGS->PIO_PDSR >> 16) & 0x1)
#define MOTORB_PC16_lowfault_PIN                  PIO_PIN_PC16

/*** Macros for MOTORB_PD11_nsleep pin ***/
#define MOTORB_PD11_nsleep_Set()               (PIOD_REGS->PIO_SODR = (1<<11))
#define MOTORB_PD11_nsleep_Clear()             (PIOD_REGS->PIO_CODR = (1<<11))
#define MOTORB_PD11_nsleep_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<11))
#define MOTORB_PD11_nsleep_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<11))
#define MOTORB_PD11_nsleep_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<11))
#define MOTORB_PD11_nsleep_Get()               ((PIOD_REGS->PIO_PDSR >> 11) & 0x1)
#define MOTORB_PD11_nsleep_PIN                  PIO_PIN_PD11

/*** Macros for MOTORB_IN2 pin ***/
#define MOTORB_IN2_Get()               ((PIOA_REGS->PIO_PDSR >> 1) & 0x1)
#define MOTORB_IN2_PIN                  PIO_PIN_PA1

/*** Macros for MOTORC_PC14_nsleep pin ***/
#define MOTORC_PC14_nsleep_Set()               (PIOC_REGS->PIO_SODR = (1<<14))
#define MOTORC_PC14_nsleep_Clear()             (PIOC_REGS->PIO_CODR = (1<<14))
#define MOTORC_PC14_nsleep_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<14))
#define MOTORC_PC14_nsleep_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<14))
#define MOTORC_PC14_nsleep_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<14))
#define MOTORC_PC14_nsleep_Get()               ((PIOC_REGS->PIO_PDSR >> 14) & 0x1)
#define MOTORC_PC14_nsleep_PIN                  PIO_PIN_PC14

/*** Macros for MOTORB_PC11_tofflow pin ***/
#define MOTORB_PC11_tofflow_Set()               (PIOC_REGS->PIO_SODR = (1<<11))
#define MOTORB_PC11_tofflow_Clear()             (PIOC_REGS->PIO_CODR = (1<<11))
#define MOTORB_PC11_tofflow_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<11))
#define MOTORB_PC11_tofflow_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<11))
#define MOTORB_PC11_tofflow_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<11))
#define MOTORB_PC11_tofflow_Get()               ((PIOC_REGS->PIO_PDSR >> 11) & 0x1)
#define MOTORB_PC11_tofflow_PIN                  PIO_PIN_PC11

/*** Macros for PROXI24V_PC30_BLACK4 pin ***/
#define PROXI24V_PC30_BLACK4_Set()               (PIOC_REGS->PIO_SODR = (1<<30))
#define PROXI24V_PC30_BLACK4_Clear()             (PIOC_REGS->PIO_CODR = (1<<30))
#define PROXI24V_PC30_BLACK4_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<30))
#define PROXI24V_PC30_BLACK4_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<30))
#define PROXI24V_PC30_BLACK4_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<30))
#define PROXI24V_PC30_BLACK4_Get()               ((PIOC_REGS->PIO_PDSR >> 30) & 0x1)
#define PROXI24V_PC30_BLACK4_PIN                  PIO_PIN_PC30
#define PROXI24V_PC30_BLACK4_InterruptEnable()   (PIOC_REGS->PIO_IER = (1<<30))
#define PROXI24V_PC30_BLACK4_InterruptDisable()  (PIOC_REGS->PIO_IDR = (1<<30))

/*** Macros for MOTORA_IN1 pin ***/
#define MOTORA_IN1_Set()               (PIOA_REGS->PIO_SODR = (1<<2))
#define MOTORA_IN1_Clear()             (PIOA_REGS->PIO_CODR = (1<<2))
#define MOTORA_IN1_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<2))
#define MOTORA_IN1_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<2))
#define MOTORA_IN1_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<2))
#define MOTORA_IN1_Get()               ((PIOA_REGS->PIO_PDSR >> 2) & 0x1)
#define MOTORA_IN1_PIN                  PIO_PIN_PA2

/*** Macros for GPIO_PD12_decay pin ***/
#define GPIO_PD12_decay_Set()               (PIOD_REGS->PIO_SODR = (1<<12))
#define GPIO_PD12_decay_Clear()             (PIOD_REGS->PIO_CODR = (1<<12))
#define GPIO_PD12_decay_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<12))
#define GPIO_PD12_decay_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<12))
#define GPIO_PD12_decay_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<12))
#define GPIO_PD12_decay_Get()               ((PIOD_REGS->PIO_PDSR >> 12) & 0x1)
#define GPIO_PD12_decay_PIN                  PIO_PIN_PD12

/*** Macros for PROXI24V_PC13_BLACK1 pin ***/
#define PROXI24V_PC13_BLACK1_Set()               (PIOC_REGS->PIO_SODR = (1<<13))
#define PROXI24V_PC13_BLACK1_Clear()             (PIOC_REGS->PIO_CODR = (1<<13))
#define PROXI24V_PC13_BLACK1_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<13))
#define PROXI24V_PC13_BLACK1_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<13))
#define PROXI24V_PC13_BLACK1_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<13))
#define PROXI24V_PC13_BLACK1_Get()               ((PIOC_REGS->PIO_PDSR >> 13) & 0x1)
#define PROXI24V_PC13_BLACK1_PIN                  PIO_PIN_PC13
#define PROXI24V_PC13_BLACK1_InterruptEnable()   (PIOC_REGS->PIO_IER = (1<<13))
#define PROXI24V_PC13_BLACK1_InterruptDisable()  (PIOC_REGS->PIO_IDR = (1<<13))

/*** Macros for PROXI24V_PC15_BLACK2 pin ***/
#define PROXI24V_PC15_BLACK2_Set()               (PIOC_REGS->PIO_SODR = (1<<15))
#define PROXI24V_PC15_BLACK2_Clear()             (PIOC_REGS->PIO_CODR = (1<<15))
#define PROXI24V_PC15_BLACK2_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<15))
#define PROXI24V_PC15_BLACK2_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<15))
#define PROXI24V_PC15_BLACK2_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<15))
#define PROXI24V_PC15_BLACK2_Get()               ((PIOC_REGS->PIO_PDSR >> 15) & 0x1)
#define PROXI24V_PC15_BLACK2_PIN                  PIO_PIN_PC15
#define PROXI24V_PC15_BLACK2_InterruptEnable()   (PIOC_REGS->PIO_IER = (1<<15))
#define PROXI24V_PC15_BLACK2_InterruptDisable()  (PIOC_REGS->PIO_IDR = (1<<15))

/*** Macros for MOTORB_PD13_toff pin ***/
#define MOTORB_PD13_toff_Set()               (PIOD_REGS->PIO_SODR = (1<<13))
#define MOTORB_PD13_toff_Clear()             (PIOD_REGS->PIO_CODR = (1<<13))
#define MOTORB_PD13_toff_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<13))
#define MOTORB_PD13_toff_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<13))
#define MOTORB_PD13_toff_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<13))
#define MOTORB_PD13_toff_Get()               ((PIOD_REGS->PIO_PDSR >> 13) & 0x1)
#define MOTORB_PD13_toff_PIN                  PIO_PIN_PD13

/*** Macros for MOTORC_PC10_tofflow pin ***/
#define MOTORC_PC10_tofflow_Set()               (PIOC_REGS->PIO_SODR = (1<<10))
#define MOTORC_PC10_tofflow_Clear()             (PIOC_REGS->PIO_CODR = (1<<10))
#define MOTORC_PC10_tofflow_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<10))
#define MOTORC_PC10_tofflow_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<10))
#define MOTORC_PC10_tofflow_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<10))
#define MOTORC_PC10_tofflow_Get()               ((PIOC_REGS->PIO_PDSR >> 10) & 0x1)
#define MOTORC_PC10_tofflow_PIN                  PIO_PIN_PC10

/*** Macros for MOTORC_PA20_decaylow pin ***/
#define MOTORC_PA20_decaylow_Set()               (PIOA_REGS->PIO_SODR = (1<<20))
#define MOTORC_PA20_decaylow_Clear()             (PIOA_REGS->PIO_CODR = (1<<20))
#define MOTORC_PA20_decaylow_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<20))
#define MOTORC_PA20_decaylow_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<20))
#define MOTORC_PA20_decaylow_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<20))
#define MOTORC_PA20_decaylow_Get()               ((PIOA_REGS->PIO_PDSR >> 20) & 0x1)
#define MOTORC_PA20_decaylow_PIN                  PIO_PIN_PA20

/*** Macros for PROXI24V_PA19_BLACK3 pin ***/
#define PROXI24V_PA19_BLACK3_Set()               (PIOA_REGS->PIO_SODR = (1<<19))
#define PROXI24V_PA19_BLACK3_Clear()             (PIOA_REGS->PIO_CODR = (1<<19))
#define PROXI24V_PA19_BLACK3_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<19))
#define PROXI24V_PA19_BLACK3_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<19))
#define PROXI24V_PA19_BLACK3_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<19))
#define PROXI24V_PA19_BLACK3_Get()               ((PIOA_REGS->PIO_PDSR >> 19) & 0x1)
#define PROXI24V_PA19_BLACK3_PIN                  PIO_PIN_PA19
#define PROXI24V_PA19_BLACK3_InterruptEnable()   (PIOA_REGS->PIO_IER = (1<<19))
#define PROXI24V_PA19_BLACK3_InterruptDisable()  (PIOA_REGS->PIO_IDR = (1<<19))

/*** Macros for GPIO_PA18_Proxi24v_IN pin ***/
#define GPIO_PA18_Proxi24v_IN_Set()               (PIOA_REGS->PIO_SODR = (1<<18))
#define GPIO_PA18_Proxi24v_IN_Clear()             (PIOA_REGS->PIO_CODR = (1<<18))
#define GPIO_PA18_Proxi24v_IN_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<18))
#define GPIO_PA18_Proxi24v_IN_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<18))
#define GPIO_PA18_Proxi24v_IN_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<18))
#define GPIO_PA18_Proxi24v_IN_Get()               ((PIOA_REGS->PIO_PDSR >> 18) & 0x1)
#define GPIO_PA18_Proxi24v_IN_PIN                  PIO_PIN_PA18

/*** Macros for WRITE_PROTECT pin ***/
#define WRITE_PROTECT_Set()               (PIOA_REGS->PIO_SODR = (1<<17))
#define WRITE_PROTECT_Clear()             (PIOA_REGS->PIO_CODR = (1<<17))
#define WRITE_PROTECT_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<17))
#define WRITE_PROTECT_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<17))
#define WRITE_PROTECT_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<17))
#define WRITE_PROTECT_Get()               ((PIOA_REGS->PIO_PDSR >> 17) & 0x1)
#define WRITE_PROTECT_PIN                  PIO_PIN_PA17

/*** Macros for MOTORA_IN2 pin ***/
#define MOTORA_IN2_Get()               ((PIOB_REGS->PIO_PDSR >> 12) & 0x1)
#define MOTORA_IN2_PIN                  PIO_PIN_PB12

/*** Macros for MOTORC_PD14_lowfault pin ***/
#define MOTORC_PD14_lowfault_Set()               (PIOD_REGS->PIO_SODR = (1<<14))
#define MOTORC_PD14_lowfault_Clear()             (PIOD_REGS->PIO_CODR = (1<<14))
#define MOTORC_PD14_lowfault_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<14))
#define MOTORC_PD14_lowfault_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<14))
#define MOTORC_PD14_lowfault_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<14))
#define MOTORC_PD14_lowfault_Get()               ((PIOD_REGS->PIO_PDSR >> 14) & 0x1)
#define MOTORC_PD14_lowfault_PIN                  PIO_PIN_PD14

/*** Macros for GPIO_PB2 pin ***/
#define GPIO_PB2_Set()               (PIOB_REGS->PIO_SODR = (1<<2))
#define GPIO_PB2_Clear()             (PIOB_REGS->PIO_CODR = (1<<2))
#define GPIO_PB2_Toggle()            (PIOB_REGS->PIO_ODSR ^= (1<<2))
#define GPIO_PB2_OutputEnable()      (PIOB_REGS->PIO_OER = (1<<2))
#define GPIO_PB2_InputEnable()       (PIOB_REGS->PIO_ODR = (1<<2))
#define GPIO_PB2_Get()               ((PIOB_REGS->PIO_PDSR >> 2) & 0x1)
#define GPIO_PB2_PIN                  PIO_PIN_PB2

/*** Macros for PROX1_PE4 pin ***/
#define PROX1_PE4_Set()               (PIOE_REGS->PIO_SODR = (1<<4))
#define PROX1_PE4_Clear()             (PIOE_REGS->PIO_CODR = (1<<4))
#define PROX1_PE4_Toggle()            (PIOE_REGS->PIO_ODSR ^= (1<<4))
#define PROX1_PE4_OutputEnable()      (PIOE_REGS->PIO_OER = (1<<4))
#define PROX1_PE4_InputEnable()       (PIOE_REGS->PIO_ODR = (1<<4))
#define PROX1_PE4_Get()               ((PIOE_REGS->PIO_PDSR >> 4) & 0x1)
#define PROX1_PE4_PIN                  PIO_PIN_PE4
#define PROX1_PE4_InterruptEnable()   (PIOE_REGS->PIO_IER = (1<<4))
#define PROX1_PE4_InterruptDisable()  (PIOE_REGS->PIO_IDR = (1<<4))

/*** Macros for MOTORA_PC8_nsleep pin ***/
#define MOTORA_PC8_nsleep_Set()               (PIOC_REGS->PIO_SODR = (1<<8))
#define MOTORA_PC8_nsleep_Clear()             (PIOC_REGS->PIO_CODR = (1<<8))
#define MOTORA_PC8_nsleep_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<8))
#define MOTORA_PC8_nsleep_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<8))
#define MOTORA_PC8_nsleep_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<8))
#define MOTORA_PC8_nsleep_Get()               ((PIOC_REGS->PIO_PDSR >> 8) & 0x1)
#define MOTORA_PC8_nsleep_PIN                  PIO_PIN_PC8

/*** Macros for PROX2_PD30 pin ***/
#define PROX2_PD30_Set()               (PIOD_REGS->PIO_SODR = (1<<30))
#define PROX2_PD30_Clear()             (PIOD_REGS->PIO_CODR = (1<<30))
#define PROX2_PD30_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<30))
#define PROX2_PD30_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<30))
#define PROX2_PD30_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<30))
#define PROX2_PD30_Get()               ((PIOD_REGS->PIO_PDSR >> 30) & 0x1)
#define PROX2_PD30_PIN                  PIO_PIN_PD30
#define PROX2_PD30_InterruptEnable()   (PIOD_REGS->PIO_IER = (1<<30))
#define PROX2_PD30_InterruptDisable()  (PIOD_REGS->PIO_IDR = (1<<30))

/*** Macros for GPIO_PC4_LED2 pin ***/
#define GPIO_PC4_LED2_Set()               (PIOC_REGS->PIO_SODR = (1<<4))
#define GPIO_PC4_LED2_Clear()             (PIOC_REGS->PIO_CODR = (1<<4))
#define GPIO_PC4_LED2_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<4))
#define GPIO_PC4_LED2_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<4))
#define GPIO_PC4_LED2_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<4))
#define GPIO_PC4_LED2_Get()               ((PIOC_REGS->PIO_PDSR >> 4) & 0x1)
#define GPIO_PC4_LED2_PIN                  PIO_PIN_PC4

/*** Macros for GPIO_PA16_LED4 pin ***/
#define GPIO_PA16_LED4_Set()               (PIOA_REGS->PIO_SODR = (1<<16))
#define GPIO_PA16_LED4_Clear()             (PIOA_REGS->PIO_CODR = (1<<16))
#define GPIO_PA16_LED4_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<16))
#define GPIO_PA16_LED4_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<16))
#define GPIO_PA16_LED4_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<16))
#define GPIO_PA16_LED4_Get()               ((PIOA_REGS->PIO_PDSR >> 16) & 0x1)
#define GPIO_PA16_LED4_PIN                  PIO_PIN_PA16

/*** Macros for GPIO_PC7_LED4 pin ***/
#define GPIO_PC7_LED4_Set()               (PIOC_REGS->PIO_SODR = (1<<7))
#define GPIO_PC7_LED4_Clear()             (PIOC_REGS->PIO_CODR = (1<<7))
#define GPIO_PC7_LED4_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<7))
#define GPIO_PC7_LED4_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<7))
#define GPIO_PC7_LED4_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<7))
#define GPIO_PC7_LED4_Get()               ((PIOC_REGS->PIO_PDSR >> 7) & 0x1)
#define GPIO_PC7_LED4_PIN                  PIO_PIN_PC7

/*** Macros for GPIO_PD25_SD_CS pin ***/
#define GPIO_PD25_SD_CS_Set()               (PIOD_REGS->PIO_SODR = (1<<25))
#define GPIO_PD25_SD_CS_Clear()             (PIOD_REGS->PIO_CODR = (1<<25))
#define GPIO_PD25_SD_CS_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<25))
#define GPIO_PD25_SD_CS_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<25))
#define GPIO_PD25_SD_CS_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<25))
#define GPIO_PD25_SD_CS_Get()               ((PIOD_REGS->PIO_PDSR >> 25) & 0x1)
#define GPIO_PD25_SD_CS_PIN                  PIO_PIN_PD25

/*** Macros for GPIO_PC6_FAN_OUT pin ***/
#define GPIO_PC6_FAN_OUT_Set()               (PIOC_REGS->PIO_SODR = (1<<6))
#define GPIO_PC6_FAN_OUT_Clear()             (PIOC_REGS->PIO_CODR = (1<<6))
#define GPIO_PC6_FAN_OUT_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<6))
#define GPIO_PC6_FAN_OUT_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<6))
#define GPIO_PC6_FAN_OUT_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<6))
#define GPIO_PC6_FAN_OUT_Get()               ((PIOC_REGS->PIO_PDSR >> 6) & 0x1)
#define GPIO_PC6_FAN_OUT_PIN                  PIO_PIN_PC6

/*** Macros for GPIO_PC28_decay pin ***/
#define GPIO_PC28_decay_Set()               (PIOC_REGS->PIO_SODR = (1<<28))
#define GPIO_PC28_decay_Clear()             (PIOC_REGS->PIO_CODR = (1<<28))
#define GPIO_PC28_decay_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<28))
#define GPIO_PC28_decay_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<28))
#define GPIO_PC28_decay_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<28))
#define GPIO_PC28_decay_Get()               ((PIOC_REGS->PIO_PDSR >> 28) & 0x1)
#define GPIO_PC28_decay_PIN                  PIO_PIN_PC28

/*** Macros for GPIO_PA22_LimitSW_IN1 pin ***/
#define GPIO_PA22_LimitSW_IN1_Set()               (PIOA_REGS->PIO_SODR = (1<<22))
#define GPIO_PA22_LimitSW_IN1_Clear()             (PIOA_REGS->PIO_CODR = (1<<22))
#define GPIO_PA22_LimitSW_IN1_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<22))
#define GPIO_PA22_LimitSW_IN1_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<22))
#define GPIO_PA22_LimitSW_IN1_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<22))
#define GPIO_PA22_LimitSW_IN1_Get()               ((PIOA_REGS->PIO_PDSR >> 22) & 0x1)
#define GPIO_PA22_LimitSW_IN1_PIN                  PIO_PIN_PA22
#define GPIO_PA22_LimitSW_IN1_InterruptEnable()   (PIOA_REGS->PIO_IER = (1<<22))
#define GPIO_PA22_LimitSW_IN1_InterruptDisable()  (PIOA_REGS->PIO_IDR = (1<<22))

/*** Macros for GPIO_PC1_LimitSW_IN2 pin ***/
#define GPIO_PC1_LimitSW_IN2_Set()               (PIOC_REGS->PIO_SODR = (1<<1))
#define GPIO_PC1_LimitSW_IN2_Clear()             (PIOC_REGS->PIO_CODR = (1<<1))
#define GPIO_PC1_LimitSW_IN2_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<1))
#define GPIO_PC1_LimitSW_IN2_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<1))
#define GPIO_PC1_LimitSW_IN2_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<1))
#define GPIO_PC1_LimitSW_IN2_Get()               ((PIOC_REGS->PIO_PDSR >> 1) & 0x1)
#define GPIO_PC1_LimitSW_IN2_PIN                  PIO_PIN_PC1
#define GPIO_PC1_LimitSW_IN2_InterruptEnable()   (PIOC_REGS->PIO_IER = (1<<1))
#define GPIO_PC1_LimitSW_IN2_InterruptDisable()  (PIOC_REGS->PIO_IDR = (1<<1))

/*** Macros for            pin ***/
#define           _Get()               ((PIOA_REGS->PIO_PDSR >> 14) & 0x1)
#define           _PIN                  PIO_PIN_PA14

/*** Macros for GPIO_PC5_LED1 pin ***/
#define GPIO_PC5_LED1_Set()               (PIOC_REGS->PIO_SODR = (1<<5))
#define GPIO_PC5_LED1_Clear()             (PIOC_REGS->PIO_CODR = (1<<5))
#define GPIO_PC5_LED1_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<5))
#define GPIO_PC5_LED1_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<5))
#define GPIO_PC5_LED1_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<5))
#define GPIO_PC5_LED1_Get()               ((PIOC_REGS->PIO_PDSR >> 5) & 0x1)
#define GPIO_PC5_LED1_PIN                  PIO_PIN_PC5

/*** Macros for MOTORC_PD17_toff pin ***/
#define MOTORC_PD17_toff_Set()               (PIOD_REGS->PIO_SODR = (1<<17))
#define MOTORC_PD17_toff_Clear()             (PIOD_REGS->PIO_CODR = (1<<17))
#define MOTORC_PD17_toff_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<17))
#define MOTORC_PD17_toff_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<17))
#define MOTORC_PD17_toff_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<17))
#define MOTORC_PD17_toff_Get()               ((PIOD_REGS->PIO_PDSR >> 17) & 0x1)
#define MOTORC_PD17_toff_PIN                  PIO_PIN_PD17

/*** Macros for GPIO_PC2_BUZ_OUT pin ***/
#define GPIO_PC2_BUZ_OUT_Set()               (PIOC_REGS->PIO_SODR = (1<<2))
#define GPIO_PC2_BUZ_OUT_Clear()             (PIOC_REGS->PIO_CODR = (1<<2))
#define GPIO_PC2_BUZ_OUT_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<2))
#define GPIO_PC2_BUZ_OUT_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<2))
#define GPIO_PC2_BUZ_OUT_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<2))
#define GPIO_PC2_BUZ_OUT_Get()               ((PIOC_REGS->PIO_PDSR >> 2) & 0x1)
#define GPIO_PC2_BUZ_OUT_PIN                  PIO_PIN_PC2

/*** Macros for GPIO_PA15_LED3 pin ***/
#define GPIO_PA15_LED3_Set()               (PIOA_REGS->PIO_SODR = (1<<15))
#define GPIO_PA15_LED3_Clear()             (PIOA_REGS->PIO_CODR = (1<<15))
#define GPIO_PA15_LED3_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<15))
#define GPIO_PA15_LED3_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<15))
#define GPIO_PA15_LED3_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<15))
#define GPIO_PA15_LED3_Get()               ((PIOA_REGS->PIO_PDSR >> 15) & 0x1)
#define GPIO_PA15_LED3_PIN                  PIO_PIN_PA15

/*** Macros for MOTORC_IN1 pin ***/
#define MOTORC_IN1_Set()               (PIOD_REGS->PIO_SODR = (1<<23))
#define MOTORC_IN1_Clear()             (PIOD_REGS->PIO_CODR = (1<<23))
#define MOTORC_IN1_Toggle()            (PIOD_REGS->PIO_ODSR ^= (1<<23))
#define MOTORC_IN1_OutputEnable()      (PIOD_REGS->PIO_OER = (1<<23))
#define MOTORC_IN1_InputEnable()       (PIOD_REGS->PIO_ODR = (1<<23))
#define MOTORC_IN1_Get()               ((PIOD_REGS->PIO_PDSR >> 23) & 0x1)
#define MOTORC_IN1_PIN                  PIO_PIN_PD23

/*** Macros for CHIP_SELECT pin ***/
#define CHIP_SELECT_Set()               (PIOA_REGS->PIO_SODR = (1<<11))
#define CHIP_SELECT_Clear()             (PIOA_REGS->PIO_CODR = (1<<11))
#define CHIP_SELECT_Toggle()            (PIOA_REGS->PIO_ODSR ^= (1<<11))
#define CHIP_SELECT_OutputEnable()      (PIOA_REGS->PIO_OER = (1<<11))
#define CHIP_SELECT_InputEnable()       (PIOA_REGS->PIO_ODR = (1<<11))
#define CHIP_SELECT_Get()               ((PIOA_REGS->PIO_PDSR >> 11) & 0x1)
#define CHIP_SELECT_PIN                  PIO_PIN_PA11
        
#define GPIO_PC3_Set()               (PIOC_REGS->PIO_SODR = (1<<3))
#define GPIO_PC3_Clear()             (PIOC_REGS->PIO_CODR = (1<<3))
#define GPIO_PC3_Toggle()            (PIOC_REGS->PIO_ODSR ^= (1<<3))
#define GPIO_PC3_OutputEnable()      (PIOC_REGS->PIO_OER = (1<<3))
#define GPIO_PC3_InputEnable()       (PIOC_REGS->PIO_ODR = (1<<3))
#define GPIO_PC3_Get()               ((PIOC_REGS->PIO_PDSR >> 3) & 0x1)
#define GPIO_PC3_PIN                  PIO_PIN_PC3


// *****************************************************************************
/* PIO Port

  Summary:
    Identifies the available PIO Ports.

  Description:
    This enumeration identifies the available PIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/

typedef enum
{
    PIO_PORT_A = PIOA_BASE_ADDRESS,
    PIO_PORT_B = PIOB_BASE_ADDRESS,
    PIO_PORT_C = PIOC_BASE_ADDRESS,
    PIO_PORT_D = PIOD_BASE_ADDRESS,
    PIO_PORT_E = PIOE_BASE_ADDRESS
} PIO_PORT;

// *****************************************************************************
/* PIO Port Pins

  Summary:
    Identifies the available PIO port pins.

  Description:
    This enumeration identifies the available PIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/

typedef enum
{
    PIO_PIN_PA0 = 0,
    PIO_PIN_PA1 = 1,
    PIO_PIN_PA2 = 2,
    PIO_PIN_PA3 = 3,
    PIO_PIN_PA4 = 4,
    PIO_PIN_PA5 = 5,
    PIO_PIN_PA6 = 6,
    PIO_PIN_PA7 = 7,
    PIO_PIN_PA8 = 8,
    PIO_PIN_PA9 = 9,
    PIO_PIN_PA10 = 10,
    PIO_PIN_PA11 = 11,
    PIO_PIN_PA12 = 12,
    PIO_PIN_PA13 = 13,
    PIO_PIN_PA14 = 14,
    PIO_PIN_PA15 = 15,
    PIO_PIN_PA16 = 16,
    PIO_PIN_PA17 = 17,
    PIO_PIN_PA18 = 18,
    PIO_PIN_PA19 = 19,
    PIO_PIN_PA20 = 20,
    PIO_PIN_PA21 = 21,
    PIO_PIN_PA22 = 22,
    PIO_PIN_PA23 = 23,
    PIO_PIN_PA24 = 24,
    PIO_PIN_PA25 = 25,
    PIO_PIN_PA26 = 26,
    PIO_PIN_PA27 = 27,
    PIO_PIN_PA28 = 28,
    PIO_PIN_PA29 = 29,
    PIO_PIN_PA30 = 30,
    PIO_PIN_PA31 = 31,
    PIO_PIN_PB0 = 32,
    PIO_PIN_PB1 = 33,
    PIO_PIN_PB2 = 34,
    PIO_PIN_PB3 = 35,
    PIO_PIN_PB4 = 36,
    PIO_PIN_PB5 = 37,
    PIO_PIN_PB6 = 38,
    PIO_PIN_PB7 = 39,
    PIO_PIN_PB8 = 40,
    PIO_PIN_PB9 = 41,
    PIO_PIN_PB12 = 44,
    PIO_PIN_PB13 = 45,
    PIO_PIN_PC0 = 64,
    PIO_PIN_PC1 = 65,
    PIO_PIN_PC2 = 66,
    PIO_PIN_PC3 = 67,
    PIO_PIN_PC4 = 68,
    PIO_PIN_PC5 = 69,
    PIO_PIN_PC6 = 70,
    PIO_PIN_PC7 = 71,
    PIO_PIN_PC8 = 72,
    PIO_PIN_PC9 = 73,
    PIO_PIN_PC10 = 74,
    PIO_PIN_PC11 = 75,
    PIO_PIN_PC12 = 76,
    PIO_PIN_PC13 = 77,
    PIO_PIN_PC14 = 78,
    PIO_PIN_PC15 = 79,
    PIO_PIN_PC16 = 80,
    PIO_PIN_PC17 = 81,
    PIO_PIN_PC18 = 82,
    PIO_PIN_PC19 = 83,
    PIO_PIN_PC20 = 84,
    PIO_PIN_PC21 = 85,
    PIO_PIN_PC22 = 86,
    PIO_PIN_PC23 = 87,
    PIO_PIN_PC24 = 88,
    PIO_PIN_PC25 = 89,
    PIO_PIN_PC26 = 90,
    PIO_PIN_PC27 = 91,
    PIO_PIN_PC28 = 92,
    PIO_PIN_PC29 = 93,
    PIO_PIN_PC30 = 94,
    PIO_PIN_PC31 = 95,
    PIO_PIN_PD0 = 96,
    PIO_PIN_PD1 = 97,
    PIO_PIN_PD2 = 98,
    PIO_PIN_PD3 = 99,
    PIO_PIN_PD4 = 100,
    PIO_PIN_PD5 = 101,
    PIO_PIN_PD6 = 102,
    PIO_PIN_PD7 = 103,
    PIO_PIN_PD8 = 104,
    PIO_PIN_PD9 = 105,
    PIO_PIN_PD10 = 106,
    PIO_PIN_PD11 = 107,
    PIO_PIN_PD12 = 108,
    PIO_PIN_PD13 = 109,
    PIO_PIN_PD14 = 110,
    PIO_PIN_PD15 = 111,
    PIO_PIN_PD16 = 112,
    PIO_PIN_PD17 = 113,
    PIO_PIN_PD18 = 114,
    PIO_PIN_PD19 = 115,
    PIO_PIN_PD20 = 116,
    PIO_PIN_PD21 = 117,
    PIO_PIN_PD22 = 118,
    PIO_PIN_PD23 = 119,
    PIO_PIN_PD24 = 120,
    PIO_PIN_PD25 = 121,
    PIO_PIN_PD26 = 122,
    PIO_PIN_PD27 = 123,
    PIO_PIN_PD28 = 124,
    PIO_PIN_PD29 = 125,
    PIO_PIN_PD30 = 126,
    PIO_PIN_PD31 = 127,
    PIO_PIN_PE0 = 128,
    PIO_PIN_PE1 = 129,
    PIO_PIN_PE2 = 130,
    PIO_PIN_PE3 = 131,
    PIO_PIN_PE4 = 132,
    PIO_PIN_PE5 = 133,

    /* This element should not be used in any of the PIO APIs.
       It will be used by other modules or application to denote that none of the PIO Pin is used */
    PIO_PIN_NONE = -1

} PIO_PIN;

typedef  void (*PIO_PIN_CALLBACK) ( PIO_PIN pin, uintptr_t context);

void PIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: PIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t PIO_PortRead(PIO_PORT port);

void PIO_PortWrite(PIO_PORT port, uint32_t mask, uint32_t value);

uint32_t PIO_PortLatchRead ( PIO_PORT port );

void PIO_PortSet(PIO_PORT port, uint32_t mask);

void PIO_PortClear(PIO_PORT port, uint32_t mask);

void PIO_PortToggle(PIO_PORT port, uint32_t mask);

void PIO_PortInputEnable(PIO_PORT port, uint32_t mask);

void PIO_PortOutputEnable(PIO_PORT port, uint32_t mask);

void PIO_PortInterruptEnable(PIO_PORT port, uint32_t mask);

void PIO_PortInterruptDisable(PIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: Local Data types and Prototypes
// *****************************************************************************
// *****************************************************************************

typedef struct {

    /* target pin */
    PIO_PIN                 pin;

    /* Callback for event on target pin*/
    PIO_PIN_CALLBACK        callback;

    /* Callback Context */
    uintptr_t               context;

} PIO_PIN_CALLBACK_OBJ;

// *****************************************************************************
// *****************************************************************************
// Section: PIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void PIO_PinWrite(PIO_PIN pin, bool value)
{
    PIO_PortWrite((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), (uint32_t)(0x1) << (pin & 0x1f), (uint32_t)(value) << (pin & 0x1f));
}

static inline bool PIO_PinRead(PIO_PIN pin)
{
    return (bool)((PIO_PortRead((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5)))) >> (pin & 0x1F)) & 0x1);
}

static inline bool PIO_PinLatchRead(PIO_PIN pin)
{
    return (bool)((PIO_PortLatchRead((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5)))) >> (pin & 0x1F)) & 0x1);
}

static inline void PIO_PinToggle(PIO_PIN pin)
{
    PIO_PortToggle((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), 0x1 << (pin & 0x1F));
}

static inline void PIO_PinSet(PIO_PIN pin)
{
    PIO_PortSet((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), 0x1 << (pin & 0x1F));
}

static inline void PIO_PinClear(PIO_PIN pin)
{
    PIO_PortClear((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), 0x1 << (pin & 0x1F));
}

static inline void PIO_PinInputEnable(PIO_PIN pin)
{
    PIO_PortInputEnable((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), 0x1 << (pin & 0x1F));
}

static inline void PIO_PinOutputEnable(PIO_PIN pin)
{
    PIO_PortOutputEnable((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200 * (pin>>5))), 0x1 << (pin & 0x1F));
}

static inline void PIO_PinInterruptEnable(PIO_PIN pin)
{
    PIO_PortInterruptEnable((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200U * (pin>>5U))), 0x1UL << (pin & 0x1FU));
}

static inline void PIO_PinInterruptDisable(PIO_PIN pin)
{
    PIO_PortInterruptDisable((PIO_PORT)(PIOA_BASE_ADDRESS + (0x200U * (pin>>5U))), 0x1UL << (pin & 0x1FU));
}

bool PIO_PinInterruptCallbackRegister(
    PIO_PIN pin,
    const PIO_PIN_CALLBACK callback,
    uintptr_t context
);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_PIO_H
