/****************************************Copyright (c)**************************************************
**				
**	项目名称:			YS18-3 Clock
**	文件名称:			Main.c
**	模块功能:			主程序
**
**------------------------------------------------------------------------------------------------------

** 	创 建 者:			严泽远
**	E-mail  :			yanzeyuan@163.com
**	QQ		:			6626209
**	Mobile	:			18602007878
**	创建时间:			2011-01-31
**	版    本:			v1.0.0
**	描    述:			基础程序
**
********************************************************************************************************/

#include <STC12C5620AD.H>				//加载STC12C5620AD.H头文件
#include <Define.H>						//加载Define.H头文件
#include <MainVariable.H>				//加载MainVariable.H头文件
#include <LPD6803Function.h>			//加在LPD6803Function.h头文件

/********************************************************************************************************
** 	函数声明
********************************************************************************************************/
void 	IR_RX(void);
bit 	DispNum(uchar * N,uchar Mode);
void	GetDateTime();
void	AlarmReset();
void	AlarmInit();
void	AlarmSave(uchar N,uchar H,uchar M,uchar ONOFF);
void	FormatInit();
void	FormatSave();

/********************************************************************************************************
** 	函数名称:			void	Port_IO_Init()
**	功能描述:			IO端口配置
**	创 建 者:			严泽远
**	创建时间:			2011-02-01 00:02
**	版    本:			v1.0.0
********************************************************************************************************/
void	Port_IO_Init()
{
    P1M1=0x3F;  	//设置P1端口模式
    P3M1=0x80;  	//设置P3端口模式
}
																								  
/********************************************************************************************************
** 	函数名称:			void	DelayMS(uint N)
**	功能描述:			延时N毫秒
**	创 建 者:			严泽远
**	创建时间:			2011-02-01 17:48
**	版    本:			v1.0.0
********************************************************************************************************/
void	DelayMS(uint N)	  
{
	uint i,j;
	for(j=0;j<N;j++)
	for(i=0;i<2050;i++)
	{;}
}

/********************************************************************************************************
** 	函数名称:			void	TimerInit()
**	功能描述:			定时器初始化
**	创 建 者:			严泽远
**	创建时间:			2011-02-02 15:39
**	版    本:			v1.0.0
********************************************************************************************************/
void	TimerInit()
{	
	AUXR	=	0xC0;			//Timer0 12T Timer1 1T
	
	TMOD	=	0x11;			//Set Timer0 & Timer1 as mode1(16bit)
	
	TL1		=	T1MS;			//Initial timer1 low byte
	TH1		=	T1MS>>8;		//Initial timer1 high byte
	TR1		=	1;				//Timer1 start running

	TL0		=	T125US;			//Initial timer0 low byte
	TH0		=	T125US>>8;		//Initial timer0 high byte
	TR0		=	1;				//Timer0 start running
}  
							
/********************************************************************************************************
** 	函数名称:			void	Timer0Interrupt() interrupt 1
**	功能描述:			定时器0中断函数
**	创 建 者:			严泽远
**	创建时间:			2011-02-02 15:39
**	版    本:			v1.0.0
********************************************************************************************************/
void	Timer0Interrupt() interrupt 1
{	   	 
	TL0		=	T125US;			//Reload timer0 low byte
	TH0		=	T125US>>8;		//Reload timer0 high byte
	IR_RX();  	
	//***************************************************************************************************
	//冒号闪烁PWM
	if(DOTpwmloop<31)		DOTpwmloop++;	else	DOTpwmloop=0;	
	if(DOTpwmloop<=DOTcut)	DOTS=!DOTonoff;	else	DOTS=DOTonoff;				
}
							
/********************************************************************************************************
** 	函数名称:			void	Timer1Interrupt() interrupt 3
**	功能描述:			定时器1中断函数
**	创 建 者:			严泽远
**	创建时间:			2011-02-02 15:39
**	版    本:			v1.0.0
********************************************************************************************************/
void	Timer1Interrupt() interrupt 3
{	   	 
	TL1		=	T1MS;			//Reload timer1 low byte
	TH1		=	T1MS>>8;		//Reload timer1 high byte

	if(RGBloop<1024)	RGBloop++;
	if(DOTloop<DOTLOOPCOUNT)	DOTloop++;	else{DOTloop=0;if(DOTcut<31)	DOTcut++;}
	if(NUMloop<NUMLOOPCOUNT)	NUMloop++;	else{NUMloop=0;if(NUMcut<20)	NUMcut++;}
	if(NUMmode==1)
	{
		if(NUM500msloop<1000)		NUM500msloop++;
		if(NUM500msloop==300)		NUMcut=0;
	}
	
			 	
	F_50ms_Loop++;
	F_100ms_Loop++;
	F_250ms_Loop++;
	F_500ms_Loop++;
	F_1s_Loop++;
	F_2s_Loop++;

	if(F_50ms_Loop	>=50)	{F_50ms		=	1;	F_50ms_Loop			=	0;}
	if(F_100ms_Loop	>=100)	{F_100ms	=	1;	F_100ms_Loop		=	0;}
	if(F_250ms_Loop	>=250)	{F_250ms	=	1;	F_250ms_Loop		=	0;}		   
	if(F_500ms_Loop	>=500)	{F_500ms	=	1;	F_500ms_Loop		=	0;}		   
	if(F_1s_Loop	>=1000)	{F_1s		=	1;	F_1s_Loop			=	0;}	
	if(F_2s_Loop	>=2000)	{F_2s		=	1;	F_2s_Loop			=	0;}	
}

/********************************************************************************************************
** 	函数名称:			void	Beep(uchar AT)
**	功能描述:			提示音
**	创 建 者:			严泽远
**	创建时间:			2011-02-01 23:09
**	版    本:			v1.0.0
********************************************************************************************************/
void	Beep(uchar AT)
{
	ulong i,j;
	uchar D=35,E=32,Y=3;
	switch(AT)
	{
	 	case BEEPPOWERON:	 			//正常开机提示音		·
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;};
			break;
		case BEEPSHUTDOWN:				//正常关机提示音		—··
			j=0;while(j<1800){SPK=!SPK;for(i=0;i<D;i++){;}j++;}for(i=0;i<15;i++)DelayMS(Y);
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;}for(i=0;i<15;i++)DelayMS(Y);
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;}
			break;
		case BEEPERROR:					//错误提示音		····
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;}for(i=0;i<15;i++)DelayMS(Y);
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;}for(i=0;i<15;i++)DelayMS(Y);
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;}for(i=0;i<15;i++)DelayMS(Y);
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;}
			break;
		case BEEPCONFIRM:				//确认提示音			—
			j=0;while(j<2800){SPK=!SPK;for(i=0;i<D;i++){;}j++;}
			break;
		case BEEPOPERATE:				//正常操作按键音		·
			j=0;while(j<400){SPK=!SPK;for(i=0;i<E;i++){;}j++;};
			break;	   	
	}
	SPK=0;
}

/********************************************************************************************************
** 	函数名称:			void	RGB1s()
**	功能描述:			RGB变换一次模式
**	创 建 者:			严泽远
**	创建时间:			2011-02-02 16:29
**	版    本:			v1.0.0
********************************************************************************************************/
void	RGB1s()
{
	RGBloop	=	0;
	if(RGBlastcolor<13)	RGBlastcolor++;else	 RGBlastcolor=0;
}

