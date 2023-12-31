/*******************************************************************************
  TWIHS Peripheral Library Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_twihs2_master.c

  Summary
    TWIHS Master peripheral library interface.

  Description

  Remarks:

*******************************************************************************/

// DOM-IGNORE-BEGIN
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
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_twihs0_master.h"
#include "main.h"

// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// TWIHS0 PLib Interface Routines
// *****************************************************************************
// *****************************************************************************

void TWIHS0_Initialize( void )
{
    // Reset the i2c Module
    TWIHS0_REGS->TWIHS_CR = TWIHS_CR_SWRST_Msk;

    // Disable the I2C Master/Slave Mode
    TWIHS0_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk | TWIHS_CR_SVDIS_Msk;

    // Set Baud rate
    TWIHS0_REGS->TWIHS_CWGR = (TWIHS_CWGR_HOLD_Msk & TWIHS0_REGS->TWIHS_CWGR) | TWIHS_CWGR_CLDIV(192) | TWIHS_CWGR_CHDIV(177) | TWIHS_CWGR_CKDIV(0);

    // Starts the transfer by clearing the transmit hold register
    TWIHS0_REGS->TWIHS_CR = TWIHS_CR_THRCLR_Msk;

    // Enables interrupt on nack and arbitration lost
    TWIHS0_REGS->TWIHS_IER = TWIHS_IER_NACK_Msk | TWIHS_IER_ARBLST_Msk;

    // Enable Master Mode
    TWIHS0_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;

    // Initialize the twihs PLib Object
    twihs0Obj.error = TWIHS_ERROR_NONE;
    twihs0Obj.state = TWIHS_STATE_IDLE;
}

static void TWIHS0_InitiateRead( void )
{
    twihs0Obj.state = TWIHS_STATE_TRANSFER_READ;

    TWIHS0_REGS->TWIHS_MMR |= TWIHS_MMR_MREAD_Msk;

    /* When a single data byte read is performed,
     * the START and STOP bits must be set at the same time
     */
    if(twihs0Obj.readSize == 1)
    {
        TWIHS0_REGS->TWIHS_CR = TWIHS_CR_START_Msk | TWIHS_CR_STOP_Msk;
    }
    else
    {
        TWIHS0_REGS->TWIHS_CR = TWIHS_CR_START_Msk;
    }

    __enable_irq();

    TWIHS0_REGS->TWIHS_IER = TWIHS_IER_RXRDY_Msk | TWIHS_IER_TXCOMP_Msk;
}

static void TWIHS0_InitiateTransfer( uint16_t address, bool type )
{
    // 10-bit Slave Address
    if( address > 0x007F )
    {
        TWIHS0_REGS->TWIHS_MMR = TWIHS_MMR_DADR((address & 0x00007F00) >> 8) | TWIHS_MMR_IADRSZ(1);

        // Set internal address
        TWIHS0_REGS->TWIHS_IADR = TWIHS_IADR_IADR(address & 0x000000FF );
    }
    // 7-bit Slave Address
    else
    {
        TWIHS0_REGS->TWIHS_MMR = TWIHS_MMR_DADR(address) | TWIHS_MMR_IADRSZ(0);
    }

    twihs0Obj.writeCount = 0;
    twihs0Obj.readCount = 0;

    // Write transfer
    if(type == false)
    {
        // Single Byte Write
        if( twihs0Obj.writeSize == 1 )
        {
            // Single Byte write only
            if( twihs0Obj.readSize == 0 )
            {
                // Load last byte in transmit register, issue stop condition
                // Generate TXCOMP interrupt after STOP condition has been sent
                twihs0Obj.state = TWIHS_STATE_WAIT_FOR_TXCOMP;

                TWIHS0_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs0Obj.writeBuffer[twihs0Obj.writeCount++]);
                TWIHS0_REGS->TWIHS_CR = TWIHS_CR_STOP_Msk;
                TWIHS0_REGS->TWIHS_IER = TWIHS_IER_TXCOMP_Msk;
            }
            // Single Byte write and than read transfer
            else
            {
                // START bit must be set before the byte is shifted out. Hence disabled interrupt
                __disable_irq();

                TWIHS0_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs0Obj.writeBuffer[twihs0Obj.writeCount++]);

                // Wait for control byte to be transferred before initiating repeat start for read
                while((TWIHS0_REGS->TWIHS_SR & (TWIHS_SR_TXCOMP_Msk | TWIHS_SR_TXRDY_Msk)) != 0);

                while((TWIHS0_REGS->TWIHS_SR & (TWIHS_SR_TXRDY_Msk)) == 0);

                type = true;
            }
        }
        // Multi-Byte Write
        else
        {
            twihs0Obj.state = TWIHS_STATE_TRANSFER_WRITE;

            TWIHS0_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs0Obj.writeBuffer[twihs0Obj.writeCount++]);

            TWIHS0_REGS->TWIHS_IER = TWIHS_IDR_TXRDY_Msk | TWIHS_IER_TXCOMP_Msk;
        }
    }
    // Read transfer
    if(type)
    {
        TWIHS0_InitiateRead();
    }
}

