#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

// nhan PB3 dem tien
// nhan PB4 dem lui
uint16_t Led7Seg[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

// trong proteus: set clock scale: 16 times
void Delay_Ms(uint32_t time);
void Config_GPIO_Input(void);
void Config_GPIO_Segment(void);

int i = 0;

int main(void) {
	SystemInit();
	SystemCoreClockUpdate();
	Config_GPIO_Input();
	Config_GPIO_Segment();
	while (1) {
		
		if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) {
			i++;
			if (i > 9) i = 0;
			while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3));
		}
		
		if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)) {
			i--;
			if (i < 0) i = 9;
			while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4));
		}
		
		GPIOA->ODR &= ~(0x7F << 3);
		GPIOA->ODR |= (Led7Seg[i] << 3);
		Delay_Ms(1);
	}
}

void Delay_Ms(uint32_t time)
{
	while (time) {
		SysTick->CTRL = 5;
		SysTick->VAL = 0;
		SysTick->LOAD = 72000 - 1; // ms
		
		while (!(SysTick->CTRL & (1u << 16)));
		time--;
	}
}

void Config_GPIO_Input(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Config_GPIO_Segment(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 
	| GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}