/********************************************************************************************************
** 	函数名称:			void	RGBFlash(uchar M)
**	功能描述:			RGB LED闪烁
**	入口参数:			M:	RGB闪烁模式
**	创 建 者:			严泽远
**	创建时间:			2011-02-02 16:29
**	版    本:			v1.0.0
********************************************************************************************************/
void	RGBFlash(uchar M)
{
	if((RGBloop/RGBLOOPCOUNT)==RGBlaststate)	return;	  
	RGBlaststate=(RGBloop/RGBLOOPCOUNT);

	switch(M)
	{
		case	RGBMOFF:	  			//RGB颜色模式	关
			RGBr	=	0;
			RGBg	=	0;
			RGBb	=	0;
			break;
		case	RGBMR: 	  				//RGB颜色模式	红 
			if(RGBlastcolor%2)			//灭->亮
			{
				if(RGBr<RGBbright)	RGBr++;
				if(RGBg>0)	RGBg--;
				if(RGBb>0)	RGBb--;
			}	   
			else						//亮->灭
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMRG:   				//RGB颜色模式	红绿
			if(RGBlastcolor%2)			//灭->亮
			{
				if(RGBr<RGBbright)	RGBr++;
				if(RGBg<RGBbright)	RGBg++;
				if(RGBb>0)	RGBb--;	  
			}	   
			else						//亮->灭
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMG: 	  				//RGB颜色模式	绿	 
			if(RGBlastcolor%2)			//灭->亮
			{
				if(RGBr>0)	RGBr--;
				if(RGBg<RGBbright)	RGBg++;
				if(RGBb>0)	RGBb--;	
			}	   
			else						//亮->灭
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMGB: 				//RGB颜色模式	绿蓝  
			if(RGBlastcolor%2)			//灭->亮
			{
				if(RGBr>0)	RGBr--;
				if(RGBg<RGBbright)	RGBg++;
				if(RGBb<RGBbright)	RGBb++;	 
			}	   
			else						//亮->灭
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMB: 	  				//RGB颜色模式	蓝 
			if(RGBlastcolor%2)			//灭->亮
			{
				if(RGBr>0)	RGBr--;
				if(RGBg>0)	RGBg--;
				if(RGBb<RGBbright)	RGBb++;	 
			}	   
			else						//亮->灭
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMRB: 	  			//RGB颜色模式	红蓝  
			if(RGBlastcolor%2)			//灭->亮
			{
				if(RGBr<RGBbright)	RGBr++;
				if(RGBg>0)	RGBg--;
				if(RGBb<RGBbright)	RGBb++;	 
			}	   
			else						//亮->灭
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMRGB: 	  			//RGB颜色模式	白	   
			if(RGBlastcolor%2)			//灭->亮
			{
				if(RGBr<RGBbright)	RGBr++;
				if(RGBg<RGBbright)	RGBg++;
				if(RGBb<RGBbright)	RGBb++;
			}	   
			else						//亮->灭
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMFADE: 	  			//RGB颜色模式	明暗渐变
			if(RGBlastcolor==0)			//灭->亮		白
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg<RGBbright)	RGBg++;	if(RGBb<RGBbright)	RGBb++;	}
			else if(RGBlastcolor==2)	//灭->亮		红
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;	}
			else if(RGBlastcolor==4)	//灭->亮		红绿
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg<RGBbright)	RGBg++;	if(RGBb>0)	RGBb--;	}
			else if(RGBlastcolor==6)	//灭->亮		绿
				{if(RGBr>0)		RGBr--;	if(RGBg<RGBbright)	RGBg++;	if(RGBb>0)	RGBb--;	}
			else if(RGBlastcolor==8)	//灭->亮		绿蓝
				{if(RGBr>0)		RGBr--;	if(RGBg<RGBbright)	RGBg++;	if(RGBb<RGBbright)	RGBb++;	}
			else if(RGBlastcolor==10)	//灭->亮		蓝
				{if(RGBr>0)		RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb<RGBbright)	RGBb++;	}
			else if(RGBlastcolor==12)	//灭->亮		蓝红
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg>0)	RGBg--;	if(RGBb<RGBbright)	RGBb++;	}
			else						//亮->灭
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMFADEC: 	  			//RGB颜色模式	连续渐变		  
			if(RGBlastcolor%7==0)			//灭->亮		白
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg<RGBbright)	RGBg++;	if(RGBb<RGBbright)	RGBb++;	}
			else if(RGBlastcolor%7==1)	//灭->亮		红
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;	}
			else if(RGBlastcolor%7==2)	//灭->亮		红绿
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg<RGBbright)	RGBg++;	if(RGBb>0)	RGBb--;	}
			else if(RGBlastcolor%7==3)	//灭->亮		绿
				{if(RGBr>0)		RGBr--;	if(RGBg<RGBbright)	RGBg++;	if(RGBb>0)	RGBb--;	}
			else if(RGBlastcolor%7==4)	//灭->亮		绿蓝
				{if(RGBr>0)		RGBr--;	if(RGBg<RGBbright)	RGBg++;	if(RGBb<RGBbright)	RGBb++;	}
			else if(RGBlastcolor%7==5)	//灭->亮		蓝
				{if(RGBr>0)		RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb<RGBbright)	RGBb++;	}
			else if(RGBlastcolor%7==6)	//灭->亮		蓝红
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg>0)	RGBg--;	if(RGBb<RGBbright)	RGBb++;	}
			break;
	}
	LEDSendData(RGBr,RGBg,RGBb);			
}

/********************************************************************************************************
** 	函数名称:			void	DOT1s()
**	功能描述:			DOT变换一次模式
**	创 建 者:			严泽远
**	创建时间:			2011-02-03 17:33
**	版    本:			v1.0.0
********************************************************************************************************/
void	DOT1s()
{
	DOTcut	=	0;
	DOTonoff=!DOTonoff;
}

/********************************************************************************************************
** 	函数名称:			void	NUM1s()
**	功能描述:			NUM变换一次模式
**	创 建 者:			严泽远
**	创建时间:			2011-02-05 15:52
**	版    本:			v1.0.0
********************************************************************************************************/
void	NUM1s()
{
	NUM500msloop=	0;
	NUMcut		=	0;
}

/********************************************************************************************************
** 	函数名称:			bit	CheckTime(bit DT)
**	功能描述:			日期时间格式检测
**	出口参数:			0:检查日期格式	1:检查时间格式
**	出口参数:			0:有误	1:格式正确
**	创 建 者:			严泽远
**	创建时间:			2011-01-29 21:25
**	版    本:			v1.0.0
********************************************************************************************************/
bit	CheckTime(bit DT)
{
	xdata uchar Mo,Da,Ho,Mi,Se;
	xdata uint Ye;
	bit Ok=1;

	Ye=S_YEAR;
	Mo=S_MONTH;
	Da=S_DAY;
	Ho=S_HOUR;
	Mi=S_MINUTE;
	Se=S_SECOND;

	if(DT==0)
	{	
		if(Ye>99)			return 0;
		if((Mo>12)||(Mo<1))	return 0;
		if(Da<1)			return 0;
		switch(Mo)
		{
			case 1:case 3:case 5:case 7:case 8:case 10:case 12:if(Da>31)return 0;break;
			case 4:case 6:case 9:case 11:if(Da>30)return 0;break;
			case 2:
				if((Ye%4==0)&&(Ye!=0))
				{if(Da>29)return 0;}
				else
				{if(Da>28)return 0;}
		}
	}
	else
	{
		if(Ho>23) return 0;
		if(Mi>59) return 0;
		if(Se>59) return 0;
	}
	return	1;
}

/********************************************************************************************************
** 	函数名称:			void	Num_Process(uchar N)
**	功能描述:			数字按键处理函数
**	创 建 者:			严泽远
**	创建时间:			2011-02-04 16:11
**	版    本:			v1.0.0
********************************************************************************************************/
void	Num_Process(uchar N)
{	 
	switch(NOWSTATE)
	{
		case	S_SETYEAR:						//当前在调整年					   
			Beep(BEEPOPERATE);					//BEEP
			S_YEAR	=	(S_YEAR*10+N)%100;
			break;
		case	S_SETMONTH:						//当前在调整月					   
			Beep(BEEPOPERATE);					//BEEP
			S_MONTH	=	(S_MONTH*10+N)%100;
			break;
		case	S_SETDAY:						//当前在调整日					   
			Beep(BEEPOPERATE);					//BEEP
			S_DAY	=	(S_DAY*10+N)%100;
			break;
		case	S_SETHOUR:						//当前在调整时 				   
			Beep(BEEPOPERATE);					//BEEP
			S_HOUR	=	(S_HOUR*10+N)%100;
			break;
		case	S_SETMINUTE:					//当前在调整分 				   
			Beep(BEEPOPERATE);					//BEEP
			S_MINUTE=	(S_MINUTE*10+N)%100;
			break;
		case	S_SETSECOND:					//当前在调整秒 					   
			Beep(BEEPOPERATE);					//BEEP
			S_SECOND=	(S_SECOND*10+N)%100;
			break;
		case	S_NORMAL:	 					//正常开机和关机状态下调整背景光颜色
		case	S_POWEROFF:								
			if(N>0 && N<10)
			{						   
				Beep(BEEPOPERATE);				//BEEP
				RGBmode	=	N;
			}
			break;	   
		case	S_SETA1M:						//当前在调整闹钟1分钟 					   
		case	S_SETA2M:						//当前在调整闹钟2分钟 					   
			Beep(BEEPOPERATE);					//BEEP
			S_MINUTE=	(S_MINUTE*10+N)%100;
			break;
		case	S_SETA1H:						//当前在调整闹钟1小时 					   
		case	S_SETA2H:						//当前在调整闹钟2小时 					   
			Beep(BEEPOPERATE);					//BEEP
			S_HOUR=	(S_HOUR*10+N)%100;
			break;
	}
}