void TWIHS0_CallbackRegister( TWIHS_CALLBACK callback, uintptr_t contextHandle )
{
    if (callback != NULL)
    {
        twihs0Obj.callback = callback;

        twihs0Obj.context = contextHandle;
    }
}

bool TWIHS0_IsBusy( void )
{
    // Check for ongoing transfer
    if( twihs0Obj.state == TWIHS_STATE_IDLE )
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool TWIHS0_Read( uint16_t address, uint8_t *pdata, size_t length )
{
    // Check for ongoing transfer
    if( twihs0Obj.state != TWIHS_STATE_IDLE )
    {
        return false;
    }

    twihs0Obj.address = address;
    twihs0Obj.readBuffer = pdata;
    twihs0Obj.readSize = length;
    twihs0Obj.writeBuffer = NULL;
    twihs0Obj.writeSize = 0;
    twihs0Obj.error = TWIHS_ERROR_NONE;

    TWIHS0_InitiateTransfer(address, true);

    return true;
}

bool TWIHS0_Write( uint16_t address, uint8_t *pdata, size_t length )
{
    // Check for ongoing transfer
    if( twihs0Obj.state != TWIHS_STATE_IDLE )
    {
        return false;
    }

    twihs0Obj.address = address;
    twihs0Obj.readBuffer = NULL;
    twihs0Obj.readSize = 0;
    twihs0Obj.writeBuffer = pdata;
    twihs0Obj.writeSize = length;
    twihs0Obj.error = TWIHS_ERROR_NONE;

    TWIHS0_InitiateTransfer(address, false);

    return true;
}

bool TWIHS0_WriteRead( uint16_t address, uint8_t *wdata, size_t wlength, uint8_t *rdata, size_t rlength )
{

    // Check for ongoing transfer
    if( twihs0Obj.state != TWIHS_STATE_IDLE )
    {
        return false;
    }

    twihs0Obj.address = address;
    twihs0Obj.readBuffer = rdata;
    twihs0Obj.readSize = rlength;
    twihs0Obj.writeBuffer = wdata;
    twihs0Obj.writeSize = wlength;
    twihs0Obj.error = TWIHS_ERROR_NONE;

    TWIHS0_InitiateTransfer(address, false);

    return true;
}

TWIHS_ERROR TWIHS0_ErrorGet( void )
{
    TWIHS_ERROR error = TWIHS_ERROR_NONE;

    error = twihs0Obj.error;

    twihs0Obj.error = TWIHS_ERROR_NONE;

    return error;
}

bool TWIHS0_TransferSetup( TWIHS_TRANSFER_SETUP* setup, uint32_t srcClkFreq )
{
    uint32_t i2cClkSpeed;
    uint32_t cldiv;
    uint8_t ckdiv = 0;

    if (setup == NULL)
    {
        return false;
    }

    i2cClkSpeed = setup->clkSpeed;

    /* Maximum I2C clock speed in Master mode cannot be greater than 400 KHz */
    if (i2cClkSpeed > 4000000)
    {
        return false;
    }

    if(srcClkFreq == 0)
    {
        srcClkFreq = 150000000;
    }

    /* Formula for calculating baud value involves two unknowns. Fix one unknown and calculate the other.
       Fix the CKDIV value and see if CLDIV (or CHDIV) fits into the 8-bit register. */

    /* Calculate CLDIV with CKDIV set to 0 */
    cldiv = (srcClkFreq /(2 * i2cClkSpeed)) - 3;

    /* CLDIV must fit within 8-bits and CKDIV must fit within 3-bits */
    while ((cldiv > 255) && (ckdiv < 7))
    {
        ckdiv++;
        cldiv /= 2;
    }

    if (cldiv > 255)
    {
        /* Could not generate CLDIV and CKDIV register values for the requested baud rate */
        return false;
    }

    /* set clock waveform generator register */
    TWIHS0_REGS->TWIHS_CWGR = (TWIHS_CWGR_HOLD_Msk & TWIHS0_REGS->TWIHS_CWGR) | TWIHS_CWGR_CLDIV(cldiv) | TWIHS_CWGR_CHDIV(cldiv) | TWIHS_CWGR_CKDIV(ckdiv);

    return true;
}

void TWIHS0_InterruptHandler( void )
{
    uint32_t status;

    // Read the peripheral status
    status = TWIHS0_REGS->TWIHS_SR;

    /* checks if Slave has Nacked */
    if( status & TWIHS_SR_NACK_Msk )
    {
        twihs0Obj.state = TWIHS_STATE_ERROR;
        twihs0Obj.error = TWIHS_ERROR_NACK;
    }

    if( status & TWIHS_SR_TXCOMP_Msk )
    {
        /* Disable and Enable I2C Master */
        TWIHS0_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk;
        TWIHS0_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;

        /* Disable Interrupt */
        TWIHS0_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;
    }

    /* checks if the arbitration is lost in multi-master scenario */
    if( status & TWIHS_SR_ARBLST_Msk )
    {
        /* Re-initiate the transfer if arbitration is lost in
         * between of the transfer
         */
        twihs0Obj.state = TWIHS_STATE_ADDR_SEND;
    }

    if( twihs0Obj.error == TWIHS_ERROR_NONE)
    {
        switch( twihs0Obj.state )
        {
            case TWIHS_STATE_ADDR_SEND:
            {
                if (twihs0Obj.writeSize != 0 )
                {
                    // Initiate Write transfer
                    TWIHS0_InitiateTransfer(twihs0Obj.address, false);
                }
                else
                {
                    // Initiate Read transfer
                    TWIHS0_InitiateTransfer(twihs0Obj.address, true);
                }
            }
            break;

            case TWIHS_STATE_TRANSFER_WRITE:
            {
                /* checks if master is ready to transmit */
                if( status & TWIHS_SR_TXRDY_Msk )
                {
                    // Write Last Byte and then initiate read transfer
                    if( ( twihs0Obj.writeCount == (twihs0Obj.writeSize -1) ) && ( twihs0Obj.readSize != 0 ))
                    {
                        // START bit must be set before the last byte is shifted out to generate repeat start. Hence disabled interrupt
                        __disable_irq();
                        TWIHS0_REGS->TWIHS_IDR = TWIHS_IDR_TXRDY_Msk;
                        TWIHS0_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs0Obj.writeBuffer[twihs0Obj.writeCount++]);
                        TWIHS0_InitiateRead();
                    }
                    // Write Last byte and then issue STOP condition
                    else if ( twihs0Obj.writeCount == (twihs0Obj.writeSize -1))
                    {
                        // Load last byte in transmit register, issue stop condition
                        // Generate TXCOMP interrupt after STOP condition has been sent
                        TWIHS0_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs0Obj.writeBuffer[twihs0Obj.writeCount++]);
                        TWIHS0_REGS->TWIHS_CR = TWIHS_CR_STOP_Msk;
                        TWIHS0_REGS->TWIHS_IDR = TWIHS_IDR_TXRDY_Msk;

                        /* Check TXCOMP to confirm if STOP condition has been sent, otherwise wait for TXCOMP interrupt */
                        status = TWIHS0_REGS->TWIHS_SR;

                        if( status & TWIHS_SR_TXCOMP_Msk )
                        {
                            twihs0Obj.state = TWIHS_STATE_TRANSFER_DONE;
                        }
                        else
                        {
                            twihs0Obj.state = TWIHS_STATE_WAIT_FOR_TXCOMP;
                        }
                    }
                    // Write next byte
                    else
                    {
                        TWIHS0_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs0Obj.writeBuffer[twihs0Obj.writeCount++]);
                    }

                    // Dummy read to ensure that TXRDY bit is cleared
                    status = TWIHS0_REGS->TWIHS_SR;
                }

                break;
            }

            case TWIHS_STATE_TRANSFER_READ:
            {
                /* checks if master has received the data */
                if( status & TWIHS_SR_RXRDY_Msk )
                {
                    // Set the STOP (or START) bit before reading the TWIHS_RHR on the next-to-last access
                    if(  twihs0Obj.readCount == (twihs0Obj.readSize - 2) )
                    {
                        TWIHS0_REGS->TWIHS_CR = TWIHS_CR_STOP_Msk;
                    }

                    /* read the received data */
                    twihs0Obj.readBuffer[twihs0Obj.readCount++] = (uint8_t)(TWIHS0_REGS->TWIHS_RHR & TWIHS_RHR_RXDATA_Msk);

                    /* checks if transmission has reached at the end */
                    if( twihs0Obj.readCount == twihs0Obj.readSize )
                    {
                        /* Disable the RXRDY interrupt*/
                        TWIHS0_REGS->TWIHS_IDR = TWIHS_IDR_RXRDY_Msk;

                        /* Check TXCOMP to confirm if STOP condition has been sent, otherwise wait for TXCOMP interrupt */
                        status = TWIHS0_REGS->TWIHS_SR;
                        if( status & TWIHS_SR_TXCOMP_Msk )
                        {
                            twihs0Obj.state = TWIHS_STATE_TRANSFER_DONE;
                        }
                        else
                        {
                            twihs0Obj.state = TWIHS_STATE_WAIT_FOR_TXCOMP;
                        }
                    }
                }
                break;
            }

            case TWIHS_STATE_WAIT_FOR_TXCOMP:
            {
                if( status & TWIHS_SR_TXCOMP_Msk )
                {
                    twihs0Obj.state = TWIHS_STATE_TRANSFER_DONE;
                }
                break;
            }

            default:
            {
                break;
            }
        }
    }
    if (twihs0Obj.state == TWIHS_STATE_ERROR)
    {
        // NACK is received,
        twihs0Obj.state = TWIHS_STATE_IDLE;
        TWIHS0_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;

        // Disable and Enable I2C Master
        TWIHS0_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk;
        TWIHS0_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;

        if ( twihs0Obj.callback != NULL )
        {
            twihs0Obj.callback( twihs0Obj.context );
        }
    }
    // check for completion of transfer
    if( twihs0Obj.state == TWIHS_STATE_TRANSFER_DONE )
    {
        twihs0Obj.error = TWIHS_ERROR_NONE;

        // Reset the PLib objects and Interrupts
        twihs0Obj.state = TWIHS_STATE_IDLE;
        TWIHS0_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;

        // Disable and Enable I2C Master
        TWIHS0_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk;
        TWIHS0_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;

        if ( twihs0Obj.callback != NULL )
        {
            twihs0Obj.callback( twihs0Obj.context );
        }
    }

    return;
}

