#include "stm32f10x.h"                  // Device header
#include "MyRTC.h" 
#include "OLED.h"
#include "Key.h"
#include "Led.h"
#include "SetTime.h"
#include "MPU6050.h"
#include "Delay.h"
#include <math.h>
#include "dino.h"

uint8_t KeyNum;

void Peripheral_Init(void)  //外设初始化函数
{
	MyRTC_Init();
	Key_Init();
	LED_Init();
	MPU6050_Init();
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

int First_Page_Clock(void)   //首页通过按键进入不同选项
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

/*-----------------------------设置界面-----------------------------*/
void Show_SettingPage_UI(void)    //设置显示页面
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(0,16,"日期时间设置",OLED_8X16);
}

int setflag=1;

int SettingPage(void)      //按键控制光标选择选项并进入切换页面
{
	while(1)
	{
		KeyNum = Key_GetNum();
		uint8_t setflag_temp = 0;
		
		if(KeyNum == 1)   //上一项
		{
			setflag--;
			if(setflag<=0) setflag=2;
		}
		else if(KeyNum == 2)  //下一项
		{
			setflag++;
			if(setflag>=3) setflag=1;
		}
		else if(KeyNum == 3)  //确定
		{
			OLED_Clear();
			OLED_Update();
			setflag_temp = setflag;
		}
		
		if(setflag_temp==1) {return 0;}    //如果按下返回键，就会返回主页(退出循环)
		else if(setflag_temp==2) {SetTime();}    //如果按下第二行，就会进入下一个年月日页面
		
		switch(setflag)
		{
			case 1:
				Show_SettingPage_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();   //刷新一下
				break;
			
			case 2:
				Show_SettingPage_UI();
				OLED_ReverseArea(0,16,96,16);
				OLED_Update();   //刷新一下
				break;
		}
	}
}
	

/*-----------------------------滑动菜单界面-----------------------------*/

uint8_t pre_selection;      //上次选择的选项
uint8_t target_selection;  //目标选项  
uint8_t x_pre=48;         //上次选择选项的x坐标
uint8_t Speed=4;           //图标每次向左移动的像素个数
uint8_t move_flag;       //是否开始移动的标志位，1:开始移动，0:停止移动

void Menu_Animation(void)          //显示滑动菜单动画的函数
{
	OLED_Clear();
	OLED_ShowImage(42,10,44,44,Frame);   //显示边框
	
	if(pre_selection<target_selection)    //如果目标图标在当前选项右边
	{
		x_pre-=Speed;      //把整体图标往左移(光标不动)
		if(x_pre==0)          //当上次图标已经移到最左边，即目标图标已经移到屏幕中间
		{
			pre_selection++;           //目标图标变成新一轮的上一次图案
			move_flag=0;
			x_pre=48;                 //重新赋值48，开始新一轮
		}
	}
	
	if(pre_selection>target_selection)    //如果目标图标在当前选项左边
	{
		x_pre+=Speed;      
		if(x_pre==96)          
		{
			pre_selection--;
			move_flag=0;
			x_pre=48;                 
		}
	}
	
	if(pre_selection>=1)     //只有当前不是第0个图标时，左边那一张图标才会画出来
	{
		OLED_ShowImage(x_pre-48,16,32,32,Menu_Graph[pre_selection-1]);
	}
	
	if(pre_selection>=2)     //只有当前不是第0个图标时，左边那一张图标才会画出来
	{
		OLED_ShowImage(x_pre-96,16,32,32,Menu_Graph[pre_selection-2]);
	}
	
	OLED_ShowImage(x_pre,16,32,32,Menu_Graph[pre_selection]);
	OLED_ShowImage(x_pre+48,16,32,32,Menu_Graph[pre_selection+1]);
	OLED_ShowImage(x_pre+96,16,32,32,Menu_Graph[pre_selection+2]);	
	
	OLED_Update();
}

void Set_Selection(uint8_t move_flag,uint8_t Pre_Selection,uint8_t Target_Selection)  //通过按键设定上一个图案和目标图案的函数
{
	if(move_flag==1)
	{
		pre_selection=Pre_Selection;
		target_selection=Target_Selection;
	}
		Menu_Animation();
}

