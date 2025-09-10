#include "stm32f10x.h"                  // Device header
#include "SetTime.h"
#include "OLED.h"
#include "MyRTC.h"
#include "Key.h"

extern uint8_t KeyNum;//外部按键变量

//第一页
void Show_SetTime_FirstUI(void){
    OLED_ShowImage(0,0,16,16,Return);
    OLED_Printf(0,16,OLED_8X16,"年：%4d",MyRTC_Time[0]);
    OLED_Printf(0,32,OLED_8X16,"月：%2d",MyRTC_Time[1]);
    OLED_Printf(0,48,OLED_8X16,"日：%2d",MyRTC_Time[2]);
}

//第二页
void Show_SetTime_SecondUI(void){
    OLED_Printf(0,0,OLED_8X16,"时：%2d",MyRTC_Time[3]);
    OLED_Printf(0,16,OLED_8X16,"分：%2d",MyRTC_Time[4]);
    OLED_Printf(0,32,OLED_8X16,"秒：%2d",MyRTC_Time[5]);
}

void Change_RTC_Time(uint8_t i,uint8_t flag){//flag=1加，flag=0减
    if(flag==1){
        MyRTC_Time[i]++;
    }
    else if(flag==0){
        MyRTC_Time[i]--;
    }
    MyRTC_SetTime();//把修改后的时间写入RTC

}

//设置各项时间函数
int SetYear(void){
    while(1){
        KeyNum=Key_GetNum();
        if(KeyNum==1){//数值加一
            Change_RTC_Time(0,1);
        }
        else if(KeyNum==2){//数值减一
            Change_RTC_Time(0,0);
        }
        else if(KeyNum==3){//确认保存并退出
            return 0;
        }
        Show_SetTime_FirstUI();
        OLED_ReverseAera(24,16,32,16);
        OLED_Update();
    }
}

int SetMonth(void){
    while(1){
        KeyNum=Key_GetNum();
        if(KeyNum==1){//数值加一
            Change_RTC_Time(1,1);
            if(MyRTC_Time[1]>12){
                MyRTC_Time[1]=1;//月份限制在1-12
                MyRTC_SetTime();
            }
        }
        else if(KeyNum==2){//数值减一
            Change_RTC_Time(1,0);
            if(MyRTC_Time[1]<1){
                MyRTC_Time[1]=12;//月份限制在1-12
                MyRTC_SetTime();
            }
        }
        else if(KeyNum==3){//确认保存并退出
            return 0;
        }
        Show_SetTime_FirstUI();
        OLED_ReverseAera(24,32,16,16);
        OLED_Update();
    }
}

int SetDay(void){
    while(1){
        KeyNum=Key_GetNum();
        if(KeyNum==1){//数值加一
            Change_RTC_Time(2,1);
            if(MyRTC_Time[2]>31){
                MyRTC_Time[2]=1;//日期限制在1-31
                MyRTC_SetTime();
            }
        }
        else if(KeyNum==2){//数值减一
            Change_RTC_Time(2,0);
            if(MyRTC_Time[2]<1){
                MyRTC_Time[2]=31;//日期限制在1-31
                MyRTC_SetTime();
            }
        }
        else if(KeyNum==3){//确认保存并退出
            return 0;
        }
        Show_SetTime_FirstUI();
        OLED_ReverseAera(24,48,16,16);
        OLED_Update();
    }
}

int SetHour(void){
    while(1){
        KeyNum=Key_GetNum();
        if(KeyNum==1){//数值加一
            Change_RTC_Time(3,1);
            if(MyRTC_Time[3]>23){
                MyRTC_Time[3]=0;//小时限制在0-23
                MyRTC_SetTime();
            }
        }
        else if(KeyNum==2){//数值减一
            Change_RTC_Time(3,0);
            if(MyRTC_Time[3]<0){
                MyRTC_Time[3]=23;//小时限制在0-23
                MyRTC_SetTime();
            }
        }
        else if(KeyNum==3){//确认保存并退出
            return 0;
        }
        Show_SetTime_SecondUI();
        OLED_ReverseAera(24,0,16,16);
        OLED_Update();
    }
}

