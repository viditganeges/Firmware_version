/*******************************************************************************
  QSPI Peripheral Library Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_qspi_spi.c

  Summary
    QSPI peripheral library interface when in SPI mode.

  Description

  Remarks:

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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

#include "plib_qspi_spi.h"
#include "string.h" // memmove
#include "interrupts.h"

qspi_spi_obj qspiObj;


void QSPI_Initialize(void)
{
    // Reset and Disable the qspi Module
    QSPI_REGS->QSPI_CR = QSPI_CR_SWRST_Msk | QSPI_CR_QSPIDIS_Msk;

    while(QSPI_REGS->QSPI_SR& QSPI_SR_QSPIENS_Msk){
        ;   // spin lock
    }

    /* DLYCS  = 0x0 */
    /* DLYBCT = 0x0 */
    /* NBBITS = 0x0 */
    /* LLB    = 0 */
    /* CSMODE = 0x0 */
    /* WDRBT  = 0 */
    /* SMM    = SPI */
    QSPI_REGS->QSPI_MR = ( QSPI_MR_SMM_SPI | QSPI_MR_NBBITS(0x0) | QSPI_MR_LLB(0));

    /* CPOL = 0 */
    /* CPHA = 0 */
    /* SCBR = 149 */
    /* DLYBS = 0 */
    QSPI_REGS->QSPI_SCR = (QSPI_SCR_SCBR(149))  ;

    // Enable the qspi Module
    QSPI_REGS->QSPI_CR = QSPI_CR_QSPIEN_Msk;

    while(!(QSPI_REGS->QSPI_SR& QSPI_SR_QSPIENS_Msk)){
        ;   // spin lock
    }
}

bool QSPI_WriteRead (void* pTransmitData, size_t txSize, void* pReceiveData, size_t rxSize)
{
    bool isRequestAccepted = false;
    uint32_t dummyData;

    /* Verify the request */
    if((((txSize > 0) && (pTransmitData != NULL)) || ((rxSize > 0) && (pReceiveData != NULL))) && (qspiObj.transferIsBusy == false))
    {
        isRequestAccepted = true;
        qspiObj.txBuffer = pTransmitData;
        qspiObj.rxBuffer = pReceiveData;
        qspiObj.rxCount = 0;
        qspiObj.txCount = 0;
        qspiObj.dummySize = 0;
        if (pTransmitData != NULL)
        {
            qspiObj.txSize = txSize;
        }
        else
        {
            qspiObj.txSize = 0;
        }

        if (pReceiveData != NULL)
        {
            qspiObj.rxSize = rxSize;
        }
        else
        {
            qspiObj.rxSize = 0;
        }

        qspiObj.transferIsBusy = true;

        /* Flush out any unread data in SPI read buffer */
        dummyData = (QSPI_REGS->QSPI_RDR & QSPI_RDR_RD_Msk) >> QSPI_RDR_RD_Pos;
        (void)dummyData;

        if (qspiObj.rxSize > qspiObj.txSize)
        {
            qspiObj.dummySize = qspiObj.rxSize - qspiObj.txSize;
        }

        /* Start the first write here itself, rest will happen in ISR context */
        if((QSPI_REGS->QSPI_MR & QSPI_MR_NBBITS_Msk) == QSPI_MR_NBBITS_8_BIT)
        {
            if (qspiObj.txCount < qspiObj.txSize)
            {
                QSPI_REGS->QSPI_TDR = *((uint8_t*)qspiObj.txBuffer);
                qspiObj.txCount++;
            }
            else if (qspiObj.dummySize > 0)
            {
                QSPI_REGS->QSPI_TDR = (uint8_t)(0xff);
                qspiObj.dummySize--;
            }
        }
        else if((QSPI_REGS->QSPI_MR & QSPI_MR_NBBITS_Msk) == QSPI_MR_NBBITS_16_BIT)
        {
            qspiObj.txSize >>= 1;     /* divide by 2 since dealing with 2-byte quantities here */
            qspiObj.dummySize >>= 1;
            qspiObj.rxSize >>= 1;

            if (qspiObj.txCount < qspiObj.txSize)
            {
                QSPI_REGS->QSPI_TDR = *((uint16_t*)qspiObj.txBuffer);
                qspiObj.txCount++;
            }
            else if (qspiObj.dummySize > 0)
            {
                QSPI_REGS->QSPI_TDR = (uint16_t)(0xff);
                qspiObj.dummySize--;
            }
        }

        if ((int)rxSize > 0)
        {
            /* Enable receive interrupt to complete the transfer in ISR context */
            QSPI_REGS->QSPI_IER = QSPI_IER_RDRF_Msk;
        }
        else
        {
            /* Enable transmit interrupt to complete the transfer in ISR context */
            QSPI_REGS->QSPI_IER = QSPI_IER_TDRE_Msk;
        }
    }

    return isRequestAccepted;
}

bool QSPI_Write(void* pTransmitData, size_t txSize)
{
    return(QSPI_WriteRead(pTransmitData, txSize, NULL, 0));
}

bool QSPI_Read(void* pReceiveData, size_t rxSize)
{
    return(QSPI_WriteRead(NULL, 0, pReceiveData, rxSize));
}

