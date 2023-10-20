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
#include "plib_twihs2_master.h"
#include "peripheral/systick/plib_systick.h"

// *****************************************************************************
// *****************************************************************************
// Global Data
// *****************************************************************************
// *****************************************************************************

//static TWIHS_OBJ twihs2Obj;

// *****************************************************************************
// *****************************************************************************
// TWIHS2 PLib Interface Routines
// *****************************************************************************
// *****************************************************************************

void TWIHS2_Initialize( void )
{
    // Reset the i2c Module
    TWIHS2_REGS->TWIHS_CR = TWIHS_CR_SWRST_Msk;

    // Disable the I2C Master/Slave Mode
    TWIHS2_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk | TWIHS_CR_SVDIS_Msk;

    // Set Baud rate
    TWIHS2_REGS->TWIHS_CWGR = (TWIHS_CWGR_HOLD_Msk & TWIHS2_REGS->TWIHS_CWGR) | TWIHS_CWGR_CLDIV(167) | TWIHS_CWGR_CHDIV(167) | TWIHS_CWGR_CKDIV(3);

    // Starts the transfer by clearing the transmit hold register
    TWIHS2_REGS->TWIHS_CR = TWIHS_CR_THRCLR_Msk;

    // Enables interrupt on nack and arbitration lost
    TWIHS2_REGS->TWIHS_IER = TWIHS_IER_NACK_Msk | TWIHS_IER_ARBLST_Msk;

    // Enable Master Mode
    TWIHS2_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;

    // Initialize the twihs PLib Object
    twihs2Obj.error = TWIHS_ERROR_NONE;
    twihs2Obj.state = TWIHS_STATE_IDLE;
}

static void TWIHS2_InitiateRead( void )
{
    twihs2Obj.state = TWIHS_STATE_TRANSFER_READ;

    TWIHS2_REGS->TWIHS_MMR |= TWIHS_MMR_MREAD_Msk;

    /* When a single data byte read is performed,
     * the START and STOP bits must be set at the same time
     */
    if(twihs2Obj.readSize == 1)
    {
        TWIHS2_REGS->TWIHS_CR = TWIHS_CR_START_Msk | TWIHS_CR_STOP_Msk;
    }
    else
    {
        TWIHS2_REGS->TWIHS_CR = TWIHS_CR_START_Msk;
    }

    __enable_irq();

    TWIHS2_REGS->TWIHS_IER = TWIHS_IER_RXRDY_Msk | TWIHS_IER_TXCOMP_Msk;
}

static void TWIHS2_InitiateTransfer( uint16_t address, bool type )
{
    // 10-bit Slave Address
    if( address > 0x007F )
    {
        TWIHS2_REGS->TWIHS_MMR = TWIHS_MMR_DADR((address & 0x00007F00) >> 8) | TWIHS_MMR_IADRSZ(1);

        // Set internal address
        TWIHS2_REGS->TWIHS_IADR = TWIHS_IADR_IADR(address & 0x000000FF );
    }
    // 7-bit Slave Address
    else
    {
        TWIHS2_REGS->TWIHS_MMR = TWIHS_MMR_DADR(address) | TWIHS_MMR_IADRSZ(0);
    }

    twihs2Obj.writeCount = 0;
    twihs2Obj.readCount = 0;

    // Write transfer
    if(type == false)
    {
        // Single Byte Write
        if( twihs2Obj.writeSize == 1 )
        {
            // Single Byte write only
            if( twihs2Obj.readSize == 0 )
            {
                // Load last byte in transmit register, issue stop condition
                // Generate TXCOMP interrupt after STOP condition has been sent
                twihs2Obj.state = TWIHS_STATE_WAIT_FOR_TXCOMP;

                TWIHS2_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs2Obj.writeBuffer[twihs2Obj.writeCount++]);
                TWIHS2_REGS->TWIHS_CR = TWIHS_CR_STOP_Msk;
                TWIHS2_REGS->TWIHS_IER = TWIHS_IER_TXCOMP_Msk;
            }
            // Single Byte write and than read transfer
            else
            {
                // START bit must be set before the byte is shifted out. Hence disabled interrupt
                __disable_irq();

                TWIHS2_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs2Obj.writeBuffer[twihs2Obj.writeCount++]);

                // Wait for control byte to be transferred before initiating repeat start for read
                while((TWIHS2_REGS->TWIHS_SR & (TWIHS_SR_TXCOMP_Msk | TWIHS_SR_TXRDY_Msk)) != 0);

                while((TWIHS2_REGS->TWIHS_SR & (TWIHS_SR_TXRDY_Msk)) == 0);

                type = true;
            }
        }
        // Multi-Byte Write
        else
        {
            twihs2Obj.state = TWIHS_STATE_TRANSFER_WRITE;

            TWIHS2_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs2Obj.writeBuffer[twihs2Obj.writeCount++]);

            TWIHS2_REGS->TWIHS_IER = TWIHS_IDR_TXRDY_Msk | TWIHS_IER_TXCOMP_Msk;
        }
    }
    // Read transfer
    if(type)
    {
        TWIHS2_InitiateRead();
    }
}

