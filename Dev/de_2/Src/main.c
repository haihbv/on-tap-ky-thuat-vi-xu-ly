#include "stm32f10x.h"

void delay_ms(uint32_t ms)
{
	volatile uint32_t i, j;
	for (i = 0; i < ms; i++)
	{
		for (j = 0; j < 0x1995; j++);
	}
}

int led_arr[] = {0x000, 0x008, 0x038, 0x1F8, 0x7F8};
int led_arr2[] = {0x000, 0x600, 0x7C0, 0x7F0, 0x7F8};
int counter = 0;

void hieu_ung_chu_A1(void) // tu dinh chu A den chan chu A
{
	int i;
	for (i = 0; i < sizeof(led_arr) / sizeof(led_arr[0]); i++)
	{
		GPIOB->ODR &= ~0x7F8;
		GPIOB->ODR |= led_arr[i];
		delay_ms(10);
	}
}
void hieu_ung_chu_A2(void) // tu chan chu A den dinh chu A
{
	int i;
	for (i = 0; i < sizeof(led_arr) / sizeof(led_arr[0]); i++)
	{
		GPIOB->ODR &= ~0x7F8;
		GPIOB->ODR |= led_arr2[i];
		delay_ms(10);
	}
}
void hieu_ung_nhap_nhay(void)
{
	GPIOB->ODR &= ~0x7F8;
	delay_ms(25);
	GPIOB->ODR |= 0x7F8;
	delay_ms(25);
}

void gpio_config(void)
{
	// enable clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

int main()
{
	gpio_config();
	while (1)
	{
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
		{
			// counter++;
			counter = (counter + 1) % 4;
			// while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);
		}
		if (counter == 0)
		{
			GPIOB->ODR &= ~0x7F8;
		}
		if (counter == 1)
		{
			hieu_ung_chu_A1();
		}
		else if (counter == 2)
		{
			hieu_ung_chu_A2();
		}
		else if (counter == 3)
		{
			hieu_ung_nhap_nhay();
		}
	}
}
