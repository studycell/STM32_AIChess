
#include "stm32f10x.h"
#include <stm32f10x_usart.h>


extern int sDataX_all_BLACK;
extern int sDataY_all_BLACK;
extern int dDataX_all_BLACK;
extern int dDataY_all_BLACK;

int sDataX_all_WHITE;
int sDataY_all_WHITE;
int dDataX_all_WHITE;
int dDataY_all_WHITE;

int LCD_to_USART_X(int x) {
	if(x == 25) {
		x = 0;
	}
	else if(x == 75) {
		x = 1;
	}
	else if(x == 125) {
		x = 2;
	}
	else if(x == 175) {
		x = 3;
	}
	else if(x == 225) {
		x = 4;
	}
	else {
		x = 5;
	}
	return x;
}

int LCD_to_USART_Y(int y) {
	if(y == 75) {
		y = 0;
	}
	else if(y == 125) {
		y = 1;
	}
	else if(y == 175) {
		y = 2;
	}
	else if(y == 225) {
		y = 3;
	}
	else if(y == 275) {
		y = 4;
	}
	else {
		y = 5;
	}
	return y;
}

int USART_to_LCD_X(int x) {
	if(x == 0) {
		x = 25;
	}
	else if(x == 1) {
		x = 75;
	}
	else if(x == 2) {
		x = 125;
	}
	else if(x == 3) {
		x = 175;
	}
	else if(x == 4) {
		x = 225;
	}
	
	return x;
}

int USART_to_LCD_Y(int y) {
	if(y == 0) {
		y = 75;
	}
	else if(y == 1) {
		y = 125;
	}
	else if(y == 2) {
		y = 175;
	}
	else if(y == 3) {
		y = 225;
	}
	else if(y == 4) {
		y = 275;
	}
	return y;
}




void RS232_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	USART_InitTypeDef USART_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure1;
	//????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);


  /*
  *  USART1_TX -> PA9 , USART1_RX ->	PA10
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		//??1???
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//????
	USART_Cmd(USART1, ENABLE);
	USART_ClearITPendingBit(USART1, USART_IT_TC);//????TC?
	
}


void NVIC_Configuration(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  
  /* Enable the Ethernet global Interrupt */
  
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
}

char RS232InData;
char recvData;
int recvDealFlag = 0;
int cnt = 0;

void USART1_IRQHandler(void)  
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{	
		//RS232InData = USART1->DR;//
		recvData = USART1 -> DR;
		//recvData -= 0x30;
		recvDealFlag ++;
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) 
	{
        USART_ClearITPendingBit(USART1, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
  }	
}

void RS232SendByte(char data)
{
		USART1->DR = data; 
}


void uart_test()
{
		NVIC_Configuration();
		RS232_Configuration();
		//LCD_to_USART();
	  //RS232SendByte(sDataX_all_BLACK * 10 + sDataY_all_BLACK);
		//RS232SendByte(dDataX_all_BLACK * 10 + dDataY_all_BLACK);
	/*	
	while(1) {
			
			if(recvDealFlag == 1) {
				if(cnt % 2 == 1) {
						sDataX_all_WHITE = recvData / 10;
						sDataY_all_WHITE = recvData % 10;
				}
				else if(cnt % 2 == 0) {
						dDataX_all_WHITE = recvData / 10;
						dDataY_all_WHITE = recvData % 10;
						USART_to_LCD();
				}
				recvDealFlag = 0;
			}
		}
		*/
}