void MenuToFunction(void)            //过渡动画
{
	for(uint8_t i=0;i<=6;i++)
	{
		OLED_Clear();
		if(pre_selection>=1)     //只有当前不是第0个图标时，左边那一张图标才会画出来
		{
			OLED_ShowImage(x_pre-48,16+8*i,32,32,Menu_Graph[pre_selection-1]);
		}
		
		
		OLED_ShowImage(x_pre,16+8*i,32,32,Menu_Graph[pre_selection]);
		OLED_ShowImage(x_pre+48,16+8*i,32,32,Menu_Graph[pre_selection+1]);
		
		OLED_Update();		
	}

}
	

uint8_t menu_flag=1;
int Menu(void)
{
	move_flag=1;
	uint8_t DirectFlag=2;   //指代移动到上一项还是下一项的标志位，1:移动到上一项；2:移动到下一项
	
	while(1)
	{
		KeyNum = Key_GetNum();
		uint8_t menu_flag_temp = 0;
		
		if(KeyNum == 1)   //上一项
		{
			DirectFlag=1;
			move_flag=1;
			menu_flag--;
			if(menu_flag<=0) menu_flag=7;
		}
		else if(KeyNum == 2)  //下一项
		{
			DirectFlag=2;
			move_flag=1;
			menu_flag++;
			if(menu_flag>=8) menu_flag=1;
		}
		else if(KeyNum == 3)  //确定
		{
			OLED_Clear();
			OLED_Update();
			menu_flag_temp = menu_flag;
		}
		
		if(menu_flag_temp==1) {return 0;}    //如果按下返回键，就会返回主页(退出循环)
		else if(menu_flag_temp==2) {MenuToFunction();StopWatch();}    //如果按下第二行，就会进入第二个秒表页面
		else if(menu_flag_temp==3) {MenuToFunction();LED();}
		else if(menu_flag_temp==4) {MenuToFunction();MPU6050();}
		else if(menu_flag_temp==5) {MenuToFunction();Game();}	
		else if(menu_flag_temp==6) {MenuToFunction();Emoji();}
		else if(menu_flag_temp==7) {}
			
//			switch(menu_flag)
//		{
//			case 1:
//				if(DirectFlag==1) Set_Selection(move_flag,1,0);
//			else if(DirectFlag==2) Set_Selection(move_flag,0,0);
//				break;
//			
//			case 2:
//				if(DirectFlag==1) Set_Selection(move_flag,2,1);
//			else if(DirectFlag==2) Set_Selection(move_flag,0,1);				
//				break;
//			
//			case 3:
//				if(DirectFlag==1) Set_Selection(move_flag,3,2);
//			else if(DirectFlag==2) Set_Selection(move_flag,1,2);
//				break;
//			
//			case 4:
//				if(DirectFlag==1) Set_Selection(move_flag,4,3);
//			else if(DirectFlag==2) Set_Selection(move_flag,2,3);
//				break;
//			
//			case 5:
//				if(DirectFlag==1) Set_Selection(move_flag,5,4);
//			else if(DirectFlag==2) Set_Selection(move_flag,3,4);
//				break;
//			
//			case 6:
//				if(DirectFlag==1) Set_Selection(move_flag,6,5);
//			else if(DirectFlag==2) Set_Selection(move_flag,4,5);
//				break;
//			
//			case 7:
//				if(DirectFlag==1) Set_Selection(move_flag,7,6);
//			else if(DirectFlag==2) Set_Selection(move_flag,5,6);
//				break;
//			
//			
//		}

		if(menu_flag==1)     //简化上面步骤
		{
			if(DirectFlag==1) Set_Selection(move_flag,1,0);
			else if(DirectFlag==2) Set_Selection(move_flag,0,0);
		}	
		else
		{
			if(DirectFlag==1) Set_Selection(move_flag,menu_flag,menu_flag-1);
			else if(DirectFlag==2) Set_Selection(move_flag,menu_flag-2,menu_flag-1);
		}
	}	
}


/*-----------------------------秒表-----------------------------*/

uint8_t hour,min,sec;
void Show_StopWatch_UI(void)    //显示秒表界面UI
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(32,20,OLED_8X16,"%02d:%02d:%02d",hour,min,sec);
	OLED_ShowString(8,44,"开始",OLED_8X16);
	OLED_ShowString(48,44,"停止",OLED_8X16);
	OLED_ShowString(88,44,"清除",OLED_8X16);
}

