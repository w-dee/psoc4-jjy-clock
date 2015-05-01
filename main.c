/******************************************************************************
* Project Name		: Bootloadable Blinking LED
* File Name			: main.c
* Version 			: 1.0
* Device Used		: CY8C4245AXI-483
* Software Used		: PSoC Creator 3.0 Service Pack 2
* Compiler    		: ARMGCC 4.7.3, ARM RVDS Generic, ARM MDK Generic
* Related Hardware	: CY8CKIT-049-42xx PSoC 4 Prototyping Kit 
*
********************************************************************************
* Copyright (2014), Cypress Semiconductor Corporation. All Rights Reserved.
********************************************************************************
* This software is owned by Cypress Semiconductor Corporation (Cypress)
* and is protected by and subject to worldwide patent protection (United
* States and foreign), United States copyright laws and international treaty
* provisions. Cypress hereby grants to licensee a personal, non-exclusive,
* non-transferable license to copy, use, modify, create derivative works of,
* and compile the Cypress Source Code and derivative works for the sole
* purpose of creating custom software in support of licensee product to be
* used only in conjunction with a Cypress integrated circuit as specified in
* the applicable agreement. Any reproduction, modification, translation,
* compilation, or representation of this software except as specified above 
* is prohibited without the express written permission of Cypress.
*
* Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH 
* REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
* Cypress reserves the right to make changes without further notice to the 
* materials described herein. Cypress does not assume any liability arising out 
* of the application or use of any product or circuit described herein. Cypress 
* does not authorize its products for use as critical components in life-support 
* systems where a malfunction or failure may reasonably be expected to result in 
* significant injury to the user. The inclusion of Cypress' product in a life-
* support systems application implies that the manufacturer assumes all risk of 
* such use and in doing so indemnifies Cypress against all charges. 
*
* Use of this Software may be limited by and subject to the applicable Cypress
* software license agreement. 
*******************************************************************************/

/******************************************************************************
*                           THEORY OF OPERATION
* This is a blinking LED project. A PWM component drives the pin to blink the 
* LED at regular intervals. This project contains a bootloadable component so 
* that it can be bootloaded into PSoC 4 which has a bootloader already programmed 
* into it.
* Default UART Port Configuration for bootloading the PSoC 4 in CY8CKIT-049-42xx
* Baud Rate : 115200 bps
* Data Bits : 8
* Stop Bits : 1
* Parity    : None
******************************************************************************/
#include <project.h>
#include <string.h>

//#define SAMPLING
#define JJYSIM


#define CLOCK_FREQ CYDEV_BCLK__SYSCLK__HZ    //クロック周波数
#define int_freq 1000u          //割り込み周波数
//プロト
CY_ISR_PROTO(SysTic_Handler);
 //変数
volatile uint32 system_time;


CY_ISR(SysTic_Handler)
{
 system_time++;
}



static void uart_putc(char c)
{
    
    
    UART_UartPutChar(c);

}

static void uart_print(const char *c)
{
    while(*c)
    {
            uart_putc(*c);
            c++;
    }
}

static const uint32_t l_digits[] = 
{
1000000000UL,
100000000UL ,
10000000UL  ,
1000000   ,
100000    ,
10000     ,
1000      ,
100       ,
10        ,
1,
};

/**
 * 10進数を送信する(32bit)
 */
void uart_send_udec32(uint32_t val)
{
	// 最大値は   4294967295
	uint32_t l;
	int zs = 1;
	int n;
	int d;

	for(d = 0; d < 10; d ++)
	{
		l = l_digits[d];
		if(d == 9) zs = 0;

		n = 0;
		while(val >= l)
		{
			val -= l;
			n++;
		}
		if(n != 0 || !zs)
		{
			zs = 0;
			uart_putc('0' + n);
		}
	}
}


/**
 * 符号つき10進数を送信する(32bit)
 */
void uart_send_dec32(int32_t val)
{
	if(val < 0)
	{
		uart_putc('-');
		val = -val;
	}
	uart_send_udec32(val);
}

#define ADC_CLOCK_DIVIDER 1250


#define SHIFT_FREQ (20000000.0 / (251) / 2)
#define INPUT_FREQ (40000.0 - SHIFT_FREQ)

#define ADC_INTERRUPT_FREQ (5000000.0 / ADC_CLOCK_DIVIDER) 

static volatile uint16_t p_i = 0;

static volatile int irq_count;


static volatile 	int32_t ys[2] = {0,0};
static volatile 	uint32_t sigma = 0;
static volatile int16_t last_adc_value = 0;
static volatile 	uint32_t output_value = 0;

static volatile	uint32_t th_u = 0, th_l = 0;
static volatile	uint32_t threshold = 0;


