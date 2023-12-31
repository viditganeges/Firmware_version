/*******************************************************************************
  SYS PORTS Static Functions for PORTS System Service

  Company:
    Microchip Technology Inc.

  File Name:
    plib_pio.c

  Summary:
    PIO function implementations for the PIO PLIB.

  Description:
    The PIO PLIB provides a simple interface to manage peripheral
    input-output controller.

*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#include "plib_pio.h"
#include "interrupts.h"
#include "main.h"


#define PIO_MAX_NUM_OF_CHANNELS     5U

/* Array to store callback objects of each configured interrupt */
static PIO_PIN_CALLBACK_OBJ portPinCbObj[1 + 0 + 1 + 1 + 1];

/* Array to store number of interrupts in each PORT Channel + previous interrupt count */
static uint8_t portNumCb[PIO_MAX_NUM_OF_CHANNELS + 1] = {0U, 1U, 1U, 2U, 3U, 4U};
 void PIO_Interrupt_Handler ( PIO_PORT port );

/******************************************************************************
  Function:
    PIO_Initialize ( void )

  Summary:
    Initialize the PIO library.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_Initialize ( void )
{
    MATRIX_REGS->CCFG_SYSIO = 0x0;
    /* Selected System IO pins are configured as GPIO */
    MATRIX_REGS->CCFG_SYSIO |= 0x1010;

    /************************ PIO A Initialization ************************/
    /* PORTA Peripheral Function Selection */
    ((pio_registers_t*)PIO_PORT_A)->PIO_ABCDSR[0]= 0x2000000;
    ((pio_registers_t*)PIO_PORT_A)->PIO_ABCDSR[1]= 0xde000020;
    /* PORTA PIO Disable and Peripheral Enable*/
    ((pio_registers_t*)PIO_PORT_A)->PIO_PDR = 0xde20763A;           //0xde207627;
    ((pio_registers_t*)PIO_PORT_A)->PIO_PER = ~0xde20763A;           //~0xde207627;
    ((pio_registers_t*)PIO_PORT_A)->PIO_MDDR = 0xFFFFFFFF;
    /* PORTA Pull Up Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_A)->PIO_PUDR = 0xFFFFFFFF;
    /* PORTA Pull Down Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_A)->PIO_PPDDR = 0xFFFFFFFF;
    /* PORTA Output Write Enable */
    ((pio_registers_t*)PIO_PORT_A)->PIO_OWER = PIO_OWER_Msk;
    /* PORTA Output Direction Enable */
    ((pio_registers_t*)PIO_PORT_A)->PIO_OER = 0x20138825;           //0x20138824;
    ((pio_registers_t*)PIO_PORT_A)->PIO_ODR = ~0x20138825;          //~0x20138824;
    /* PORTA Initial state High */
    ((pio_registers_t*)PIO_PORT_A)->PIO_ODSR = 0x20800;
    /* If PIO Interrupt is selected for both edge, it doesn't need any register
       configuration */
    /* PORTA Interrupt Status Clear */
    ((pio_registers_t*)PIO_PORT_A)->PIO_ISR;
    /* PORTA system level interrupt will be enabled by NVIC Manager */
    /* PORTA module level Interrupt for every pin has to be enabled by user
       by calling PIO_PinInterruptEnable() API dynamically as and when needed*/
    /* PORTA drive control */
    ((pio_registers_t*)PIO_PORT_A)->PIO_DRIVER = 0x0;

    /************************ PIO B Initialization ************************/
    /* PORTB Peripheral Function Selection */
    ((pio_registers_t*)PIO_PORT_B)->PIO_ABCDSR[0]= 0x10;
    ((pio_registers_t*)PIO_PORT_B)->PIO_ABCDSR[1]= 0x13;
    /* PORTB PIO Disable and Peripheral Enable*/
    ((pio_registers_t*)PIO_PORT_B)->PIO_PDR = 0x1013;
    ((pio_registers_t*)PIO_PORT_B)->PIO_PER = ~0x1013;
    ((pio_registers_t*)PIO_PORT_B)->PIO_MDDR = 0xFFFFFFFF;
    /* PORTB Pull Up Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_B)->PIO_PUDR = 0xFFFFFFFF;
    /* PORTB Pull Down Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_B)->PIO_PPDDR = 0xFFFFFFFF;
    /* PORTB Output Write Enable */
    ((pio_registers_t*)PIO_PORT_B)->PIO_OWER = PIO_OWER_Msk;
    /* PORTB Output Direction Enable */
    ((pio_registers_t*)PIO_PORT_B)->PIO_OER = 0x1004;
    ((pio_registers_t*)PIO_PORT_B)->PIO_ODR = ~0x1004;
    /* PORTB Initial state High */
    ((pio_registers_t*)PIO_PORT_B)->PIO_ODSR = 0x0;
    /* PORTB drive control */
    ((pio_registers_t*)PIO_PORT_B)->PIO_DRIVER = 0x0;

    /************************ PIO C Initialization ************************/
    /* PORTC Peripheral Function Selection */
    ((pio_registers_t*)PIO_PORT_C)->PIO_ABCDSR[0]= 0x400000;
    ((pio_registers_t*)PIO_PORT_C)->PIO_ABCDSR[1]= 0x0;
    /* PORTC PIO Disable and Peripheral Enable*/
    ((pio_registers_t*)PIO_PORT_C)->PIO_PDR = 0x400000;
    ((pio_registers_t*)PIO_PORT_C)->PIO_PER = ~0x400000;
    ((pio_registers_t*)PIO_PORT_C)->PIO_MDDR = 0xFFFFFFFF;
    /* PORTC Pull Up Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_C)->PIO_PUDR = 0xFFFFFFFF;
    /* PORTC Pull Down Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_C)->PIO_PPDDR = 0xFFFFFFFF;
    /* PORTC Output Write Enable */
    ((pio_registers_t*)PIO_PORT_C)->PIO_OWER = PIO_OWER_Msk;
    /* PORTC Output Direction Enable */
    ((pio_registers_t*)PIO_PORT_C)->PIO_OER = 0x964dfc;
    ((pio_registers_t*)PIO_PORT_C)->PIO_ODR = ~0x964dfc;
    /* PORTC Initial state High */
    ((pio_registers_t*)PIO_PORT_C)->PIO_ODSR = 0x0;
    /* If PIO Interrupt is selected for both edge, it doesn't need any register
       configuration */
    /* PORTC Interrupt Status Clear */
    ((pio_registers_t*)PIO_PORT_C)->PIO_ISR;
    /* PORTC system level interrupt will be enabled by NVIC Manager */
    /* PORTC module level Interrupt for every pin has to be enabled by user
       by calling PIO_PinInterruptEnable() API dynamically as and when needed*/
    /* PORTC drive control */
    ((pio_registers_t*)PIO_PORT_C)->PIO_DRIVER = 0x200000;

    /************************ PIO D Initialization ************************/
    /* PORTD Peripheral Function Selection */
    ((pio_registers_t*)PIO_PORT_D)->PIO_ABCDSR[0]= 0x4718000;
    ((pio_registers_t*)PIO_PORT_D)->PIO_ABCDSR[1]= 0x1c000000;
    /* PORTD PIO Disable and Peripheral Enable*/
    ((pio_registers_t*)PIO_PORT_D)->PIO_PDR = 0x1c718000;               //0x1cf18000;
    ((pio_registers_t*)PIO_PORT_D)->PIO_PER = ~0x1c718000;              //~0x1cf18000;
    ((pio_registers_t*)PIO_PORT_D)->PIO_MDDR = 0xFFFFFFFF;
    /* PORTD Pull Up Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_D)->PIO_PUDR = 0xFFFFFFFF;
    /* PORTD Pull Down Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_D)->PIO_PPDDR = 0xFFFFFFFF;
    /* PORTD Output Write Enable */
    ((pio_registers_t*)PIO_PORT_D)->PIO_OWER = PIO_OWER_Msk;
    /* PORTD Output Direction Enable */
    ((pio_registers_t*)PIO_PORT_D)->PIO_OER = 0xa2800b1e;               //0xa2000b1e;
    ((pio_registers_t*)PIO_PORT_D)->PIO_ODR = ~0xa2800b1e;              //~0xa2000b1e;
    /* PORTD Initial state High */
    ((pio_registers_t*)PIO_PORT_D)->PIO_ODSR = 0x80000000;
    /* If PIO Interrupt is selected for both edge, it doesn't need any register
       configuration */
    /* PORTD Interrupt Status Clear */
    ((pio_registers_t*)PIO_PORT_D)->PIO_ISR;
    /* PORTD system level interrupt will be enabled by NVIC Manager */
    /* PORTD module level Interrupt for every pin has to be enabled by user
       by calling PIO_PinInterruptEnable() API dynamically as and when needed*/
    /* PORTD drive control */
    ((pio_registers_t*)PIO_PORT_D)->PIO_DRIVER = 0x0;

    /************************ PIO E Initialization ************************/
    ((pio_registers_t*)PIO_PORT_E)->PIO_PER = 0xFFFFFFFF;
    ((pio_registers_t*)PIO_PORT_E)->PIO_MDDR = 0xFFFFFFFF;
    /* PORTE Pull Up Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_E)->PIO_PUDR = 0xFFFFFFFF;
    /* PORTE Pull Down Enable/Disable as per MHC selection */
    ((pio_registers_t*)PIO_PORT_E)->PIO_PPDDR = 0xFFFFFFFF;
    /* PORTE Output Write Enable */
    ((pio_registers_t*)PIO_PORT_E)->PIO_OWER = PIO_OWER_Msk;
    /* PORTE Output Direction Enable */
    ((pio_registers_t*)PIO_PORT_E)->PIO_OER = 0x4;
    ((pio_registers_t*)PIO_PORT_E)->PIO_ODR = ~0x4;
    /* PORTE Initial state High */
    ((pio_registers_t*)PIO_PORT_E)->PIO_ODSR = 0x0;
    /* If PIO Interrupt is selected for both edge, it doesn't need any register
       configuration */
    /* PORTE Interrupt Status Clear */
    ((pio_registers_t*)PIO_PORT_E)->PIO_ISR;
    /* PORTE system level interrupt will be enabled by NVIC Manager */
    /* PORTE module level Interrupt for every pin has to be enabled by user
       by calling PIO_PinInterruptEnable() API dynamically as and when needed*/
    /* PORTE drive control */
    ((pio_registers_t*)PIO_PORT_E)->PIO_DRIVER = 0x0;

    uint32_t i;
    /* Initialize Interrupt Pin data structures */
    portPinCbObj[3 + 0].pin = PIO_PIN_PE4;
    
    portPinCbObj[2 + 0].pin = PIO_PIN_PD30;
    
    portPinCbObj[0].pin = PIO_PIN_PA22;
    
    portPinCbObj[1 + 0].pin = PIO_PIN_PC1;
    
    for(i=0U; i<4U; i++)
    {
        portPinCbObj[i].callback = NULL;
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: PIO APIs which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    uint32_t PIO_PortRead ( PIO_PORT port )

  Summary:
    Read all the I/O lines of the selected port.

  Description:
    This function reads the live data values on all the I/O lines of the
    selected port.  Bit values returned in each position indicate corresponding
    pin levels.
    1 = Pin is high.
    0 = Pin is low.

    This function reads the value regardless of pin configuration, whether it is
    set as as an input, driven by the PIO Controller, or driven by a peripheral.

  Remarks:
    If the port has less than 32-bits, unimplemented pins will read as
    low (0).
    Implemented pins are Right aligned in the 32-bit return value.
*/
uint32_t PIO_PortRead(PIO_PORT port)
{
    return ((pio_registers_t*)port)->PIO_PDSR;
}

// *****************************************************************************
/* Function:
    void PIO_PortWrite (PIO_PORT port, uint32_t mask, uint32_t value);

  Summary:
    Write the value on the masked I/O lines of the selected port.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortWrite(PIO_PORT port, uint32_t mask, uint32_t value)
{
    ((pio_registers_t*)port)->PIO_ODSR = (((pio_registers_t*)port)->PIO_ODSR & (~mask)) | (mask & value);
}

// *****************************************************************************
/* Function:
    uint32_t PIO_PortLatchRead ( PIO_PORT port )

  Summary:
    Read the latched value on all the I/O lines of the selected port.

  Remarks:
    See plib_pio.h for more details.
*/
uint32_t PIO_PortLatchRead(PIO_PORT port)
{
    return ((pio_registers_t*)port)->PIO_ODSR;
}

// *****************************************************************************
/* Function:
    void PIO_PortSet ( PIO_PORT port, uint32_t mask )

  Summary:
    Set the selected IO pins of a port.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortSet(PIO_PORT port, uint32_t mask)
{
    ((pio_registers_t*)port)->PIO_SODR = mask;
}

// *****************************************************************************
/* Function:
    void PIO_PortClear ( PIO_PORT port, uint32_t mask )

  Summary:
    Clear the selected IO pins of a port.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortClear(PIO_PORT port, uint32_t mask)
{
    ((pio_registers_t*)port)->PIO_CODR = mask;
}

// *****************************************************************************
/* Function:
    void PIO_PortToggle ( PIO_PORT port, uint32_t mask )

  Summary:
    Toggles the selected IO pins of a port.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortToggle(PIO_PORT port, uint32_t mask)
{
    /* Write into Clr and Set registers */
    ((pio_registers_t*)port)->PIO_ODSR^= mask;
}

