/***********************************************************************
�ļ����ƣ�LED.C
��    �ܣ�led  IO��ʼ��
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "stm32f10x.h"


#define LED1_ON 		GPIO_ResetBits(GPIOE , GPIO_Pin_2)
#define LED2_ON 		GPIO_ResetBits(GPIOE , GPIO_Pin_3)
#define LED3_ON 		GPIO_ResetBits(GPIOE , GPIO_Pin_4)
#define LED4_ON 		GPIO_ResetBits(GPIOE , GPIO_Pin_5)


#define LED1_OFF 		GPIO_SetBits(GPIOE , GPIO_Pin_2)
#define LED2_OFF 		GPIO_SetBits(GPIOE , GPIO_Pin_3)
#define LED3_OFF 		GPIO_SetBits(GPIOE , GPIO_Pin_4)
#define LED4_OFF 		GPIO_SetBits(GPIOE , GPIO_Pin_5)

void LED_Configuration(void);
void LED_Blink(void);
void One_LED_ON(unsigned char led_num);

int Delay(uint32_t nCount);
/***********************************************************************
�������ƣ�LED_Configuration(void)
��    �ܣ����LED������
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺LED1->PE2,
		  LED2->PE3,
		  LED3->PE4,
		  LED4->PE5,
***********************************************************************/
void LED_Configuration(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable the GPIO_LED Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE , ENABLE); 						 
			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/***********************************************************************
�������ƣ�LED_Blink(void)
��    �ܣ����LED��˸
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
void LED_Blink(void)
{
	while(1)
	{
		GPIO_SetBits(GPIOE , GPIO_Pin_8);
		GPIO_SetBits(GPIOE , GPIO_Pin_9);
		GPIO_SetBits(GPIOE , GPIO_Pin_10);
		GPIO_SetBits(GPIOE , GPIO_Pin_11);
		Delay(0xfffff);
		Delay(0xfffff);
		/*====LED-OFF=======*/ 
		GPIO_ResetBits(GPIOE , GPIO_Pin_8);
		GPIO_ResetBits(GPIOE , GPIO_Pin_9);
		GPIO_ResetBits(GPIOE , GPIO_Pin_10);
		GPIO_ResetBits(GPIOE , GPIO_Pin_11);
		Delay(0xfffff);
		Delay(0xfffff); 
	}
}
/***********************************************************************
�������ƣ�One_LED_ON(unsigned char led_num)
��    �ܣ�ʵ�ֵ���һ��LED��
���������
���������
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
void One_LED_ON(unsigned char led_num)
{	
	/*****Ϩ���ĸ�led��******/
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	switch(led_num)
	{
		case 1:
		{
			LED1_ON;
			break;
		}
		case 2:
		{
			LED2_ON;
			break;		
		}
		case 3:
		{
			LED3_ON;
			break;		
		}
		case 4:
		{
			LED4_ON;
			break;		
		}
		default:
		{
			break;	
		}
	}		
}

int Delay(uint32_t nCount)
{ 
  while(nCount > 0)
  { 
  	  nCount --;   
  }
}