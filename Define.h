/****************************************Copyright (c)**************************************************
**				
**	��Ŀ����:			YS18-3 Clock
**	�ļ�����:			Define.h
**	ģ�鹦��:			ȫ�ֳ�������
**
**------------------------------------------------------------------------------------------------------

** 	�� �� ��:			����Զ
**	E-mail  :			yanzeyuan@163.com
**	QQ		:			6626209
**	Mobile	:			18602007878
**	����ʱ��:			2011-02-01
**	��    ��:			v1.0.0
**	��    ��:			��������
**
********************************************************************************************************/

#include <intrins.h>

/********************************************************************************************************
** 	�������Ͷ���
********************************************************************************************************/
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

/********************************************************************************************************
** 	ϵͳ��Ҫ��������
********************************************************************************************************/
#define FOSC			22118400					//����Ƶ��
#define T1MS			(65536-FOSC/1000)			//1ms�ļ���ֵ
#define T125US			(65536-FOSC/8000)			//125us�ļ���ֵ

#define BEEPOPERATE		1							//��������������
#define BEEPSHUTDOWN	2							//�����ػ���ʾ��
#define BEEPERROR		3							//������ʾ��
#define BEEPCONFIRM		4							//ȷ����ʾ��
#define BEEPPOWERON		5							//����������ʾ��

#define	DOTLOOPCOUNT	25							//DOT��ɫ�����STEP������	
#define	RGBLOOPCOUNT	25							//RGB��ɫ�����STEP������
//#define	NUMLOOPCOUNT	25							//NUM��ɫ�����STEP������

#define	SHOWDATELOOPCOUNT	3						//�鿴���ڵ���ʱ����

#define	RGBMOFF			0							//RGB��ɫģʽ	��
#define	RGBMR			1							//RGB��ɫģʽ	��
#define	RGBMRG			2							//RGB��ɫģʽ	����
#define	RGBMG			3							//RGB��ɫģʽ	��
#define	RGBMGB			4							//RGB��ɫģʽ	����
#define	RGBMB			5							//RGB��ɫģʽ	��
#define	RGBMRB			6							//RGB��ɫģʽ	����
#define	RGBMRGB			7							//RGB��ɫģʽ	��
#define	RGBMFADE		8							//RGB��ɫģʽ	��������
#define	RGBMFADEC		9							//RGB��ɫģʽ	��������

#define	S_NORMAL		0							//ģʽ	����
#define	S_POWEROFF		1							//ģʽ	�ػ�
#define	S_SHOWDATE		2							//ģʽ	��ʾ����
#define	S_SHOWMOONDATE	3							//ģʽ	��ʾũ������
#define	S_SHOWTEMP		4							//ģʽ	��ʾ�¶�
#define	S_SETYEAR		5							//ģʽ	������
#define	S_SETMONTH		6							//ģʽ	������
#define	S_SETDAY		7							//ģʽ	������
#define	S_SETHOUR		8							//ģʽ	����Сʱ
#define	S_SETMINUTE		9							//ģʽ	���÷���
#define	S_SETSECOND		10							//ģʽ	������
#define	S_SHOWA1		11							//ģʽ	��ʾ����1
#define	S_SHOWA2		12							//ģʽ	��ʾ����2
#define	S_SETA1H		13							//ģʽ	��������1Сʱ
#define	S_SETA1M		14							//ģʽ	��������1����
#define	S_SETA2H		15							//ģʽ	��������2Сʱ
#define	S_SETA2M		16							//ģʽ	��������2����

/********************************************************************************************************
** 	IRӦ����Ҫ��������		  
********************************************************************************************************/
#define D_TIMER0			125						//ѡ��ʱ��ʱ��, us, �������Ҫ����60us~250us֮��
#define	D_IR_sample			D_TIMER0				//�������ʱ�䣬��60us~250us֮��
#define	D_IR_usercode		0xFF00					//������ϵͳ��

#define D_IR_SYNC_MAX		(15000/D_IR_sample)		//SYNC max time
#define D_IR_SYNC_MIN		(9700 /D_IR_sample)		//SYNC min time
#define D_IR_SYNC_DIVIDE	(12375/D_IR_sample)		//decide data 0 or 1
#define D_IR_DATA_MAX		(3000 /D_IR_sample)		//data max time
#define D_IR_DATA_MIN		(600  /D_IR_sample)		//data min time
#define D_IR_DATA_DIVIDE	(1687 /D_IR_sample)		//decide data 0 or 1
#define D_IR_BIT_NUMBER		32						//bit number

/********************************************************************************************************
** 	DS3231��������
********************************************************************************************************/
#define DS3231_WriteAddress 0xD0    //����д��ַ 
#define DS3231_ReadAddress  0xD1    //��������ַ

#define DS3231_SECOND       0x00    //��
#define DS3231_MINUTE       0x01    //��
#define DS3231_HOUR         0x02    //ʱ
#define DS3231_WEEK         0x03    //����
#define DS3231_DAY          0x04    //��
#define DS3231_MONTH        0x05    //��
#define DS3231_YEAR         0x06    //��
//����1            
#define DS3231_ALARM1SECOND 0x07    //��
#define DS3231_ALARM1MINUTE 0x08    //��
#define DS3231_ALARM1HOUR   0x09    //ʱ
#define DS3231_ALARM1WEEK   0x0A    //����/��
//����2
#define DS3231_ALARM2MINUTE 0x0b    //��
#define DS3231_ALARM2HOUR   0x0c    //ʱ
#define DS3231_ALARM2WEEK   0x0d    //����/��

#define DS3231_CONTROL      0x0e    //���ƼĴ���
#define DS3231_STATUS       0x0f    //״̬�Ĵ���
#define BSY                 2       //æ
#define OSF                 7       //����ֹͣ��־
#define DS3231_XTAL         0x10    //�����ϻ��Ĵ���
#define DS3231_TEMPERATUREH 0x11    //�¶ȼĴ������ֽ�(8λ)
#define DS3231_TEMPERATUREL 0x12    //�¶ȼĴ������ֽ�(��2λ)  	  
