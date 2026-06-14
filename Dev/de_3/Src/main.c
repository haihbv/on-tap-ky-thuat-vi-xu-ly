#include "stm32f10x.h"

void delay_ms(uint32_t ms);
void gpio_output(void);
void gpio_input(void);

int cnt = 0;

int main()
{
	gpio_output();
	gpio_input();
	cnt = 0;
	GPIOB->ODR = 0;
	while (1)
	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == RESET)
		{
			cnt++;
			if (cnt > 255)
			{
				cnt = 0;
			}
			GPIOB->ODR = cnt;
			while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == RESET);
		}
		
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == RESET)
		{
			cnt--;
			if (cnt < 0)
			{
				cnt = 255;
			}
			GPIOB->ODR = cnt;
			while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == RESET);
		}
	}
}

void delay_ms(uint32_t ms)
{
	volatile uint32_t i, j;
	for (i = 0; i < ms; i++)
	{
		for (j = 0; j < 0x1995; j++);
	}
}
void gpio_output(void)
{
	// enable clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void gpio_input(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
