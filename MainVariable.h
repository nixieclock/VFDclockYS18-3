/****************************************Copyright (c)**************************************************
**				
**	��Ŀ����:			YS18-3 Clock
**	�ļ�����:			MainVariable.h
**	ģ�鹦��:			ȫ�ֱ�������
**
**------------------------------------------------------------------------------------------------------

** 	�� �� ��:			����Զ
**	E-mail  :			yanzeyuan@163.com
**	QQ		:			6626209
**	Mobile	:			18602007878
**	����ʱ��:			2011-01-31
**	��    ��:			v1.0.0
**	��    ��:			��������
**
********************************************************************************************************/

/********************************************************************************************************
** 	ȫ�ֳ�������
********************************************************************************************************/
sbit	POWERON		=	P1^7;		//��Դ����
sbit	DOTS		=	P3^7;		//������
sbit	SPK			=	P1^0;		//������

sbit	LEDDATA		=	P1^5;		//LPD6803���ݽӿ�
sbit	LEDCLOCK	=	P1^4;		//LPD6803���ݽӿ�
sbit	IR		 	= 	P3^2;		//��������������˿�
sbit	ALARM	 	= 	P3^3;		//������������˿�

sbit	DATA	 	= 	P1^1;		//74HC595���ݽӿ�
sbit	SHCP	 	= 	P1^3;		//74HC595���ݽӿ�
sbit	STCP	 	= 	P1^2;		//74HC595���ݽӿ�

/********************************************************************************************************
** 	ȫ�ֱ�������
********************************************************************************************************/
uint	DOTloop		=	0;			//DOT��˸������ѭ��
uchar	DOTpwmloop	=	0;			//DOT��˸����ѭ��
uchar	DOTcut		=	0;			//DOT�ϴα任��ʱ����
bit		DOTonoff	=	0;			//DOT����״̬
//uchar	DOTbright	=	10;			//DOT����

uchar	RGBmode		=	9;			//RGBģʽ
uchar	RGBmodeold	=	0xFF;		//RGB�ݴ�ģʽ
uchar	RGBmodesave	=	0;			//RGB�ر�ʱ�ݴ�ģʽ
uint	RGBloop		=	0;			//RGB����ѭ��
uint	RGBlaststate=	0;			//RGB�ϴα任��ʱ����
uchar	RGBbright	=	31;			//RGB����
uchar	RGBlastcolor=	0;			//RGB�ϴ�ɫ��

uchar	RGBr		=	0;			//RGB��
uchar	RGBg		=	0;			//RGB��
uchar	RGBb		=	0;			//RGB��

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

uchar	F_Alarm	=	0;				//�Ƿ�������
																									 
uchar	Num[6]="      ";			//ӫ�����ʾ���ַ�����
uchar	LASTN[6]=	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
uchar	NUMcut	=	0;
uint	NUMloop	=	0;
uint	NUM500msloop=0;				//500ms����ģʽ����
uint	NUMpwmloop=	0; 
uint	NUMmode	=	0;				//NUMģʽ

uchar	NUMLOOPCOUNT=25;			//NUM��ɫ�����STEP������

uchar	ShowDateLoop=	0;			//�鿴���ڵ���ʱ

uchar	NOWSTATE	=	0;			//��ǰ��ģʽ

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

uchar	FormatTemp;		//�¶ȸ�ʽ 0:C 1:F
uchar	FormatDate;		//���ڸ�ʽ 0:YY-MM-DD 1:MM-DD-YY
uchar	FormatTime;		//ʱ���ʽ 0:24h 1:12h

/********************************************************************************************************
** 	IR��������
********************************************************************************************************/
bit		P_IR_RX_temp;		//Last sample
bit		B_IR_Sync;			//���յ�ͬ����־
uchar	IR_SampleCnt;		//��������
uchar	IR_BitCnt;			//����λ��
uchar	IR_UserH;			//�û���(��ַ)���ֽ�
uchar	IR_UserL;			//�û���(��ַ)���ֽ�
uchar	IR_data;			//����ԭ��
uchar	IR_DataShit;		//���ݷ���
bit		B_IrUserErr;		//User code error flag
bit		B_IR_Press;			//Key press flag,include repeat key.
uchar	IR_code;			//IR code	�������

/********************************************************************************************************
** 	��չ��������
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

extern	uchar	Byte_Read(uint add);            	  	//��һ�ֽڣ�����ǰ���IAP ����
extern	void	Byte_Program(uint add, uchar ch); 		//�ֽڱ�̣�����ǰ���IAP ����
extern	void 	Sector_Erase(uint add);