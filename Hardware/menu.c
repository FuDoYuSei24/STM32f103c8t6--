#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MyRTC.h"                                                                                                                                                                                                     
#include "Key.h"
#include "LED.h"
#include "SetTime.h"
#include "menu.h"
#include "MPU6050.h"
#include "Delay.h"
#include "MyI2C.h"
#include <math.h>
#include "dino.h"
#include "AD.h"

uint8_t KeyNum;

void Peripheral_Init(void)
{
	MyRTC_Init();
	Key_Init();
	LED_Init();
	MPU6050_Init();
	AD_Init();
}

/*----------------------------------电池界面-------------------------------------*/
//显示电池电量
uint16_t ADValue;
float VBAT;
int Battery_Capacity;

void Show_Battery(void){
	//均值滤波
	int sum;
	for(int i=0;i<3000;i++){
		ADValue = AD_GetValue();
		sum+=ADValue;
	}
	ADValue=sum/3000;

	VBAT = (float)ADValue/4095*3.3;
	Battery_Capacity=(ADValue-3276)*100/819;

	if(Battery_Capacity<0) Battery_Capacity=0;
	
	//OLED_ShowImage(64,0,ADValue,4,OLED_6X8);
	//OLED_Printf(64,8,OLED_6X8,"VBAT:%.2f",VBAT);
	OLED_ShowNum(85,4,Battery_Capacity,3,OLED_6X8);
	OLED_ShowChar(103,4,'%',OLED_6X8);

	if(Battery_Capacity==100)OLED_ShowImage(110,0,16,16,Battery);
	else if(Battery_Capacity>=10&&Battery_Capacity<100){
		OLED_ShowImage(110,0,16,16,Battery);
		OLED_ClearArea((112+Battery_Capacity/10),5,(10-Battery_Capacity/10),6);
		OLED_ClearArea(85,4,6,8);
	}
	else{
		OLED_ShowImage(110,0,16,16,Battery);
		OLED_ClearArea(112,5,10,6);
		OLED_ClearArea(85,4,12,8);
	}
}

/*----------------------------------首页时钟-------------------------------------*/


void Show_Clock_UI(void)
{
	Show_Battery();
	MyRTC_ReadTime();
	OLED_Printf(0,0,OLED_6X8,"%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
	OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
	OLED_ShowString(0,48,"菜单",OLED_8X16);
	OLED_ShowString(96,48,"设置",OLED_8X16);
}

int clkflag=1;

int First_Page_Clock(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==1)//上一项
		{
			clkflag--;
			if(clkflag<=0)clkflag=2;
		}
		else if(KeyNum==2)//下一项
		{
			clkflag++;
			if(clkflag>=3)clkflag=1;
		}
		else if(KeyNum==3)//确认
		{
			OLED_Clear();
			OLED_Update();
			return clkflag;
		}
		else if(KeyNum==4)
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	        GPIO_SetBits(GPIOB, GPIO_Pin_12);
		}
		switch(clkflag)
		{
			case 1:
				Show_Clock_UI();
				OLED_ReverseArea(0,48,32,16);
				OLED_Update();
				break;
			
			case 2:
				Show_Clock_UI();
				OLED_ReverseArea(96,48,32,16);
				OLED_Update();
				break;
		}
	}
}

/*----------------------------------设置界面-------------------------------------*/
void Show_SettingPage_UI(void){
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(0,16.0,"设置时间",OLED_8X16);
}

int setflage = 1;
int SettingPage(void){
	while(1)
	{
		KeyNum=Key_GetNum();
		uint8_t setflage_temp=0;
		if(KeyNum==1)//上一项
		{
			setflage--;
			if(setflage<=0)setflage=2;
		}
		else if(KeyNum==2)//下一项
		{
			setflage++;
			if(setflage>=3)setflage=1;
		}
		else if(KeyNum==3)//确认
		{
			OLED_Clear();
			OLED_Update();
			setflage_temp=setflage;
		}

		if(setflage_temp==1){return 0;};//说明退回键被按下，返回上一级菜单
		else if(setflage_temp==2){SetTime();};//说明设置时间键被按下，进入设置时间界面
		
		switch(setflage)
		{
			case 1:
				Show_SettingPage_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			
			case 2:
				Show_SettingPage_UI();
				OLED_ReverseArea(0,16,96,16);
				OLED_Update();
				break;
		}
	}
}

