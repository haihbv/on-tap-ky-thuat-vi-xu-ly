#include "stm32f10x.h"

// fibonacci: 0 1 1 2 3 5 8 13 21
uint16_t fibo[20];
int f_cnt = 0;
int f_idx = 0;
void solve(void);

void gpio_output(void);
void gpio_input(void);

int main()
{
	gpio_output();
	gpio_input();
	solve();
	while (1)
	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == RESET)
		{
			GPIOB->ODR = fibo[f_idx];
			f_idx++;
			if (f_idx > f_cnt)
			{
				f_idx = 0;
			}
			while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == RESET);
		}
	}
}

void solve()
{
	uint32_t a = 0, b = 1;
	
	while (b <= 0xffff)
	{
		fibo[f_cnt++] = (uint16_t)b;
		uint32_t c = a + b;
		a = b;
		b = c;
	}
}

void gpio_output()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = 0xffff;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void gpio_input()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
