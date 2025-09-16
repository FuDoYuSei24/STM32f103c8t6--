#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"
#include <stdlib.h>

//每隔100ms，游戏分数加一
int Score;//游戏分数
uint16_t Ground_Pos;//指向地面数组数据索引值的标志位
uint8_t barrier_flag;//指向Barrier数组索引值的标志位
uint8_t Barrier_Pos;//障碍物跟地面一起移动
uint8_t Cloud_Pos;//云朵坐标
void Show_Score(void){
    OLED_ShowNum(98,0,Score,5,OLED_6X8);

}

//整合小恐龙动画的函数
void DinoGame_Animation(void){
    OLED_Clear();
    Show_Score();
    Show_Ground();
    Show_Barrier();
    OLED_Update();

}

//给Score分频的函数
void Dino_Tick(void){
    static uint8_t Score_Count,Ground_Count,Cloud_Count;
    Score_Count++;
    Ground_Count++;
    Cloud_Count++;
    if(Score_Count>=100){
        Score_Count=0;
        Score++;
    }
    if(Ground_Count>=20){
        Ground_Count=0;
        Ground_Pos++;
        Barrier_Pos++;
        if(Ground_Pos>=256){
            Ground_Pos=0;
        }
        if(Barrier_Pos>=144){
            Barrier_Pos=0;
        }
    }
    if(Cloud_Count>=50){
        Cloud_Count=0;
        Cloud_Pos++;
        if(Cloud_Pos>=200){
            Cloud_Pos=0;
        }
    }
}

//显示地面的函数
void Show_Ground(void){
    if(Ground_Pos<128){
        for(uint8_t i=0;i<128;i++){
        OLED_DisplayBuf[7][i]=Ground[i+Ground_Pos];
        }
    }
    else{
        for(uint8_t i=0;i<255-Ground_Pos;i++){
            OLED_DisplayBuf[7][i]=Ground[i+Ground_Pos];
        }
        for(uint8_t i=255-Ground_Pos;i<128;i++){
            OLED_DisplayBuf[7][i]=Ground[i-(255-Ground_Pos)];
        }
    }
}

//显示地面障碍物的函数
void Show_Barrier(void){
    if(Barrier_Pos>=143){
        barrier_flag=rand()%3;//barrier_flag随机等于0，1，2
    }
    OLED_ShowImage(127-Barrier_Pos,44,16,18,Barrier[barrier_flag]);

}

//显示云朵移动的函数
void Show_Cloud(void){
    OLED_ShowImage(127-Cloud_Pos,9,16,8,Cloud);
}