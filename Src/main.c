#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

// 1111: tat led
// 1110=0x0E: PA0 sang
// 1100=0x0C: PA0 - PA1 sang
// 1000=0x08: PA0 - PA1 - PA2 sang
// 0000=0x00: PA0 - PA1 - PA2 - PA3 sang

uint16_t ledSangDan[4] = {0x0E, 0x0C, 0x08, 0x00};




void GPIO_Output(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void GPIO_Input(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Delay_Ms(uint32_t time)
{
    while (time--)
    {
        SysTick->CTRL = 5;
        SysTick->VAL = 0;
        SysTick->LOAD = 72000 - 1;
        while (!(SysTick->CTRL & (1u << 16)))
            ;
    }
}


int cnt = 0;

void CauA(void);
void CauB(void);


int main(void)
{
    GPIO_Output();
    GPIO_Input();
	
		
		GPIOA->ODR = (GPIOA->ODR & ~0x0F) | 0x0F;
    while (1)
    {
        CauB();
    }
}

void CauA(void) {
	if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))
	{
			Delay_Ms(20);
			if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))
			{
					++cnt;
			}
			while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0));
	}

	if (cnt >= 4)
	{
			cnt = 0;
			GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
	}

	GPIOA->ODR = ledSangDan[cnt];
}

void CauB(void) {
    if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))
    {
        Delay_Ms(20);
        if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))
        {
            while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)); 
            for (int i = 0; i < 4; i++) {
                GPIOA->ODR = (GPIOA->ODR & ~0x0F) | ledSangDan[i];
                Delay_Ms(100);
                if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)) break;
            }
        }
    }
    else if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1))
    {
        Delay_Ms(20);
        if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1))
        {
            while (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)); 
            for (int i = 3; i >= 0; i--) {
                GPIOA->ODR = (GPIOA->ODR & ~0x0F) | ledSangDan[i];
                Delay_Ms(100);
                if (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)) break;
            }
            GPIOA->ODR = (GPIOA->ODR & ~0x0F) | 0x0F; 
        }
    }
}