int SetMinute(void){
    while(1){
        KeyNum=Key_GetNum();
        if(KeyNum==1){//数值加一
            Change_RTC_Time(4,1);
            if(MyRTC_Time[4]>59){
                MyRTC_Time[4]=0;//分钟限制在0-59
                MyRTC_SetTime();
            }
        }
        else if(KeyNum==2){//数值减一
            Change_RTC_Time(4,0);
            if(MyRTC_Time[4]<0){
                MyRTC_Time[4]=59;//分钟限制在0-59
                MyRTC_SetTime();
            }
        }
        else if(KeyNum==3){//确认保存并退出
            return 0;
        }
        Show_SetTime_SecondUI();
        OLED_ReverseAera(24,16,16,16);
        OLED_Update();
    }
}

int SetSecond(void){
    while(1){
        KeyNum=Key_GetNum();
        if(KeyNum==1){//数值加一
            Change_RTC_Time(5,1);
            if(MyRTC_Time[5]>59){
                MyRTC_Time[5]=0;//秒钟限制在0-59
                MyRTC_SetTime();
            }
        }
        else if(KeyNum==2){//数值减一
            Change_RTC_Time(5,0);
            if(MyRTC_Time[5]<0){
                MyRTC_Time[5]=59;//秒钟限制在0-59
                MyRTC_SetTime();
            }
        }
        else if(KeyNum==3){//确认保存并退出
            return 0;
        }
        Show_SetTime_SecondUI();
        OLED_ReverseAera(24,32,16,16);
        OLED_Update();
    }
}

//按键设置时间
int set_time_flag=1;
int SetTime(void){
    while(1)
	{
		KeyNum=Key_GetNum();
		uint8_t set_time_flag_temp=0;
		if(KeyNum==1)//上一项
		{
			set_time_flag--;
			if(sset_time_flag<=0)set_time_flag=7;
		}
		else if(KeyNum==2)//下一项
		{
			set_time_flag++;
			if(set_time_flag>=8)set_time_flag=1;
		}
		else if(KeyNum==3)//确认
		{
			OLED_Clear();
			OLED_Update();
            set_time_flag_temp=set_time_flag;
		}
        if(set_time_flag_temp==1){return 0;};//返回上一级菜单
        else if(set_time_flag_temp==2){SetYear();};//年选项按下
		else if(set_time_flag_temp==3){ SetMonth();};//月选项按下
        else if(set_time_flag_temp==4){SetDay();};//日选项按下
        else if(set_time_flag_temp==5){SetHour();};//时选项按下
        else if(set_time_flag_temp==6){SetMinute();};//分选项按下
        else if(set_time_flag_temp==7){ SetSecond();};//秒选项按下

		switch(set_time_flag)
		{
			case 1://返回图标
                OLED_Clear();  
				Show_SetTime_FirstUI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			
			case 2://年
                OLED_Clear(); 
				Show_SetTime_FirstUI();
				OLED_ReverseArea(0,16,16,16);
				OLED_Update();
				break;
            case 3://月
                OLED_Clear(); 
                Show_SetTime_FirstUI();
                OLED_ReverseArea(0,32,16,16);
				OLED_Update();
				break;
            case 4://日
                OLED_Clear(); 
                Show_SetTime_FirstUI();
                OLED_ReverseArea(0,48,16,16);
				OLED_Update();
				break;
            case 5://时
                OLED_Clear(); 
                Show_SetTime_SecondUI();
                OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
            case 6://分
                OLED_Clear(); 
                Show_SetTime_SecondUI();
                OLED_ReverseArea(0,16,16,16);
				OLED_Update();
				break;
            case 7://秒
                OLED_Clear(); 
                Show_SetTime_SecondUI();
                OLED_ReverseArea(0,32,16,16);
				OLED_Update();
				break;
		}
	}
}