void Twihs0_BusywaitandRecover(void)
{
    Twihswait_Timer = 0;
    
    //busywait
    while(TWIHS0_IsBusy())
    {
        if(Twihswait_Timer>=100) //>100
        {
            //recover
            
            //Disable TWIHS0 peripheral clock
            PMC_REGS->PMC_PCDR1 |= (1 << (ID_TWIHS0));
    
            //enable the GPIO peripheral
        	PIOD_REGS->PIO_PER |= (PIO_PD27);
            PIOD_REGS->PIO_PER |= (PIO_PD28);

            PIOD_REGS->PIO_ODR |= (PIO_PD27);
            PIOD_REGS->PIO_OER |= (PIO_PD28);

//            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_SODR |= (PIO_PD27);

//            for(int l=0;l<9;l++)
            while(1)
            {
                PIOD_REGS->PIO_CODR |= (PIO_PD28);
                SYSTICK_DelayUs(1);
                PIOD_REGS->PIO_SODR |= (PIO_PD28);
                SYSTICK_DelayUs(1);
                
                if(((PIOD_REGS->PIO_PDSR & PIO_PD27) >> 27) == 0x1)
                {
                    Twihswait_Timer = Twihswait_Timer+1;
                    break;
                }
            }
            
            PIOD_REGS->PIO_CODR |= (PIO_PD28);
            
            PIOD_REGS->PIO_OER |= (PIO_PD27);
            
            PIOD_REGS->PIO_CODR |= (PIO_PD27);
            SYSTICK_DelayUs(1);
            PIOD_REGS->PIO_SODR |= (PIO_PD28);
            SYSTICK_DelayUs(1);
            PIOD_REGS->PIO_SODR |= (PIO_PD27);
            SYSTICK_DelayUs(1);
            
            
            //Disable interrupts on the I2C pins
            PIOD_REGS->PIO_IDR |= (PIO_PD27);
            PIOD_REGS->PIO_IDR |= (PIO_PD28);
            
            //No pull-up resistors
            PIOD_REGS->PIO_PUDR |= (PIO_PD27);
            PIOD_REGS->PIO_PUDR |= (PIO_PD28);
            
            //Assign corresponding pin to Peripheral A function
            PIOD_REGS->PIO_ABCDSR[0] &= (PIO_PD27);
            PIOD_REGS->PIO_ABCDSR[1] &= ~(PIO_PD27);

            PIOD_REGS->PIO_ABCDSR[0] &= (PIO_PD28);
            PIOD_REGS->PIO_ABCDSR[1] &= ~(PIO_PD28);
            
            PIOD_REGS->PIO_PDR |= (PIO_PD27);
            PIOD_REGS->PIO_PDR |= (PIO_PD28);
            
//            Enable TWIHS0 peripheral clock
            PMC_REGS->PMC_PCER1 |= (1 << (ID_TWIHS0));

            TWIHS0_Initialize();
            Twihswait_Timer = 0;
        }
    }   
}