/********************************************************************************************************
** 	函数名称:			void	IR_Process(uchar IrData)
**	功能描述:			红外线解码处理函数
**	创 建 者:			严泽远
**	创建时间:			2011-02-04 16:06
**	版    本:			v1.0.0
********************************************************************************************************/
void	IR_Process(uchar IrData)
{
	uchar	i;
	uint	j;

	if(F_Alarm)
	{	  					   
		Beep(BEEPCONFIRM);							//BEEP
		AlarmReset();								//闹钟复位
		F_Alarm	=	0;
		return;
	}

	switch(IrData)
	{
		//********************************************************************** 
		case	0x45:	 							//POWER
			if(NOWSTATE==S_POWEROFF)
			{						   
				Beep(BEEPPOWERON);					//BEEP
				DelayMS(200);
				LASTN[0]=	' ';
				LASTN[1]=	' ';
				LASTN[2]=	' ';
				LASTN[3]=	' ';
				LASTN[4]=	' ';
				LASTN[5]=	' ';
				NUM1s();		  
				NOWSTATE	=	S_NORMAL;			//切换为开机状态
				POWERON		=	0;					//开机
			}
			else
			{
				NOWSTATE	=	S_POWEROFF;			//切换为关机状态
				Beep(BEEPSHUTDOWN);					//BEEP
				for(i=0;i<6;i++){LASTN[i]=Num[i];Num[i]=' ';}					//渐变关闭
				DOTonoff	=	0;					//DOTS灭 
				DOT1s(); 							//更新DOTS状态
				NUM1s(); 							//更新NUMS状态
			}
			break;	
		//********************************************************************** 
		case	0x46:	 							//LIGHT	 
			if(NOWSTATE==S_NORMAL || NOWSTATE==S_POWEROFF)		//如果在正常显示时间状态或关机状态下
			{
				Beep(BEEPOPERATE);						//BEEP	
				if(RGBmode)
				{
					RGBmodesave	=	RGBmode;
					RGBmode		=	RGBMOFF;
				}
				else
					RGBmode		=	RGBmodesave;
			}
			break;	 	   
		//********************************************************************** 
		case	0x47:	 							//SET	  
			if(NOWSTATE==S_NORMAL)					//如果在正常显示时间状态下
			{
				Beep(BEEPPOWERON);					//BEEP
				
				DOTonoff	=	1;					//DOTS亮 
				DOT1s(); 							//更新DOTS状态

				S_YEAR	=	YEAR;
				S_MONTH	=	MONTH;
				S_DAY	=	DAY;
				S_HOUR	=	HOUR;
				S_MINUTE=	MINUTE;
				S_SECOND=	SECOND;
				
				NUMLOOPCOUNT=	5;					//调整为快速闪烁状态
				RGBmodeold	=	RGBmode;			//调整为调时状态的背景灯
				RGBmode		=	RGBMR;				//调整为调时状态的背景灯 
				RGBlastcolor=	0;					//灭->亮
				RGB1s();
					
				NOWSTATE=	S_SETSECOND;			//当前处于调整秒状态
			}		  
			else if(NOWSTATE==S_SHOWDATE)			//如果在正常显示日期状态下
			{
				Beep(BEEPPOWERON);					//BEEP
				
				DOTonoff	=	0;					//DOTS灭 
				DOT1s(); 							//更新DOTS状态

				S_YEAR	=	YEAR;
				S_MONTH	=	MONTH;
				S_DAY	=	DAY;
				S_HOUR	=	HOUR;
				S_MINUTE=	MINUTE;
				S_SECOND=	SECOND;
				
				NUMLOOPCOUNT=	5;					//调整为快速闪烁状态
				RGBmodeold	=	RGBmode;			//调整为调时状态的背景灯
				RGBmode		=	RGBMG;				//调整为调时状态的背景灯  
				RGBlastcolor=	0;					//灭->亮 
				RGB1s();
					
				NOWSTATE=	S_SETDAY;				//当前处于调整日状态
			}		  
			else if(NOWSTATE==S_SHOWA1)				//如果在正常显示闹钟1状态下
			{
				Beep(BEEPPOWERON);					//BEEP

				S_SECOND=	A1ONOFF;
				S_HOUR	=	A1H;
				S_MINUTE=	A1M;
				
				NUMLOOPCOUNT=	5;					//调整为快速闪烁状态
				RGBmodeold	=	RGBmode;			//调整为调时状态的背景灯
				RGBmode		=	RGBMB;				//调整为调时状态的背景灯  
				RGBlastcolor=	0;					//灭->亮 
				RGB1s();
					
				NOWSTATE=	S_SETA1M;				//当前处于调整闹钟1分钟状态
			}			  
			else if(NOWSTATE==S_SHOWA2)				//如果在正常显示闹钟2状态下
			{
				Beep(BEEPPOWERON);					//BEEP

				S_SECOND=	A2ONOFF;
				S_HOUR	=	A2H;
				S_MINUTE=	A2M;
				
				NUMLOOPCOUNT=	5;					//调整为快速闪烁状态
				RGBmodeold	=	RGBmode;			//调整为调时状态的背景灯
				RGBmode		=	RGBMB;				//调整为调时状态的背景灯  
				RGBlastcolor=	0;					//灭->亮 
				RGB1s();
					
				NOWSTATE=	S_SETA2M;				//当前处于调整闹钟2分钟状态
			}	
			else if(NOWSTATE==S_SETYEAR || NOWSTATE==S_SETMONTH || NOWSTATE==S_SETDAY || NOWSTATE==S_SETHOUR || NOWSTATE==S_SETMINUTE || NOWSTATE==S_SETSECOND || NOWSTATE==S_SETA1H || NOWSTATE==S_SETA1M || NOWSTATE==S_SETA2H || NOWSTATE==S_SETA2M)	//如果在调整时间日期状态下
			{
				Beep(BEEPERROR);					//不予保存保存
				DelayMS(200);
				NUMLOOPCOUNT=	25;					//调整为正常闪烁状态
				RGBmode		=	RGBmodeold;			//恢复背景灯模式		    
				RGB1s();
				NOWSTATE	=	S_NORMAL;			//当前处于正常显示时间状态 不保存设置
			}	   
			break;
		//********************************************************************** 
		case	0x44:	 							//DATE
			if(NOWSTATE==S_NORMAL || NOWSTATE==S_SHOWMOONDATE || NOWSTATE==S_SHOWTEMP || NOWSTATE==S_SHOWA1 || NOWSTATE==S_SHOWA2)
			{						   
				Beep(BEEPOPERATE);					//BEEP
				
				Num[0]	=	'0'+(FormatDate?MONTH:YEAR)/10;			//赋值当前日期
				Num[1]	=	'0'+(FormatDate?MONTH:YEAR)%10;			//赋值当前日期
				Num[2]	=	'0'+(FormatDate?DAY:MONTH)/10;			//赋值当前日期
				Num[3]	=	'0'+(FormatDate?DAY:MONTH)%10;			//赋值当前日期
				Num[4]	=	'0'+(FormatDate?YEAR:DAY)/10;				//赋值当前日期
				Num[5]	=	'0'+(FormatDate?YEAR:DAY)%10;				//赋值当前日期
		
				DOTonoff	=	0;					//DOTS灭 
				DOT1s(); 							//更新DOTS状态
				NUM1s();										 
				NOWSTATE	=	S_SHOWDATE;			//切换为日期显示状态  
				ShowDateLoop=	0;					//计时开始
			}
			else if(NOWSTATE==S_SHOWDATE)
			{
			  	Conversion(0,(YEAR/10)*16+YEAR%10,(MONTH/10)*16+MONTH%10,(DAY/10)*16+DAY%10);	  	//计算农历日期
					   
				Beep(BEEPOPERATE);					//BEEP
				
				Num[0]	=	'0'+(FormatDate?month_moon:year_moon)/16;		//赋值当前日期
				Num[1]	=	'0'+(FormatDate?month_moon:year_moon)%16;		//赋值当前日期
				Num[2]	=	'0'+(FormatDate?day_moon:month_moon)/16;		//赋值当前日期
				Num[3]	=	'0'+(FormatDate?day_moon:month_moon)%16;		//赋值当前日期
				Num[4]	=	'0'+(FormatDate?year_moon:day_moon)/16;		//赋值当前日期
				Num[5]	=	'0'+(FormatDate?year_moon:day_moon)%16;		//赋值当前日期
		
				DOTonoff	=	1;					//DOTS亮 
				DOT1s(); 							//更新DOTS状态
				NUM1s();										 
				NOWSTATE	=	S_SHOWMOONDATE;		//切换为日期显示状态  
				ShowDateLoop=	0;					//计时开始
			}
			break;
		//********************************************************************** 
		case	0x40:	 							//ALARM	  
			if(NOWSTATE==S_NORMAL || NOWSTATE==S_SHOWMOONDATE || NOWSTATE==S_SHOWDATE || NOWSTATE==S_SHOWTEMP || NOWSTATE==S_SHOWA2)
			{						   
				Beep(BEEPOPERATE);					//BEEP	
					 
				Num[0]	=	'a';					//赋值当前闹钟显示
				Num[1]	=	'1';					//赋值当前闹钟显示
				Num[2]	=	(A1ONOFF?'0'+A1H/10:' ');				//赋值当前闹钟显示
				Num[3]	=	(A1ONOFF?'0'+A1H%10:'0');				//赋值当前闹钟显示
				Num[4]	=	(A1ONOFF?'0'+A1M/10:'f');				//赋值当前闹钟显示
				Num[5]	=	(A1ONOFF?'0'+A1M%10:'f');				//赋值当前闹钟显示
		
				DOTonoff	=	1;					//DOTS亮 
				DOT1s(); 							//更新DOTS状态
				NUM1s();										 
				NOWSTATE	=	S_SHOWA1;			//切换为显示闹钟1状态  
				ShowDateLoop=	0;					//计时开始	 
			} 
			else if(NOWSTATE==S_SHOWA1)
			{						   
				Beep(BEEPOPERATE);					//BEEP	
					 
				Num[0]	=	'a';					//赋值当前闹钟显示
				Num[1]	=	'2';					//赋值当前闹钟显示
				Num[2]	=	(A2ONOFF?'0'+A2H/10:' ');				//赋值当前闹钟显示
				Num[3]	=	(A2ONOFF?'0'+A2H%10:'0');				//赋值当前闹钟显示
				Num[4]	=	(A2ONOFF?'0'+A2M/10:'f');				//赋值当前闹钟显示
				Num[5]	=	(A2ONOFF?'0'+A2M%10:'f');				//赋值当前闹钟显示
		
				DOTonoff	=	1;					//DOTS亮 
				DOT1s(); 							//更新DOTS状态
				NUM1s();										 
				NOWSTATE	=	S_SHOWA2;			//切换为显示闹钟2状态  
				ShowDateLoop=	0;					//计时开始

			}
			break;
		//********************************************************************** 
		case	0x43:	 							//TEMP	  
			if(NOWSTATE==S_NORMAL || NOWSTATE==S_SHOWMOONDATE || NOWSTATE==S_SHOWDATE || NOWSTATE==S_SHOWA1 || NOWSTATE==S_SHOWA2)
			{						   
				Beep(BEEPPOWERON);					//BEEP
				
				for(i=1;i<20;i++)
				{
					j=GetTemp();
					if(j>60)	DelayMS(50);		//刷新温度信息
					else i=20;
				}			 

				if(FormatTemp)	j=32+j*1.8;			//℉

				Num[0]	=	' ';					//赋值当前温度
				Num[1]	=	((j>99)?'0'+(j%1000)/100:' ');	//赋值当前温度
				Num[2]	=	((j>9)?'0'+(j%100)/10:' ');		//赋值当前温度
				Num[3]	=	j%10+'0';				//赋值当前温度
				Num[4]	=	' ';					//赋值当前温度
				Num[5]	=	(FormatTemp?'f':'c');	//赋值当前温度
			
				DOTonoff	=	0;					//DOTS灭 
				DOT1s(); 							//更新DOTS状态
				NUM1s();										 
				NOWSTATE	=	S_SHOWTEMP;			//切换为温度显示状态  
				ShowDateLoop=	0;					//计时开始
			}
			break;	 		
		//********************************************************************** 
		case	0x07:	 							//LEFT	 
			if(NOWSTATE==S_NORMAL || NOWSTATE==S_POWEROFF)
			{						   
				if(RGBbright>=10)
				{
					RGBbright-=7;
					RGBr=0;RGBg=0;RGBb=0;
					Beep(BEEPOPERATE);				//BEEP	 
				}
			}
			else if(NOWSTATE==S_SETSECOND)	{NOWSTATE=S_SETMINUTE;	Beep(BEEPOPERATE);}
			else if(NOWSTATE==S_SETMINUTE)	{NOWSTATE=S_SETHOUR;	Beep(BEEPOPERATE);}
			else if(NOWSTATE==S_SETHOUR)	{NOWSTATE=S_SETSECOND;	Beep(BEEPOPERATE);}	 
			else if(NOWSTATE==S_SETYEAR)	{NOWSTATE=S_SETDAY;		Beep(BEEPOPERATE);}
			else if(NOWSTATE==S_SETDAY)		{NOWSTATE=S_SETMONTH;	Beep(BEEPOPERATE);}
			else if(NOWSTATE==S_SETMONTH)	{NOWSTATE=S_SETYEAR;	Beep(BEEPOPERATE);}	 
			else if(NOWSTATE==S_SETA1M)		{NOWSTATE=S_SETA1H;		Beep(BEEPOPERATE);}	 
			else if(NOWSTATE==S_SETA1H)		{NOWSTATE=S_SETA1M;		Beep(BEEPOPERATE);}	 
			else if(NOWSTATE==S_SETA2M)		{NOWSTATE=S_SETA2H;		Beep(BEEPOPERATE);}	 
			else if(NOWSTATE==S_SETA2H)		{NOWSTATE=S_SETA2M;		Beep(BEEPOPERATE);}	 
			break;
		//********************************************************************** 
		case	0x15:	 							//RIGHT	  
			if(NOWSTATE==S_NORMAL || NOWSTATE==S_POWEROFF)
			{						   
				if(RGBbright<=24)
				{
					RGBbright+=7;		 
					RGBr=0;RGBg=0;RGBb=0;
					Beep(BEEPOPERATE);				//BEEP	 
				}						 
			}
			else if(NOWSTATE==S_SETSECOND)	{NOWSTATE=S_SETHOUR;	Beep(BEEPOPERATE);}
			else if(NOWSTATE==S_SETHOUR)	{NOWSTATE=S_SETMINUTE;	Beep(BEEPOPERATE);}
			else if(NOWSTATE==S_SETMINUTE)	{NOWSTATE=S_SETSECOND;	Beep(BEEPOPERATE);}	  
			else if(NOWSTATE==S_SETYEAR)	{NOWSTATE=S_SETMONTH;	Beep(BEEPOPERATE);}
			else if(NOWSTATE==S_SETMONTH)	{NOWSTATE=S_SETDAY;		Beep(BEEPOPERATE);}	
			else if(NOWSTATE==S_SETDAY)		{NOWSTATE=S_SETYEAR;	Beep(BEEPOPERATE);}	
			else if(NOWSTATE==S_SETA1M)		{NOWSTATE=S_SETA1H;		Beep(BEEPOPERATE);}	 
			else if(NOWSTATE==S_SETA1H)		{NOWSTATE=S_SETA1M;		Beep(BEEPOPERATE);}	 
			else if(NOWSTATE==S_SETA2M)		{NOWSTATE=S_SETA2H;		Beep(BEEPOPERATE);}	 
			else if(NOWSTATE==S_SETA2H)		{NOWSTATE=S_SETA2M;		Beep(BEEPOPERATE);}	 
			break;																	
		//********************************************************************** 
		case	0x09:	 							//FUNC	
			if(NOWSTATE==S_NORMAL)
			{						   
				Beep(BEEPPOWERON);					//BEEP
				if(NUMmode<2)NUMmode++;else NUMmode=0;//NUMmode切换
				if(NUMmode==1)NUMLOOPCOUNT=15;else NUMLOOPCOUNT=25;	//调整各模式的颜色渐变STEP毫秒数
			}
			else if(NOWSTATE==S_SETA1H || NOWSTATE==S_SETA1M || NOWSTATE==S_SETA2H || NOWSTATE==S_SETA2M)
			{
				Beep(BEEPPOWERON);					//BEEP
				if(S_SECOND)S_SECOND=0;else S_SECOND=1;
			}
			else if(NOWSTATE==S_SHOWTEMP)			//显示温度状态
			{			
				Beep(BEEPPOWERON);					//BEEP
				if(FormatTemp)FormatTemp=0;else FormatTemp=1;
				FormatSave();
				
				for(i=1;i<20;i++)
				{
					j=GetTemp();
					if(j>60)	DelayMS(50);		//刷新温度信息
					else i=20;
				}			 

				if(FormatTemp)	j=32+j*1.8;			//℉

				Num[0]	=	' ';					//赋值当前温度
				Num[1]	=	((j>99)?'0'+(j%1000)/100:' ');	//赋值当前温度
				Num[2]	=	((j>9)?'0'+(j%100)/10:' ');		//赋值当前温度
				Num[3]	=	j%10+'0';				//赋值当前温度
				Num[4]	=	' ';					//赋值当前温度
				Num[5]	=	(FormatTemp?'f':'c');	//赋值当前温度
			
				DOTonoff	=	0;					//DOTS灭 
				DOT1s(); 							//更新DOTS状态
				NUM1s();										 
				NOWSTATE	=	S_SHOWTEMP;			//切换为温度显示状态  
				ShowDateLoop=	0;					//计时开始
			}
			else if(NOWSTATE==S_SHOWDATE)			//显示阳历日期状态
		   	{		
				Beep(BEEPOPERATE);					//BEEP 
				if(FormatDate)FormatDate=0;else FormatDate=1;
				FormatSave();
				
				Num[0]	=	'0'+(FormatDate?MONTH:YEAR)/10;			//赋值当前日期
				Num[1]	=	'0'+(FormatDate?MONTH:YEAR)%10;			//赋值当前日期
				Num[2]	=	'0'+(FormatDate?DAY:MONTH)/10;			//赋值当前日期
				Num[3]	=	'0'+(FormatDate?DAY:MONTH)%10;			//赋值当前日期
				Num[4]	=	'0'+(FormatDate?YEAR:DAY)/10;				//赋值当前日期
				Num[5]	=	'0'+(FormatDate?YEAR:DAY)%10;				//赋值当前日期
		
				DOTonoff	=	0;					//DOTS灭 
				DOT1s(); 							//更新DOTS状态
				NUM1s();										 
				NOWSTATE	=	S_SHOWDATE;			//切换为日期显示状态  
				ShowDateLoop=	0;					//计时开始
			}		 
			else if(NOWSTATE==S_SHOWMOONDATE)
			{
			  	Conversion(0,(YEAR/10)*16+YEAR%10,(MONTH/10)*16+MONTH%10,(DAY/10)*16+DAY%10);	  	//计算农历日期	  
				 
				if(FormatDate)FormatDate=0;else FormatDate=1;
				FormatSave();
					   
				Beep(BEEPOPERATE);					//BEEP
				
				Num[0]	=	'0'+(FormatDate?month_moon:year_moon)/16;		//赋值当前日期
				Num[1]	=	'0'+(FormatDate?month_moon:year_moon)%16;		//赋值当前日期
				Num[2]	=	'0'+(FormatDate?day_moon:month_moon)/16;		//赋值当前日期
				Num[3]	=	'0'+(FormatDate?day_moon:month_moon)%16;		//赋值当前日期
				Num[4]	=	'0'+(FormatDate?year_moon:day_moon)/16;		//赋值当前日期
				Num[5]	=	'0'+(FormatDate?year_moon:day_moon)%16;		//赋值当前日期
		
				DOTonoff	=	1;					//DOTS亮 
				DOT1s(); 							//更新DOTS状态
				NUM1s();										 
				NOWSTATE	=	S_SHOWMOONDATE;		//切换为日期显示状态  
				ShowDateLoop=	0;					//计时开始
			}
			else if(NOWSTATE==S_SHOWA1)				//显示闹钟1状态下
			{																  
				Beep(BEEPOPERATE);					//BEEP

				if(A1ONOFF)A1ONOFF=0;else A1ONOFF=1;
				AlarmSave(1,A1H,A1M,A1ONOFF);	  	
					 
				Num[0]	=	'a';					//赋值当前闹钟显示
				Num[1]	=	'1';					//赋值当前闹钟显示
				Num[2]	=	(A1ONOFF?'0'+A1H/10:' ');				//赋值当前闹钟显示
				Num[3]	=	(A1ONOFF?'0'+A1H%10:'0');				//赋值当前闹钟显示
				Num[4]	=	(A1ONOFF?'0'+A1M/10:'f');				//赋值当前闹钟显示
				Num[5]	=	(A1ONOFF?'0'+A1M%10:'f');				//赋值当前闹钟显示
		
				DOTonoff	=	1;					//DOTS亮 
				DOT1s(); 							//更新DOTS状态
				NUM1s();										 
				NOWSTATE	=	S_SHOWA1;			//切换为显示闹钟1状态  
				ShowDateLoop=	0;					//计时开始	 
			}
			else if(NOWSTATE==S_SHOWA2)				//显示闹钟2状态下
			{																  
				Beep(BEEPOPERATE);					//BEEP

				if(A2ONOFF)A2ONOFF=0;else A2ONOFF=1;
				AlarmSave(2,A2H,A2M,A2ONOFF);	  	
					 
				Num[0]	=	'a';					//赋值当前闹钟显示
				Num[1]	=	'2';					//赋值当前闹钟显示
				Num[2]	=	(A2ONOFF?'0'+A2H/10:' ');				//赋值当前闹钟显示
				Num[3]	=	(A2ONOFF?'0'+A2H%10:'0');				//赋值当前闹钟显示
				Num[4]	=	(A2ONOFF?'0'+A2M/10:'f');				//赋值当前闹钟显示
				Num[5]	=	(A2ONOFF?'0'+A2M%10:'f');				//赋值当前闹钟显示
		
				DOTonoff	=	1;					//DOTS亮 
				DOT1s(); 							//更新DOTS状态
				NUM1s();										 
				NOWSTATE	=	S_SHOWA2;			//切换为显示闹钟2状态  
				ShowDateLoop=	0;					//计时开始	 
			}
			break;													  
		//********************************************************************** 
		case	0x16:								//OK键	 
			if(NOWSTATE==S_NORMAL)					//如果在正常显示时间状态下
			{				 			
				Beep(BEEPOPERATE);					//BEEP 
				if(FormatTime)FormatTime=0;else FormatTime=1;
				FormatSave();	
				
				Num[0]	=	(FormatTime?'1':'2');
				Num[1]	=	(FormatTime?'2':'4');
				Num[2]	=	'h';
				Num[3]	=	' ';
				Num[4]	=	' ';
				Num[5]	=	' ';
			
				DOTonoff	=	0;					//DOTS灭 
				DOT1s(); 							//更新DOTS状态
				NUM1s();										 
				NOWSTATE	=	S_SHOWTEMP;			//切换为温度显示状态  
				ShowDateLoop=	0;					//计时开始			
			}	 	
			if(NOWSTATE==S_SETHOUR || NOWSTATE==S_SETMINUTE || NOWSTATE==S_SETSECOND)	//如果在调整时间状态下
			{
				GetDateTime();						//刷新当前时间
															  				
				S_YEAR	=	YEAR;
				S_MONTH	=	MONTH;
				S_DAY	=	DAY;
									 
				if(CheckTime(1))					//检查时间格式是否正确
				{
					ModifyTime(S_YEAR,S_MONTH,S_DAY,S_HOUR,S_MINUTE,S_SECOND);
					Beep(BEEPCONFIRM);				//BEEP
				}
				else
				{
					Beep(BEEPERROR);				//BEEP
				}	
												
				DelayMS(200);
				NUMLOOPCOUNT=	25;					//调整为正常闪烁状态
				RGBmode		=	RGBmodeold;			//恢复背景灯模式
				NOWSTATE	=	S_NORMAL;			//当前处于正常显示时间状态
			}  			 	 	
			if(NOWSTATE==S_SETYEAR || NOWSTATE==S_SETMONTH || NOWSTATE==S_SETDAY)	//如果在调整日期状态下
			{
				GetDateTime();						//刷新当前时间
															  				
				S_HOUR		=	HOUR;
				S_MINUTE	=	MINUTE;
				S_SECOND	=	SECOND;
									 
				if(CheckTime(0))					//检查日期格式是否正确
				{
					ModifyTime(S_YEAR,S_MONTH,S_DAY,S_HOUR,S_MINUTE,S_SECOND);
					Beep(BEEPCONFIRM);				//BEEP
				}
				else
				{
					Beep(BEEPERROR);				//BEEP
				}	
												
				DelayMS(200);
				NUMLOOPCOUNT=	25;					//调整为正常闪烁状态
				RGBmode		=	RGBmodeold;			//恢复背景灯模式		    
				RGB1s();
				NOWSTATE	=	S_NORMAL;			//当前处于正常显示时间状态
			}			 	 	
			if(NOWSTATE==S_SETA1H || NOWSTATE==S_SETA1M || NOWSTATE==S_SETA2H || NOWSTATE==S_SETA2M)	//如果在调整闹钟1闹钟2状态下
			{												  				
				if(CheckTime(1))					//检查闹钟格式正确
				{
					if(NOWSTATE==S_SETA1H || NOWSTATE==S_SETA1M)		AlarmSave(1,S_HOUR,S_MINUTE,S_SECOND);
					else if(NOWSTATE==S_SETA2H || NOWSTATE==S_SETA2M)	AlarmSave(2,S_HOUR,S_MINUTE,S_SECOND);
					Beep(BEEPCONFIRM);				//BEEP
				}
				else
				{
					Beep(BEEPERROR);				//BEEP
				}	
												
				DelayMS(500);
				NUMLOOPCOUNT=	25;					//调整为正常闪烁状态
				RGBmode		=	RGBmodeold;			//恢复背景灯模式		    
				RGB1s();
				NOWSTATE	=	S_NORMAL;			//当前处于正常显示时间状态
			}
			break;
		//********************************************************************** 
		case	0x19:	 							//CANCEL键	 			
			if(NOWSTATE==S_SETYEAR || NOWSTATE==S_SETMONTH || NOWSTATE==S_SETDAY || NOWSTATE==S_SETHOUR || NOWSTATE==S_SETMINUTE || NOWSTATE==S_SETSECOND || NOWSTATE==S_SETA1H || NOWSTATE==S_SETA1M || NOWSTATE==S_SETA2H || NOWSTATE==S_SETA2M)	//如果在调整时间日期状态下
			{
				Beep(BEEPERROR);					//不予保存保存
				DelayMS(200);
				NUMLOOPCOUNT=	25;					//调整为正常闪烁状态
				RGBmode		=	RGBmodeold;			//恢复背景灯模式		    
				RGB1s();
				NOWSTATE	=	S_NORMAL;			//当前处于正常显示时间状态 不保存设置
			}
			break;
		//********************************************************************** 
		case	0x0D:	Num_Process(0); 	break;				//0键	 	
		case	0x0C:	Num_Process(1); 	break;				//1键	 	
		case	0x18:	Num_Process(2); 	break;				//2键	 	
		case	0x5E:	Num_Process(3); 	break;				//3键	 	
		case	0x08:	Num_Process(4); 	break;				//4键	 	
		case	0x1C:	Num_Process(5); 	break;				//5键	 	
		case	0x5A:	Num_Process(6); 	break;				//6键	 	
		case	0x42:	Num_Process(7); 	break;				//7键	   
		case	0x52:	Num_Process(8); 	break;				//8键	   
		case	0x4A:	Num_Process(9); 	break;				//9键	 
	}
}

