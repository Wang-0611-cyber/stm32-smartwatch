#include "stm32f10x.h"                  // Device header
#include "MyRTC.h" 
#include "OLED.h"
#include "Key.h"
#include "Led.h"

uint8_t KeyNum;

void Peripheral_Init(void)  //外设初始化函数
{
	MyRTC_Init();
	Key_Init();
	LED_Init();
}

/*-----------------------------首页时钟-----------------------------*/

void Show_Clock_UI(void)     //显示时钟首页页面
{
	MyRTC_ReadTime();
	OLED_Printf(0,0,OLED_6X8,"%d-%d-%d,",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
	OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d,",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
	OLED_ShowString(0,48,"菜单",OLED_8X16);
	OLED_ShowString(96,48,"设置",OLED_8X16);
}

int clkflag=1;  //定义一个全局变量，菜单=1,设置=2,初始默认光标在菜单处

int First_Page_Clock(void)   //存放首页时钟通过按键进入不同选项的代码
{
	while(1)
	{
		KeyNum = Key_GetNum();
		
		if(KeyNum == 1)   //上一项
		{
			clkflag--;
			if(clkflag<=0) clkflag=2;
		}
		else if(KeyNum == 2)  //下一项
		{
			clkflag++;
			if(clkflag>=3) clkflag=1;
		}
		else if(KeyNum == 3)  //确定
		{
			OLED_Clear();
			OLED_Update();
			return clkflag;
		}
		
		switch(clkflag)
		{
			case 1:
				Show_Clock_UI();
				OLED_ReverseArea(0,48,32,16);
				OLED_Update();   //刷新一下
				break;
			
			case 2:
				Show_Clock_UI();
				OLED_ReverseArea(96,48,32,16);
				OLED_Update();   //刷新一下
				break;
		}
	}
}