void Twihs0_Recover(void)
{
//    Twihswait_Timer = 0;
    
    //busywait
//    while(TWIHS0_IsBusy())
//    {
//        if(Twihswait_Timer>=1) //>100
//        {
            //recover
            
            //Disable TWIHS0 peripheral clock
//            PMC_REGS->PMC_PCDR1 |= (1 << (ID_TWIHS0-32));
    
            //enable the GPIO peripheral
//        	((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_PER |= (PIO_PD27);
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_PER |= (PIO_PD28);

//            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_OER |= (PIO_PD27);
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_OER |= (PIO_PD28);

//            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_SODR |= (PIO_PD27);

            for(int l=0;l<10;l++)
            {
                ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_CODR |= (PIO_PD28);
                SYSTICK_DelayUs(1);
                ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_SODR |= (PIO_PD28);
                SYSTICK_DelayUs(1);
            }
            
//            SYSTICK_DelayMs(1000);
            
            PMC_REGS->PMC_PCDR1 |= (1 << (ID_TWIHS0));
            
            //Assign corresponding pin to Peripheral A function
//            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_ABCDSR[0] &= ~(PIO_PD27);
//            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_ABCDSR[1] &= (PIO_PD27);
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_PER |= (PIO_PD27);

//            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_OER |= (PIO_PD27);
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_ODR |= (PIO_PD28);
            
            
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_ABCDSR[0] &= ~(PIO_PD28);
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_ABCDSR[1] &= (PIO_PD28);
            
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_ABCDSR[0] &= ~(PIO_PD27);
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_ABCDSR[1] &= (PIO_PD27);
            
            //Disable interrupts on the I2C pins
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_IDR |= (PIO_PD27);
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_IDR |= (PIO_PD28);
            
            //No pull-up resistors
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_PUDR |= (PIO_PD27);
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_PUDR |= (PIO_PD28);
            
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_PDR |= (PIO_PD27);
            ((pio_registers_t*)PIOD_BASE_ADDRESS)->PIO_PDR |= (PIO_PD28);
            
            
            //Ensable TWIHS0 peripheral clock
            PMC_REGS->PMC_PCER1 |= (1 << (ID_TWIHS0));

            TWIHS0_Initialize();
//        }
//    }
    
//    Twihswait_Timer = 0;
}