/********************************************************************************************************
** 	函数名称:			void	IR_RX()
**	功能描述:			红外线中断接收处理
**	创 建 者:			严泽远
**	创建时间:			2011-02-04 16:07
**	版    本:			v1.0.0
********************************************************************************************************/
void IR_RX()
{
	uchar	SampleTime;

	IR_SampleCnt++;							//Sample + 1

	F0 = P_IR_RX_temp;						//Save Last sample status
	P_IR_RX_temp = IR;						//Read current status
	if(F0 && !P_IR_RX_temp)					//Last sample is high，and current sample is low, so is fall edge
	{
		SampleTime = IR_SampleCnt;			//get the sample time
		IR_SampleCnt = 0;					//Clear the sample counter

			 if(SampleTime > D_IR_SYNC_MAX)		B_IR_Sync = 0;	//large the Maxim SYNC time, then error
		else if(SampleTime >= D_IR_SYNC_MIN)					//SYNC
		{
			if(SampleTime >= D_IR_SYNC_DIVIDE)
			{
				B_IR_Sync = 1;					//has received SYNC
				IR_BitCnt = D_IR_BIT_NUMBER;	//Load bit number
			}
		}
		else if(B_IR_Sync)						//has received SYNC
		{
			if(SampleTime > D_IR_DATA_MAX)		B_IR_Sync=0;	//data samlpe time to large
			else
			{
				IR_DataShit >>= 1;					//data shift right 1 bit
				if(SampleTime >= D_IR_DATA_DIVIDE)	IR_DataShit |= 0x80;	//devide data 0 or 1
				if(--IR_BitCnt == 0)				//bit number is over?
				{
					B_IR_Sync = 0;					//Clear SYNC
					if(~IR_DataShit == IR_data)		//判断数据正反码
					{  	
						if((IR_UserH == (D_IR_usercode / 256)) &&
							IR_UserL == (D_IR_usercode % 256))		//系统码正确
						{
							IR_code      = IR_data;
							B_IR_Press   = 1;		//数据有效
						}
					}
				}
				else if((IR_BitCnt & 7)== 0)		//one byte receive
				{
					IR_UserL = IR_UserH;			//Save the User code high byte
					IR_UserH = IR_data;				//Save the User code low byte
					IR_data  = IR_DataShit;			//Save the IR data byte
				}
			}
		}
	}
}	

