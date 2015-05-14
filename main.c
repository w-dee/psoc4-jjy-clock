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

extern volatile uint32_t system_time;

/**
 * epoch からの経過ミリ秒数を得る
 */
static inline int32_t get_elapsed_time(uint32_t epoch)
{
	return (int32_t) (system_time - epoch);
}

/**
 * epoch から指定ミリ秒数が経過したか否かをえる。
 * 単純にsystem_timeと比較すると整数ラウンドアップの罠にハマって痛い目をみるため、
 * できる限りこの関数を使うこと
 */
static inline int is_elapsed(uint32_t epoch, int32_t timeout)
{
	return get_elapsed_time(epoch) >= (int32_t)timeout;
}

#define AMux_CapSW_FRANKLIN 0 //!< Franklin_FB mux ch
#define AMux_CapSW_BAND40K_1 1 //!< 40k band capacitor mux 1
#define AMux_CapSW_BAND40K_2 2 //!< 40k band capacitor mux 2




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
    AMux_CapSw_Start();
	if(enable_franklin_fb)
	{
		AMux_CapSw_Connect(AMux_CapSW_FRANKLIN); // 1=Franklin_FB
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
	AMux_CapSw_Disconnect(AMux_CapSW_FRANKLIN); // 1=Franklin_FB
	
	// コンパレーター LPComp_Franklinと
	// TCPWM Timer_Franklinを停止する
	LPComp_Franklin_Stop();
	Timer_Franklin_Stop();
}

#define TUNE_MAX 1024
#define TUNE_INITIAL_TUNE_STEP 10

/**
 * Pin_MixClockOut の出力周波数を設定する
 * @param		band		受信バンド設定(0=60kHzバンド, 1=40kHzバンド)
*/
static void set_calibration_ref_clock(int band)
{
	// Clock_MIX_SetDividerValue 関数への設定値は、所望の値の半分、つまり
	// 倍の周波数を設定する。これは、クロック出力がT-FFで半分の周波数にされているため。回路図を参照のこと。
    Clock_MIX_Start();
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
		// 40kHz バンドの場合は、AMux_CapSWの該当チャンネルを接続する
		AMux_CapSw_Connect(AMux_CapSW_BAND40K_1);
		AMux_CapSw_Connect(AMux_CapSW_BAND40K_2);

	}
	else
	{
		// そうでなければ切断する
		AMux_CapSw_Disconnect(AMux_CapSW_BAND40K_1);
		AMux_CapSw_Disconnect(AMux_CapSW_BAND40K_2);

	}

	// PWM_Tuneの設定
	PWM_Tune_Start();
    Clock_PWMTune_Start();
	// PWM周期はTUNE_MAXの倍の周期になっている。必ずduty比は50%以上で用いるため、TUNE_MAXを加算して用いる
	PWM_Tune_WriteCompare(finetune + TUNE_MAX); 
}



#define ADC_WATCH_TIME 300
#define ADC_BYPASS_CHAN_NUM 1

/**
 * ADCサンプリング用の初期化を行う
 */
static void init_tune_adc()
{
	// バイパス用チャンネルのみを有効にする
	ADC_SetChanMask(1<<ADC_BYPASS_CHAN_NUM);
	// Clock_ADCを無効にする
	Clock_ADC_Stop();
	// ADC割り込みを無効にする
	ADC_IRQ_Disable();
}

/**
 * ADCサンプリング用の終了処理を行う
 */