// *****************************************************************************
/* Function:
    void PIO_PortInputEnable ( PIO_PORT port, uint32_t mask )

  Summary:
    Enables selected IO pins of a port as input.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortInputEnable(PIO_PORT port, uint32_t mask)
{
    ((pio_registers_t*)port)->PIO_ODR = mask;
}

// *****************************************************************************
/* Function:
    void PIO_PortOutputEnable ( PIO_PORT port, uint32_t mask )

  Summary:
    Enables selected IO pins of a port as output(s).

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortOutputEnable(PIO_PORT port, uint32_t mask)
{
    ((pio_registers_t*)port)->PIO_OER = mask;
}

// *****************************************************************************
/* Function:
    void PIO_PortInterruptEnable(PIO_PORT port, uint32_t mask)

  Summary:
    Enables IO interrupt on selected IO pins of a port.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortInterruptEnable(PIO_PORT port, uint32_t mask)
{
    ((pio_registers_t*)port)->PIO_IER = mask;
}

// *****************************************************************************
/* Function:
    void PIO_PortInterruptDisable(PIO_PORT port, uint32_t mask)

  Summary:
    Disables IO interrupt on selected IO pins of a port.

  Remarks:
    See plib_pio.h for more details.
*/
void PIO_PortInterruptDisable(PIO_PORT port, uint32_t mask)
{
    ((pio_registers_t*)port)->PIO_IDR = mask;
}