/********************************************************************************************************
** 	函数名称:			void 	Send595(uchar count,uint dat)
**	功能描述:			向74HC595发送数据  
**	入口参数:			count:	发送数据的位数
**						dat:	发送的数据(MAX 16Bit)
**	创 建 者:			严泽远
**	创建时间:			2011-02-04 22:32
**	版    本:			v1.0.0
********************************************************************************************************/
void 	Send595(uchar count,uint dat)
{
	for(;count>0;count--)
	{
		DATA = dat&0x0001;//_nop_();
		SHCP  = 1;//_nop_();
		SHCP  = 0;//_nop_();
		dat>>=1;
	}
}

/********************************************************************************************************
** 	函数名称:			bit 	DispNum(uchar * N,uchar Mode)
**	功能描述:			刷新YS18-3荧光管的显示数字
**	入口参数:			*N:		显示的字符数组
**						Mode:	显示模式	0:crossfade 1:fade in/out 2:standard
**	出口参数:			bit:	0:渐变显示未切换完成 1:渐变显示已过渡完成
**	创 建 者:			严泽远
**	创建时间:			2011-02-04 22:38
**	版    本:			v1.0.0
**  -----------------------------------------------------------------------------------------------------
**	修 改 者:			严泽远
**	修改时间:			2011-02-05 15:02
**	描    述:			将输入参数改为ASCII码数组形势 扩展英文字母应用
**  -----------------------------------------------------------------------------------------------------
**	修 改 者:			严泽远
**	修改时间:			2011-02-05 15:59
**	描    述:			返回当前数字切换状态
********************************************************************************************************/
bit 	DispNum(uchar * N,uchar Mode)
{
	bit		r=1;
	uchar	i;
	uchar	Nnow[6],Nlast[6];
	
	for(i=0;i<6;i++)
	{
		if(Mode==2)			LASTN[i]=*(N+i);	//如果是标准模式则立即刷新 No Crossfade
		switch(*(N+i))
		{
			case	'0':	Nnow[i]	=	0x20;	break;
			case	'1':	Nnow[i]	=	0x6E;	break;
			case	'2':	Nnow[i]	=	0x14;	break;
			case	'3':	Nnow[i]	=	0x0C;	break;
			case	'4':	Nnow[i]	=	0x4A;	break;
			case	'5':	Nnow[i]	=	0x09;	break;
			case	'6':	Nnow[i]	=	0x01;	break;
			case	'7':	Nnow[i]	=	0x6C;	break;
			case	'8':	Nnow[i]	=	0x00;	break;
			case	'9':	Nnow[i]	=	0x08;	break;
			case	'a':	Nnow[i]	=	0x40;	break;
			case	'f':	Nnow[i]	=	0x51;	break;
			case	'c':	Nnow[i]	=	0xB1;	break;
			case	'h':	Nnow[i]	=	0x42;	break;
			default:		Nnow[i]	=	0xFF;
		}
		if(Mode==1 && LASTN[i]!=*(N+i))
		{
			if(NUM500msloop<=300)
			 	Nnow[i]=0xFF;
			else
				LASTN[i]=' ';
		}
		switch(LASTN[i])
		{
			case	'0':	Nlast[i]=	0x20;	break;
			case	'1':	Nlast[i]=	0x6E;	break;
			case	'2':	Nlast[i]=	0x14;	break;
			case	'3':	Nlast[i]=	0x0C;	break;
			case	'4':	Nlast[i]=	0x4A;	break;
			case	'5':	Nlast[i]=	0x09;	break;
			case	'6':	Nlast[i]=	0x01;	break;
			case	'7':	Nlast[i]=	0x6C;	break;
			case	'8':	Nlast[i]=	0x00;	break;
			case	'9':	Nlast[i]=	0x08;	break; 
			case	'a':	Nlast[i]=	0x40;	break;
			case	'f':	Nlast[i]=	0x51;	break;
			case	'c':	Nlast[i]=	0xB1;	break;	  
			case	'h':	Nlast[i]=	0x42;	break;
			default:		Nlast[i]=	0xFF;
		}
		Send595(8,((NUMpwmloop%20)<=NUMcut?Nnow[i]:Nlast[i]));
		if(NUMcut>=20)			LASTN[i]=*(N+i);
		if(LASTN[i]!=*(N+i))	r=0;
	}

	if((NUMpwmloop)<1000) NUMpwmloop++;else NUMpwmloop=0;
	
	STCP = 1;//_nop_();
	STCP = 0;//_nop_();		//数据锁存

	return	r;
}