void TWIHS2_CallbackRegister( TWIHS_CALLBACK callback, uintptr_t contextHandle )
{
    if (callback != NULL)
    {
        twihs2Obj.callback = callback;

        twihs2Obj.context = contextHandle;
    }
}

bool TWIHS2_IsBusy( void )
{
    // Check for ongoing transfer
    if( twihs2Obj.state == TWIHS_STATE_IDLE )
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool TWIHS2_Read( uint16_t address, uint8_t *pdata, size_t length )
{
    // Check for ongoing transfer
    if( twihs2Obj.state != TWIHS_STATE_IDLE )
    {
        return false;
    }

    twihs2Obj.address = address;
    twihs2Obj.readBuffer = pdata;
    twihs2Obj.readSize = length;
    twihs2Obj.writeBuffer = NULL;
    twihs2Obj.writeSize = 0;
    twihs2Obj.error = TWIHS_ERROR_NONE;

    TWIHS2_InitiateTransfer(address, true);

    return true;
}

bool TWIHS2_Write( uint16_t address, uint8_t *pdata, size_t length )
{
    // Check for ongoing transfer
    if( twihs2Obj.state != TWIHS_STATE_IDLE )
    {
        return false;
    }

    twihs2Obj.address = address;
    twihs2Obj.readBuffer = NULL;
    twihs2Obj.readSize = 0;
    twihs2Obj.writeBuffer = pdata;
    twihs2Obj.writeSize = length;
    twihs2Obj.error = TWIHS_ERROR_NONE;

    TWIHS2_InitiateTransfer(address, false);

    return true;
}

bool TWIHS2_WriteRead( uint16_t address, uint8_t *wdata, size_t wlength, uint8_t *rdata, size_t rlength )
{

    // Check for ongoing transfer
    if( twihs2Obj.state != TWIHS_STATE_IDLE )
    {
        return false;
    }

    twihs2Obj.address = address;
    twihs2Obj.readBuffer = rdata;
    twihs2Obj.readSize = rlength;
    twihs2Obj.writeBuffer = wdata;
    twihs2Obj.writeSize = wlength;
    twihs2Obj.error = TWIHS_ERROR_NONE;

    TWIHS2_InitiateTransfer(address, false);
    
//    while(twihs2Obj.state != TWIHS_STATE_IDLE);

    return true;
}

TWIHS_ERROR TWIHS2_ErrorGet( void )
{
    TWIHS_ERROR error = TWIHS_ERROR_NONE;

    error = twihs2Obj.error;

    twihs2Obj.error = TWIHS_ERROR_NONE;

    return error;
}

bool TWIHS2_TransferSetup( TWIHS_TRANSFER_SETUP* setup, uint32_t srcClkFreq )
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
    TWIHS2_REGS->TWIHS_CWGR = (TWIHS_CWGR_HOLD_Msk & TWIHS2_REGS->TWIHS_CWGR) | TWIHS_CWGR_CLDIV(cldiv) | TWIHS_CWGR_CHDIV(cldiv) | TWIHS_CWGR_CKDIV(ckdiv);

    return true;
}