uint8_t start_timing_flag;       //开始计时标志位，1：开始计时，0：停止计时
void StopWatch_Tick(void)       //中断进行秒表计数
{
	static uint16_t Count;
	Count++;
	if(Count>=1000)
	{
		Count=0;
		if(start_timing_flag==1)     //开始计时
		{
			sec++;
			if(sec>=60)
			{
				sec=0;
				min++;
				if(min>=60)
				{
					min=0;
					hour++;
					if(hour>99) hour=0;
				}
			}
		}
	}
}


uint8_t stopwatch_flag=1;      //光标在第几项
int StopWatch(void)            //光标在秒表界面中移动
{
	while(1)
	{
		KeyNum = Key_GetNum();
		uint8_t stopwatch_flag_temp = 0;
		
		if(KeyNum == 1)   //上一项
		{
			stopwatch_flag--;
			if(stopwatch_flag<=0) stopwatch_flag=4;
		}
		else if(KeyNum == 2)  //下一项
		{
			stopwatch_flag++;
			if(stopwatch_flag>=5) stopwatch_flag=1;
		}
		else if(KeyNum == 3)  //确定
		{
			OLED_Clear();
			OLED_Update();
			stopwatch_flag_temp = stopwatch_flag;
		}
		
		if(stopwatch_flag_temp==1) {return 0;}    //如果按下返回键，就会返回主页(退出循环)
		
		switch(stopwatch_flag)
		{
			case 1:
				Show_StopWatch_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();   //刷新一下
				break;
			
			case 2:      //开始计时按键
				Show_StopWatch_UI();
				start_timing_flag=1;
				OLED_ReverseArea(8,44,32,16);
				OLED_Update();   //刷新一下
				break;
			
			case 3:      //停止计时按键
				Show_StopWatch_UI();
				start_timing_flag=0;
				OLED_ReverseArea(48,44,32,16);
				OLED_Update();   //刷新一下
				break;

			case 4:      //清除数据按键
				Show_StopWatch_UI();
				start_timing_flag=0;
				hour=min=sec=0;
				OLED_ReverseArea(88,44,32,16);
				OLED_Update();   //刷新一下
				break;
			
		}
	}	
}


/*-----------------------------手电筒-----------------------------*/

void Show_LED_UI(void)    //显示手电筒界面UI
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(20,20,"OFF",OLED_12X24);
	OLED_ShowString(72,20,"ON",OLED_12X24);
}

uint8_t led_flag=1;
int LED(void)         //光标在LED界面中移动
{
	while(1)
	{
		KeyNum = Key_GetNum();
		uint8_t led_flag_temp = 0;
		
		if(KeyNum == 1)   //上一项
		{
			led_flag--;
			if(led_flag<=0) led_flag=3;
		}
		else if(KeyNum == 2)  //下一项
		{
			led_flag++;
			if(led_flag>=4) led_flag=1;
		}
		else if(KeyNum == 3)  //确定
		{
			OLED_Clear();
			OLED_Update();
			led_flag_temp = led_flag;
		}
		
		if(led_flag_temp==1) {return 0;}    //如果按下返回键，就会返回主页(退出循环)
		
		switch(led_flag)
		{
			case 1:
				Show_LED_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();   //刷新一下
				break;
			
			case 2:      
				Show_LED_UI();
				LED_OFF();
				OLED_ReverseArea(20,20,32,24);
				OLED_Update();   //刷新一下
				break;
			
			case 3:    
				Show_LED_UI();
				LED_ON();
				OLED_ReverseArea(72,20,24,24);
				OLED_Update();   //刷新一下
				break;
			
		}
	}		
}


/*-----------------------------MPU6050-----------------------------*/

int16_t ax,ay,az,gx,gy,gz;             //存放MPU6050测得的三轴加速度和角速度
float roll_g,pitch_g,yaw_g;             //存放陀螺仪解算的欧拉角
float roll_a,pitch_a;                   //存放加速度计解算的欧拉角
float Roll,Pitch,Yaw;                   //存放互补滤波后的欧拉角
float a=0.9;                            //互补滤波器系数
float Delta_t=0.005;                        //采样周期
double pi=3.1415927;                    //Π

