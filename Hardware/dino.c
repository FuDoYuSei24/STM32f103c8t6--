#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"
#include <stdlib.h>
#include <math.h>
#include "Delay.h"


//物体碰撞体积结构体
struct Object_Position
{
    uint8_t minX,minY,maxX,maxY;
};
//每隔100ms，游戏分数加一
int Score;//游戏分数
uint16_t Ground_Pos;//指向地面数组数据索引值的标志位
uint8_t barrier_flag;//指向Barrier数组索引值的标志位
uint8_t Barrier_Pos;//障碍物跟地面一起移动
uint8_t Cloud_Pos;//云朵坐标
uint8_t dino_jump_flag=0;//小恐龙是否跳跃标志位
extern uint8_t KeyNum;//按键值
uint16_t jump_t;//用来给跳跃分频的值
uint8_t Jump_Pos;//用来存放跳跃唯一的变量
extern double pi;
struct Object_Position barrier;//障碍物的碰撞体积
struct Object_Position dino;//小恐龙的碰撞体积


void Show_Score(void){
    OLED_ShowNum(98,0,Score,5,OLED_6X8);

}

//整合小恐龙动画的函数
void DinoGame_Animation(void){
    OLED_Clear();
    Show_Score();
    Show_Ground();
    Show_Barrier();
    Show_Dino();
    OLED_Update();
    isColliding(&dino,&barrier);

}

//给Score分频的函数
void Dino_Tick(void){
    static uint8_t Score_Count,Ground_Count,Cloud_Count;
    Score_Count++;
    Ground_Count++;
    Cloud_Count++;

    //游戏分数
    if(Score_Count>=100){
        Score_Count=0;
        Score++;
    }

    //地面移动
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

    //云朵移动
    if(Cloud_Count>=50){
        Cloud_Count=0;
        Cloud_Pos++;
        if(Cloud_Pos>=200){
            Cloud_Pos=0;
        }
    }

    //小恐龙跳跃
    if(dino_jump_flag==1){
        jump_t++;
        if(jump_t>=1000){
            dino_jump_flag=0;
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

    barrier.minX=127-Barrier_Pos;
    barrier.maxX=143-Barrier_Pos;
    barrier.minY=44;
    barrier.maxY=62;
}

//显示云朵移动的函数
void Show_Cloud(void){
    OLED_ShowImage(127-Cloud_Pos,9,16,8,Cloud);
}

//显示小恐龙的函数
void Show_Dino(void){
    //通过按键判断小恐龙是否进行跳跃
    KeyNum=Key_GetNum();//
    if(KeyNum==1){
        dino_jump_flag=1;//跳跃
    }
    else{
        dino_jump_flag=0;//奔跑
    }

    Jump_Pos=28*sin((float)(pi*jump_t/1000));

    if(dino_jump_flag==0){//小恐龙处于奔跑状态
       if(Cloud_Pos%2==0){
        OLED_ShowImage(0,44,16,18,Dino[0]);
       }
       else 
       {
        OLED_ShowImage(0,44,16,18,Dino[1]);
       }
    }
    else{//小恐龙处于跳跃状态
        OLED_ShowImage(0,4-Jump_Pos,16,18,Dino[2]);
    }

    dino.minX=0;
    dino.maxX=16;
    dino.minY=44-Jump_Pos;
    dino.maxY=62-Jump_Pos;

}

//碰撞检测函数
void isColliding(struct Object_Position *a,struct Object_Position *b){
    if((a->maxX>b->minX)&&(a->minX<b->maxX)&&(a->maxX>b->minY)&&(a->minY<b->maxX)){//检测到碰撞发生
        OLED_Clear();
        OLED_ShowImage(28,24,"Game Over",OLED_8X16);
        OLED_Update();
        Delay_s(1);
        OLED_Clear();
        OLED_Update();
    }
}