// *****************************************************************************
// *****************************************************************************
// Section: PIO APIs which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    bool PIO_PinInterruptCallbackRegister(
        PIO_PIN pin,
        const PIO_PIN_CALLBACK callback,
        uintptr_t context
    );

  Summary:
    Allows application to register callback for configured pin.

  Remarks:
    See plib_pio.h for more details.
*/
bool PIO_PinInterruptCallbackRegister(
    PIO_PIN pin,
    const PIO_PIN_CALLBACK callback,
    uintptr_t context
)
{
    uint8_t i;
    uint8_t portIndex;

    portIndex = (uint8_t)pin >> 5U;

    for(i = portNumCb[portIndex]; i < portNumCb[portIndex +1U]; i++)
    {
        if (portPinCbObj[i].pin == pin)
        {
            portPinCbObj[i].callback = callback;
            portPinCbObj[i].context  = context;
            return true;
        }
    }
    return false;
}
// *****************************************************************************
// *****************************************************************************
// Section: Local Function Implementation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void PIO_Interrupt_Handler ( PIO_PORT port )

  Summary:
    Interrupt handler for a selected port.

  Description:
    This function defines the Interrupt handler for a selected port.

  Remarks:
	It is an internal function used by the library, user should not call it.
*/
void PIO_Interrupt_Handler ( PIO_PORT port )
{
    uint32_t status;
    uint32_t i, portIndex;

    status  = ((pio_registers_t*)port)->PIO_ISR;
    status &= ((pio_registers_t*)port)->PIO_IMR;

    /* get the index of the port channel: PIO_PORT_A--> 0, PIO_PORT_B--> 1 ... */
    portIndex = (port - PIOA_BASE_ADDRESS) >> 9U;

    /* Check pending events and call callback if registered */
    for(i = portNumCb[portIndex]; i < portNumCb[portIndex +1U]; i++)
    {
        if(((status & (1UL << (portPinCbObj[i].pin & 0x1FU))) != 0U) && (portPinCbObj[i].callback != NULL))
        {
            portPinCbObj[i].callback (portPinCbObj[i].pin, portPinCbObj[i].context);
        }
    }

}
// *****************************************************************************
// *****************************************************************************
// Section: Interrupt Service Routine (ISR) Implementation(s)
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
/* Function:
    void PIOA_InterruptHandler (void)

  Summary:
    Interrupt handler for PORTA.

  Description:
    This function defines the Interrupt service routine for PORTA.
    This is the function which by default gets into Interrupt Vector Table.

  Remarks:
    User should not call this function.
*/
void PIOA_InterruptHandler(void)
{
    /* Local PIO Interrupt Handler */
//    PIO_Interrupt_Handler(PIO_PORT_A);
    uint32_t status;
    status  = ((pio_registers_t*)PIO_PORT_A)->PIO_ISR;
    status &= ((pio_registers_t*)PIO_PORT_A)->PIO_IMR;
    if(status & (1 << GPIO_PA22_LimitSW_IN1_PIN))
    {
      Edge_Sense1_Irq();
    }
}