bool QSPI_TransferSetup (QSPI_TRANSFER_SETUP * setup, uint32_t spiSourceClock )
{
    uint32_t scbr;
    if ((setup == NULL) || (setup->clockFrequency == 0))
    {
        return false;
    }
    if(spiSourceClock == 0)
    {
        // Fetch Master Clock Frequency directly
        spiSourceClock = 1000000;
    }

    scbr = spiSourceClock/setup->clockFrequency;

    if(scbr > 255)
    {
        scbr = 255;
    }

    QSPI_REGS->QSPI_SCR= (uint32_t)setup->clockPolarity | (uint32_t)setup->clockPhase | QSPI_SCR_SCBR(scbr);

    QSPI_REGS->QSPI_MR = (QSPI_REGS->QSPI_MR & ~QSPI_MR_NBBITS_Msk) | (uint32_t)setup->dataBits;

    return true;
}

void QSPI_CallbackRegister (QSPI_CALLBACK callback, uintptr_t context)
{
    qspiObj.callback = callback;
    qspiObj.context = context;
}

bool QSPI_IsBusy(void)
{
    return ((qspiObj.transferIsBusy) || ((QSPI_REGS->QSPI_SR & QSPI_SR_TXEMPTY_Msk ) == 0));
}

void QSPI_InterruptHandler(void)
{
    uint32_t dataBits ;
    uint32_t receivedData;
    static bool isLastByteTransferInProgress = false;

    dataBits = QSPI_REGS->QSPI_MR & QSPI_MR_NBBITS_Msk;

    /* See if there's received data (MOSI) to be processed */
    if ((QSPI_REGS->QSPI_SR & QSPI_SR_RDRF_Msk ) == QSPI_SR_RDRF_Msk)
    {
        receivedData = (QSPI_REGS->QSPI_RDR & QSPI_RDR_RD_Msk) >> QSPI_RDR_RD_Pos;

        if (qspiObj.rxCount < qspiObj.rxSize)
        {
            if(dataBits == QSPI_MR_NBBITS_8_BIT)
            {
                ((uint8_t*)qspiObj.rxBuffer)[qspiObj.rxCount++] = receivedData;
            }
            else
            {
                ((uint16_t*)qspiObj.rxBuffer)[qspiObj.rxCount++] = receivedData;
            }
        }
    }

    /* If there are more words to be transmitted, then transmit them here and keep track of the count */
    if((QSPI_REGS->QSPI_SR & QSPI_SR_TDRE_Msk) == QSPI_SR_TDRE_Msk)
    {
        /* Disable the TDRE interrupt. This will be enabled back if more than
         * one byte is pending to be transmitted */
        QSPI_REGS->QSPI_IDR = QSPI_IDR_TDRE_Msk;

        if(dataBits == QSPI_MR_NBBITS_8_BIT)
        {
            if (qspiObj.txCount < qspiObj.txSize)
            {
                QSPI_REGS->QSPI_TDR = ((uint8_t*)qspiObj.txBuffer)[qspiObj.txCount++];
            }
            else if (qspiObj.dummySize > 0)
            {
                QSPI_REGS->QSPI_TDR = (uint8_t)(0xff);
                qspiObj.dummySize--;
            }
        }
        else
        {
            if (qspiObj.txCount < qspiObj.txSize)
            {
                QSPI_REGS->QSPI_TDR = ((uint16_t*)qspiObj.txBuffer)[qspiObj.txCount++];
            }
            else if (qspiObj.dummySize > 0)
            {
                QSPI_REGS->QSPI_TDR = (uint16_t)(0xff);
                qspiObj.dummySize--;
            }
        }
        if ((qspiObj.txCount == qspiObj.txSize) && (qspiObj.dummySize == 0))
        {
            /* At higher baud rates, the data in the shift register can be
             * shifted out and TXEMPTY flag can get set resulting in a
             * callback been given to the application with the SPI interrupt
             * pending with the application. This will then result in the
             * interrupt handler being called again with nothing to transmit.
             * To avoid this, a software flag is set, but
             * the TXEMPTY interrupt is not enabled until the very end.
             */

            isLastByteTransferInProgress = true;
            /* Set Last transfer to deassert NPCS after the last byte written in TDR has been transferred. */
            QSPI_REGS->QSPI_CR = QSPI_CR_LASTXFER_Msk;
        }
        else if (qspiObj.rxCount == qspiObj.rxSize)
        {
            /* Enable TDRE interrupt as all the requested bytes are received
             * and can now make use of the internal transmit shift register.
             */
            QSPI_REGS->QSPI_IDR = QSPI_IDR_RDRF_Msk;
            QSPI_REGS->QSPI_IER = QSPI_IER_TDRE_Msk;
        }
    }

    /* See if Exchange is complete */
    if ((isLastByteTransferInProgress == true) && ((QSPI_REGS->QSPI_SR & QSPI_SR_TXEMPTY_Msk) == QSPI_SR_TXEMPTY_Msk))
    {
        if (qspiObj.rxCount == qspiObj.rxSize)
        {
            qspiObj.transferIsBusy = false;

            /* Disable TDRE, RDRF and TXEMPTY interrupts */
            QSPI_REGS->QSPI_IDR = QSPI_IDR_TDRE_Msk | QSPI_IDR_RDRF_Msk | QSPI_IDR_TXEMPTY_Msk;

            isLastByteTransferInProgress = false;

            if(qspiObj.callback != NULL)
            {
                qspiObj.callback(qspiObj.context);
            }
        }
    }
    if (isLastByteTransferInProgress == true)
    {
        /* For the last byte transfer, the TDRE interrupt is already disabled.
         * Enable TXEMPTY interrupt to ensure no data is present in the shift
         * register before application callback is called.
         */
        QSPI_REGS->QSPI_IER = QSPI_IER_TXEMPTY_Msk;
    }

}
/*******************************************************************************
 End of File
*/