/********************************************************************************************************
** 	函数名称:			void	GetDateTime()
**	功能描述:			获取当前日期和时间
**	创 建 者:			严泽远
**	创建时间:			2011-02-05 15:45
**	版    本:			v1.0.0
********************************************************************************************************/
void	GetDateTime()
{
	uchar	Tmp;
	Tmp=read_random(DS3231_YEAR);	YEAR	=	BCD2HEX(Tmp);	//年
    Tmp=read_random(DS3231_MONTH);	MONTH	=	BCD2HEX(Tmp);	//月
    Tmp=read_random(DS3231_DAY);	DAY		=	BCD2HEX(Tmp);	//日
    Tmp=read_random(DS3231_HOUR);	Tmp&=0x3f;	HOUR	=	BCD2HEX(Tmp);	//时 	
    Tmp=read_random(DS3231_MINUTE);	MINUTE	=	BCD2HEX(Tmp);	//分
    Tmp=read_random(DS3231_SECOND);	SECOND	=	BCD2HEX(Tmp);	//秒
}		

void	AlarmReset()
{	    
	write_byte(DS3231_STATUS,0x00);
}

void	AlarmInit()
{	
	uchar	Tmp;

	A1ONOFF	=	Byte_Read(0x6000);
	A1H		=	Byte_Read(0x6001);
	A1M		=	Byte_Read(0x6002);
	A2ONOFF	=	Byte_Read(0x6003);
	A2H		=	Byte_Read(0x6004);
	A2M		=	Byte_Read(0x6005);

	if(A1ONOFF>1)	A1ONOFF	=	0;
	if(A1H>23)		A1H		=	0;
	if(A1M>59)		A1M		=	0;

	if(A2ONOFF>1)	A2ONOFF	=	0;
	if(A2H>23)		A2H		=	0;
	if(A2M>59)		A2M		=	0;	 
	

	write_byte(DS3231_ALARM1SECOND,0x00);
	Tmp=HEX2BCD(A1M);
	write_byte(DS3231_ALARM1MINUTE,Tmp);
	Tmp=HEX2BCD(A1H);
	write_byte(DS3231_ALARM1HOUR,Tmp);
	write_byte(DS3231_ALARM1WEEK,0x80);
		
	Tmp=HEX2BCD(A2M);
	write_byte(DS3231_ALARM2MINUTE,Tmp);
	Tmp=HEX2BCD(A2H);
	write_byte(DS3231_ALARM2HOUR,Tmp);
	write_byte(DS3231_ALARM2WEEK,0x80);
		
	Tmp	=	0x04;
	if(A1ONOFF)	Tmp|=0x01;
	if(A2ONOFF)	Tmp|=0x02;

	write_byte(DS3231_CONTROL,Tmp);
	AlarmReset();
}		
   
