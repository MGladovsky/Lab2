#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_tim.h>

//Var 3
//C15 C14
//0.25C
//B1, B10, B11

#define SYSCLK 72000000
#define PRESCALER 72

GPIO_InitTypeDef port;
TIM_TimeBaseInitTypeDef timer;
TIM_OCInitTypeDef timerPWM;
void servo_init(void) {
	
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
				GPIO_StructInit(&port);

				port.GPIO_Mode = GPIO_Mode_AF_PP;
				port.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11;
				port.GPIO_Speed = GPIO_Speed_2MHz;
				GPIO_Init(GPIOB, &port);
	
				TIM_TimeBaseStructInit(&timer);
				timer.TIM_Prescaler = PRESCALER;
				timer.TIM_Period = SYSCLK / PRESCALER / 50;
				timer.TIM_ClockDivision = 0;
				timer.TIM_CounterMode = TIM_CounterMode_Up;
				TIM_TimeBaseInit(TIM2, &timer);	
	      TIM_TimeBaseInit(TIM3, &timer);	
				TIM_OCStructInit(&timerPWM);
	
	
				timerPWM.TIM_Pulse = 1000;
				timerPWM.TIM_OCMode = TIM_OCMode_PWM1;
				timerPWM.TIM_OutputState = TIM_OutputState_Enable;
				timerPWM.TIM_OCPolarity = TIM_OCPolarity_High;
				TIM_OC1Init(TIM2, &timerPWM);
				TIM_Cmd(TIM2, ENABLE);
				TIM_OC1Init(TIM3, &timerPWM);
				TIM_Cmd(TIM3, ENABLE);
}
int main(void)
{
			int TIM_Pulse;
			int i;
			//Init buttons
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
			GPIO_StructInit(&port);
			port.GPIO_Mode = GPIO_Mode_IPU;
			port.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14;
			port.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(GPIOC, &port);
			
			servo_init();
			TIM_Pulse = timerPWM.TIM_Pulse;
			while(1)
			{
					if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14) == 0) {
							if (TIM_Pulse < 2000)
								TIM_Pulse++;
							TIM3->CCR1 = TIM_Pulse;
					}
					if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_15) == 0) {
						if (TIM_Pulse > 1000)
									TIM_Pulse--;
						TIM3->CCR1 = TIM_Pulse;
					}
					// delay
					for(i=0;i<0x1000;i++);
			}		
}