// *****************************************************************************
/* Function:
    void PIOC_InterruptHandler (void)

  Summary:
    Interrupt handler for PORTC.

  Description:
    This function defines the Interrupt service routine for PORTC.
    This is the function which by default gets into Interrupt Vector Table.

  Remarks:
    User should not call this function.
*/
void PIOC_InterruptHandler(void)
{
    /* Local PIO Interrupt Handler */
//    PIO_Interrupt_Handler(PIO_PORT_C);
    uint32_t status;
    status  = ((pio_registers_t*)PIO_PORT_C)->PIO_ISR;
    status &= ((pio_registers_t*)PIO_PORT_C)->PIO_IMR;
    if(status & (1 << (GPIO_PC1_LimitSW_IN2_PIN-64)))
    {
      Edge_Sense2_Irq();
    }
//    if(status & (1 << (MOTORA_PC21_lowfault_PIN-64)))
//    {
//      Motor1FaultCheck();
//    }
//    if(status & (1 << (MOTORB_PC16_lowfault_PIN-64)))
//    {
//      Motor2FaultCheck();
//    }
}

// *****************************************************************************
/* Function:
    void PIOD_InterruptHandler (void)

  Summary:
    Interrupt handler for PORTD.

  Description:
    This function defines the Interrupt service routine for PORTD.
    This is the function which by default gets into Interrupt Vector Table.

  Remarks:
    User should not call this function.
*/
void PIOD_InterruptHandler(void)
{
    /* Local PIO Interrupt Handler */
//    PIO_Interrupt_Handler(PIO_PORT_D);
    uint32_t status;
    status  = ((pio_registers_t*)PIO_PORT_D)->PIO_ISR;
    status &= ((pio_registers_t*)PIO_PORT_D)->PIO_IMR;
    if(status & (1 << (PROX2_PD30_PIN-96)))
    {
      Rotate_Sense2_Irq();
    }
//    if(status & (1 << (MOTORC_PD14_lowfault_PIN-96)))
//    {
//      BrushMotorFaultCheck();
//    }
}

// *****************************************************************************
/* Function:
    void PIOE_InterruptHandler (void)

  Summary:
    Interrupt handler for PORTE.

  Description:
    This function defines the Interrupt service routine for PORTE.
    This is the function which by default gets into Interrupt Vector Table.

  Remarks:
    User should not call this function.
*/
void PIOE_InterruptHandler(void)
{
    /* Local PIO Interrupt Handler */
//    PIO_Interrupt_Handler(PIO_PORT_E);
    uint32_t status;
    status  = ((pio_registers_t*)PIO_PORT_E)->PIO_ISR;
    status &= ((pio_registers_t*)PIO_PORT_E)->PIO_IMR;
    if(status & (1 << (PROX1_PE4_PIN-128)))
    {
      Rotate_Sense1_Irq();
    }
}




/*******************************************************************************
 End of File
*/
