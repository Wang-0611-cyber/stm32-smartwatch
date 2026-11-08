#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include "OLED.h"
#include "Key.h"

void Show_SetTime_FirstUI(void)    //日期时间设置页面第一页
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(0,16,OLED_8X16,"年:%4d",MyRTC_Time[0]);
	OLED_Printf(0,32,OLED_8X16,"月:%2d",MyRTC_Time[1]);
	OLED_Printf(0,48,OLED_8X16,"日:%2d",MyRTC_Time[2]);
}

void Show_SetTime_SecondUI(void)    //日期时间设置页面第二页
{
	OLED_Printf(0,0,OLED_8X16,"时:%2d",MyRTC_Time[3]);
	OLED_Printf(0,16,OLED_8X16,"分:%2d",MyRTC_Time[4]);
	OLED_Printf(0,32,OLED_8X16,"秒:%2d",MyRTC_Time[5]);
}

void Change_RTC_Time(uint8_t i,uint8_t flag)    //改变MYrtc中的时钟的函数,flag=1:+1,flag=0:-1
{
	if(flag==1) {MyRTC_Time[i]++;}
	else {MyRTC_Time[i]--;}
	MyRTC_SetTime();     //把更新后的值刷新到外电路
}

extern uint8_t KeyNum; 

int SetYear(void)    //修改年份的函数
{
	while(1)
	{
		KeyNum=Key_GetNum();   //获取按键值
		if(KeyNum==1)     //如果按的是第一个按键，数值+1
		{
			Change_RTC_Time(0,1);
		}
		else if(KeyNum==2)     //如果按的是第二个按键，数值-1
		{
			Change_RTC_Time(0,0);
		}
		else if(KeyNum==3)     //如果按的是第三个按键，数值保存并推出
		{
			return 0;      //回到上一级菜单
		}

		Show_SetTime_FirstUI();
		OLED_ReverseArea(24,16,32,16);
		OLED_Update();
	}
}

int SetMonth(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();   //获取按键值
		if(KeyNum==1)     //如果按的是第一个按键，数值+1
		{
			Change_RTC_Time(1,1);
			if(MyRTC_Time[1]>=13)
			{
				MyRTC_Time[1]=1;
				MyRTC_SetTime();
			}
		}
		else if(KeyNum==2)     //如果按的是第二个按键，数值-1
		{
			Change_RTC_Time(1,0);
			if(MyRTC_Time[1]<1)
			{
				MyRTC_Time[1]=12;
				MyRTC_SetTime();
			}

		}
		else if(KeyNum==3)     //如果按的是第三个按键，数值保存并推出
		{
			return 0;      //回到上一级菜单
		}

		Show_SetTime_FirstUI();
		OLED_ReverseArea(24,32,16,16);
		OLED_Update();
	}
}

int SetDay(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();   //获取按键值
		if(KeyNum==1)     //如果按的是第一个按键，数值+1
		{
			Change_RTC_Time(2,1);
			if(MyRTC_Time[2]>=32)
			{
				MyRTC_Time[2]=1;
				MyRTC_SetTime();
			}
		}
		else if(KeyNum==2)     //如果按的是第二个按键，数值-1
		{
			Change_RTC_Time(2,0);
			if(MyRTC_Time[2]<1)
			{
				MyRTC_Time[2]=31;   //不用判断哪几个月有31or30天，RTC内部已经写好
				MyRTC_SetTime();
			}

		}
		else if(KeyNum==3)     //如果按的是第三个按键，数值保存并推出
		{
			return 0;      //回到上一级菜单
		}

		Show_SetTime_FirstUI();
		OLED_ReverseArea(24,48,16,16);
		OLED_Update();
	}
}

int SetHour(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();   //获取按键值
		if(KeyNum==1)     //如果按的是第一个按键，数值+1
		{
			Change_RTC_Time(3,1);
			if(MyRTC_Time[3]>=24)
			{
				MyRTC_Time[3]=0;
				MyRTC_SetTime();
			}
		}
		if(KeyNum==2)     //如果按的是第二个按键，数值-1
		{
			Change_RTC_Time(3,0);
			if(MyRTC_Time[3]<0)
			{
				MyRTC_Time[3]=23;   //不用判断哪几个月有31or30天，RTC内部已经写好
				MyRTC_SetTime();
			}

		}
		if(KeyNum==3)     //如果按的是第三个按键，数值保存并推出
		{
			return 0;      //回到上一级菜单
		}

		Show_SetTime_SecondUI();
		OLED_ReverseArea(24,0,16,16);
		OLED_Update();
	}
}