/*----------------------------------滑动菜单界面-------------------------------------*/
uint8_t pre_selection;//上次选择的选项
uint8_t target_selection;//目标选项
uint8_t x_pre = 48;//上次选项的x坐标
uint8_t Speed = 4;//滑动速度
uint8_t move_flage=0;//开始移动的标志位。1表示开始移动，0表示不移动
//显示菜单各种功能图标图像
void Menu_Animation(void){
	OLED_Clear();
	OLED_ShowImag(42,10,44,44,Frame);//边框

	if(pre_selection < target_selection){
		x_pre -= Speed;
		if(x_pre==0){
			pre_selection++;
			move_flage=0;
			x_pre=48;
		}

	}
	if(pre_selection > target_selection){
		x_pre += Speed;
		if(x_pre==96){
			pre_selection--;
			move_flage=0;
			x_pre=48;
		}
	}

	if(pre_selection>=1){
		OLED_ShowImage(x_pre-48,16,32,32,Menu_Grap[pre_selection-1]);
	}
	if(pre_selection>=2){
		OLED_ShowImage(x_pre-96,16,32,32,Menu_Grap[pre_selection-2]);
	}
	OLED_ShowImage(x_pre,16,32,32,Menu_Grap[pre_selection]);
	OLED_ShowImage(x_pre+48,16,32,32,Menu_Grap[pre_selection+1]);
	OLED_ShowImage(x_pre+96,16,32,32,Menu_Grap[pre_selection+2]);

	OLED_Update();

}

//通过按键设定pre_selection和target_selection
void Set_Selection(uint8_t move_flag,uint8_t Pre_Selection,uint8_t Target_Selection){
	if(move_flag==1){
		pre_selection=Pre_Selection;
		target_selection=Target_Selection;
	}
	Menue_Animation();
}

//菜单到具体功能的转场函数
void MenuToFunction(void){
	for(uint8_t i=0;i<=6;i++){
		OLED_Clear();
		if(pre_selection>=1){
		OLED_ShowImage(x_pre-48,16+8*i,32,32,Menu_Grap[pre_selection-1]);
	}
	OLED_ShowImage(x_pre,16+8*i,32,32,Menu_Grap[pre_selection]);
	OLED_ShowImage(x_pre+48,16+8*i,32,32,Menu_Grap[pre_selection+1]);

	OLED_Update();
	}
	
}


//滑动菜单界面要显示7个图标
uint8_t menuflage=1;
int Menu(void){
	move_flag=1;
	uint8_t DirectFlag=2;//指代移动到上一项还是下一项的标志位，1表示上一项，2表示下一项
	while(1)
	{
		KeyNum=Key_GetNum();
		uint8_t menuflage_temp=0;
		if(KeyNum==1)//上一项，DirectFlag要置1，
		{
			DirectFlag=1;
			move_flage=1;//准备移动
			menuflage--;
			if(menuflage<=0)menuflage=7;
		}
		else if(KeyNum==2)//下一项，DirectFlag要置2
		{
			DirectFlag=2;
			move_flage=1;//准备移动
			menuflage++;
			if(menuflage>=8)menuflage=1;
		}
		else if(KeyNum==3)//确认
		{
			OLED_Clear();
			OLED_Update();
			menuflage_temp=menuflage;
		}

		if(menuflage_temp==1){return 0;};
		else if(menuflage_temp==2){MenuToFunction();StopWatch();}//秒表界面
		else if(menuflage_temp==3){MenuToFunction();LED();}//手电筒界面
		else if(menuflage_temp==4){MenuToFunction();MPU6050();}//MPU6050界面
		else if(menuflage_temp==5){MenuToFunction();Game();}//小恐龙游戏界面
		else if(menuflage_temp==6){MenuToFunction();Emoji();}//动态表情包界面
		else if(menuflage_temp==7){MenuToFunction();Gradienter();}//水平仪函数


		if(move_flage==1){
			if(DirectFlag==1){Set_Selection(move_flage,1,0);}
     	    else if(DirectFlag==2){Set_Selection(move_flage,0,0);}
		}
		else{
			if(DirectFlag==1){Set_Selection(move_flage,menu_flage,menu_flage-1);}
     	    else if(DirectFlag==2){Set_Selection(move_flage,menu_flage-2,menu_flage-1);}
		}
	}
}

/*----------------------------------秒表界面-------------------------------------*/
//显示秒表UI
uint8_t hour,min,sec;
void Show_StopWatch_UI(void){
	OLED_ShowImage(0,0,16,16,Return);//左上角退回键
	OLED_Printf(32,20,OLED_8X16,"%02d:%02d:%02d",hour,min,sec);
	OLED_ShowString(8,44,"开始",OLED_8X16);
	OLED_ShowString(48,44,"停止",OLED_8X16);
	OLED_ShowString(88,44,"清除",OLED_8X16);
}

