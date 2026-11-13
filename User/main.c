#include "stm32f10x.h"

void App_OnBoardLED_Init(void);
void App_Button_Init(void);

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	App_OnBoardLED_Init();
	App_Button_Init();

	while (1)
	{
	}
}

void App_OnBoardLED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
}
void App_Button_Init(void)
{
	// 初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	// PA5
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	// PA6
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// EXTI分配引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	// 初始化
	EXTI_InitTypeDef EXTI_InitStruct;

	EXTI_InitStruct.EXTI_Line = EXTI_Line5;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	EXTI_InitStruct.EXTI_Line = EXTI_Line6;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);

	//配置中断
	NVIC_InitTypeDef NVIC_InitSturct;

	NVIC_InitSturct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitSturct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitSturct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitSturct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitSturct);

}

void EXTI9_5_IRQHandler(void)
{
	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
	if(EXTI_GetFlagStatus(EXTI_Line5) == SET)
	{
		EXTI_ClearFlag(EXTI_Line5);
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
	}
	if(EXTI_GetFlagStatus(EXTI_Line6) == SET)
	{
		EXTI_ClearFlag(EXTI_Line6);
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
	}
}