int SetMin(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();   //获取按键值
		if(KeyNum==1)     //如果按的是第一个按键，数值+1
		{
			Change_RTC_Time(4,1);
			if(MyRTC_Time[4]>=60)
			{
				MyRTC_Time[4]=0;
				MyRTC_SetTime();
			}
		}
		if(KeyNum==2)     //如果按的是第二个按键，数值-1
		{
			Change_RTC_Time(4,0);
			if(MyRTC_Time[4]<0)
			{
				MyRTC_Time[4]=59;   //不用判断哪几个月有31or30天，RTC内部已经写好
				MyRTC_SetTime();
			}

		}
		if(KeyNum==3)     //如果按的是第三个按键，数值保存并推出
		{
			return 0;      //回到上一级菜单
		}

		Show_SetTime_SecondUI();
		OLED_ReverseArea(24,16,16,16);
		OLED_Update();
	}	
}

int SetSec(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();   //获取按键值
		if(KeyNum==1)     //如果按的是第一个按键，数值+1
		{
			Change_RTC_Time(5,1);
			if(MyRTC_Time[5]>=60)
			{
				MyRTC_Time[5]=0;
				MyRTC_SetTime();
			}
		}
		if(KeyNum==2)     //如果按的是第二个按键，数值-1
		{
			Change_RTC_Time(5,0);
			if(MyRTC_Time[5]<0)
			{
				MyRTC_Time[5]=59;   //不用判断哪几个月有31or30天，RTC内部已经写好
				MyRTC_SetTime();
			}

		}
		if(KeyNum==3)     //如果按的是第三个按键，数值保存并推出
		{
			return 0;      //回到上一级菜单
		}

		Show_SetTime_SecondUI();
		OLED_ReverseArea(24,32,16,16);
		OLED_Update();
	}		
}


int Set_Time_flag=1;    //标志位(其实就是每个页面标志位名称不一样)
int SetTime(void)    //用按键选择光标选项修改时间
{
		while(1)
	{
		KeyNum = Key_GetNum();
		uint8_t set_time_flag_temp=0;
		
		if(KeyNum == 1)   //上一项
		{
			Set_Time_flag--;
			if(Set_Time_flag<=0) Set_Time_flag=7;
		}
		else if(KeyNum == 2)  //下一项
		{
			Set_Time_flag++;
			if(Set_Time_flag>=8) Set_Time_flag=1;
		}
		else if(KeyNum == 3)  //确定
		{
			OLED_Clear();
			OLED_Update();
			set_time_flag_temp = Set_Time_flag;
		}
		
		if(set_time_flag_temp==1) {return 0;}  //在返回处按下，返回上一页
		else if(set_time_flag_temp==2) {SetYear();}     //在年处按下，进入年的页面
		else if(set_time_flag_temp==3) {SetMonth();}     //月
		else if(set_time_flag_temp==4) {SetDay();}     //日
		else if(set_time_flag_temp==5) {SetHour();}	  //时
		else if(set_time_flag_temp==6) {SetMin();}	  //分
		else if(set_time_flag_temp==7) {SetSec();}     //秒
			
		switch(Set_Time_flag)    //亮的是哪个选项
		{
			case 1:       //此处指向光标
				OLED_Clear();
				Show_SetTime_FirstUI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();   //刷新一下
				break;
			
			case 2:       //年
				OLED_Clear();
				Show_SetTime_FirstUI();
				OLED_ReverseArea(0,16,16,16);
				OLED_Update();   //刷新一下
				break;
			
			case 3:
				OLED_Clear();
				Show_SetTime_FirstUI();
				OLED_ReverseArea(0,32,16,16);
				OLED_Update();   //刷新一下
				break;

			case 4:
				OLED_Clear();
				Show_SetTime_FirstUI();
				OLED_ReverseArea(0,48,16,16);
				OLED_Update();   //刷新一下
				break;
			
			case 5:
				OLED_Clear();
				Show_SetTime_SecondUI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();   //刷新一下
				break;
			
			case 6:
				OLED_Clear();
				Show_SetTime_SecondUI();
				OLED_ReverseArea(0,16,16,16);
				OLED_Update();   //刷新一下
				break;
			
			case 7:
				OLED_Clear();
				Show_SetTime_SecondUI();
				OLED_ReverseArea(0,32,16,16);
				OLED_Update();   //刷新一下
				break;
		}
	}

}
