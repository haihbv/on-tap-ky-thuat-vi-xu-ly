#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <math.h>

uint16_t Led7Seg[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

int So_Nguyen_To(int n) {
    if (n < 2) return 0;
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int So_Chinh_Phuong(int n) {
    int m = sqrt(n);
    return (m * m == n);
}

int So_Chia_Het_Cho3(int n) {
    if (n % 3 != 0) return 0;
    return 1;
}


void Delay_Ms(uint32_t time) {
    while (time--) {
        SysTick->CTRL = 5;
        SysTick->LOAD = 72000 - 1;
        SysTick->VAL = 0;

        while (!(SysTick->CTRL & (1u << 16)));
    }
}

void GPIO_Input(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void GPIO_Output_Digit(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void GPIO_Output_Segment(void) 
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |
    GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Display_2Segment(int n) {
    if (n < 10) {
        GPIOA->ODR &= ~(0xFF << 3);
        GPIOA->ODR |= (Led7Seg[n % 10] << 3);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0); // tat led dieu khien hang chuc
        GPIO_SetBits(GPIOA, GPIO_Pin_1); // bat led dieu khien hang don vi
        Delay_Ms(200); 
    }
    else {
        for (int j = 0; j < 100; j++) {
            // hang chuc
            GPIOA->ODR &= ~(0xff << 3);
            GPIOA->ODR |= (Led7Seg[n / 10] << 3);
            GPIO_SetBits(GPIOA, GPIO_Pin_0);
            Delay_Ms(2);
            GPIO_ResetBits(GPIOA, GPIO_Pin_0);

            // hang don vi
            GPIOA->ODR &= ~(0xff << 3);
            GPIOA->ODR |= (Led7Seg[n % 10] << 3);
            GPIO_SetBits(GPIOA, GPIO_Pin_1);
            Delay_Ms(2);
            GPIO_ResetBits(GPIOA, GPIO_Pin_1);
        }
    }
}

int mode = 0;

int main() {
    GPIO_Input();
    GPIO_Output_Digit();
    GPIO_Output_Segment();
    while (1) {
        if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) {
            Delay_Ms(20);
            if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) {
                ++mode;
            }
            while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3));
        }

        if (mode > 3) {
            mode = 0;
        }

        if (mode == 1) {
            for (int i = 0; i < 100; i++) {
                if (So_Nguyen_To(i)) {
                    Display_2Segment(i);
                }
            }
        }

        if (mode == 2) {
            for (int i = 0; i < 100; i++) {
                if (So_Chinh_Phuong(i)) {
                    Display_2Segment(i);
                }
            }
        }

        if (mode == 3) {
            for (int i = 0; i < 100; i++) {
                if (So_Chia_Het_Cho3(i)) {
                    Display_2Segment(i);
                }
            }
        }
    }
}