void TWIHS2_InterruptHandler( void )
{
    uint32_t status;

    // Read the peripheral status
    status = TWIHS2_REGS->TWIHS_SR;

    /* checks if Slave has Nacked */
    if( status & TWIHS_SR_NACK_Msk )
    {
        twihs2Obj.state = TWIHS_STATE_ERROR;
        twihs2Obj.error = TWIHS_ERROR_NACK;
    }

    if( status & TWIHS_SR_TXCOMP_Msk )
    {
        /* Disable and Enable I2C Master */
        TWIHS2_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk;
        TWIHS2_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;

        /* Disable Interrupt */
        TWIHS2_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;
    }

    /* checks if the arbitration is lost in multi-master scenario */
    if( status & TWIHS_SR_ARBLST_Msk )
    {
        /* Re-initiate the transfer if arbitration is lost in
         * between of the transfer
         */
        twihs2Obj.state = TWIHS_STATE_ADDR_SEND;
    }

    if( twihs2Obj.error == TWIHS_ERROR_NONE)
    {
        switch( twihs2Obj.state )
        {
            case TWIHS_STATE_ADDR_SEND:
            {
                if (twihs2Obj.writeSize != 0 )
                {
                    // Initiate Write transfer
                    TWIHS2_InitiateTransfer(twihs2Obj.address, false);
                }
                else
                {
                    // Initiate Read transfer
                    TWIHS2_InitiateTransfer(twihs2Obj.address, true);
                }
            }
            break;

            case TWIHS_STATE_TRANSFER_WRITE:
            {
                /* checks if master is ready to transmit */
                if( status & TWIHS_SR_TXRDY_Msk )
                {
                    // Write Last Byte and then initiate read transfer
                    if( ( twihs2Obj.writeCount == (twihs2Obj.writeSize -1) ) && ( twihs2Obj.readSize != 0 ))
                    {
                        // START bit must be set before the last byte is shifted out to generate repeat start. Hence disabled interrupt
                        __disable_irq();
                        TWIHS2_REGS->TWIHS_IDR = TWIHS_IDR_TXRDY_Msk;
                        TWIHS2_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs2Obj.writeBuffer[twihs2Obj.writeCount++]);
                        TWIHS2_InitiateRead();
                    }
                    // Write Last byte and then issue STOP condition
                    else if ( twihs2Obj.writeCount == (twihs2Obj.writeSize -1))
                    {
                        // Load last byte in transmit register, issue stop condition
                        // Generate TXCOMP interrupt after STOP condition has been sent
                        TWIHS2_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs2Obj.writeBuffer[twihs2Obj.writeCount++]);
                        TWIHS2_REGS->TWIHS_CR = TWIHS_CR_STOP_Msk;
                        TWIHS2_REGS->TWIHS_IDR = TWIHS_IDR_TXRDY_Msk;

                        /* Check TXCOMP to confirm if STOP condition has been sent, otherwise wait for TXCOMP interrupt */
                        status = TWIHS2_REGS->TWIHS_SR;

                        if( status & TWIHS_SR_TXCOMP_Msk )
                        {
                            twihs2Obj.state = TWIHS_STATE_TRANSFER_DONE;
                        }
                        else
                        {
                            twihs2Obj.state = TWIHS_STATE_WAIT_FOR_TXCOMP;
                        }
                    }
                    // Write next byte
                    else
                    {
                        TWIHS2_REGS->TWIHS_THR = TWIHS_THR_TXDATA(twihs2Obj.writeBuffer[twihs2Obj.writeCount++]);
                    }

                    // Dummy read to ensure that TXRDY bit is cleared
                    status = TWIHS2_REGS->TWIHS_SR;
                }

                break;
            }

            case TWIHS_STATE_TRANSFER_READ:
            {
                /* checks if master has received the data */
                if( status & TWIHS_SR_RXRDY_Msk )
                {
                    // Set the STOP (or START) bit before reading the TWIHS_RHR on the next-to-last access
                    if(  twihs2Obj.readCount == (twihs2Obj.readSize - 2) )
                    {
                        TWIHS2_REGS->TWIHS_CR = TWIHS_CR_STOP_Msk;
                    }

                    /* read the received data */
                    twihs2Obj.readBuffer[twihs2Obj.readCount++] = (uint8_t)(TWIHS2_REGS->TWIHS_RHR & TWIHS_RHR_RXDATA_Msk);

                    /* checks if transmission has reached at the end */
                    if( twihs2Obj.readCount == twihs2Obj.readSize )
                    {
                        /* Disable the RXRDY interrupt*/
                        TWIHS2_REGS->TWIHS_IDR = TWIHS_IDR_RXRDY_Msk;

                        /* Check TXCOMP to confirm if STOP condition has been sent, otherwise wait for TXCOMP interrupt */
                        status = TWIHS2_REGS->TWIHS_SR;
                        if( status & TWIHS_SR_TXCOMP_Msk )
                        {
                            twihs2Obj.state = TWIHS_STATE_TRANSFER_DONE;
                        }
                        else
                        {
                            twihs2Obj.state = TWIHS_STATE_WAIT_FOR_TXCOMP;
                        }
                    }
                }
                break;
            }

            case TWIHS_STATE_WAIT_FOR_TXCOMP:
            {
                if( status & TWIHS_SR_TXCOMP_Msk )
                {
                    twihs2Obj.state = TWIHS_STATE_TRANSFER_DONE;
                }
                break;
            }

            default:
            {
                break;
            }
        }
    }
    if (twihs2Obj.state == TWIHS_STATE_ERROR)
    {
        // NACK is received,
        twihs2Obj.state = TWIHS_STATE_IDLE;
        TWIHS2_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;

        // Disable and Enable I2C Master
        TWIHS2_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk;
        TWIHS2_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;

        if ( twihs2Obj.callback != NULL )
        {
            twihs2Obj.callback( twihs2Obj.context );
        }
    }
    // check for completion of transfer
    if( twihs2Obj.state == TWIHS_STATE_TRANSFER_DONE )
    {
        twihs2Obj.error = TWIHS_ERROR_NONE;

        // Reset the PLib objects and Interrupts
        twihs2Obj.state = TWIHS_STATE_IDLE;
        TWIHS2_REGS->TWIHS_IDR = TWIHS_IDR_TXCOMP_Msk | TWIHS_IDR_TXRDY_Msk | TWIHS_IDR_RXRDY_Msk;

        // Disable and Enable I2C Master
        TWIHS2_REGS->TWIHS_CR = TWIHS_CR_MSDIS_Msk;
        TWIHS2_REGS->TWIHS_CR = TWIHS_CR_MSEN_Msk;

        if ( twihs2Obj.callback != NULL )
        {
            twihs2Obj.callback( twihs2Obj.context );
        }
    }

    return;
}