static void uninit_tune_adc()
{
	// ADC割り込みを有効にする
	ADC_IRQ_Enable();
	// ADC の有効チャンネル設定を元に戻す
	ADC_SetChanMask(ADC_DEFAULT_EN_CHANNELS);
	// Clock_ADCを有効にする
	Clock_ADC_Start();
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


/**
 * ステートマシン用構造体
 */
typedef struct {
	uint32_t start_time; //!< 計測開始時間
	unsigned int best40 : 16; //!< 40kHzでの最良のチューニング値
	unsigned int best60 : 16; //!< 60kHzでの最良のチューニング値
	unsigned int current : 16; //!< 現在調整中のチューニング値
	unsigned int low : 16; //!< チューニング値下限
	unsigned int high : 16; //!< チューニング値上限
	unsigned int step : 16; //!< チューニングのステップ値
	int adc_low : 16; //!< ADC計測値、下限
	int adc_high : 16; //!< ADC計測値、上限
	int16_t best; //!< 現在計測中の最良のチューニング値
	int16_t best_amp; //!< 現在計測中の最良の振幅
	unsigned int band : 1; //!< 現在スキャン中のバンド(0=60k, 1=40k)
	unsigned int state : 4; //!< ステートマシンのステート
	unsigned int substate : 4; //!< サブステート
	unsigned int calibration_done : 1; //!< キャリブレーション終了を表すフラグ
} tuning_state_t;
static tuning_state_t tuning_state; //!< チューニング状態

#define TUNE_DUMP(X) \
	uart_print(#X " : "); \
	uart_send_dec32((int)tuning_state. X ); \
	uart_print("\r\n");

/**
 * ステートマシン用構造体のダンプ
 */
static void dump_tuning_state()
{
	uart_print("--tuning state dump--\r\n");
	TUNE_DUMP(start_time);
	TUNE_DUMP(best40);
	TUNE_DUMP(best60);
	TUNE_DUMP(current);
	TUNE_DUMP(low);
	TUNE_DUMP(high);
	TUNE_DUMP(step);
	TUNE_DUMP(adc_low);
	TUNE_DUMP(adc_high);
	TUNE_DUMP(best);
	TUNE_DUMP(best_amp);
	TUNE_DUMP(band);
	TUNE_DUMP(state);
	TUNE_DUMP(substate);
	TUNE_DUMP(calibration_done);
}


/**
 * state 設定関数
 */
static void set_tuning_state(int state)
{
	tuning_state.state = state;
uart_print("tuning state set to : ");
uart_send_dec32(state);
uart_print("\r\n");
dump_tuning_state();
}

/**
 * substate 設定関数
 */
static void set_tuning_substate(int substate)
{
	tuning_state.substate = substate;
uart_print("tuning substate set to : ");
uart_send_dec32(substate);
uart_print("\r\n");
dump_tuning_state();
}


/*
状態遷移
*/
#define TUNE_STATE_INIT 0  //!< 最初。各種ペリフェラルの初期化など
#define TUNE_STATE_40K 1 //!< 40k 調整中
#define TUNE_STATE_60K 2 //!< 60k 調整中
#define TUNE_STATE_FIN 3 //!< 調整終了

// 上記各「調整中」は以下のサブステートに別れる。
#define TUNE_SUBSTATE_TUNE_INIT 0 //!< 初期化
#define TUNE_SUBSTATE_TUNE_START_TUNE 1 //!< 一回のイテレーションの開始
#define TUNE_SUBSTATE_TUNE_WAIT_STABLE 2 //!< チューニングPWMnの最初の値を設定し、出力が安定するまで待つ
#define TUNE_SUBSTATE_TUNE_PREPARE 3 //!< チューニングPWMやADCを設定
#define TUNE_SUBSTATE_TUNE_WAIT 4 //!< チューニングPWM変更直後のウェイト
#define TUNE_SUBSTATE_MEASURE 5 //!< 値を計測中
#define TUNE_SUBSTATE_DONE 6 //!< 計測終了



#define TUNE_PWM_STABILIZATION_WAIT 2500 //!< TunePWMを変更したあとに安定するまで待つミリ秒数
#define TUNE_PWM_LARGE_STABILIZATION_WAIT 5000 //!< TunePWMを変更したあとに安定するまで待つミリ秒数(大きな値の変更時)
#define TUNE_MEASURE_TIME 500 //!< ADCにて値を計測する時間


/**
 * チューニングステートを初期化する
 */
static void init_tuning_state()
{
	memset(&tuning_state, 0, sizeof(tuning_state));
}





/**
 * チューニングのためのADCを開始する
 */
static void tuning_start_adc()
{
	ADC_StartConvert();
}

static void tuning_measure_adc()
{
	// 値を得て振幅の最大値と最小値を得る
	ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);

	int16_t res = (uint16_t) ADC_GetResult16(ADC_BYPASS_CHAN_NUM);

	// ここで注意したいのは、突発的なノイズによる異常値の観測である。
	// ここではJJY受信時の、検波後の値の上限と下限を設定するような
	// アルゴリズムを採用する。
	int16_t th_l = tuning_state.adc_low;
	int16_t th_h = tuning_state.adc_high;
/*
    int16_t p_th_l = th_l;
	int16_t p_th_h = th_h;
	th_h += (int)(p_th_l - p_th_h) >> 10;
	th_l += (int)(p_th_h - p_th_l) >> 11;

	if(th_h < res)
		th_h += (int)(res - th_h) >> 6;
	if(th_l > res)
		th_l += (int)(res - th_l) >> 6;
*/
    if(th_l > res) th_l = res;
    if(th_h < res) th_h = res;

	tuning_state.adc_low = th_l;
	tuning_state.adc_high = th_h;
}


/**
 * 最大の振幅を探すための各値を初期化する
 */
static void init_tuning_best()
{
	tuning_state.low = 0;
	tuning_state.high = TUNE_MAX;
	tuning_state.step = TUNE_INITIAL_TUNE_STEP;
	tuning_state.best = -1;
	tuning_state.best_amp = 0;
    tuning_state.current = 0;
}

/**
 * 次にスキャンすべき範囲とステップを設定する
 */
static void tuning_set_next_loop_parameters()
{
	// best を中心に、 best_value-TUNE_INITIAL_TUNE_STEP*2,
	// best_value+TUNE_INITIAL_TUNE_STEP*2あたりをスキャンする
	uint16_t step = tuning_state.step * 2;
	int16_t low = tuning_state.best - step;
	int16_t high = tuning_state.best + step;
	step = 1;

	// 範囲外を補正する
	if(low < 0)
	{
		high += -low;
		low = 0;
	}
	if(high >= TUNE_MAX)
	{
		low -= (high - TUNE_MAX);
		high = TUNE_MAX - 1;
	}

	// 値を構造体にセット
	tuning_state.step = step;
	tuning_state.low = low;
	tuning_state.high = high;
	tuning_state.best = -1;
	tuning_state.best_amp = 0;
    tuning_state.current = low;
}



/**
 * 調整サブハンドラ
 */
static void tuning_subhandler()
{
	switch(tuning_state.substate)
	{
	case TUNE_SUBSTATE_TUNE_INIT: //!< チューニングPWMやADCを設定
		init_tune_adc();
		init_tuning_best();
       	set_calibration_ref_clock(tuning_state.band);
        tuning_state.start_time = system_time;
		set_tuning_substate(TUNE_SUBSTATE_TUNE_START_TUNE);
		break;

    case TUNE_SUBSTATE_TUNE_START_TUNE:
        set_calibrarion_parameters(tuning_state.band, tuning_state.current);
		tuning_state.start_time = system_time;
        set_tuning_substate(TUNE_SUBSTATE_TUNE_WAIT_STABLE);
        break;
        
        
    case TUNE_SUBSTATE_TUNE_WAIT_STABLE:
        if(is_elapsed(tuning_state.start_time, TUNE_PWM_LARGE_STABILIZATION_WAIT))
        {
            set_tuning_substate(TUNE_SUBSTATE_TUNE_PREPARE);
        }
        break;
        
	case TUNE_SUBSTATE_TUNE_PREPARE:
		set_calibrarion_parameters(tuning_state.band, tuning_state.current);
		tuning_state.start_time = system_time;
		set_tuning_substate(TUNE_SUBSTATE_TUNE_WAIT);
		break;
		

	case TUNE_SUBSTATE_TUNE_WAIT: //!< チューニングPWM変更直後のウェイト
		if(is_elapsed(tuning_state.start_time, TUNE_PWM_STABILIZATION_WAIT))
		{
			tuning_state.start_time = system_time;
			set_tuning_substate(TUNE_SUBSTATE_MEASURE);
			tuning_state.adc_high = SHRT_MIN;
			tuning_state.adc_low = SHRT_MAX;
			tuning_start_adc(); // 最初のADC計測をトリガー
uart_print("measuring tune ");
uart_send_dec32(tuning_state.current);
uart_print(" ... ");
		}
		break;

	case TUNE_SUBSTATE_MEASURE: //!< 値を計測中
		if(!is_elapsed(tuning_state.start_time, TUNE_MEASURE_TIME))
		{
			tuning_measure_adc();
			tuning_start_adc(); // 次のADC計測を開始する
		}
		else
		{
			// 計測終了

			// 現在の値を格納
			uint16_t amp = tuning_state.adc_high - tuning_state.adc_low;
uart_print("result : ");
uart_send_dec32(amp);
uart_print("\r\n");
			if(tuning_state.best == -1 ||
				tuning_state.best_amp < amp)
			{
				tuning_state.best = tuning_state.current;
				tuning_state.best_amp = amp;
			}

			// current を加算する
			tuning_state.current += tuning_state.step;

			// 最後か？
			if(tuning_state.current >= tuning_state.high)
			{
				// 最後を示している
				// 再度繰り返す必要があるか？
				if(tuning_state.step == 1)
				{
					// 最終ステップである
					// 繰り返す必要はないので
					// TUNE_SUBSTATE_DONE に移行する
					// すでに一番細かい単位でのスキャンが終わった
					if(tuning_state.band == 1)
						tuning_state.best40 = tuning_state.best;
					else
						tuning_state.best60 = tuning_state.best;
					set_tuning_substate(TUNE_SUBSTATE_DONE);
				}
				else
				{
					// 再びTUNE_SUBSTATE_TUNE_PREPAREに戻る
					tuning_set_next_loop_parameters();
					set_tuning_substate(TUNE_SUBSTATE_TUNE_START_TUNE);
					tuning_state.start_time = system_time;
				}
			}
            else
            {
                // 最後ではない。
                //  TUNE_SUBSTATE_TUNE_PREPAREにもどる
                set_tuning_substate(TUNE_SUBSTATE_TUNE_PREPARE);
            }
        }
		break;
		

	case TUNE_SUBSTATE_DONE:
		uninit_tune_adc();
	}
}


/**
 * 共振アンテナL調整を行う。所要時間が長いため、ステートマシンで実装する。
 */
static void tuning_handler()
{
	switch(tuning_state.state)
	{
	case TUNE_STATE_INIT:  //!< 最初。各種ペリフェラルの初期化など
		init_calibration(0);
		set_tuning_state(TUNE_STATE_40K);
		set_tuning_substate(TUNE_SUBSTATE_TUNE_INIT);
		break;

	case TUNE_STATE_40K: //!< 40k 調整中
	case TUNE_STATE_60K: //!< 60k 調整中
        if(tuning_state.state == TUNE_STATE_40K)
            tuning_state.band = 1;
        else
            tuning_state.band = 0;

		tuning_subhandler();

		if(tuning_state.substate == TUNE_SUBSTATE_DONE)
		{
			// 終わった
			if(tuning_state.state == TUNE_STATE_40K)
			{
				// 40kが終わったので60kの調整を行う
				set_tuning_state(TUNE_STATE_60K);
				set_tuning_substate(TUNE_SUBSTATE_TUNE_INIT);
			}
			else
			{
				// 60k も終わった
				// 調整終了
				set_tuning_state(TUNE_STATE_FIN);
			}
		}
		break;


	case TUNE_STATE_FIN: //!< 調整終了
		if(!tuning_state.calibration_done) uninit_calibration();
		tuning_state.calibration_done = 1;
		break;
	}
}







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
	int16_t min = SHRT_MAX;
	int16_t max = SHRT_MIN;
	for(uint32_t s = system_time; (int32_t)(system_time - s) < ADC_WATCH_TIME; )
	{
		ADC_StartConvert();
		ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);

		int16_t res = (int16_t) ADC_GetResult16(ADC_BYPASS_CHAN_NUM);
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

	
	for(;;)
	{

		tuning_handler();

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