#if ADC_CLOCK_DIVIDER == 1250

static int32_t b = 31622;
	
static int32_t c = 16255;
#else
    #error unknown ADC_CLOCK_DIVIDER value
#endif
    
static volatile int timecode = 0;
static volatile int avg_cnt = 0;
static volatile int onesec_cnt = 0;



static volatile uint32_t max_adc;
static volatile uint32_t max_adc_tmp;
void     adc_interrupt()
{
irq_count ++;
    
#define  AVG_INTERVAL   (int)(ADC_INTERRUPT_FREQ / INPUT_FREQ *4 + 0.5)
#define  ONE_SEC_INTERVAL ((int)(ADC_INTERRUPT_FREQ / AVG_INTERVAL) * AVG_INTERVAL)

   


	int16_t adc_value = ADC_GetResult16(0);
    int16_t v = adc_value - last_adc_value;
#ifdef SAMPLING
uart_putc(v & 0xff);
uart_putc((v >> 8) & 0xff);
#endif
	last_adc_value = adc_value;

    uint32_t vv = v<0?-v:v;
    if(vv>max_adc_tmp) max_adc_tmp = vv;
    
    
	int32_t t1 = v;
	int32_t t2 = (int64_t)b * ys[0] >> (14);
	int32_t t3 = (int64_t)c * ys[1] >> (14);
	int32_t y = t1 + t2 - t3; //±25415 ??
	ys[1] = ys[0];
	ys[0] = y;

	int32_t add_value = y<0?-y:y;

	sigma += add_value;



	avg_cnt ++;
	if(avg_cnt == AVG_INTERVAL)
	{
		avg_cnt = 0;
		output_value = sigma;
		sigma  = 0;

			uint32_t p_th_l = th_l;
			uint32_t p_th_u = th_u;
			th_u += (int)(p_th_l - p_th_u) >> 10;
			th_l += (int)(p_th_u - p_th_l) >> 11;

			if(th_u < output_value)
				th_u += (int)(output_value - th_u) >> 6;
			if(th_l > output_value)
				th_l += (int)(output_value - th_l) >> 2;

	}


	onesec_cnt ++;
	if(onesec_cnt == ONE_SEC_INTERVAL)
	{
		onesec_cnt = 0;

		threshold += (int)(th_l + ((th_u - th_l) >> 1) - threshold) >> 1;

        
        max_adc = max_adc_tmp;
        max_adc_tmp = 0;
    }
        
    timecode = output_value > threshold; 
			Pin_LED_Write(timecode);
}











#include <project.h>
#include <string.h>

extern volatile uint32 system_time;


/**
 * システムを、キャリブレーションに適した設定にする
 * @param enable_franklin_fb フランクリン発振器を有効にするかどうか
 */
static void init_calibration(int enable_franklin_fb)
{
	// Pin_MixClockOutをStrong Driveのdigital outに設定。
	// Pin_MixClockOutはClocl_MIXの出力をT-FFで1/2分周した出力に
	// つながっていて、普段は周波数ミキサーを駆動するために使われているが、
	// キャリブレーション時はこれをキャリブレーションの基準周波数を
	// フェライトバーアンテナのコイルに供給するために用いる
	Pin_MixClockOut_SetDriveMode(Pin_MixClockOut_DM_STRONG);

	// enable_franklin_fbが有効の場合は、AMux_CapSWを設定する
	if(enable_franklin_fb)
	{
		AMux_CapSw_Connect(1); // 1=Franklin_FB
	}

	// enable_franklin_fbが有効の場合は、コンパレーター LPComp_Franklinと
	// TCPWM Timer_Franklinをスタートする
	if(enable_franklin_fb)
	{
		LPComp_Franklin_Start();
		Timer_Franklin_Start();
	}
}


/**
 * システムを、キャリブレーションに適した設定から元に戻す
 */
static void uninit_calibration()
{
	// Pin_MixClockOutをHigh impedance analogに変更し、出力
	// をオフにする
	Pin_MixClockOut_SetDriveMode(Pin_MixClockOut_DM_ALG_HIZ);


	// AMux_CapSWのうち、フランクリン発振器を有効にするためのラインを切断する
	AMux_CapSw_Disconnect(1); // 1=Franklin_FB
	
	// コンパレーター LPComp_Franklinと
	// TCPWM Timer_Franklinを停止する
	LPComp_Franklin_Stop();
	Timer_Franklin_Stop();
}
#define TUNE_MAX 1024
#define TUNE_STEP 10