void MPU6050_Calculation(void)         //读取MPU6050数据进行姿态解算
{
	Delay_ms(5);
	MPU6050_GetData(&ax,&ay,&az,&gx,&gy,&gz);
	
	//通过陀螺仪结算欧拉角
	roll_g=Roll+(float)gx*Delta_t;
	pitch_g=Pitch+(float)gy*Delta_t;
	yaw_g=Yaw+(float)gz*Delta_t;
	
	//通过加速度计解算欧拉角
	pitch_a=atan2((-1)*ax,az)*180/pi;
	roll_a=atan2(ay,az)*180/pi;
	
	//通过互补滤波器进行数据融合
	Roll=a*roll_g+(1-a)*roll_a;
	Pitch=a*pitch_g+(1-a)*pitch_a;
	Yaw=a*yaw_g;
}

void Show_MPU6050_UI(void)          //显示欧拉角数值的页面
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_Printf(0,16,OLED_8X16,"Roll: %.2f",Roll);
	OLED_Printf(0,32,OLED_8X16,"Pitch:%.2f",Pitch);
	OLED_Printf(0,48,OLED_8X16,"Yaw:  %.2f",Yaw);
}

int MPU6050(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==3)
		{
			OLED_Clear();
			OLED_Update();    //更新一下
			return 0;
		}
		
		OLED_Clear();
		MPU6050_Calculation();
		Show_MPU6050_UI();
		OLED_ReverseArea(0,0,16,16);
		OLED_Update();
	}
}

/*-----------------------------恐龙游戏-----------------------------*/

void Show_Game_UI(void)          //游戏界面
{
	OLED_ShowImage(0,0,16,16,Return);
	OLED_ShowString(0,16,"谷歌小恐龙",OLED_8X16);
}

uint8_t game_flag=1;
int Game(void)
{
	while(1)
	{
		KeyNum = Key_GetNum();
		uint8_t game_flag_temp = 0;
		
		if(KeyNum == 1)   //上一项
		{
			game_flag--;
			if(game_flag<=0) game_flag=2;
		}
		else if(KeyNum == 2)  //下一项
		{
			game_flag++;
			if(game_flag>=3) game_flag=1;
		}
		else if(KeyNum == 3)  //确定
		{
			OLED_Clear();
			OLED_Update();
			game_flag_temp = game_flag;
		}
		
		if(game_flag_temp==1) {return 0;}    //如果按下返回键，就会返回主页(退出循环)
		else if(game_flag_temp==2) {DinoGame_Pos_Init();DinoGame_Animation();}
		
		switch(game_flag)
		{
			case 1:
				Show_Game_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();   //刷新一下
				break;
			
			case 2:      
				Show_Game_UI();
				OLED_ReverseArea(0,16,80,16);
				OLED_Update();   //刷新一下
				break;			
		}
	}			
}


/*-----------------------------动态表情包-----------------------------*/

void Show_Emoji_UI(void)
{
      //闭眼操作
	for(uint8_t i=0;i<3;i++)
	{
		OLED_Clear();
		OLED_ShowImage(30,10+i,16,16,Eyebrow[0]);    //左眉毛
		OLED_ShowImage(82,10+i,16,16,Eyebrow[1]);    //右眉毛
		OLED_DrawEllipse(40,32,6,6-i,1);             //睁眼睁到最大时(左眼)
		OLED_DrawEllipse(88,32,6,6-i,1);             //睁眼睁到最大时(右眼)
		OLED_ShowImage(54,40,20,20,Mouth);
		OLED_Update();
		Delay_ms(100);
	}
	
	    //睁眼操作
	for(uint8_t i=0;i<3;i++)
	{
		OLED_Clear();
		OLED_ShowImage(30,12-i,16,16,Eyebrow[0]);    //左眉毛
		OLED_ShowImage(82,12-i,16,16,Eyebrow[1]);    //右眉毛
		OLED_DrawEllipse(40,32,6,4+i,1);             //睁眼睁到最大时(左眼)
		OLED_DrawEllipse(88,32,6,4+i,1);             //睁眼睁到最大时(右眼)
		OLED_ShowImage(54,40,20,20,Mouth);
		OLED_Update();
		Delay_ms(100);
	}
		Delay_ms(500);
}

int Emoji(void)
{
	while(1)
	{
		KeyNum=Key_GetNum();
		if(KeyNum==3)
		{
			OLED_Clear();
			OLED_Update();    //更新一下
			return 0;
		}
	Show_Emoji_UI();
	}		
}

