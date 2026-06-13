#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

/**
 * PA3 -> PA10
 * ver1: nhấp nháy 8 led đơn
 * ver2: led sáng dần
 * ver3: nhấn nút thì hiển thị 8 led đơn theo số nhị phân
 */

void Delay_Ms(uint32_t time);
void Config_GPIO_Output(void);

// 0000 0000 1000 = 0x008
// 0000 0001 1000 = 0x018
// 0000 0011 1000 = 0x038
// 0000 0111 1000 = 0x078
// 0000 1111 1000 = 0x0F8
// 0001 1111 1000 = 0x1F8
// 0011 1111 1000 = 0x3F8
// 0111 1111 1000 = 0x7F8
uint16_t ledPins[8] = {0x008, 0x018, 0x038, 0x078, 0x0F8, 0x1F8, 0x3F8, 0x7F8};

int main() {
	SystemInit();
	SystemCoreClockUpdate();
	Config_GPIO_Output();
	
	while (1) {
		int i;
		for (i = 0; i < 8; i++) {
			GPIOA->ODR &= ~(0x7F8);
			GPIOA->ODR |= ledPins[i];
			Delay_Ms(100);
		}
		
		GPIOA->ODR &= ~(0x7F8);
		Delay_Ms(1000);
	}
}

void Delay_Ms(uint32_t time) {
	while (time) {
		SysTick->CTRL = 5;
		SysTick->VAL = 0;
		SysTick->LOAD = 72000 - 1;
		
		while (!(SysTick->CTRL & (1u << 16)));
		time--;
	}
}
void Config_GPIO_Output(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6
	| GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}


