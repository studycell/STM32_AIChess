/**
  ******************************************************************************
  * @file InputCaptureMode/main.c 
  * @author   MCD Application Team
  * @version  V3.0.0
  * @date     04/27/2009
  * @brief    Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/** @addtogroup InputCaptureMode
  * @{
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stm32f10x_usart.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);

//#define _Exp_LED_  //ok
//#define _Exp_KEY_	//ok
//#define _EXP_BEEP_	//ok
#define _EXP_UART_	//ok
//#define _EXP_LCD_		//OK
//#define _EXP_SPI_  //OK
//#define _EXP_AIAO_  //AI OK�� AO not supported by 103VB
//#define _EXP_COUNTER_  /OK
//#define _EXP_IIC_		 //OK
//#define _EXP_PWM_		 //�������OK������û�С������޷�����ͬʱ����ͨ����remap������
//#define _EXP_7SEG_		 //OK

void BoardInit()
{
  /* System Clocks Configuration */
	SystemInit();

}

int FlashID;

void ExpTest()
{
		#ifdef _Exp_LED_
		LED_Configuration();	//LED���� 
		LED_Blink();
		#endif

		#ifdef _EXP_BEEP_
		BEEP_Configuration();		
		Beep();

		#endif
		
		#ifdef 	_Exp_KEY_
			key_test();
		#endif
	
#ifdef 	_EXP_UART_
		uart_test();
#endif	
	
#ifdef _EXP_SPI_
	ENC25Q80_SPI2_Init();
		FlashID = SPI_FLASH_ReadDeviceID(); //ID = 0x13
#endif	

#ifdef _EXP_AIAO_
		AI_Configuration();
		DAC1_Configuration();
		AIAO_test();
#endif

#ifdef _EXP_COUNTER_
		Counter_Configuration();
		Counter_test();
#endif		

#ifdef _EXP_IIC_
		IIC_Configuration();
		IIC_test();
#endif

#ifdef _EXP_PWM_
		PWM_Configuration();
		PWM_test();
#endif

#ifdef _EXP_LCD_
		LCD_Configuration();
		LCD_test();
		Touch_test();
#endif

#ifdef _EXP_7SEG_
		SEG_Configuration();
		SEG_test();		
#endif

}




int main(void)
{

	BoardInit();

	//ExpTest();
	LCD_Configuration();
	
	uart_test();
	//LCD_test();
	Touch_test();
	

}




/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