/**
 * Pin_MixClockOut の出力周波数を設定する
 * @param		band		受信バンド設定(0=60kHzバンド, 1=40kHzバンド)
*/
static void set_calibration_ref_clock(int band)
{
	// Clock_MIX_SetDividerValue 関数への設定値は、所望の値の半分、つまり
	// 倍の周波数を設定する。これは、クロック出力がT-FFで半分の周波数にされているため。回路図を参照のこと。
	if(band)
	{
		// 40kHz
		Clock_MIX_SetDividerValue(CLOCK_FREQ / 40000ul / 2);
	}
	else
	{
		// 60kHz
		Clock_MIX_SetDividerValue(CLOCK_FREQ / 60000ul / 2);
	}
}


/**
 * キャリブレーションに必要なパラメーターを設定する
 * @param		band		受信バンド設定(0=60kHzバンド, 1=40kHzバンド)
 * @param		finetune	チューニング設定(0 to 1024)
 * @note		PWM_Tuneは時定数が大きいので、この関数を呼んですぐに設定が反映さ
*  				れるとは限らない。回路図中のCR時定数を確認して十分なウェイトを取ること
 */
static void set_calibrarion_parameters(int band, int finetune)
{
	// Pin_BandSelの設定
    AMux_CapSw_Start();
	if(band)
	{
		// 40kHz バンドの場合は、AMux_CapSWの0を接続する
		AMux_CapSw_Connect(0);
	}
	else
	{
		// そうでなければ切断する
		AMux_CapSw_Disconnect(0);
	}

	// PWM_Tuneの設定
	PWM_Tune_Start();
    Clock_PWMTune_Start();
	// PWM周期はTUNE_MAXの倍の周期になっている。必ずduty非は50%以上で用いるため、TUNE_MAXを加算して用いる
	PWM_Tune_WriteCompare(finetune + TUNE_MAX); 
}

#define ADC_WATCH_TIME 300
#define ADC_BYPASS_CHAN_NUM 1

/**
 * ADCをしばらくサンプリングし、最大振幅を得る
 */
static unsigned int get_adc_amplitude()
{
	// バイパス用チャンネルのみを有効にする
	ADC_SetChanMask(1<<ADC_BYPASS_CHAN_NUM);
	// Clock_ADCを無効にする
	Clock_ADC_Stop();
	// ADC割り込みを無効にする
	ADC_IRQ_Disable();


	// 一定期間サンプリングを続け最大値と最小値を得る
	uint16_t min = 0xffffu;
	uint16_t max = 0x0;
	for(uint32_t s = system_time; (int32_t)(system_time - s) < ADC_WATCH_TIME; )
	{
		ADC_StartConvert();
		ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);

		uint16_t res = (uint16_t) ADC_GetResult16(ADC_BYPASS_CHAN_NUM);
		if(min > res) min = res;
		if(max < res) max = res;
	}
	
	// ADC割り込みを有効にする
	ADC_IRQ_Enable();
	// ADC の有効チャンネル設定を元に戻す
	ADC_SetChanMask(ADC_DEFAULT_EN_CHANNELS);
	// Clock_ADCを有効にする
	Clock_ADC_Start();

	// 結果を返す
	return max - min;
}






/**
 * フランクリン発振器の発振周波数を得る
 */
static unsigned int get_franklin_frequency()
{
	int high16 = 0;
	uint16_t prev_cnt = 0;

	for(uint32_t s = system_time; s == system_time; ) /**/;

	Timer_Franklin_WriteCounter(0);

	// wait for 1sec and let counter counting the comparator
	for(uint32_t s = system_time; (int32_t)(system_time - s) < 1000; )
	{
			uint16_t now_cnt  = Timer_Franklin_ReadCounter();
	        if(now_cnt < prev_cnt) high16++; // check for roundup
			prev_cnt = now_cnt;
	}
	return Timer_Franklin_ReadCounter() + (high16 << 16);
}



typedef struct {
	int best40 : 16; // 40kHzでの最良のチューニング値
	int best60 : 16; // 60kHzでの最良のチューニング値
	int current : 16; // 現在調整中のチューニング値
	int low : 16; // チューニング値下限
	int high : 16; // チューニング値上限
	int step : 16; // チューニングのステップ値
	int16_t values[TUNE_STEP]; // 各チューニング値における振幅の値
	int index; // values中のインデックス値
	int level: 2; // スキャンレベル: 0=rough, 1=mid, 2=fine
	int band: 1; // 現在スキャン中のバンド(0=60k, 1=40k)
} tuning_state_t;


/**
 * チューニングステートを初期化する
 */
static void init_tuning_state(tuning_state_t * state)
{
	memset(state, 0, sizeof(*state));

	// stepを roughの初期値に設定する
	state->step = TUNE_MAX / TUNE_STEP;
}



