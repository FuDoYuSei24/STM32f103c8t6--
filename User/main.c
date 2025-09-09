#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "menu.h"
#include "Timer.h"
#include "Key.h"
/**
  * �����ᶨ�壺
  * ���Ͻ�Ϊ(0, 0)��
  * ��������ΪX�ᣬȡֵ��Χ��0~127
  * ��������ΪY�ᣬȡֵ��Χ��0~63
  * 
  *       0             X��           127 
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  Y�� |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  * 
  */

int main(void)
{
	/*OLED��ʼ��*/
	OLED_Init();
	OLED_Clear();
	Peripheral_Init();
	int clkflag1;
	
	
	Timer_Init();
	while (1)
	{
		clkflag1=First_Page_Clock();
		if(clkflag1==1){}//�˵�
		else if(clkflag1==2){}//����
	}
}

// ��ʱ���жϺ��������Ը��Ƶ�ʹ�����ĵط�
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

