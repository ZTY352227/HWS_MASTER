/*!
    \file    main.c
    \brief   USART transmit and receive interrupt

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

#include "gd32e50x.h"
#include "gd32e503v_eval.h"
#include "systick.h"
#include "uart.h"
#include <stdio.h>
#include "invertor.h"
#include "gd32e50x_it.h"
#include "bms.h"
#include "timer.h"
#include "key.h"
#include "gpio.h"
#include "pwm.h"
#include "ht1621b.h"
#include "buzzer.h"
#include "systick.h"




/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/


int main(void)
{
    uint16_t led_status = 0;
    systick_config();
    nvic_priority_group_set(NVIC_PRIGROUP_PRE3_SUB1);

    timer_config();	//定时器初始化
    uart_init();	//uart初始化
    key_init();	//按键初始化
    user_gpio_init();	//控制12V\AC\USB输输出  gpio初始化
    buzzer_gpio_init(); //蜂鸣器初始化
    pwm_init();	//风扇、升压MOS PWM初始化

    nvic_irq_enable(USART1_IRQn, 3, 0);
    nvic_irq_enable(USART5_IRQn, 3, 0);

    nvic_irq_enable(TIMER2_IRQn, 0, 1);
    ht1621b_init(); //上电初始化LCD

    while(1)
    {
        AC_Handel_Modbus();	//处理AC通讯事件
        BMS_Handel_Modbus();	//处理BMS通讯事件
        KEY_Handel();	//处理按键事件
        if (Timer2_500msFlag == 1)
        {
            Timer2_500msFlag = 0;
            AC_Diag();	//读取AC信息
            BMS_Diag();	//读取BMS信息
        }
		
		
// continue;
        if(led_status)
        {
			Ht1621WrOneNum(0x00,0xff);
//            Ht1621WrOneNum(0x00,0xf);
//            Ht1621WrOneNum(0x01,0xf);
//            Ht1621WrOneNum(0x02,0xf);
//            Ht1621WrOneNum(0x03,0xf);
//            Ht1621WrOneNum(0x04,0xf);
//            Ht1621WrOneNum(0x05,0xf);
//            Ht1621WrOneNum(0x06,0xf);
//            Ht1621WrOneNum(0x07,0xf);
//            Ht1621WrOneNum(0x08,0xf);
//            Ht1621WrOneNum(0x09,0xf);
//            Ht1621WrOneNum(0x0A,0xf);
//            Ht1621WrOneNum(0x0B,0xf);
//            Ht1621WrOneNum(0x0C,0xf);
//            Ht1621WrOneNum(0x0D,0xf);
//            Ht1621WrOneNum(0x0e,0xf);
//            Ht1621WrOneNum(0x0f,0xf);
//            Ht1621WrOneNum(0x10,0xf);
//            Ht1621WrOneNum(0x11,0xf);
//            Ht1621WrOneNum(0x12,0xf);
//            Ht1621WrOneNum(0x13,0xf);
//            Ht1621WrOneNum(0x14,0xf);
//            Ht1621WrOneNum(0x15,0xf);
//            Ht1621WrOneNum(0x16,0xf);
//            Ht1621WrOneNum(0x17,0xf);
//            Ht1621WrOneNum(0x18,0xf);
//            Ht1621WrOneNum(0x19,0xf);
//            Ht1621WrOneNum(0x1a,0xf);
//            Ht1621WrOneNum(0x1b,0xf);
//            Ht1621WrOneNum(0x1c,0xf);
//            Ht1621WrOneNum(0x1d,0xf);
//            Ht1621WrOneNum(0x1e,0xf);
//            Ht1621WrOneNum(0x1f,0xf);
            DelayMS(1000);

            led_status = 0;
        }
        else
        {
			Ht1621WrOneNum(0x00,0x00);
//            Ht1621WrOneNum(0x00,0x0);
//            Ht1621WrOneNum(0x01,0x0);
//            Ht1621WrOneNum(0x02,0x0);
//            Ht1621WrOneNum(0x03,0x0);
//            Ht1621WrOneNum(0x04,0x0);
//            Ht1621WrOneNum(0x05,0x0);
//            Ht1621WrOneNum(0x06,0x0);
//            Ht1621WrOneNum(0x07,0x0);
//            Ht1621WrOneNum(0x08,0x0);
//            Ht1621WrOneNum(0x09,0x0);
//            Ht1621WrOneNum(0x0A,0x0);
//            Ht1621WrOneNum(0x0B,0x0);
//            Ht1621WrOneNum(0x0C,0x0);
//            Ht1621WrOneNum(0x0D,0x0);
//            Ht1621WrOneNum(0x0e,0x0);
//            Ht1621WrOneNum(0x0f,0x00);
//            Ht1621WrOneNum(0x10,0x00);
//            Ht1621WrOneNum(0x11,0x00);
//            Ht1621WrOneNum(0x12,0x00);
//            Ht1621WrOneNum(0x13,0x00);
//            Ht1621WrOneNum(0x14,0x00);
//            Ht1621WrOneNum(0x15,0x00);
//            Ht1621WrOneNum(0x16,0x00);
//            Ht1621WrOneNum(0x17,0x00);
//            Ht1621WrOneNum(0x18,0x00);
//            Ht1621WrOneNum(0x19,0x00);
//            Ht1621WrOneNum(0x1a,0x00);
//            Ht1621WrOneNum(0x1b,0x00);
//            Ht1621WrOneNum(0x1c,0x00);
//            Ht1621WrOneNum(0x1d,0x00);
//            Ht1621WrOneNum(0x1e,0x00);
//            Ht1621WrOneNum(0x1f,0x00);
            DelayMS(1000);

            led_status = 1;
        }
    }
}