void	AlarmSave(uchar N,uchar H,uchar M,uchar ONOFF)
{	 
	uchar	Tmp;

	if(N==1)
	{
		A1ONOFF	=	ONOFF;
		A1H		=	H;
		A1M		=	M; 
	}
	else if(N==2)
	{
		A2ONOFF	=	ONOFF;
		A2H		=	H;
		A2M		=	M; 
  	}		
  
  	write_byte(DS3231_ALARM1SECOND,0x00);
	Tmp=HEX2BCD(A1M);
	write_byte(DS3231_ALARM1MINUTE,Tmp);
	Tmp=HEX2BCD(A1H);
	write_byte(DS3231_ALARM1HOUR,Tmp);
	write_byte(DS3231_ALARM1WEEK,0x80);
											  
	Tmp=HEX2BCD(A2M);
	write_byte(DS3231_ALARM2MINUTE,Tmp);
	Tmp=HEX2BCD(A2H);
	write_byte(DS3231_ALARM2HOUR,Tmp);
	write_byte(DS3231_ALARM2WEEK,0x80);

	Sector_Erase(0x6000);
	Byte_Program(0x6000,A1ONOFF);
	Byte_Program(0x6001,A1H);
	Byte_Program(0x6002,A1M);
	Byte_Program(0x6003,A2ONOFF);
	Byte_Program(0x6004,A2H);
	Byte_Program(0x6005,A2M);
		
	Tmp	=	0x04;
	if(A1ONOFF)	Tmp|=0x01;
	if(A2ONOFF)	Tmp|=0x02;

	write_byte(DS3231_CONTROL,Tmp);
	AlarmReset();
}	

void	FormatInit()
{	   		   
	FormatTemp	=	Byte_Read(0x6400);
	FormatDate	=	Byte_Read(0x6401);
	FormatTime	=	Byte_Read(0x6402);
}	

void	FormatSave()
{	
	Sector_Erase(0x6400);   		   
	Byte_Program(0x6400,FormatTemp);
	Byte_Program(0x6401,FormatDate);
	Byte_Program(0x6402,FormatTime);
}	