//------------------------------------------------------------------------------------------------------
//mod I2C functions

void i2c_master_read(uint8_t slave_addr, uint8_t *data, uint32_t length)
{
	uint32_t status, cnt = length;
//	uint8_t *buffer = p_packet->buffer;
    
    while( twihs2Obj.state != TWIHS_STATE_IDLE );
//    {
//        return false;
//    }

	/* Check argument */
//	if (cnt == 0) {
//		//return TWIHS_INVALID_ARGUMENT;
//	}

	/* Set read mode, slave address and 3 internal address byte lengths */
	TWIHS2_REGS->TWIHS_MMR = 0;
	TWIHS2_REGS->TWIHS_MMR = TWIHS_MMR_MREAD(1) | TWIHS_MMR_DADR(slave_addr); //|
			//((p_packet->addr_length << TWIHS_MMR_IADRSZ_Pos) &
			//TWIHS_MMR_IADRSZ_Msk);

//	/* Set internal address for remote chip */
//	TWIHS2_REGS->TWIHS_IADR = 0;
//	TWIHS2_REGS->TWIHS_IADR = twihs_mk_addr(p_packet->addr, p_packet->addr_length);

	/* Send a START Condition */
	TWIHS2_REGS->TWIHS_CR = TWIHS_CR_START(1);

	while (cnt > 0) {
		status = TWIHS2_REGS->TWIHS_SR;
		if (status & TWIHS_SR_NACK(1)) {
			//return TWIHS_RECEIVE_NACK;
		}
		//if (!timeout--) {
			////return TWIHS_ERROR_TIMEOUT;
		//}
		if (!(status & TWIHS_SR_RXRDY(1))) {
			continue;
		}
		/* Last byte ? */
		if (cnt == 1) {
			TWIHS2_REGS->TWIHS_CR = TWIHS_CR_STOP(1);
		}

		*data++ = TWIHS2_REGS->TWIHS_RHR;

		cnt--;
		//timeout = TWIHS_TIMEOUT;
	}

	while (!(TWIHS2_REGS->TWIHS_SR & TWIHS_SR_TXCOMP(1))) {
	}

	TWIHS2_REGS->TWIHS_SR;

	//return TWIHS_SUCCESS;
}

