/*******************************************************************************
  RTC Peripheral Library

  Company:
    Microchip Technology Inc.

  File Name:
    plib_rtc.c

  Summary:
    RTC Source File

  Description:
    None

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

#include "device.h"
#include "plib_rtc.h"
#include "interrupts.h"


__STATIC_INLINE uint32_t decimaltobcd( uint32_t aDecValue )
{
    uint32_t  decValueDiv10 = aDecValue / 10;

    return( (decValueDiv10 << 4) + ( aDecValue - (decValueDiv10 * 10) ) );
}

__STATIC_INLINE uint32_t bcdtodecimal( uint32_t aBcdValue )
{
    return( (10 * ((aBcdValue & 0xF0) >> 4)) + (aBcdValue & 0x0F) );
}

void RTC_Initialize( void )
{
    RTC_REGS->RTC_MR = RTC_MR_PERSIAN( 0 ) | RTC_MR_OUT1_NO_WAVE | RTC_MR_OUT0_NO_WAVE | RTC_MR_TPERIOD_P_1S | RTC_MR_THIGH_H_31MS | RTC_MR_HRMOD(0);

    RTC_REGS->RTC_CR = RTC_CR_TIMEVSEL_MINUTE | RTC_CR_CALEVSEL_WEEK;

    // disable all interrupts
    RTC_REGS->RTC_IDR = RTC_IDR_Msk;

    // clear any stale interrupts
    RTC_REGS->RTC_SCCR = RTC_SCCR_Msk;
}

bool RTC_TimeSet( struct tm * Time )
{
    bool retval = true;
//    Time->tm_year += 1900;
    uint32_t data_cal = (decimaltobcd( Time->tm_mday ) << RTC_CALR_DATE_Pos)
                        | (decimaltobcd( Time->tm_wday + 1 ) << RTC_CALR_DAY_Pos)
                        | (decimaltobcd( Time->tm_mon + 1 ) << RTC_CALR_MONTH_Pos)
                        | (decimaltobcd( Time->tm_year - ( (Time->tm_year/100) * 100 ) ) << RTC_CALR_YEAR_Pos)
                        | (decimaltobcd( Time->tm_year/100 ) << RTC_CALR_CENT_Pos);

    uint32_t data_time = (decimaltobcd( Time->tm_hour ) << RTC_TIMR_HOUR_Pos )
                        | (decimaltobcd( Time->tm_min ) << RTC_TIMR_MIN_Pos)
                        | (decimaltobcd( Time->tm_sec ) << RTC_TIMR_SEC_Pos);

    RTC_REGS->RTC_CR &= ~(RTC_CR_UPDCAL_Msk | RTC_CR_UPDTIM_Msk);

    RTC_REGS->RTC_SCCR = RTC_SCCR_CALCLR_Msk;

    while( (RTC_REGS->RTC_SR & RTC_SR_SEC_Msk) != RTC_SR_SEC_Msk )
    {
        ;   // spin lock
    }

    // request RTC Configuration
    RTC_REGS->RTC_CR |= RTC_CR_UPDCAL_Msk | RTC_CR_UPDTIM_Msk;

    // Wait for ack
    while( !(RTC_REGS->RTC_SR & RTC_SR_ACKUPD_Msk) )
    {
        ;   // spin lock
    }

    // clear ack flag
    RTC_REGS->RTC_SCCR |= RTC_SCCR_ACKCLR_Msk | RTC_SCCR_ALRCLR_Msk | RTC_SCCR_TIMCLR_Msk;
    RTC_REGS->RTC_CALR = data_cal;
    RTC_REGS->RTC_TIMR = data_time;
    RTC_REGS->RTC_CR &= ~(RTC_CR_UPDCAL_Msk | RTC_CR_UPDTIM_Msk);

    if( RTC_REGS->RTC_VER & (RTC_VER_NVTIM_Msk | RTC_VER_NVCAL_Msk) )
    {
        retval = false;     // valid entry register indicates a problem
    }

    return retval;
}

void RTC_TimeGet( struct tm * Time )
{
    // two sequential read should be the same to insure syncrhonization
    uint32_t data_time = RTC_REGS->RTC_TIMR;

    while( data_time != RTC_REGS->RTC_TIMR )
    {
        data_time = RTC_REGS->RTC_TIMR;
    }

    // two sequential read should be the same to insure synchronization
    uint32_t data_cal = RTC_REGS->RTC_CALR;

    while( data_cal != RTC_REGS->RTC_CALR )
    {
        data_cal = RTC_REGS->RTC_CALR;
    }

    Time->tm_hour = bcdtodecimal( (data_time & RTC_TIMR_HOUR_Msk) >> RTC_TIMR_HOUR_Pos );
    Time->tm_sec =  bcdtodecimal( data_time & RTC_TIMR_SEC_Msk );
    Time->tm_min =  bcdtodecimal( (data_time & RTC_TIMR_MIN_Msk) >> RTC_TIMR_MIN_Pos );
    Time->tm_mday = bcdtodecimal( (data_cal & RTC_CALR_DATE_Msk) >> RTC_CALR_DATE_Pos );
    Time->tm_wday = bcdtodecimal( (data_cal & RTC_CALR_DAY_Msk) >> RTC_CALR_DAY_Pos ) - 1;
    Time->tm_mon =  bcdtodecimal( (data_cal & RTC_CALR_MONTH_Msk) >> RTC_CALR_MONTH_Pos ) - 1;
    Time->tm_year = ( (100 * bcdtodecimal( data_cal & RTC_CALR_CENT_Msk ))
                        + bcdtodecimal( (data_cal & RTC_CALR_YEAR_Msk) >> RTC_CALR_YEAR_Pos )
                    ); //- 1900;
}


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SECONDS_IN_A_DAY (86400U)
#define SECONDS_IN_A_HOUR (3600U)
#define SECONDS_IN_A_MINUTE (60U)
#define DAYS_IN_A_YEAR (365U)
#define YEAR_RANGE_START (1970U)
#define YEAR_RANGE_END (2099U)

uint32_t RTC_GetStatusFlags(void)
{
    uint32_t tmp32 = 0U;
    tmp32 = RTC_REGS->RTC_SR;
    return tmp32;
}

bool RTC_CheckDatetimeFormat(const struct tm *datetime)      /* Ensemble Removed Static */
{
//    assert(datetime);

    /* Table of days in a month for a non leap year. First entry in the table is not used,
     * valid months start from 1
     */
    uint8_t daysPerMonth[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

    /* Check year, month, hour, minute, seconds */
    if ((datetime->tm_year < YEAR_RANGE_START) || (datetime->tm_year > YEAR_RANGE_END) || (datetime->tm_mon > 12U) ||
        (datetime->tm_mon < 1U) || (datetime->tm_hour >= 24U) || (datetime->tm_min >= 60U) || (datetime->tm_sec >= 60U))
    {
        /* If not correct then error*/
        return false;
    }

    /* Adjust the days in February for a leap year */
    if ((((datetime->tm_year & 3U) == 0) && (datetime->tm_year % 100 != 0)) || (datetime->tm_year % 400 == 0))
    {
        daysPerMonth[2] = 29U;
    }

    /* Check the validity of the day */
    if ((datetime->tm_mday > daysPerMonth[datetime->tm_mon]) || (datetime->tm_mday < 1U))
    {
        return false;
    }

    return true;
}

uint32_t RTC_ConvertDatetimeToSeconds(const struct tm *datetime)     /* Ensemble Removed Static */
{
//    assert(datetime);

    /* Number of days from begin of the non Leap-year*/
    /* Number of days from begin of the non Leap-year*/
    uint16_t monthDays[] = {0U, 0U, 31U, 59U, 90U, 120U, 151U, 181U, 212U, 243U, 273U, 304U, 334U};
    uint32_t seconds;

    /* Compute number of days from 1970 till given year*/
    seconds = (datetime->tm_year - 1970U) * DAYS_IN_A_YEAR;
    /* Add leap year days */
    seconds += ((datetime->tm_year / 4) - (1970U / 4));
    /* Add number of days till given month*/
    seconds += monthDays[datetime->tm_mon];
    /* Add days in given month. We subtract the current day as it is
     * represented in the hours, minutes and seconds field*/
    seconds += (datetime->tm_mday - 1);
    /* For leap year if month less than or equal to February, decrement day counter*/
    if ((!(datetime->tm_year & 3U)) && (datetime->tm_mon <= 2U))
    {
        seconds--;
    }

    seconds = (seconds * SECONDS_IN_A_DAY) + (datetime->tm_hour * SECONDS_IN_A_HOUR) +
              (datetime->tm_min * SECONDS_IN_A_MINUTE) + datetime->tm_sec;

    return seconds;
}

void RTC_ConvertSecondsToDatetime(uint32_t seconds, struct tm *datetime)     /* Ensemble Removed Static */
{
//    assert(datetime);

    uint32_t x;
    uint32_t secondsRemaining, days;
    uint16_t daysInYear;
    /* Table of days in a month for a non leap year. First entry in the table is not used,
     * valid months start from 1
     */
    uint8_t daysPerMonth[] = {0U, 31U, 28U, 31U, 30U, 31U, 30U, 31U, 31U, 30U, 31U, 30U, 31U};

    /* Start with the seconds value that is passed in to be converted to date time format */
    secondsRemaining = seconds;

    /* Calculate the number of days, we add 1 for the current day which is represented in the
     * hours and seconds field
     */
    days = secondsRemaining / SECONDS_IN_A_DAY + 1;

    /* Update seconds left*/
    secondsRemaining = secondsRemaining % SECONDS_IN_A_DAY;

    /* Calculate the datetime hour, minute and second fields */
    datetime->tm_hour = secondsRemaining / SECONDS_IN_A_HOUR;
    secondsRemaining = secondsRemaining % SECONDS_IN_A_HOUR;
    datetime->tm_min = secondsRemaining / 60U;
    datetime->tm_sec = secondsRemaining % SECONDS_IN_A_MINUTE;

    /* Calculate year */
    daysInYear = DAYS_IN_A_YEAR;
    datetime->tm_year = YEAR_RANGE_START;
    while (days > daysInYear)
    {
        /* Decrease day count by a year and increment year by 1 */
        days -= daysInYear;
        datetime->tm_year++;

        /* Adjust the number of days for a leap year */
        if (datetime->tm_year & 3U)
        {
            daysInYear = DAYS_IN_A_YEAR;
        }
        else
        {
            daysInYear = DAYS_IN_A_YEAR + 1;
        }
    }

    /* Adjust the days in February for a leap year */
    if (!(datetime->tm_year & 3U))
    {
        daysPerMonth[2] = 29U;
    }

    for (x = 1U; x <= 12U; x++)
    {
        if (days <= daysPerMonth[x])
        {
            datetime->tm_mon = x;
            break;
        }
        else
        {
            days -= daysPerMonth[x];
        }
    }

    datetime->tm_mday = days;
}