/**
 * チューニングステートを次のステップに進める
 * @return 0=次のステップに進めた, 1=バンドの終わりである
 */
static int step_tuning_state(tuning_state_t *state)
{
	// すでに indexが最後を示しているか？
	if(state->index == TUNE_STEP - 1)
		return 1; // バンドの終わり

	// current と index を加算する
	state->current += state->step;
	state->index ++;
	return 0;
}



/**
 * 共振アンテナL調整を行う。所要時間が長いため、ステートマシンで実装する。
 */
















int main()
{
    ADC_Init();
    
    ADC_Start();

    AMux_Gain_Start();

	AMux_CapSw_Start();
	
    Opamp_1_Start();

    Opamp_2_Start();

    UART_Start();
	uart_print("\r\n\r\nwelcome\r\n\r\n");
	

    CyGlobalIntEnable; 

    AMux_Gain_Connect(1);
   
    ADC_IRQ_Enable();

	Timer_Franklin_Start();
	
	CyIntSetSysVector(15,SysTic_Handler);//Int アドレス設定
	SysTick_Config(CLOCK_FREQ/int_freq);//int_freqを設定 

	
	init_calibration(0);
	set_calibration_ref_clock(1);
	for(;;)
	{
		set_calibrarion_parameters(1, 512);
		get_adc_amplitude(); // ダミー呼び出し
/*
        for(uint32_t i = 0; i < 1024; i += 51)
		{
			uart_print("set ");
			set_calibrarion_parameters(1, i);
			uart_print("wait ");
			for(uint32_t s = system_time; (int32_t)(system_time - s) < 3000; ) ;
			uart_send_dec32(i);
			uart_print(" : ");
			uart_print("measure ");
			uart_send_udec32(get_adc_amplitude());
			uart_print("\r\n");
		}
*/
        for(uint32_t i = 2; i < 1024; i += 51)
		{
			uart_print("set ");
    		Clock_MIX_SetDividerValue(i);
            uart_print("wait ");
			for(uint32_t s = system_time; (int32_t)(system_time - s) < 3000; ) /**/;
			uart_send_dec32(i);
			uart_print(" : ");
			uart_print("measure ");
			uart_send_udec32(get_adc_amplitude());
			uart_print("\r\n");
		}
    
    uart_print("\r\n");
	}
		
#if 0
		uart_print("int count");
		Timer_Franklin_WriteCounter(0);
		int high16 = 0;
		uint16_t prev_cnt = 0;
		for(uint32_t s = system_time; system_time < s+ 1000; )
		{
				uint16_t now_cnt  = Timer_Franklin_ReadCounter();
		        if(now_cnt < prev_cnt) high16++;
				prev_cnt = now_cnt;
		}
		uart_send_udec32(Timer_Franklin_ReadCounter() + (high16 << 16));
		uart_print("\r\n\r\n");
#endif


for(uint32_t s = system_time; system_time < s+ 1000; )
{
        
}
uart_print("int count");
uart_send_udec32(irq_count);
uart_print("\r\n\r\n");

    uint32_t next_tick = system_time + 20;
    for(;;)
    {
#ifdef JJYSIM

	for(int n = 0; n < 10; n++)
	{
		int bits;
		switch(n)
		{
			case 0: bits=0b11000000; break;
			case 1: bits=0b11110000; break;
			case 2: bits=0b11111100; break;
			case 3: bits=0b11111100; break;
			case 4: bits=0b11111100; break;
			case 5: bits=0b11110000; break;
			case 6: bits=0b11111100; break;
			case 7: bits=0b11110000; break;
			case 8: bits=0b11111100; break;
			case 9: bits=0b11110000; break;
		}

		for(int p = 0; p < 8; p++)
		{
			while(system_time < next_tick) /**/;
			next_tick += 125;
			int one_or_zero = (bits & 0b10000000) ? 1: 0;
			Control_Reg_JJYSim_Write(one_or_zero);
			bits <<=1;
		}
	
#endif

#ifndef SAMPLING
        uart_print("\r\n");

	uart_print("\r\n");
	uart_print("\r\n output_value:");
	uart_send_dec32(output_value);
	uart_print("\r\n threshold   :");
	uart_send_dec32(threshold);
	uart_print("\r\n th_l        :");
	uart_send_dec32(th_l);
	uart_print("\r\n th_u        :");
	uart_send_dec32(th_u);
	uart_print("\r\n max_adc     :");
	uart_send_dec32(max_adc);
	uart_print("\r\n");
#endif
	}


}
    for(;;)
    {
        /* The PSoC 4 is put into Sleep Mode as the PWM component is used to 
		blink the LED */
		CySysPmSleep();
    }
}
