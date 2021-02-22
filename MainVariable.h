/****************************************Copyright (c)**************************************************
**				
**	项目名称:			YS18-3 Clock
**	文件名称:			MainVariable.h
**	模块功能:			全局变量定义
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

/********************************************************************************************************
** 	全局常数定义
********************************************************************************************************/
sbit	POWERON		=	P1^7;		//电源开关
sbit	DOTS		=	P3^7;		//秒针点点
sbit	SPK			=	P1^0;		//蜂鸣器

sbit	LEDDATA		=	P1^5;		//LPD6803数据接口
sbit	LEDCLOCK	=	P1^4;		//LPD6803数据接口
sbit	IR		 	= 	P3^2;		//定义红外接收输入端口
sbit	ALARM	 	= 	P3^3;		//定义闹钟输入端口

sbit	DATA	 	= 	P1^1;		//74HC595数据接口
sbit	SHCP	 	= 	P1^3;		//74HC595数据接口
sbit	STCP	 	= 	P1^2;		//74HC595数据接口

/********************************************************************************************************
** 	全局变量定义
********************************************************************************************************/
uint	DOTloop		=	0;			//DOT闪烁计算总循环
uchar	DOTpwmloop	=	0;			//DOT闪烁计算循环
uchar	DOTcut		=	0;			//DOT上次变换计时次数
bit		DOTonoff	=	0;			//DOT明暗状态
//uchar	DOTbright	=	10;			//DOT亮度

uchar	RGBmode		=	9;			//RGB模式
uchar	RGBmodeold	=	0xFF;		//RGB暂存模式
uchar	RGBmodesave	=	0;			//RGB关闭时暂存模式
uint	RGBloop		=	0;			//RGB计算循环
uint	RGBlaststate=	0;			//RGB上次变换计时次数
uchar	RGBbright	=	31;			//RGB亮度
uchar	RGBlastcolor=	0;			//RGB上次色彩

uchar	RGBr		=	0;			//RGB红
uchar	RGBg		=	0;			//RGB绿
uchar	RGBb		=	0;			//RGB蓝

xdata	uint	F_50ms_Loop		=	0;
xdata	uint	F_100ms_Loop	=	0;
xdata	uint	F_250ms_Loop	=	0;
xdata	uint	F_500ms_Loop	=	0;
xdata	uint	F_1s_Loop		=	0;
xdata	uint	F_2s_Loop		=	0;

bit	F_50ms	=	1;
bit	F_100ms	=	1;
bit	F_250ms	=	1;
bit	F_500ms	=	1;
bit	F_1s	=	1; 
bit	F_2s	=	1; 

uchar	F_Alarm	=	0;				//是否在闹铃
																									 
uchar	Num[6]="      ";			//荧光管显示的字符变量
uchar	LASTN[6]=	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uchar	NUMcut	=	0;
uint	NUMloop	=	0;
uint	NUM500msloop=0;				//500ms渐变模式计数
uint	NUMpwmloop=	0; 
uint	NUMmode	=	0;				//NUM模式

uchar	NUMLOOPCOUNT=25;			//NUM颜色渐变的STEP毫秒数

uchar	ShowDateLoop=	0;			//查看日期的延时

uchar	NOWSTATE	=	0;			//当前的模式

xdata	uchar	YEAR;
xdata	uchar	MONTH;
xdata	uchar	DAY;
xdata	uchar	WEEK;
xdata	uchar	HOUR;
xdata	uchar	MINUTE;
xdata	uchar	SECOND;

xdata	uchar	S_YEAR;
xdata	uchar	S_MONTH;
xdata	uchar	S_DAY;
xdata	uchar	S_HOUR;
xdata	uchar	S_MINUTE;
xdata	uchar	S_SECOND;

xdata	uchar	OLDYEAR=0xFF;
xdata	uchar	OLDMONTH=0xFF;
xdata	uchar	OLDDAY=0xFF;
xdata	uchar	OLDWEEK=0xFF;
xdata	uchar	OLDHOUR=0xFF;
xdata	uchar	OLDMINUTE=0xFF;
xdata	uchar	OLDSECOND=0xFF;

xdata	uchar	A1ONOFF,A1H,A1M,A2ONOFF,A2H,A2M;

uchar	FormatTemp;		//温度格式 0:C 1:F
uchar	FormatDate;		//日期格式 0:YY-MM-DD 1:MM-DD-YY
uchar	FormatTime;		//时间格式 0:24h 1:12h

/********************************************************************************************************
** 	IR变量定义
********************************************************************************************************/
bit		P_IR_RX_temp;		//Last sample
bit		B_IR_Sync;			//已收到同步标志
uchar	IR_SampleCnt;		//采样计数
uchar	IR_BitCnt;			//编码位数
uchar	IR_UserH;			//用户码(地址)高字节
uchar	IR_UserL;			//用户码(地址)低字节
uchar	IR_data;			//数据原码
uchar	IR_DataShit;		//数据反码
bit		B_IrUserErr;		//User code error flag
bit		B_IR_Press;			//Key press flag,include repeat key.
uchar	IR_code;			//IR code	红外键码

/********************************************************************************************************
** 	扩展函数定义
********************************************************************************************************/
extern	data 	uchar year_moon,month_moon,day_moon,week;
extern	uchar 	read_random(uchar random_addr);
extern	uchar	BCD2HEX(uchar val);
extern	void	ModifyTime(uchar yea,uchar mon,uchar da,uchar hou,uchar min,uchar sec);
extern	void 	Conversion(bit c,uchar year,uchar month,uchar day);
extern	uint 	GetTemp();

extern	uchar	HEX2BCD(uchar val);
extern	uchar	write_byte(uchar addr, uchar write_data);
extern	uchar	read_random(uchar random_addr);

extern	uchar	Byte_Read(uint add);            	  	//读一字节，调用前需打开IAP 功能
extern	void	Byte_Program(uint add, uchar ch); 		//字节编程，调用前需打开IAP 功能
extern	void 	Sector_Erase(uint add);