void i2c_master_write(uint8_t slave_addr, uint8_t *data, uint32_t length)
{
	uint32_t status, cnt = length;
//	uint8_t *buffer = p_packet.buffer;
    
    while( twihs2Obj.state != TWIHS_STATE_IDLE );

	/* Set write mode, slave address and 3 internal address byte lengths */
	TWIHS2_REGS->TWIHS_MMR = 0;
	TWIHS2_REGS->TWIHS_MMR = TWIHS_MMR_DADR(slave_addr);// |
		//	((p_packet->addr_length << TWIHS_MMR_IADRSZ_Pos) &
			//TWIHS_MMR_IADRSZ_Msk);

	/* Set internal address for remote chip */
//	TWIHS2_REGS->TWIHS_IADR = 0;
//	TWIHS2_REGS->TWIHS_IADR = twihs_mk_addr(p_packet.addr, p_packet.addr_length);

	/* Send all bytes */
	while (cnt > 0) {
		status = TWIHS2_REGS->TWIHS_SR;
		if (status & TWIHS_SR_NACK(1)) {
		}

		if (!(status & TWIHS_SR_TXRDY(1))) {
			continue;
		}
		TWIHS2_REGS->TWIHS_THR = *data++;

		cnt--;
	}

	while (1) {
		status = TWIHS2_REGS->TWIHS_SR;
		if (status & TWIHS_SR_NACK(1)) {
		}

		if (status & TWIHS_SR_TXRDY(1)) {
			break;
		}
	}

	TWIHS2_REGS->TWIHS_CR = TWIHS_CR_STOP(1);

	while (!(TWIHS2_REGS->TWIHS_SR & TWIHS_SR_TXCOMP(1))) {
	}
}


void Twihs2_BusywaitandRecover(void)
{
    Twihswait_Timer = 0;
    
    //busywait
    while(TWIHS2_IsBusy())
    {
        if(Twihswait_Timer>=100) //>100
        {
            //recover
            
            //Disable TWIHS2 peripheral clock
            PMC_REGS->PMC_PCDR1 |= (1 << (ID_TWIHS2-32));
    
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
            
//            Enable TWIHS2 peripheral clock
            PMC_REGS->PMC_PCER1 |= (1 << (ID_TWIHS2-32));

            TWIHS2_Initialize();
            Twihswait_Timer = 0;
        }
    }   
}

void Twihs2_Recover(void)
{
//    Twihswait_Timer = 0;
    
    //busywait
//    while(TWIHS2_IsBusy())
//    {
//        if(Twihswait_Timer>=1) //>100
//        {
            //recover
            
            //Disable TWIHS2 peripheral clock
//            PMC_REGS->PMC_PCDR1 |= (1 << (ID_TWIHS2-32));
    
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
            
            PMC_REGS->PMC_PCDR1 |= (1 << (ID_TWIHS2-32));
            
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
            
            
            //Ensable TWIHS2 peripheral clock
            PMC_REGS->PMC_PCER1 |= (1 << (ID_TWIHS2-32));

            TWIHS2_Initialize();
//        }
//    }
    
//    Twihswait_Timer = 0;
}