/****************************************Copyright (c)**************************************************
**				
**	项目名称:			YS18-3 Clock
**	文件名称:			Define.h
**	模块功能:			全局常量定义
**
**------------------------------------------------------------------------------------------------------

** 	创 建 者:			严泽远
**	E-mail  :			yanzeyuan@163.com
**	QQ		:			6626209
**	Mobile	:			18602007878
**	创建时间:			2011-02-01
**	版    本:			v1.0.0
**	描    述:			基础程序
**
********************************************************************************************************/

#include <intrins.h>

/********************************************************************************************************
** 	数据类型定义
********************************************************************************************************/
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

/********************************************************************************************************
** 	系统主要参数定义
********************************************************************************************************/
#define FOSC			22118400					//晶振频率
#define T1MS			(65536-FOSC/1000)			//1ms的计算值
#define T125US			(65536-FOSC/8000)			//125us的计算值

#define BEEPOPERATE		1							//正常操作按键音
#define BEEPSHUTDOWN	2							//正常关机提示音
#define BEEPERROR		3							//错误提示音
#define BEEPCONFIRM		4							//确认提示音
#define BEEPPOWERON		5							//正常开机提示音

#define	DOTLOOPCOUNT	25							//DOT颜色渐变的STEP毫秒数	
#define	RGBLOOPCOUNT	25							//RGB颜色渐变的STEP毫秒数
//#define	NUMLOOPCOUNT	25							//NUM颜色渐变的STEP毫秒数

#define	SHOWDATELOOPCOUNT	3						//查看日期的延时秒数

#define	RGBMOFF			0							//RGB颜色模式	关
#define	RGBMR			1							//RGB颜色模式	红
#define	RGBMRG			2							//RGB颜色模式	红绿
#define	RGBMG			3							//RGB颜色模式	绿
#define	RGBMGB			4							//RGB颜色模式	绿蓝
#define	RGBMB			5							//RGB颜色模式	蓝
#define	RGBMRB			6							//RGB颜色模式	红蓝
#define	RGBMRGB			7							//RGB颜色模式	白
#define	RGBMFADE		8							//RGB颜色模式	明暗渐变
#define	RGBMFADEC		9							//RGB颜色模式	连续渐变

#define	S_NORMAL		0							//模式	正常
#define	S_POWEROFF		1							//模式	关机
#define	S_SHOWDATE		2							//模式	显示日期
#define	S_SHOWMOONDATE	3							//模式	显示农历日期
#define	S_SHOWTEMP		4							//模式	显示温度
#define	S_SETYEAR		5							//模式	设置年
#define	S_SETMONTH		6							//模式	设置月
#define	S_SETDAY		7							//模式	设置日
#define	S_SETHOUR		8							//模式	设置小时
#define	S_SETMINUTE		9							//模式	设置分钟
#define	S_SETSECOND		10							//模式	设置秒
#define	S_SHOWA1		11							//模式	显示闹钟1
#define	S_SHOWA2		12							//模式	显示闹钟2
#define	S_SETA1H		13							//模式	设置闹钟1小时
#define	S_SETA1M		14							//模式	设置闹钟1分钟
#define	S_SETA2H		15							//模式	设置闹钟2小时
#define	S_SETA2M		16							//模式	设置闹钟2分钟

/********************************************************************************************************
** 	IR应用主要参数定义		  
********************************************************************************************************/
#define D_TIMER0			125						//选择定时器时间, us, 红外接收要求在60us~250us之间
#define	D_IR_sample			D_TIMER0				//定义采样时间，在60us~250us之间
#define	D_IR_usercode		0xFF00					//红外线系统码

#define D_IR_SYNC_MAX		(15000/D_IR_sample)		//SYNC max time
#define D_IR_SYNC_MIN		(9700 /D_IR_sample)		//SYNC min time
#define D_IR_SYNC_DIVIDE	(12375/D_IR_sample)		//decide data 0 or 1
#define D_IR_DATA_MAX		(3000 /D_IR_sample)		//data max time
#define D_IR_DATA_MIN		(600  /D_IR_sample)		//data min time
#define D_IR_DATA_DIVIDE	(1687 /D_IR_sample)		//decide data 0 or 1
#define D_IR_BIT_NUMBER		32						//bit number

/********************************************************************************************************
** 	DS3231常数定义
********************************************************************************************************/
#define DS3231_WriteAddress 0xD0    //器件写地址 
#define DS3231_ReadAddress  0xD1    //器件读地址

#define DS3231_SECOND       0x00    //秒
#define DS3231_MINUTE       0x01    //分
#define DS3231_HOUR         0x02    //时
#define DS3231_WEEK         0x03    //星期
#define DS3231_DAY          0x04    //日
#define DS3231_MONTH        0x05    //月
#define DS3231_YEAR         0x06    //年
//闹铃1            
#define DS3231_ALARM1SECOND 0x07    //秒
#define DS3231_ALARM1MINUTE 0x08    //分
#define DS3231_ALARM1HOUR   0x09    //时
#define DS3231_ALARM1WEEK   0x0A    //星期/日
//闹铃2
#define DS3231_ALARM2MINUTE 0x0b    //分
#define DS3231_ALARM2HOUR   0x0c    //时
#define DS3231_ALARM2WEEK   0x0d    //星期/日

#define DS3231_CONTROL      0x0e    //控制寄存器
#define DS3231_STATUS       0x0f    //状态寄存器
#define BSY                 2       //忙
#define OSF                 7       //振荡器停止标志
#define DS3231_XTAL         0x10    //晶体老化寄存器
#define DS3231_TEMPERATUREH 0x11    //温度寄存器高字节(8位)
#define DS3231_TEMPERATUREL 0x12    //温度寄存器低字节(高2位)  	  