/********************************************************************************************************
*********************************************************************************************************
*********************************************************************************************************
*********************************************************************************************************
*********************************************************************************************************
*********************************************************************************************************
********************************************     Main     ***********************************************
*********************************************************************************************************
*********************************************************************************************************
*********************************************************************************************************
*********************************************************************************************************
*********************************************************************************************************
** 	函数名称:			void	main()
**	功能描述:			主函数
**	创 建 者:			严泽远
**	创建时间:			2011-02-01 14:11
**	版    本:			v1.0.0
********************************************************************************************************/
void	main()
{
	bit		r,Flash;					//Temp
	uchar	i=0;						//Temp

	POWERON	=	1;						//关闭电源开关
	SPK		=	0;						//关闭蜂鸣器
	
	Port_IO_Init();						//IO资源配置
	TimerInit();						//定时器1初始化
	
	LEDSendData(0,0,0);	   				//初始化关掉RGB	

	ET0		=	1;						//允许定时器0中断
	ET1		=	1;						//允许定时器1中断
	EA		=	1;						//允许所有中断

	DelayMS(500);						//---------------------
	Beep(BEEPPOWERON);					//---------------------
	AlarmInit();						//闹钟初始化
	FormatInit();						//格式初始化
	
	POWERON	=	0;						//打开电源开关	  
	
	while(1)
    {	
		//***********************************************************************************************
		//主循环必须执行
		r	=	DispNum(Num,NUMmode);			//动态实时刷新显示数字
		RGBFlash(RGBmode);						//RGB控制
		if(ALARM==0 && F_Alarm==0)	F_Alarm	=1;	//闹钟时间到
		if(B_IR_Press)	 						//红外线遥控数据解码
		{
			IR_Process(IR_code);
			B_IR_Press	=	0;
		}
		//***********************************************************************************************
		//50ms标志
		if(F_50ms)
		{
			if(r)	GetDateTime();
			F_50ms	=	0;
		}	
		//***********************************************************************************************
		//1s标志
		if(F_1s)
		{
			if(NOWSTATE==S_POWEROFF)			//如果是关机状态则继续执行1s的判断
			{
				RGB1s();
			}
			F_1s	=	0;
		}
		//***********************************************************************************************
		//2s标志
		if(F_2s)
		{
			if(r && F_Alarm)
			{
				Beep(BEEPERROR);				//BEEP
				F_Alarm++;
				if(F_Alarm>15)
				{
					AlarmReset();				//30s后闹钟复位
					F_Alarm	=	0;
				}
				F_2s	=	0;
			}
		}
		//***********************************************************************************************
		//根据当前状态执行
		switch(NOWSTATE)
		{
			case	S_NORMAL:					//正常显示时间状态
				if(OLDSECOND!=SECOND)
				{
					if(FormatTime)
					{	
						if(HOUR>12)i=HOUR-12;else i=HOUR;
					}
					else	i=HOUR;

					Num[0]	=	'0'+i/10;		//赋值当前时间
					Num[1]	=	'0'+i%10;		//赋值当前时间
					Num[2]	=	'0'+MINUTE/10;	//赋值当前时间
					Num[3]	=	'0'+MINUTE%10;	//赋值当前时间
					Num[4]	=	'0'+SECOND/10;	//赋值当前时间
					Num[5]	=	'0'+SECOND%10;	//赋值当前时间
		
					DOT1s();
					RGB1s();
					NUM1s();
		
					OLDSECOND=SECOND;
				}
				break;
			case	S_POWEROFF:					//关机状态
				if(POWERON==0 && LASTN[0]==' ')POWERON=1;	//渐变完成后关机
				break;
			case	S_SHOWDATE:					//显示日期状态
			case	S_SHOWMOONDATE:				//显示农历日期状态
			case	S_SHOWA1:					//显示闹钟1
			case	S_SHOWA2:					//显示闹钟2
			case	S_SHOWTEMP:					//显示温度状态	 
				if(OLDSECOND!=SECOND)
				{
					RGB1s();
							
					OLDSECOND=SECOND;
				}
				if(F_100ms)
				{
					if(ShowDateLoop<255)ShowDateLoop++;
					if(ShowDateLoop>=(SHOWDATELOOPCOUNT*10)) 	//结束日期显示
					{
						NOWSTATE	=	S_NORMAL;
					}
					F_100ms=0;
				}
				break;
			case	S_SETHOUR:					//调整小时状态
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	(Flash?' ':'0'+S_HOUR/10);	//赋值当前时间
					Num[1]	=	(Flash?' ':'0'+S_HOUR%10);	//赋值当前时间
					Num[2]	=	'0'+S_MINUTE/10;//赋值当前时间
					Num[3]	=	'0'+S_MINUTE%10;//赋值当前时间
					Num[4]	=	'0'+S_SECOND/10;//赋值当前时间
					Num[5]	=	'0'+S_SECOND%10;//赋值当前时间
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETMINUTE:				//调整分钟状态
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	'0'+S_HOUR/10;	//赋值当前时间
					Num[1]	=	'0'+S_HOUR%10;	//赋值当前时间
					Num[2]	=	(Flash?' ':'0'+S_MINUTE/10);	//赋值当前时间
					Num[3]	=	(Flash?' ':'0'+S_MINUTE%10);	//赋值当前时间
					Num[4]	=	'0'+S_SECOND/10;//赋值当前时间
					Num[5]	=	'0'+S_SECOND%10;//赋值当前时间
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETSECOND:				//调整秒状态
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	'0'+S_HOUR/10;	//赋值当前时间
					Num[1]	=	'0'+S_HOUR%10;	//赋值当前时间
					Num[2]	=	'0'+S_MINUTE/10;//赋值当前时间
					Num[3]	=	'0'+S_MINUTE%10;//赋值当前时间
					Num[4]	=	(Flash?' ':'0'+S_SECOND/10);	//赋值当前时间
					Num[5]	=	(Flash?' ':'0'+S_SECOND%10);	//赋值当前时间
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETYEAR:					//调整年状态
				if(F_250ms)
				{
					Flash	=	!Flash;
					if(FormatDate)				//MM-DD-YY
					{										   
						Num[0]	=	'0'+S_MONTH/10;//赋值当前日期
						Num[1]	=	'0'+S_MONTH%10;//赋值当前日期
						Num[2]	=	'0'+S_DAY/10;//赋值当前日期
						Num[3]	=	'0'+S_DAY%10;//赋值当前日期
						Num[4]	=	(Flash?' ':'0'+S_YEAR/10);	//赋值当前日期
						Num[5]	=	(Flash?' ':'0'+S_YEAR%10);	//赋值当前日期
					}
					else
					{											  
						Num[0]	=	(Flash?' ':'0'+S_YEAR/10);	//赋值当前日期
						Num[1]	=	(Flash?' ':'0'+S_YEAR%10);	//赋值当前日期
						Num[2]	=	'0'+S_MONTH/10;//赋值当前日期
						Num[3]	=	'0'+S_MONTH%10;//赋值当前日期
						Num[4]	=	'0'+S_DAY/10;//赋值当前日期
						Num[5]	=	'0'+S_DAY%10;//赋值当前日期
					}
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETMONTH:					//调整月状态
				if(F_250ms)
				{
					Flash	=	!Flash;	 										   
					if(FormatDate)				//MM-DD-YY
					{										   
						Num[0]	=	(Flash?' ':'0'+S_MONTH/10);	//赋值当前日期
						Num[1]	=	(Flash?' ':'0'+S_MONTH%10);	//赋值当前日期
						Num[2]	=	'0'+S_DAY/10;//赋值当前日期
						Num[3]	=	'0'+S_DAY%10;//赋值当前日期
						Num[4]	=	'0'+S_YEAR/10;	//赋值当前日期
						Num[5]	=	'0'+S_YEAR%10;	//赋值当前日期
					}
					else
					{													
						Num[0]	=	'0'+S_YEAR/10;	//赋值当前日期
						Num[1]	=	'0'+S_YEAR%10;	//赋值当前日期
						Num[2]	=	(Flash?' ':'0'+S_MONTH/10);	//赋值当前日期
						Num[3]	=	(Flash?' ':'0'+S_MONTH%10);	//赋值当前日期
						Num[4]	=	'0'+S_DAY/10;//赋值当前日期
						Num[5]	=	'0'+S_DAY%10;//赋值当前日期
					}
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETDAY:					//调整日状态
				if(F_250ms)
				{
					Flash	=	!Flash;									   
					if(FormatDate)				//MM-DD-YY
					{														  
						Num[0]	=	'0'+S_MONTH/10;//赋值当前日期
						Num[1]	=	'0'+S_MONTH%10;//赋值当前日期
						Num[2]	=	(Flash?' ':'0'+S_DAY/10);	//赋值当前日期
						Num[3]	=	(Flash?' ':'0'+S_DAY%10);	//赋值当前日期
						Num[4]	=	'0'+S_YEAR/10;	//赋值当前日期
						Num[5]	=	'0'+S_YEAR%10;	//赋值当前日期
					}
					else
					{											   
						Num[0]	=	'0'+S_YEAR/10;	//赋值当前日期
						Num[1]	=	'0'+S_YEAR%10;	//赋值当前日期
						Num[2]	=	'0'+S_MONTH/10;//赋值当前日期
						Num[3]	=	'0'+S_MONTH%10;//赋值当前日期
						Num[4]	=	(Flash?' ':'0'+S_DAY/10);	//赋值当前日期
						Num[5]	=	(Flash?' ':'0'+S_DAY%10);	//赋值当前日期
					}
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETA1H:					//调整闹钟1小时状态
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	'a';					//赋值当前闹钟显示
					Num[1]	=	'1';					//赋值当前闹钟显示
					Num[2]	=	(Flash?' ':(S_SECOND?'0'+S_HOUR/10:' '));	//赋值当前闹钟显示
					Num[3]	=	(Flash?' ':(S_SECOND?'0'+S_HOUR%10:'0'));	//赋值当前闹钟显示
					Num[4]	=	(S_SECOND?'0'+S_MINUTE/10:(Flash?' ':'f'));	//赋值当前闹钟显示
					Num[5]	=	(S_SECOND?'0'+S_MINUTE%10:(Flash?' ':'f'));	//赋值当前闹钟显示
		
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETA1M:					//调整闹钟1分钟状态
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	'a';					//赋值当前闹钟显示
					Num[1]	=	'1';					//赋值当前闹钟显示
					Num[2]	=	(S_SECOND?'0'+S_HOUR/10:' ');				//赋值当前闹钟显示
					Num[3]	=	(S_SECOND?'0'+S_HOUR%10:(Flash?' ':'0'));	//赋值当前闹钟显示
					Num[4]	=	(Flash?' ':(S_SECOND?'0'+S_MINUTE/10:'f'));	//赋值当前闹钟显示
					Num[5]	=	(Flash?' ':(S_SECOND?'0'+S_MINUTE%10:'f'));	//赋值当前闹钟显示
		
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETA2H:					//调整闹钟2小时状态
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	'a';					//赋值当前闹钟显示
					Num[1]	=	'2';					//赋值当前闹钟显示
					Num[2]	=	(Flash?' ':(S_SECOND?'0'+S_HOUR/10:' '));	//赋值当前闹钟显示
					Num[3]	=	(Flash?' ':(S_SECOND?'0'+S_HOUR%10:'0'));	//赋值当前闹钟显示
					Num[4]	=	(S_SECOND?'0'+S_MINUTE/10:(Flash?' ':'f'));	//赋值当前闹钟显示
					Num[5]	=	(S_SECOND?'0'+S_MINUTE%10:(Flash?' ':'f'));	//赋值当前闹钟显示
		
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETA2M:					//调整闹钟2分钟状态
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	'a';					//赋值当前闹钟显示
					Num[1]	=	'2';					//赋值当前闹钟显示
					Num[2]	=	(S_SECOND?'0'+S_HOUR/10:' ');				//赋值当前闹钟显示
					Num[3]	=	(S_SECOND?'0'+S_HOUR%10:(Flash?' ':'0'));	//赋值当前闹钟显示
					Num[4]	=	(Flash?' ':(S_SECOND?'0'+S_MINUTE/10:'f'));	//赋值当前闹钟显示
					Num[5]	=	(Flash?' ':(S_SECOND?'0'+S_MINUTE%10:'f'));	//赋值当前闹钟显示
		
					NUM1s();

					F_250ms	=	0;
				}
				break;
		}
	}					
}