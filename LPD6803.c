/****************************************Copyright (c)**************************************************
**				
**	项目名称:			YS18-3 Clock
**	文件名称:			LPD6803.c
**	模块功能:			实现LPD6803的全功能控制
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

#include <STC12C5620AD.H>				//加载STC12C5620AD.H头文件
#include <Define.H>						//加载Define.H头文件
#include <LPD6803Variable.H>			//加载LPD6803Variable.H头文件

/********************************************************************************************************
** 	函数名称:			void	LEDSendData(uchar dr,uchar dg,uchar db)
**	功能描述:			输出3个5位RGB颜色数据子程序，参数格式：红颜色数据、绿颜色数据、蓝颜色数据    
**	创 建 者:			严泽远
**	创建时间:			2011-02-01 00:02
**	版    本:			v1.0.0
********************************************************************************************************/
void	LEDSendData(uchar dr,uchar dg,uchar db)
{  
	uchar k,mask;         
	LEDDATA=0;	LEDCLOCK=0;
	for(k=0;k<32;k++)	{LEDCLOCK=1;LEDCLOCK=0;}	//送起始位32bits 0
    LEDDATA=1;LEDCLOCK=1;LEDCLOCK=0;   	//首先输出1个'1'起始位
    mask=0x10;   
    for(k=0;k<5;k++)   		//输出5位红色数据    
    {    if(mask & dg)    LEDDATA=1;   
         else             LEDDATA=0;   
         LEDCLOCK=1;LEDCLOCK=0;   
         mask>>=1;      
	}   	        
    mask=0x10;   
    for(k=0;k<5;k++)   	  	//输出5位绿色数据  
    {   if(mask & dr)   LEDDATA=1;   
        else            LEDDATA=0;   
        LEDCLOCK=1;LEDCLOCK=0;   
        mask>>=1;       
	}   		     
    mask=0x10;   
    for(k=0;k<5;k++)   	 	//输出5位兰色数据   
    {   if(mask & db)   LEDDATA=1;   
        else            LEDDATA=0;   
        LEDCLOCK=1;LEDCLOCK=0;   
        mask>>=1;       
	}   
	LEDDATA=1;LEDCLOCK=1;LEDCLOCK=0;   	//首先输出1个'1'起始位
}  