//放在定时器中断里对秒表秒数进行自增操作的函数
uint8_t start_timing_flag;//秒表开始标志位，1开始，0停止
void StopWatch_Tick(void){

	//用于计数的变量,1000分频
	static uint16_t Count;
	Count++;
	if(Count>=1000){
		Count=0;
		//秒表的时间变化
		if(start_timing_flag){
		if(start_timing_flag==1){
			sec++;
			if(sec>=60){
				sec=0;
				min++;
				if(min>=60){
					min=0;
					hour++;
					if(hour>99) hour=0;
				}
				
			}
		}
	}
	}
}

uint8_t stopwatch_flag=1;
int StopWatch(void){
	while(1)
	{
		KeyNum=Key_GetNum();
		uint8_t stopwatch_flag_temp=0;
		if(KeyNum==1)//上一项
		{
			stopwatch_flag--;
			if(stopwatch_flag<=0)stopwatch_flag=4;
		}
		else if(KeyNum==2)//下一项
		{
			stopwatch_flag++;
			if(stopwatch_flag>=5)stopwatch_flag=1;
		}
		else if(KeyNum==3)//确认
		{
			OLED_Clear();
			OLED_Update();
			stopwatch_flag_temp=stopwatch_flag;
		}

		if(stopwatch_flag_temp==1){return 0;};//说明退回键被按下，返回上一级菜单
		
		switch(stopwatch_flag)
		{
			case 1:
				Show_StopWatch_UI();//退回键处
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			
			case 2:
				Show_StopWatch_UI();//开始键处
				start_timing_flag=1;//开始计时
				OLED_ReverseArea(8,44,32,16);
				OLED_Update();
				break;

			case 3:
			    Show_StopWatch_UI();//停止键处
				start_timing_flag=0;//停止计时
				OLED_ReverseArea(48,44,32,16);
				OLED_Update();
				break;
			case 4:
			    Show_StopWatch_UI();//清除键处
				start_timing_flag=0;
				hour=min=sec=0;//清除时间
				OLED_ReverseArea(88,44,32,16);
				OLED_Update();
				break;
		}
	}
}

/*----------------------------------手电筒界面-------------------------------------*/
void Show_LED_UI(void){
	OLED_ShowImage(0,0,16,16,Return);//退回键图标
	OLED_ShowString(20,20,"OFF",OLED_12X24);
	OLED_ShowString(72,20,"ON",OLED_12X24);
}


uint8_t led_flag=1;//手电筒标志位
int LED(void){
	while(1)
	{
		KeyNum=Key_GetNum();
		uint8_t led_flag_temp=0;
		if(KeyNum==1)//上一项
		{
			led_flag--;
			if(led_flag<=0)led_flag=3;
		}
		else if(KeyNum==2)//下一项
		{
			led_flag++;
			if(led_flag>=4)led_flag=1;
		}
		else if(KeyNum==3)//确认
		{
			OLED_Clear();
			OLED_Update();
			led_flag_temp=led_flag;
		}

		if(led_flag_temp==1){return 0;};//说明退回键被按下，返回上一级菜单
		
		switch(led_flag)
		{
			case 1:
				Show_LED_UI();//退回键处
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			
			case 2:
				Show_LED_UI();//OFF键处
				LED_OFF();
				OLED_ReverseArea(20,20,36,24);
				OLED_Update();
				break;

			case 3:
			    Show_LED_UI();//ON键处
				LED_ON();
				OLED_ReverseArea(72,20,24,24);
				OLED_Update();
				break;
			
		}
	}
}

/*----------------------------------MPU6050界面-------------------------------------*/
int16_t ax,ay,az,gx,gy,gz;//三轴加速度和角速度
float roll_g,pitch_g,yaw_g;//陀螺仪的欧拉角
float roll_a,pitch_a;//加速度计解算的欧拉角
float Roll,Pitch,Yaw;//互补滤波后的欧拉角
float a=0.9;//滤波器系数
float Delta_t=0.005;//采样周期
double pi=3.1415927;//圆周率
//读取MPU6050数据进行姿态解算的函数
void MPU6050_Calculation(void){
	Delay_ms(5);
	MPU6050_GetData(&ax,&ay,&az,&gx,&gy,&gz);

	//陀螺仪解算欧拉角
	roll_g=roll_g+(float)gx*Delta_t;
	pitch_g=pitch_g+(float)gy*Delta_t;
	yaw_g=yaw_g+(float)gz*Delta_t;

	//加速度计解算欧拉角
	pitch_a=atan2((-1)*ax,az)*180/pi;
	roll_a=atan2(ay,az)*180/pi;

	//互补滤波
	Roll=a*roll_g+(1-a)*roll_a;
	Pitch=a*pitch_g+(1-a)*pitch_a;
	Yaw=a*yaw_g;
}

