#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MyRTC.h"                                                                                                                                                                                                     
#include "Key.h"
#include "LED.h"
#include "SetTime.h"


uint8_t KeyNum;

void Peripheral_Init(void)
{
	MyRTC_Init();
	Key_Init();
	LED_Init();
}



/*----------------------------------首页时钟-------------------------------------*/


void Show_Clock_UI(void)
{
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
		uint8_t setflag_temp;
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
			setflag_temp=setflag;
		}
		if(setflag_temp==1){return 0;};
		else if(setflag_temp==2){sETTime();};

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
		Menue_Animation();
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
		else if(menuflage_temp==2){};
		else if(menuflage_temp==3){};
		else if(menuflage_temp==4){};
		else if(menuflage_temp==5){};
		else if(menuflage_temp==6){};
		else if(menuflage_temp==7){};


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