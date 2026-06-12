#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <math.h>

// PA3 - PA9: Segment
// PA0: hang chuc
// PA1: hang don vi
uint16_t Led7Seg[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

void Delay_Ms(uint32_t time);
void Config_GPIO_Input(void);
void Config_GPIO_Digit(void);
void Config_GPIO_Segment(void);

/*
* Ver1: hien thi tu so 17 den 100 roi cu the lap lai
* Ver2: dem len tu 0 den 99 roi tu 99 dem lui ve 0, va lap lai
* Ver3: su dung nut nhan (GPIOB_Pin_3)
*				- nhan 1 lan: hien thi cac so nguyen to
*				- nhan 2 lan: hien thi cac so chinh phuong
*				- nhan 3 lan: hien thi so chan hoac le
*/

int So_Nguyen_To(int n) {
	if (n < 2) return 0;
	int i;
	for (i = 2; i * i <= n; i++) {
		if (n % i == 0) return 0;
	}
	return 1;
}

int So_Chinh_Phuong(int n) {
	int temp = n;
	int m = sqrt(n);
	if (m * m != temp) return 0;
	return 1;
}

int So_Chan(int n) {
	if (n % 2 != 0) return 0;
	return 1;
}

void Effect_2Digit_Ver1(void);
void Effect_2Digit_Ver2(void);
void Effect_2Digit_Ver3(void); 

int cnt = 0;

int main() {
	Config_GPIO_Input();
	Config_GPIO_Digit();
	Config_GPIO_Segment();
	while (1) {
		Effect_2Digit_Ver3();
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

void Config_GPIO_Input(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}
void Config_GPIO_Digit(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // bat clock port A

	// cau hinh chan
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	// luu cau hinh chan
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Config_GPIO_Segment(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // bat clock port A

	// cau hinh chan
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	// luu cau hinh chan
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Effect_2Digit_Ver1(void) {
	int i, j;
	for (i = 0; i < 100; i++) {
		if (i >= 17) {
			for (j = 0; j < 60; j++) {
				// hang chuc
				GPIO_SetBits(GPIOA, GPIO_Pin_0); // tat led dieu khien hang chuc
				// clear bit ODR truoc
				GPIOA->ODR &= ~(0x7F << 3);
				GPIOA->ODR |= (Led7Seg[i / 10] << 3);
				Delay_Ms(2);
				GPIO_ResetBits(GPIOA, GPIO_Pin_0); // bat led dieu khien hang chuc

				// hang don vi
				GPIO_SetBits(GPIOA, GPIO_Pin_1); // tat led dieu khien hang don vi
				// clear bit ODR truoc
				GPIOA->ODR &= ~(0x7F << 3);
				GPIOA->ODR |= (Led7Seg[i % 10] << 3);
				Delay_Ms(2);
				GPIO_ResetBits(GPIOA, GPIO_Pin_1); // bat led dieu khien hang don vi
			}
		}
	}
}
void Effect_2Digit_Ver2(void) {
	int i, j;
	for (i = 0; i < 100; i++) {
		for (j = 0; j < 105; j++) {
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			GPIOA->ODR &= ~(0x7F << 3);
			GPIOA->ODR |= (Led7Seg[i / 10] << 3);
			Delay_Ms(1);
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);

			GPIO_SetBits(GPIOA, GPIO_Pin_1);
			GPIOA->ODR &= ~(0x7F << 3);
			GPIOA->ODR |= (Led7Seg[i % 10] << 3);
			Delay_Ms(1);
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		}
	}

	for (i = 99; i >= 0; i--) {
		for (j = 0; j < 105; j++) {
			GPIO_SetBits(GPIOA, GPIO_Pin_0);
			GPIOA->ODR &= ~(0x7F << 3);
			GPIOA->ODR |= (Led7Seg[i / 10] << 3);
			Delay_Ms(1);
			GPIO_ResetBits(GPIOA, GPIO_Pin_0);

			GPIO_SetBits(GPIOA, GPIO_Pin_1);
			GPIOA->ODR &= ~(0x7F << 3);
			GPIOA->ODR |= (Led7Seg[i % 10] << 3);
			Delay_Ms(1);
			GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		}
	}
}
void Effect_2Digit_Ver3(void) {
	int i, j;
		if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) {
			Delay_Ms(20);
			if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) {
				++cnt;
			}
			while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3));
		}

		if (cnt > 3) {
			cnt = 0;
		}
		
		if (cnt == 1) {
			for (i = 0; i < 100; i++) {
				if (So_Nguyen_To(i)) {
					for (j = 0; j < 105; j++) {
						GPIO_SetBits(GPIOA, GPIO_Pin_0);
						GPIOA->ODR &= ~(0x7F << 3);
						GPIOA->ODR |= (Led7Seg[i / 10] << 3);
						Delay_Ms(1);
						GPIO_ResetBits(GPIOA, GPIO_Pin_0);
						
						GPIO_SetBits(GPIOA, GPIO_Pin_1);
						GPIOA->ODR &= ~(0x7F << 3);
						GPIOA->ODR |= (Led7Seg[i % 10] << 3);
						Delay_Ms(1);
						GPIO_ResetBits(GPIOA, GPIO_Pin_1);
					}
				}
				
			}
		}

		if (cnt == 2) {
			for (i = 0; i < 100; i++) {
				if (So_Chinh_Phuong(i)) {
					for (j = 0; j < 105; j++) {
						GPIO_SetBits(GPIOA, GPIO_Pin_0);
						GPIOA->ODR &= ~(0x7F << 3);
						GPIOA->ODR |= (Led7Seg[i / 10] << 3);
						Delay_Ms(1);
						GPIO_ResetBits(GPIOA, GPIO_Pin_0);
						
						GPIO_SetBits(GPIOA, GPIO_Pin_1);
						GPIOA->ODR &= ~(0x7F << 3);
						GPIOA->ODR |= (Led7Seg[i % 10] << 3);
						Delay_Ms(1);
						GPIO_ResetBits(GPIOA, GPIO_Pin_1);
					}
				}
				
			}
		}

		if (cnt == 3) {
			for (i = 0; i < 100; i++) {
				if (So_Chan(i)) {
					for (j = 0; j < 105; j++) {
						GPIO_SetBits(GPIOA, GPIO_Pin_0);
						GPIOA->ODR &= ~(0x7F << 3);
						GPIOA->ODR |= (Led7Seg[i / 10] << 3);
						Delay_Ms(1);
						GPIO_ResetBits(GPIOA, GPIO_Pin_0);
						
						GPIO_SetBits(GPIOA, GPIO_Pin_1);
						GPIOA->ODR &= ~(0x7F << 3);
						GPIOA->ODR |= (Led7Seg[i % 10] << 3);
						Delay_Ms(1);
						GPIO_ResetBits(GPIOA, GPIO_Pin_1);
					}
				}
				
			}
		}

}
