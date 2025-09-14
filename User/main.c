#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "menu.h"
#include "Timer.h"
#include "Key.h"
#include "dino.h"
/**
  * 坐标轴定义：
  * 左上角为（0，0）
  * 横向向右是x轴，0~127
  * 纵向向下是y轴，0~63
  * 
  *       0             X轴           127 
  *      .------------------------------->
  *    0 |
  *      |
  *      |
  *      |
  *  Y轴 |
  *      |
  *      |
  *      |
  *   63 |
  *      v
  * 
  */

int main(void)
{
	/*OLED初始化*/
	OLED_Init();
	OLED_Clear();
	Peripheral_Init();
	int clkflag1;
	
	
	Timer_Init();
	while (1)
	{
		clkflag1=First_Page_Clock();
		if(clkflag1==1){Menu();}//菜单
		else if(clkflag1==2){SettingPage}//设置

    DinoGame_Animation();
	}
}

// 定时器2中断服务程序
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Key_Tick();
    StopWatch_Tick();//秒表的时间增加函数
    Dino_Tick();//恐龙游戏分数增加函数
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

