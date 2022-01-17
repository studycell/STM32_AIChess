/***********************************************************************
�ļ����ƣ�LED.C
��    �ܣ�led  IO��ʼ��
��дʱ�䣺2013.4.25
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "stm32f10x.h"
void PWM_Configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE|RCC_APB2Periph_TIM1, ENABLE);	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); //Timer1?????  ?????????? tim1??????? ???tim1
	

	
		GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_9 | GPIO_Pin_11  ;  		//
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);	
	
		//TIM_DeInit(TIM1); //
	
    //����ʱ������������ã�TIM2Ƶ��Ϊ72M/TIM_Period
		TIM_TimeBaseStructure.TIM_Period = 10000;   						//����       
		TIM_TimeBaseStructure.TIM_Prescaler = 71;                        //��Ƶ10K
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    //ʱ�ӷָ�Ϊ0  
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //���ϼ���ģʽ    
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 

    //�������ģʽ
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               //�����ȵ���ģʽ1 
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //���״̬ʹ��  
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;        //������Ե�


		TIM_OC1Init(TIM1, &TIM_OCInitStructure);						//T3-CH1
		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);		


		TIM_Cmd(TIM1, ENABLE) ;

             
		TIM_CtrlPWMOutputs(TIM1,ENABLE);        //TIM1��ʱ��

   		
}


void PWM_test()
{
	while(1)
	{
		TIM1->CCR1 =  5000; //ʱ�Ӽ���ֵ���ֵ5000�����ѹ���ֵ5000��Χ��ͬ������ֱ�Ӹ�ֵ

	}
}