void Show_MPU6050_UI(void){
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(0,16,OLED_8X16,"Roll: %.2f",Roll);
	OLED_Printf(0,32,OLED_8X16,"Pitch:%.2f",Pitch);
	OLED_Printf(0,48,OLED_8X16,"Yaw:  %.2f",Yaw);
}

int MPU6050(void){
	while(1){
		KeyNum=Key_GetNum();
		if(KeyNum==3){
			OLED_Clear();
			OLED_Update();
			return 0;
		}
		OLED_Clear();
		MPU6050_Calculation();
		Show_MPU6050_UI();
		OLED_ReverseArea(0,0,16,16);
		OLED_Update();
	}
}

/*----------------------------------小恐龙游戏界面-------------------------------------*/
void Show_Game_UI(void){
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowImage(0,16,"谷歌小恐龙",OLED_8X16);
}


uint8_t game_flag=1;//游戏标志位
//光标移动函数
int Game(void){
	while(1)
	{
		KeyNum=Key_GetNum();
		uint8_t game_flag_temp=0;
		if(KeyNum==1)//上一项
		{
			game_flag--;
			if(game_flag<=0)game_flag=2;
		}
		else if(KeyNum==2)//下一项
		{
			game_flag++;
			if(game_flag>=3)game_flag=1;
		}
		else if(KeyNum==3)//确认
		{
			OLED_Clear();
			OLED_Update();
			game_flag_temp=game_flag;
		}

		if(game_flag_temp==1){return 0;};//说明退回键被按下，返回上一级菜单
		else is(game_flag_temp==2){
			DinoGame_Pos_Init();
			DinoGame_Animation();
		}
		
		switch(game_flag)
		{
			case 1:
				Show_Game_UI();//退回键处
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			
			case 2:
				Show_Game_UI();//
				LED_OFF();
				OLED_ReverseArea(0,16,80,16);
				OLED_Update();
				break;


		}
	}
}


/*----------------------------------动态表情包界面-------------------------------------*/
//显示动态表情包
void Show_Emoji_UI(void){
	//闭眼
	for(uint8_t i=0;i<3;i++){
		OLED_ShowImage(30,10+i,16,16,Eyebrow[0]);//左眉毛
		OLED_ShowImage(82,10+i,16,16,Eyebrow[1]);//右眉毛
		OLED_DrawEllipse(40,32,6,6-i,1);//画椭圆当眼睛
		OLED_DrawEllipse(88,32,6,6-i,1);
		OLED_ShowImage(54,40,20,20,Mouth);//嘴巴
		OLED_Update();
		Delay_ms(100);
	}

	//睁眼
	for(uint8_t i=0;i<3;i++){
		OLED_ShowImage(30,12-i,16,16,Eyebrow[0]);//左眉毛
		OLED_ShowImage(82,12-i,16,16,Eyebrow[1]);//右眉毛
		OLED_DrawEllipse(40,32,6,4+i,1);//画椭圆当眼睛
		OLED_DrawEllipse(88,32,6,4+i,1);
		OLED_ShowImage(54,40,20,20,Mouth);//嘴巴
		OLED_Update();
		Delay_ms(100);
	}

	Delay_ms(500);
}

int Emoji(void){
	while(1){
		KeyNum=Key_GetNum();
		if(KeyNum==3){
			OLED_Clear();
			OLED_Update();
			return 0;
		}
		Show_Emoji_UI();
	}
}

/*----------------------------------水平仪界面-------------------------------------*/
//显示水平仪函数
void Show_Gradienter_UI(void){
	MPU6050_Calculation();
	OLED_DrawCircle(64,32,30,0);//外面的大圆
	OLED_DrawCircle(64-Roll,32+Pitch,4,1);//中心的小圆
}

int Gradienter(void){
	while(1){
		KeyNum=Key_GetNum();
		if(KeyNum==3){
			OLED_Clear();
			OLED_Update();
			return 0;
		}
		OLED_Clear();
		Show_Gradienter_UI();
		OLED_Update();
	}

}