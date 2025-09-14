#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"

//每隔100ms，游戏分数加一
int Score;//游戏分数
void Show_Score(void){
    OLED_ShowNum(98,0,Score,5,OLED_6X8);

}

//整合小恐龙动画的函数
void DinoGame_Animation(void){
    OLED_Clear();
    Show_Score();
    OLED_Update();
}

//给Score分频的函数
void Dino_Tick(void){
    static uint8_t Score_Count;
    Score_Count++;
    if(Score_Count>=100){
        Score_Count=0;
        Score++;
    }
}