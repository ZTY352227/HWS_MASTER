/*!
    \file    gd32e50x_it.c
    \brief   interrupt service routines

    \version 2020-09-04, V1.0.0, demo for GD32E50x
    \version 2021-03-31, V1.1.0, demo for GD32E50x
*/

/*
    Copyright (c) 2021, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32e50x_it.h"
#include "gd32e503v_eval.h"
#include "systick.h"
#include "invertor.h"
#include <string.h>
#include "bms.h"

extern uint8_t  tx_buffer[];
extern uint8_t  rx_buffer[];
extern uint32_t rx_buffer_size, tx_buffer_size;
extern uint16_t tx_count, rx_count;

uint8_t           ac_uart_recv[AC_BUFFER_LEN] = { 0x00 };
uint16_t          ac_uart_cnt     = 0;
volatile uint16_t ac_uart_timeout = 0;

uint8_t           bms_uart_recv[BMS_BUFFER_LEN] = { 0x00 };
uint16_t          bms_uart_cnt     = 0;
volatile uint16_t bms_uart_timeout = 0;


volatile uint8_t Timer2_500msCounter = 0;
volatile uint8_t Timer2_1sCounter    = 0;
volatile uint8_t Timer2_500msFlag    = 0;
volatile uint8_t Timer2_1sFlag       = 0;
volatile uint8_t Timer2_10msCounter  = 0;
volatile uint8_t Timer2_10msFlag     = 0;


volatile uint32_t u32Tim500msCnt = 0;


/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
    delay_decrement();
}

/*!
    \brief      this function handles USART RBNE interrupt request and TBE interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/




//AC re
void USART1_IRQHandler(void)
{
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_RBNE))
    {
        /* receive data */
        ac_uart_recv[ac_uart_cnt++] = usart_data_receive(USART1);
        ac_uart_timeout          = 1;

        if (ac_uart_cnt >= AC_BUFFER_LEN)         //越界处理
        {
            ac_uart_cnt     = 0;
            ac_uart_timeout = 0;
        }
		usart_interrupt_flag_clear(USART1, USART_INT_FLAG_RBNE);
    }
	
}

//BMS re
void USART5_IRQHandler(void)
{
    if(RESET != usart5_interrupt_flag_get(USART5, USART5_INT_FLAG_RBNE))
    {
        /* receive data */
        bms_uart_recv[bms_uart_cnt++] = usart_data_receive(USART5);
        bms_uart_timeout          = 1;

        if (bms_uart_cnt >= BMS_BUFFER_LEN)         //越界处理
        {
            bms_uart_cnt     = 0;
            bms_uart_timeout = 0;
        }
		usart5_interrupt_flag_clear(USART5, USART5_INT_FLAG_RBNE);
    }
}

void TIMER2_IRQHandler(void)
{
    if(SET == timer_interrupt_flag_get(TIMER2, TIMER_INT_UP))
    {
        if ((ac_uart_timeout > 0) && (ac_uart_timeout < 1000))//485开始接收后计时
        {
            ac_uart_timeout++;
        }

        if (ac_uart_timeout >= AC_MODBUS_TIMEOUT)
        {
            ac_uart_timeout = 0;
            memcpy(AC_Modbus_Recv, ac_uart_recv, ac_uart_cnt);
            AC_RecvOK   = 1;
            AC_RecvLen  = ac_uart_cnt;
            ac_uart_cnt = 0;
        }

        if ((bms_uart_timeout > 0) && (bms_uart_timeout < 1000))//485开始接收后计时
        {
            bms_uart_timeout++;
        }

        if (bms_uart_timeout >= BMS_MODBUS_TIMEOUT)
        {
            bms_uart_timeout = 0;
            memcpy(BMS_Modbus_Recv, bms_uart_recv, bms_uart_cnt);
            BMS_RecvOK   = 1;
            BMS_RecvLen  = bms_uart_cnt;
            bms_uart_cnt = 0;
        }

        Timer2_10msCounter++;
        if (Timer2_10msCounter >= 10)
        {
            Timer2_500msCounter++;
            Timer2_10msFlag    = 1;
            Timer2_10msCounter = 0;
        }

        if (Timer2_500msCounter >= 50)
        {
            Timer2_500msCounter = 0;
            Timer2_500msFlag    = 1;
            Timer2_1sCounter++;
            u32Tim500msCnt++;
        }

        if (Timer2_1sCounter >= 2)
        {
            Timer2_1sCounter = 0;
            Timer2_1sFlag    = 1;
        }
        /* clear channel 0 interrupt bit */
        timer_interrupt_flag_clear(TIMER2, TIMER_INT_UP);
    }
}
