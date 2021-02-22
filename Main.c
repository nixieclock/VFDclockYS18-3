/****************************************Copyright (c)**************************************************
**				
**	��Ŀ����:			YS18-3 Clock
**	�ļ�����:			Main.c
**	ģ�鹦��:			������
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

#include <STC12C5620AD.H>				//����STC12C5620AD.Hͷ�ļ�
#include <Define.H>						//����Define.Hͷ�ļ�
#include <MainVariable.H>				//����MainVariable.Hͷ�ļ�
#include <LPD6803Function.h>			//����LPD6803Function.hͷ�ļ�

/********************************************************************************************************
** 	��������
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
** 	��������:			void	Port_IO_Init()
**	��������:			IO�˿�����
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-01 00:02
**	��    ��:			v1.0.0
********************************************************************************************************/
void	Port_IO_Init()
{
    P1M1=0x3F;  	//����P1�˿�ģʽ
    P3M1=0x80;  	//����P3�˿�ģʽ
}
																								  
/********************************************************************************************************
** 	��������:			void	DelayMS(uint N)
**	��������:			��ʱN����
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-01 17:48
**	��    ��:			v1.0.0
********************************************************************************************************/
void	DelayMS(uint N)	  
{
	uint i,j;
	for(j=0;j<N;j++)
	for(i=0;i<2050;i++)
	{;}
}

/********************************************************************************************************
** 	��������:			void	TimerInit()
**	��������:			��ʱ����ʼ��
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-02 15:39
**	��    ��:			v1.0.0
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
** 	��������:			void	Timer0Interrupt() interrupt 1
**	��������:			��ʱ��0�жϺ���
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-02 15:39
**	��    ��:			v1.0.0
********************************************************************************************************/
void	Timer0Interrupt() interrupt 1
{	   	 
	TL0		=	T125US;			//Reload timer0 low byte
	TH0		=	T125US>>8;		//Reload timer0 high byte
	IR_RX();  	
	//***************************************************************************************************
	//ð����˸PWM
	if(DOTpwmloop<31)		DOTpwmloop++;	else	DOTpwmloop=0;	
	if(DOTpwmloop<=DOTcut)	DOTS=!DOTonoff;	else	DOTS=DOTonoff;				
}
							
/********************************************************************************************************
** 	��������:			void	Timer1Interrupt() interrupt 3
**	��������:			��ʱ��1�жϺ���
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-02 15:39
**	��    ��:			v1.0.0
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
** 	��������:			void	Beep(uchar AT)
**	��������:			��ʾ��
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-01 23:09
**	��    ��:			v1.0.0
********************************************************************************************************/
void	Beep(uchar AT)
{
	ulong i,j;
	uchar D=35,E=32,Y=3;
	switch(AT)
	{
	 	case BEEPPOWERON:	 			//����������ʾ��		��
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;};
			break;
		case BEEPSHUTDOWN:				//�����ػ���ʾ��		������
			j=0;while(j<1800){SPK=!SPK;for(i=0;i<D;i++){;}j++;}for(i=0;i<15;i++)DelayMS(Y);
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;}for(i=0;i<15;i++)DelayMS(Y);
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;}
			break;
		case BEEPERROR:					//������ʾ��		��������
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;}for(i=0;i<15;i++)DelayMS(Y);
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;}for(i=0;i<15;i++)DelayMS(Y);
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;}for(i=0;i<15;i++)DelayMS(Y);
			j=0;while(j<400){SPK=!SPK;for(i=0;i<D;i++){;}j++;}
			break;
		case BEEPCONFIRM:				//ȷ����ʾ��			��
			j=0;while(j<2800){SPK=!SPK;for(i=0;i<D;i++){;}j++;}
			break;
		case BEEPOPERATE:				//��������������		��
			j=0;while(j<400){SPK=!SPK;for(i=0;i<E;i++){;}j++;};
			break;	   	
	}
	SPK=0;
}

/********************************************************************************************************
** 	��������:			void	RGB1s()
**	��������:			RGB�任һ��ģʽ
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-02 16:29
**	��    ��:			v1.0.0
********************************************************************************************************/
void	RGB1s()
{
	RGBloop	=	0;
	if(RGBlastcolor<13)	RGBlastcolor++;else	 RGBlastcolor=0;
}

/********************************************************************************************************
** 	��������:			void	RGBFlash(uchar M)
**	��������:			RGB LED��˸
**	��ڲ���:			M:	RGB��˸ģʽ
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-02 16:29
**	��    ��:			v1.0.0
********************************************************************************************************/
void	RGBFlash(uchar M)
{
	if((RGBloop/RGBLOOPCOUNT)==RGBlaststate)	return;	  
	RGBlaststate=(RGBloop/RGBLOOPCOUNT);

	switch(M)
	{
		case	RGBMOFF:	  			//RGB��ɫģʽ	��
			RGBr	=	0;
			RGBg	=	0;
			RGBb	=	0;
			break;
		case	RGBMR: 	  				//RGB��ɫģʽ	�� 
			if(RGBlastcolor%2)			//��->��
			{
				if(RGBr<RGBbright)	RGBr++;
				if(RGBg>0)	RGBg--;
				if(RGBb>0)	RGBb--;
			}	   
			else						//��->��
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMRG:   				//RGB��ɫģʽ	����
			if(RGBlastcolor%2)			//��->��
			{
				if(RGBr<RGBbright)	RGBr++;
				if(RGBg<RGBbright)	RGBg++;
				if(RGBb>0)	RGBb--;	  
			}	   
			else						//��->��
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMG: 	  				//RGB��ɫģʽ	��	 
			if(RGBlastcolor%2)			//��->��
			{
				if(RGBr>0)	RGBr--;
				if(RGBg<RGBbright)	RGBg++;
				if(RGBb>0)	RGBb--;	
			}	   
			else						//��->��
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMGB: 				//RGB��ɫģʽ	����  
			if(RGBlastcolor%2)			//��->��
			{
				if(RGBr>0)	RGBr--;
				if(RGBg<RGBbright)	RGBg++;
				if(RGBb<RGBbright)	RGBb++;	 
			}	   
			else						//��->��
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMB: 	  				//RGB��ɫģʽ	�� 
			if(RGBlastcolor%2)			//��->��
			{
				if(RGBr>0)	RGBr--;
				if(RGBg>0)	RGBg--;
				if(RGBb<RGBbright)	RGBb++;	 
			}	   
			else						//��->��
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMRB: 	  			//RGB��ɫģʽ	����  
			if(RGBlastcolor%2)			//��->��
			{
				if(RGBr<RGBbright)	RGBr++;
				if(RGBg>0)	RGBg--;
				if(RGBb<RGBbright)	RGBb++;	 
			}	   
			else						//��->��
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMRGB: 	  			//RGB��ɫģʽ	��	   
			if(RGBlastcolor%2)			//��->��
			{
				if(RGBr<RGBbright)	RGBr++;
				if(RGBg<RGBbright)	RGBg++;
				if(RGBb<RGBbright)	RGBb++;
			}	   
			else						//��->��
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMFADE: 	  			//RGB��ɫģʽ	��������
			if(RGBlastcolor==0)			//��->��		��
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg<RGBbright)	RGBg++;	if(RGBb<RGBbright)	RGBb++;	}
			else if(RGBlastcolor==2)	//��->��		��
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;	}
			else if(RGBlastcolor==4)	//��->��		����
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg<RGBbright)	RGBg++;	if(RGBb>0)	RGBb--;	}
			else if(RGBlastcolor==6)	//��->��		��
				{if(RGBr>0)		RGBr--;	if(RGBg<RGBbright)	RGBg++;	if(RGBb>0)	RGBb--;	}
			else if(RGBlastcolor==8)	//��->��		����
				{if(RGBr>0)		RGBr--;	if(RGBg<RGBbright)	RGBg++;	if(RGBb<RGBbright)	RGBb++;	}
			else if(RGBlastcolor==10)	//��->��		��
				{if(RGBr>0)		RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb<RGBbright)	RGBb++;	}
			else if(RGBlastcolor==12)	//��->��		����
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg>0)	RGBg--;	if(RGBb<RGBbright)	RGBb++;	}
			else						//��->��
				{if(RGBr>0)	RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;		}
			break;
		case	RGBMFADEC: 	  			//RGB��ɫģʽ	��������		  
			if(RGBlastcolor%7==0)			//��->��		��
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg<RGBbright)	RGBg++;	if(RGBb<RGBbright)	RGBb++;	}
			else if(RGBlastcolor%7==1)	//��->��		��
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg>0)	RGBg--;	if(RGBb>0)	RGBb--;	}
			else if(RGBlastcolor%7==2)	//��->��		����
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg<RGBbright)	RGBg++;	if(RGBb>0)	RGBb--;	}
			else if(RGBlastcolor%7==3)	//��->��		��
				{if(RGBr>0)		RGBr--;	if(RGBg<RGBbright)	RGBg++;	if(RGBb>0)	RGBb--;	}
			else if(RGBlastcolor%7==4)	//��->��		����
				{if(RGBr>0)		RGBr--;	if(RGBg<RGBbright)	RGBg++;	if(RGBb<RGBbright)	RGBb++;	}
			else if(RGBlastcolor%7==5)	//��->��		��
				{if(RGBr>0)		RGBr--;	if(RGBg>0)	RGBg--;	if(RGBb<RGBbright)	RGBb++;	}
			else if(RGBlastcolor%7==6)	//��->��		����
				{if(RGBr<RGBbright)	RGBr++;	if(RGBg>0)	RGBg--;	if(RGBb<RGBbright)	RGBb++;	}
			break;
	}
	LEDSendData(RGBr,RGBg,RGBb);			
}

/********************************************************************************************************
** 	��������:			void	DOT1s()
**	��������:			DOT�任һ��ģʽ
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-03 17:33
**	��    ��:			v1.0.0
********************************************************************************************************/
void	DOT1s()
{
	DOTcut	=	0;
	DOTonoff=!DOTonoff;
}

/********************************************************************************************************
** 	��������:			void	NUM1s()
**	��������:			NUM�任һ��ģʽ
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-05 15:52
**	��    ��:			v1.0.0
********************************************************************************************************/
void	NUM1s()
{
	NUM500msloop=	0;
	NUMcut		=	0;
}

/********************************************************************************************************
** 	��������:			bit	CheckTime(bit DT)
**	��������:			����ʱ���ʽ���
**	���ڲ���:			0:������ڸ�ʽ	1:���ʱ���ʽ
**	���ڲ���:			0:����	1:��ʽ��ȷ
**	�� �� ��:			����Զ
**	����ʱ��:			2011-01-29 21:25
**	��    ��:			v1.0.0
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
** 	��������:			void	Num_Process(uchar N)
**	��������:			���ְ���������
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-04 16:11
**	��    ��:			v1.0.0
********************************************************************************************************/
void	Num_Process(uchar N)
{	 
	switch(NOWSTATE)
	{
		case	S_SETYEAR:						//��ǰ�ڵ�����					   
			Beep(BEEPOPERATE);					//BEEP
			S_YEAR	=	(S_YEAR*10+N)%100;
			break;
		case	S_SETMONTH:						//��ǰ�ڵ�����					   
			Beep(BEEPOPERATE);					//BEEP
			S_MONTH	=	(S_MONTH*10+N)%100;
			break;
		case	S_SETDAY:						//��ǰ�ڵ�����					   
			Beep(BEEPOPERATE);					//BEEP
			S_DAY	=	(S_DAY*10+N)%100;
			break;
		case	S_SETHOUR:						//��ǰ�ڵ���ʱ 				   
			Beep(BEEPOPERATE);					//BEEP
			S_HOUR	=	(S_HOUR*10+N)%100;
			break;
		case	S_SETMINUTE:					//��ǰ�ڵ����� 				   
			Beep(BEEPOPERATE);					//BEEP
			S_MINUTE=	(S_MINUTE*10+N)%100;
			break;
		case	S_SETSECOND:					//��ǰ�ڵ����� 					   
			Beep(BEEPOPERATE);					//BEEP
			S_SECOND=	(S_SECOND*10+N)%100;
			break;
		case	S_NORMAL:	 					//���������͹ػ�״̬�µ�����������ɫ
		case	S_POWEROFF:								
			if(N>0 && N<10)
			{						   
				Beep(BEEPOPERATE);				//BEEP
				RGBmode	=	N;
			}
			break;	   
		case	S_SETA1M:						//��ǰ�ڵ�������1���� 					   
		case	S_SETA2M:						//��ǰ�ڵ�������2���� 					   
			Beep(BEEPOPERATE);					//BEEP
			S_MINUTE=	(S_MINUTE*10+N)%100;
			break;
		case	S_SETA1H:						//��ǰ�ڵ�������1Сʱ 					   
		case	S_SETA2H:						//��ǰ�ڵ�������2Сʱ 					   
			Beep(BEEPOPERATE);					//BEEP
			S_HOUR=	(S_HOUR*10+N)%100;
			break;
	}
}

/********************************************************************************************************
** 	��������:			void	IR_Process(uchar IrData)
**	��������:			�����߽��봦����
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-04 16:06
**	��    ��:			v1.0.0
********************************************************************************************************/
void	IR_Process(uchar IrData)
{
	uchar	i;
	uint	j;

	if(F_Alarm)
	{	  					   
		Beep(BEEPCONFIRM);							//BEEP
		AlarmReset();								//���Ӹ�λ
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
				NOWSTATE	=	S_NORMAL;			//�л�Ϊ����״̬
				POWERON		=	0;					//����
			}
			else
			{
				NOWSTATE	=	S_POWEROFF;			//�л�Ϊ�ػ�״̬
				Beep(BEEPSHUTDOWN);					//BEEP
				for(i=0;i<6;i++){LASTN[i]=Num[i];Num[i]=' ';}					//����ر�
				DOTonoff	=	0;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬
				NUM1s(); 							//����NUMS״̬
			}
			break;	
		//********************************************************************** 
		case	0x46:	 							//LIGHT	 
			if(NOWSTATE==S_NORMAL || NOWSTATE==S_POWEROFF)		//�����������ʾʱ��״̬��ػ�״̬��
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
			if(NOWSTATE==S_NORMAL)					//�����������ʾʱ��״̬��
			{
				Beep(BEEPPOWERON);					//BEEP
				
				DOTonoff	=	1;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬

				S_YEAR	=	YEAR;
				S_MONTH	=	MONTH;
				S_DAY	=	DAY;
				S_HOUR	=	HOUR;
				S_MINUTE=	MINUTE;
				S_SECOND=	SECOND;
				
				NUMLOOPCOUNT=	5;					//����Ϊ������˸״̬
				RGBmodeold	=	RGBmode;			//����Ϊ��ʱ״̬�ı�����
				RGBmode		=	RGBMR;				//����Ϊ��ʱ״̬�ı����� 
				RGBlastcolor=	0;					//��->��
				RGB1s();
					
				NOWSTATE=	S_SETSECOND;			//��ǰ���ڵ�����״̬
			}		  
			else if(NOWSTATE==S_SHOWDATE)			//�����������ʾ����״̬��
			{
				Beep(BEEPPOWERON);					//BEEP
				
				DOTonoff	=	0;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬

				S_YEAR	=	YEAR;
				S_MONTH	=	MONTH;
				S_DAY	=	DAY;
				S_HOUR	=	HOUR;
				S_MINUTE=	MINUTE;
				S_SECOND=	SECOND;
				
				NUMLOOPCOUNT=	5;					//����Ϊ������˸״̬
				RGBmodeold	=	RGBmode;			//����Ϊ��ʱ״̬�ı�����
				RGBmode		=	RGBMG;				//����Ϊ��ʱ״̬�ı�����  
				RGBlastcolor=	0;					//��->�� 
				RGB1s();
					
				NOWSTATE=	S_SETDAY;				//��ǰ���ڵ�����״̬
			}		  
			else if(NOWSTATE==S_SHOWA1)				//�����������ʾ����1״̬��
			{
				Beep(BEEPPOWERON);					//BEEP

				S_SECOND=	A1ONOFF;
				S_HOUR	=	A1H;
				S_MINUTE=	A1M;
				
				NUMLOOPCOUNT=	5;					//����Ϊ������˸״̬
				RGBmodeold	=	RGBmode;			//����Ϊ��ʱ״̬�ı�����
				RGBmode		=	RGBMB;				//����Ϊ��ʱ״̬�ı�����  
				RGBlastcolor=	0;					//��->�� 
				RGB1s();
					
				NOWSTATE=	S_SETA1M;				//��ǰ���ڵ�������1����״̬
			}			  
			else if(NOWSTATE==S_SHOWA2)				//�����������ʾ����2״̬��
			{
				Beep(BEEPPOWERON);					//BEEP

				S_SECOND=	A2ONOFF;
				S_HOUR	=	A2H;
				S_MINUTE=	A2M;
				
				NUMLOOPCOUNT=	5;					//����Ϊ������˸״̬
				RGBmodeold	=	RGBmode;			//����Ϊ��ʱ״̬�ı�����
				RGBmode		=	RGBMB;				//����Ϊ��ʱ״̬�ı�����  
				RGBlastcolor=	0;					//��->�� 
				RGB1s();
					
				NOWSTATE=	S_SETA2M;				//��ǰ���ڵ�������2����״̬
			}	
			else if(NOWSTATE==S_SETYEAR || NOWSTATE==S_SETMONTH || NOWSTATE==S_SETDAY || NOWSTATE==S_SETHOUR || NOWSTATE==S_SETMINUTE || NOWSTATE==S_SETSECOND || NOWSTATE==S_SETA1H || NOWSTATE==S_SETA1M || NOWSTATE==S_SETA2H || NOWSTATE==S_SETA2M)	//����ڵ���ʱ������״̬��
			{
				Beep(BEEPERROR);					//���豣�汣��
				DelayMS(200);
				NUMLOOPCOUNT=	25;					//����Ϊ������˸״̬
				RGBmode		=	RGBmodeold;			//�ָ�������ģʽ		    
				RGB1s();
				NOWSTATE	=	S_NORMAL;			//��ǰ����������ʾʱ��״̬ ����������
			}	   
			break;
		//********************************************************************** 
		case	0x44:	 							//DATE
			if(NOWSTATE==S_NORMAL || NOWSTATE==S_SHOWMOONDATE || NOWSTATE==S_SHOWTEMP || NOWSTATE==S_SHOWA1 || NOWSTATE==S_SHOWA2)
			{						   
				Beep(BEEPOPERATE);					//BEEP
				
				Num[0]	=	'0'+(FormatDate?MONTH:YEAR)/10;			//��ֵ��ǰ����
				Num[1]	=	'0'+(FormatDate?MONTH:YEAR)%10;			//��ֵ��ǰ����
				Num[2]	=	'0'+(FormatDate?DAY:MONTH)/10;			//��ֵ��ǰ����
				Num[3]	=	'0'+(FormatDate?DAY:MONTH)%10;			//��ֵ��ǰ����
				Num[4]	=	'0'+(FormatDate?YEAR:DAY)/10;				//��ֵ��ǰ����
				Num[5]	=	'0'+(FormatDate?YEAR:DAY)%10;				//��ֵ��ǰ����
		
				DOTonoff	=	0;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬
				NUM1s();										 
				NOWSTATE	=	S_SHOWDATE;			//�л�Ϊ������ʾ״̬  
				ShowDateLoop=	0;					//��ʱ��ʼ
			}
			else if(NOWSTATE==S_SHOWDATE)
			{
			  	Conversion(0,(YEAR/10)*16+YEAR%10,(MONTH/10)*16+MONTH%10,(DAY/10)*16+DAY%10);	  	//����ũ������
					   
				Beep(BEEPOPERATE);					//BEEP
				
				Num[0]	=	'0'+(FormatDate?month_moon:year_moon)/16;		//��ֵ��ǰ����
				Num[1]	=	'0'+(FormatDate?month_moon:year_moon)%16;		//��ֵ��ǰ����
				Num[2]	=	'0'+(FormatDate?day_moon:month_moon)/16;		//��ֵ��ǰ����
				Num[3]	=	'0'+(FormatDate?day_moon:month_moon)%16;		//��ֵ��ǰ����
				Num[4]	=	'0'+(FormatDate?year_moon:day_moon)/16;		//��ֵ��ǰ����
				Num[5]	=	'0'+(FormatDate?year_moon:day_moon)%16;		//��ֵ��ǰ����
		
				DOTonoff	=	1;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬
				NUM1s();										 
				NOWSTATE	=	S_SHOWMOONDATE;		//�л�Ϊ������ʾ״̬  
				ShowDateLoop=	0;					//��ʱ��ʼ
			}
			break;
		//********************************************************************** 
		case	0x40:	 							//ALARM	  
			if(NOWSTATE==S_NORMAL || NOWSTATE==S_SHOWMOONDATE || NOWSTATE==S_SHOWDATE || NOWSTATE==S_SHOWTEMP || NOWSTATE==S_SHOWA2)
			{						   
				Beep(BEEPOPERATE);					//BEEP	
					 
				Num[0]	=	'a';					//��ֵ��ǰ������ʾ
				Num[1]	=	'1';					//��ֵ��ǰ������ʾ
				Num[2]	=	(A1ONOFF?'0'+A1H/10:' ');				//��ֵ��ǰ������ʾ
				Num[3]	=	(A1ONOFF?'0'+A1H%10:'0');				//��ֵ��ǰ������ʾ
				Num[4]	=	(A1ONOFF?'0'+A1M/10:'f');				//��ֵ��ǰ������ʾ
				Num[5]	=	(A1ONOFF?'0'+A1M%10:'f');				//��ֵ��ǰ������ʾ
		
				DOTonoff	=	1;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬
				NUM1s();										 
				NOWSTATE	=	S_SHOWA1;			//�л�Ϊ��ʾ����1״̬  
				ShowDateLoop=	0;					//��ʱ��ʼ	 
			} 
			else if(NOWSTATE==S_SHOWA1)
			{						   
				Beep(BEEPOPERATE);					//BEEP	
					 
				Num[0]	=	'a';					//��ֵ��ǰ������ʾ
				Num[1]	=	'2';					//��ֵ��ǰ������ʾ
				Num[2]	=	(A2ONOFF?'0'+A2H/10:' ');				//��ֵ��ǰ������ʾ
				Num[3]	=	(A2ONOFF?'0'+A2H%10:'0');				//��ֵ��ǰ������ʾ
				Num[4]	=	(A2ONOFF?'0'+A2M/10:'f');				//��ֵ��ǰ������ʾ
				Num[5]	=	(A2ONOFF?'0'+A2M%10:'f');				//��ֵ��ǰ������ʾ
		
				DOTonoff	=	1;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬
				NUM1s();										 
				NOWSTATE	=	S_SHOWA2;			//�л�Ϊ��ʾ����2״̬  
				ShowDateLoop=	0;					//��ʱ��ʼ

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
					if(j>60)	DelayMS(50);		//ˢ���¶���Ϣ
					else i=20;
				}			 

				if(FormatTemp)	j=32+j*1.8;			//�H

				Num[0]	=	' ';					//��ֵ��ǰ�¶�
				Num[1]	=	((j>99)?'0'+(j%1000)/100:' ');	//��ֵ��ǰ�¶�
				Num[2]	=	((j>9)?'0'+(j%100)/10:' ');		//��ֵ��ǰ�¶�
				Num[3]	=	j%10+'0';				//��ֵ��ǰ�¶�
				Num[4]	=	' ';					//��ֵ��ǰ�¶�
				Num[5]	=	(FormatTemp?'f':'c');	//��ֵ��ǰ�¶�
			
				DOTonoff	=	0;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬
				NUM1s();										 
				NOWSTATE	=	S_SHOWTEMP;			//�л�Ϊ�¶���ʾ״̬  
				ShowDateLoop=	0;					//��ʱ��ʼ
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
				if(NUMmode<2)NUMmode++;else NUMmode=0;//NUMmode�л�
				if(NUMmode==1)NUMLOOPCOUNT=15;else NUMLOOPCOUNT=25;	//������ģʽ����ɫ����STEP������
			}
			else if(NOWSTATE==S_SETA1H || NOWSTATE==S_SETA1M || NOWSTATE==S_SETA2H || NOWSTATE==S_SETA2M)
			{
				Beep(BEEPPOWERON);					//BEEP
				if(S_SECOND)S_SECOND=0;else S_SECOND=1;
			}
			else if(NOWSTATE==S_SHOWTEMP)			//��ʾ�¶�״̬
			{			
				Beep(BEEPPOWERON);					//BEEP
				if(FormatTemp)FormatTemp=0;else FormatTemp=1;
				FormatSave();
				
				for(i=1;i<20;i++)
				{
					j=GetTemp();
					if(j>60)	DelayMS(50);		//ˢ���¶���Ϣ
					else i=20;
				}			 

				if(FormatTemp)	j=32+j*1.8;			//�H

				Num[0]	=	' ';					//��ֵ��ǰ�¶�
				Num[1]	=	((j>99)?'0'+(j%1000)/100:' ');	//��ֵ��ǰ�¶�
				Num[2]	=	((j>9)?'0'+(j%100)/10:' ');		//��ֵ��ǰ�¶�
				Num[3]	=	j%10+'0';				//��ֵ��ǰ�¶�
				Num[4]	=	' ';					//��ֵ��ǰ�¶�
				Num[5]	=	(FormatTemp?'f':'c');	//��ֵ��ǰ�¶�
			
				DOTonoff	=	0;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬
				NUM1s();										 
				NOWSTATE	=	S_SHOWTEMP;			//�л�Ϊ�¶���ʾ״̬  
				ShowDateLoop=	0;					//��ʱ��ʼ
			}
			else if(NOWSTATE==S_SHOWDATE)			//��ʾ��������״̬
		   	{		
				Beep(BEEPOPERATE);					//BEEP 
				if(FormatDate)FormatDate=0;else FormatDate=1;
				FormatSave();
				
				Num[0]	=	'0'+(FormatDate?MONTH:YEAR)/10;			//��ֵ��ǰ����
				Num[1]	=	'0'+(FormatDate?MONTH:YEAR)%10;			//��ֵ��ǰ����
				Num[2]	=	'0'+(FormatDate?DAY:MONTH)/10;			//��ֵ��ǰ����
				Num[3]	=	'0'+(FormatDate?DAY:MONTH)%10;			//��ֵ��ǰ����
				Num[4]	=	'0'+(FormatDate?YEAR:DAY)/10;				//��ֵ��ǰ����
				Num[5]	=	'0'+(FormatDate?YEAR:DAY)%10;				//��ֵ��ǰ����
		
				DOTonoff	=	0;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬
				NUM1s();										 
				NOWSTATE	=	S_SHOWDATE;			//�л�Ϊ������ʾ״̬  
				ShowDateLoop=	0;					//��ʱ��ʼ
			}		 
			else if(NOWSTATE==S_SHOWMOONDATE)
			{
			  	Conversion(0,(YEAR/10)*16+YEAR%10,(MONTH/10)*16+MONTH%10,(DAY/10)*16+DAY%10);	  	//����ũ������	  
				 
				if(FormatDate)FormatDate=0;else FormatDate=1;
				FormatSave();
					   
				Beep(BEEPOPERATE);					//BEEP
				
				Num[0]	=	'0'+(FormatDate?month_moon:year_moon)/16;		//��ֵ��ǰ����
				Num[1]	=	'0'+(FormatDate?month_moon:year_moon)%16;		//��ֵ��ǰ����
				Num[2]	=	'0'+(FormatDate?day_moon:month_moon)/16;		//��ֵ��ǰ����
				Num[3]	=	'0'+(FormatDate?day_moon:month_moon)%16;		//��ֵ��ǰ����
				Num[4]	=	'0'+(FormatDate?year_moon:day_moon)/16;		//��ֵ��ǰ����
				Num[5]	=	'0'+(FormatDate?year_moon:day_moon)%16;		//��ֵ��ǰ����
		
				DOTonoff	=	1;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬
				NUM1s();										 
				NOWSTATE	=	S_SHOWMOONDATE;		//�л�Ϊ������ʾ״̬  
				ShowDateLoop=	0;					//��ʱ��ʼ
			}
			else if(NOWSTATE==S_SHOWA1)				//��ʾ����1״̬��
			{																  
				Beep(BEEPOPERATE);					//BEEP

				if(A1ONOFF)A1ONOFF=0;else A1ONOFF=1;
				AlarmSave(1,A1H,A1M,A1ONOFF);	  	
					 
				Num[0]	=	'a';					//��ֵ��ǰ������ʾ
				Num[1]	=	'1';					//��ֵ��ǰ������ʾ
				Num[2]	=	(A1ONOFF?'0'+A1H/10:' ');				//��ֵ��ǰ������ʾ
				Num[3]	=	(A1ONOFF?'0'+A1H%10:'0');				//��ֵ��ǰ������ʾ
				Num[4]	=	(A1ONOFF?'0'+A1M/10:'f');				//��ֵ��ǰ������ʾ
				Num[5]	=	(A1ONOFF?'0'+A1M%10:'f');				//��ֵ��ǰ������ʾ
		
				DOTonoff	=	1;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬
				NUM1s();										 
				NOWSTATE	=	S_SHOWA1;			//�л�Ϊ��ʾ����1״̬  
				ShowDateLoop=	0;					//��ʱ��ʼ	 
			}
			else if(NOWSTATE==S_SHOWA2)				//��ʾ����2״̬��
			{																  
				Beep(BEEPOPERATE);					//BEEP

				if(A2ONOFF)A2ONOFF=0;else A2ONOFF=1;
				AlarmSave(2,A2H,A2M,A2ONOFF);	  	
					 
				Num[0]	=	'a';					//��ֵ��ǰ������ʾ
				Num[1]	=	'2';					//��ֵ��ǰ������ʾ
				Num[2]	=	(A2ONOFF?'0'+A2H/10:' ');				//��ֵ��ǰ������ʾ
				Num[3]	=	(A2ONOFF?'0'+A2H%10:'0');				//��ֵ��ǰ������ʾ
				Num[4]	=	(A2ONOFF?'0'+A2M/10:'f');				//��ֵ��ǰ������ʾ
				Num[5]	=	(A2ONOFF?'0'+A2M%10:'f');				//��ֵ��ǰ������ʾ
		
				DOTonoff	=	1;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬
				NUM1s();										 
				NOWSTATE	=	S_SHOWA2;			//�л�Ϊ��ʾ����2״̬  
				ShowDateLoop=	0;					//��ʱ��ʼ	 
			}
			break;													  
		//********************************************************************** 
		case	0x16:								//OK��	 
			if(NOWSTATE==S_NORMAL)					//�����������ʾʱ��״̬��
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
			
				DOTonoff	=	0;					//DOTS�� 
				DOT1s(); 							//����DOTS״̬
				NUM1s();										 
				NOWSTATE	=	S_SHOWTEMP;			//�л�Ϊ�¶���ʾ״̬  
				ShowDateLoop=	0;					//��ʱ��ʼ			
			}	 	
			if(NOWSTATE==S_SETHOUR || NOWSTATE==S_SETMINUTE || NOWSTATE==S_SETSECOND)	//����ڵ���ʱ��״̬��
			{
				GetDateTime();						//ˢ�µ�ǰʱ��
															  				
				S_YEAR	=	YEAR;
				S_MONTH	=	MONTH;
				S_DAY	=	DAY;
									 
				if(CheckTime(1))					//���ʱ���ʽ�Ƿ���ȷ
				{
					ModifyTime(S_YEAR,S_MONTH,S_DAY,S_HOUR,S_MINUTE,S_SECOND);
					Beep(BEEPCONFIRM);				//BEEP
				}
				else
				{
					Beep(BEEPERROR);				//BEEP
				}	
												
				DelayMS(200);
				NUMLOOPCOUNT=	25;					//����Ϊ������˸״̬
				RGBmode		=	RGBmodeold;			//�ָ�������ģʽ
				NOWSTATE	=	S_NORMAL;			//��ǰ����������ʾʱ��״̬
			}  			 	 	
			if(NOWSTATE==S_SETYEAR || NOWSTATE==S_SETMONTH || NOWSTATE==S_SETDAY)	//����ڵ�������״̬��
			{
				GetDateTime();						//ˢ�µ�ǰʱ��
															  				
				S_HOUR		=	HOUR;
				S_MINUTE	=	MINUTE;
				S_SECOND	=	SECOND;
									 
				if(CheckTime(0))					//������ڸ�ʽ�Ƿ���ȷ
				{
					ModifyTime(S_YEAR,S_MONTH,S_DAY,S_HOUR,S_MINUTE,S_SECOND);
					Beep(BEEPCONFIRM);				//BEEP
				}
				else
				{
					Beep(BEEPERROR);				//BEEP
				}	
												
				DelayMS(200);
				NUMLOOPCOUNT=	25;					//����Ϊ������˸״̬
				RGBmode		=	RGBmodeold;			//�ָ�������ģʽ		    
				RGB1s();
				NOWSTATE	=	S_NORMAL;			//��ǰ����������ʾʱ��״̬
			}			 	 	
			if(NOWSTATE==S_SETA1H || NOWSTATE==S_SETA1M || NOWSTATE==S_SETA2H || NOWSTATE==S_SETA2M)	//����ڵ�������1����2״̬��
			{												  				
				if(CheckTime(1))					//������Ӹ�ʽ��ȷ
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
				NUMLOOPCOUNT=	25;					//����Ϊ������˸״̬
				RGBmode		=	RGBmodeold;			//�ָ�������ģʽ		    
				RGB1s();
				NOWSTATE	=	S_NORMAL;			//��ǰ����������ʾʱ��״̬
			}
			break;
		//********************************************************************** 
		case	0x19:	 							//CANCEL��	 			
			if(NOWSTATE==S_SETYEAR || NOWSTATE==S_SETMONTH || NOWSTATE==S_SETDAY || NOWSTATE==S_SETHOUR || NOWSTATE==S_SETMINUTE || NOWSTATE==S_SETSECOND || NOWSTATE==S_SETA1H || NOWSTATE==S_SETA1M || NOWSTATE==S_SETA2H || NOWSTATE==S_SETA2M)	//����ڵ���ʱ������״̬��
			{
				Beep(BEEPERROR);					//���豣�汣��
				DelayMS(200);
				NUMLOOPCOUNT=	25;					//����Ϊ������˸״̬
				RGBmode		=	RGBmodeold;			//�ָ�������ģʽ		    
				RGB1s();
				NOWSTATE	=	S_NORMAL;			//��ǰ����������ʾʱ��״̬ ����������
			}
			break;
		//********************************************************************** 
		case	0x0D:	Num_Process(0); 	break;				//0��	 	
		case	0x0C:	Num_Process(1); 	break;				//1��	 	
		case	0x18:	Num_Process(2); 	break;				//2��	 	
		case	0x5E:	Num_Process(3); 	break;				//3��	 	
		case	0x08:	Num_Process(4); 	break;				//4��	 	
		case	0x1C:	Num_Process(5); 	break;				//5��	 	
		case	0x5A:	Num_Process(6); 	break;				//6��	 	
		case	0x42:	Num_Process(7); 	break;				//7��	   
		case	0x52:	Num_Process(8); 	break;				//8��	   
		case	0x4A:	Num_Process(9); 	break;				//9��	 
	}
}

/********************************************************************************************************
** 	��������:			void	IR_RX()
**	��������:			�������жϽ��մ���
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-04 16:07
**	��    ��:			v1.0.0
********************************************************************************************************/
void IR_RX()
{
	uchar	SampleTime;

	IR_SampleCnt++;							//Sample + 1

	F0 = P_IR_RX_temp;						//Save Last sample status
	P_IR_RX_temp = IR;						//Read current status
	if(F0 && !P_IR_RX_temp)					//Last sample is high��and current sample is low, so is fall edge
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
					if(~IR_DataShit == IR_data)		//�ж�����������
					{  	
						if((IR_UserH == (D_IR_usercode / 256)) &&
							IR_UserL == (D_IR_usercode % 256))		//ϵͳ����ȷ
						{
							IR_code      = IR_data;
							B_IR_Press   = 1;		//������Ч
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
** 	��������:			void 	Send595(uchar count,uint dat)
**	��������:			��74HC595��������  
**	��ڲ���:			count:	�������ݵ�λ��
**						dat:	���͵�����(MAX 16Bit)
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-04 22:32
**	��    ��:			v1.0.0
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
** 	��������:			bit 	DispNum(uchar * N,uchar Mode)
**	��������:			ˢ��YS18-3ӫ��ܵ���ʾ����
**	��ڲ���:			*N:		��ʾ���ַ�����
**						Mode:	��ʾģʽ	0:crossfade 1:fade in/out 2:standard
**	���ڲ���:			bit:	0:������ʾδ�л���� 1:������ʾ�ѹ������
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-04 22:38
**	��    ��:			v1.0.0
**  -----------------------------------------------------------------------------------------------------
**	�� �� ��:			����Զ
**	�޸�ʱ��:			2011-02-05 15:02
**	��    ��:			�����������ΪASCII���������� ��չӢ����ĸӦ��
**  -----------------------------------------------------------------------------------------------------
**	�� �� ��:			����Զ
**	�޸�ʱ��:			2011-02-05 15:59
**	��    ��:			���ص�ǰ�����л�״̬
********************************************************************************************************/
bit 	DispNum(uchar * N,uchar Mode)
{
	bit		r=1;
	uchar	i;
	uchar	Nnow[6],Nlast[6];
	
	for(i=0;i<6;i++)
	{
		if(Mode==2)			LASTN[i]=*(N+i);	//����Ǳ�׼ģʽ������ˢ�� No Crossfade
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
	STCP = 0;//_nop_();		//��������

	return	r;
}

/********************************************************************************************************
** 	��������:			void	GetDateTime()
**	��������:			��ȡ��ǰ���ں�ʱ��
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-05 15:45
**	��    ��:			v1.0.0
********************************************************************************************************/
void	GetDateTime()
{
	uchar	Tmp;
	Tmp=read_random(DS3231_YEAR);	YEAR	=	BCD2HEX(Tmp);	//��
    Tmp=read_random(DS3231_MONTH);	MONTH	=	BCD2HEX(Tmp);	//��
    Tmp=read_random(DS3231_DAY);	DAY		=	BCD2HEX(Tmp);	//��
    Tmp=read_random(DS3231_HOUR);	Tmp&=0x3f;	HOUR	=	BCD2HEX(Tmp);	//ʱ 	
    Tmp=read_random(DS3231_MINUTE);	MINUTE	=	BCD2HEX(Tmp);	//��
    Tmp=read_random(DS3231_SECOND);	SECOND	=	BCD2HEX(Tmp);	//��
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
** 	��������:			void	main()
**	��������:			������
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-01 14:11
**	��    ��:			v1.0.0
********************************************************************************************************/
void	main()
{
	bit		r,Flash;					//Temp
	uchar	i=0;						//Temp

	POWERON	=	1;						//�رյ�Դ����
	SPK		=	0;						//�رշ�����
	
	Port_IO_Init();						//IO��Դ����
	TimerInit();						//��ʱ��1��ʼ��
	
	LEDSendData(0,0,0);	   				//��ʼ���ص�RGB	

	ET0		=	1;						//����ʱ��0�ж�
	ET1		=	1;						//����ʱ��1�ж�
	EA		=	1;						//���������ж�

	DelayMS(500);						//---------------------
	Beep(BEEPPOWERON);					//---------------------
	AlarmInit();						//���ӳ�ʼ��
	FormatInit();						//��ʽ��ʼ��
	
	POWERON	=	0;						//�򿪵�Դ����	  
	
	while(1)
    {	
		//***********************************************************************************************
		//��ѭ������ִ��
		r	=	DispNum(Num,NUMmode);			//��̬ʵʱˢ����ʾ����
		RGBFlash(RGBmode);						//RGB����
		if(ALARM==0 && F_Alarm==0)	F_Alarm	=1;	//����ʱ�䵽
		if(B_IR_Press)	 						//������ң�����ݽ���
		{
			IR_Process(IR_code);
			B_IR_Press	=	0;
		}
		//***********************************************************************************************
		//50ms��־
		if(F_50ms)
		{
			if(r)	GetDateTime();
			F_50ms	=	0;
		}	
		//***********************************************************************************************
		//1s��־
		if(F_1s)
		{
			if(NOWSTATE==S_POWEROFF)			//����ǹػ�״̬�����ִ��1s���ж�
			{
				RGB1s();
			}
			F_1s	=	0;
		}
		//***********************************************************************************************
		//2s��־
		if(F_2s)
		{
			if(r && F_Alarm)
			{
				Beep(BEEPERROR);				//BEEP
				F_Alarm++;
				if(F_Alarm>15)
				{
					AlarmReset();				//30s�����Ӹ�λ
					F_Alarm	=	0;
				}
				F_2s	=	0;
			}
		}
		//***********************************************************************************************
		//���ݵ�ǰ״ִ̬��
		switch(NOWSTATE)
		{
			case	S_NORMAL:					//������ʾʱ��״̬
				if(OLDSECOND!=SECOND)
				{
					if(FormatTime)
					{	
						if(HOUR>12)i=HOUR-12;else i=HOUR;
					}
					else	i=HOUR;

					Num[0]	=	'0'+i/10;		//��ֵ��ǰʱ��
					Num[1]	=	'0'+i%10;		//��ֵ��ǰʱ��
					Num[2]	=	'0'+MINUTE/10;	//��ֵ��ǰʱ��
					Num[3]	=	'0'+MINUTE%10;	//��ֵ��ǰʱ��
					Num[4]	=	'0'+SECOND/10;	//��ֵ��ǰʱ��
					Num[5]	=	'0'+SECOND%10;	//��ֵ��ǰʱ��
		
					DOT1s();
					RGB1s();
					NUM1s();
		
					OLDSECOND=SECOND;
				}
				break;
			case	S_POWEROFF:					//�ػ�״̬
				if(POWERON==0 && LASTN[0]==' ')POWERON=1;	//������ɺ�ػ�
				break;
			case	S_SHOWDATE:					//��ʾ����״̬
			case	S_SHOWMOONDATE:				//��ʾũ������״̬
			case	S_SHOWA1:					//��ʾ����1
			case	S_SHOWA2:					//��ʾ����2
			case	S_SHOWTEMP:					//��ʾ�¶�״̬	 
				if(OLDSECOND!=SECOND)
				{
					RGB1s();
							
					OLDSECOND=SECOND;
				}
				if(F_100ms)
				{
					if(ShowDateLoop<255)ShowDateLoop++;
					if(ShowDateLoop>=(SHOWDATELOOPCOUNT*10)) 	//����������ʾ
					{
						NOWSTATE	=	S_NORMAL;
					}
					F_100ms=0;
				}
				break;
			case	S_SETHOUR:					//����Сʱ״̬
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	(Flash?' ':'0'+S_HOUR/10);	//��ֵ��ǰʱ��
					Num[1]	=	(Flash?' ':'0'+S_HOUR%10);	//��ֵ��ǰʱ��
					Num[2]	=	'0'+S_MINUTE/10;//��ֵ��ǰʱ��
					Num[3]	=	'0'+S_MINUTE%10;//��ֵ��ǰʱ��
					Num[4]	=	'0'+S_SECOND/10;//��ֵ��ǰʱ��
					Num[5]	=	'0'+S_SECOND%10;//��ֵ��ǰʱ��
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETMINUTE:				//��������״̬
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	'0'+S_HOUR/10;	//��ֵ��ǰʱ��
					Num[1]	=	'0'+S_HOUR%10;	//��ֵ��ǰʱ��
					Num[2]	=	(Flash?' ':'0'+S_MINUTE/10);	//��ֵ��ǰʱ��
					Num[3]	=	(Flash?' ':'0'+S_MINUTE%10);	//��ֵ��ǰʱ��
					Num[4]	=	'0'+S_SECOND/10;//��ֵ��ǰʱ��
					Num[5]	=	'0'+S_SECOND%10;//��ֵ��ǰʱ��
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETSECOND:				//������״̬
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	'0'+S_HOUR/10;	//��ֵ��ǰʱ��
					Num[1]	=	'0'+S_HOUR%10;	//��ֵ��ǰʱ��
					Num[2]	=	'0'+S_MINUTE/10;//��ֵ��ǰʱ��
					Num[3]	=	'0'+S_MINUTE%10;//��ֵ��ǰʱ��
					Num[4]	=	(Flash?' ':'0'+S_SECOND/10);	//��ֵ��ǰʱ��
					Num[5]	=	(Flash?' ':'0'+S_SECOND%10);	//��ֵ��ǰʱ��
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETYEAR:					//������״̬
				if(F_250ms)
				{
					Flash	=	!Flash;
					if(FormatDate)				//MM-DD-YY
					{										   
						Num[0]	=	'0'+S_MONTH/10;//��ֵ��ǰ����
						Num[1]	=	'0'+S_MONTH%10;//��ֵ��ǰ����
						Num[2]	=	'0'+S_DAY/10;//��ֵ��ǰ����
						Num[3]	=	'0'+S_DAY%10;//��ֵ��ǰ����
						Num[4]	=	(Flash?' ':'0'+S_YEAR/10);	//��ֵ��ǰ����
						Num[5]	=	(Flash?' ':'0'+S_YEAR%10);	//��ֵ��ǰ����
					}
					else
					{											  
						Num[0]	=	(Flash?' ':'0'+S_YEAR/10);	//��ֵ��ǰ����
						Num[1]	=	(Flash?' ':'0'+S_YEAR%10);	//��ֵ��ǰ����
						Num[2]	=	'0'+S_MONTH/10;//��ֵ��ǰ����
						Num[3]	=	'0'+S_MONTH%10;//��ֵ��ǰ����
						Num[4]	=	'0'+S_DAY/10;//��ֵ��ǰ����
						Num[5]	=	'0'+S_DAY%10;//��ֵ��ǰ����
					}
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETMONTH:					//������״̬
				if(F_250ms)
				{
					Flash	=	!Flash;	 										   
					if(FormatDate)				//MM-DD-YY
					{										   
						Num[0]	=	(Flash?' ':'0'+S_MONTH/10);	//��ֵ��ǰ����
						Num[1]	=	(Flash?' ':'0'+S_MONTH%10);	//��ֵ��ǰ����
						Num[2]	=	'0'+S_DAY/10;//��ֵ��ǰ����
						Num[3]	=	'0'+S_DAY%10;//��ֵ��ǰ����
						Num[4]	=	'0'+S_YEAR/10;	//��ֵ��ǰ����
						Num[5]	=	'0'+S_YEAR%10;	//��ֵ��ǰ����
					}
					else
					{													
						Num[0]	=	'0'+S_YEAR/10;	//��ֵ��ǰ����
						Num[1]	=	'0'+S_YEAR%10;	//��ֵ��ǰ����
						Num[2]	=	(Flash?' ':'0'+S_MONTH/10);	//��ֵ��ǰ����
						Num[3]	=	(Flash?' ':'0'+S_MONTH%10);	//��ֵ��ǰ����
						Num[4]	=	'0'+S_DAY/10;//��ֵ��ǰ����
						Num[5]	=	'0'+S_DAY%10;//��ֵ��ǰ����
					}
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETDAY:					//������״̬
				if(F_250ms)
				{
					Flash	=	!Flash;									   
					if(FormatDate)				//MM-DD-YY
					{														  
						Num[0]	=	'0'+S_MONTH/10;//��ֵ��ǰ����
						Num[1]	=	'0'+S_MONTH%10;//��ֵ��ǰ����
						Num[2]	=	(Flash?' ':'0'+S_DAY/10);	//��ֵ��ǰ����
						Num[3]	=	(Flash?' ':'0'+S_DAY%10);	//��ֵ��ǰ����
						Num[4]	=	'0'+S_YEAR/10;	//��ֵ��ǰ����
						Num[5]	=	'0'+S_YEAR%10;	//��ֵ��ǰ����
					}
					else
					{											   
						Num[0]	=	'0'+S_YEAR/10;	//��ֵ��ǰ����
						Num[1]	=	'0'+S_YEAR%10;	//��ֵ��ǰ����
						Num[2]	=	'0'+S_MONTH/10;//��ֵ��ǰ����
						Num[3]	=	'0'+S_MONTH%10;//��ֵ��ǰ����
						Num[4]	=	(Flash?' ':'0'+S_DAY/10);	//��ֵ��ǰ����
						Num[5]	=	(Flash?' ':'0'+S_DAY%10);	//��ֵ��ǰ����
					}
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETA1H:					//��������1Сʱ״̬
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	'a';					//��ֵ��ǰ������ʾ
					Num[1]	=	'1';					//��ֵ��ǰ������ʾ
					Num[2]	=	(Flash?' ':(S_SECOND?'0'+S_HOUR/10:' '));	//��ֵ��ǰ������ʾ
					Num[3]	=	(Flash?' ':(S_SECOND?'0'+S_HOUR%10:'0'));	//��ֵ��ǰ������ʾ
					Num[4]	=	(S_SECOND?'0'+S_MINUTE/10:(Flash?' ':'f'));	//��ֵ��ǰ������ʾ
					Num[5]	=	(S_SECOND?'0'+S_MINUTE%10:(Flash?' ':'f'));	//��ֵ��ǰ������ʾ
		
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETA1M:					//��������1����״̬
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	'a';					//��ֵ��ǰ������ʾ
					Num[1]	=	'1';					//��ֵ��ǰ������ʾ
					Num[2]	=	(S_SECOND?'0'+S_HOUR/10:' ');				//��ֵ��ǰ������ʾ
					Num[3]	=	(S_SECOND?'0'+S_HOUR%10:(Flash?' ':'0'));	//��ֵ��ǰ������ʾ
					Num[4]	=	(Flash?' ':(S_SECOND?'0'+S_MINUTE/10:'f'));	//��ֵ��ǰ������ʾ
					Num[5]	=	(Flash?' ':(S_SECOND?'0'+S_MINUTE%10:'f'));	//��ֵ��ǰ������ʾ
		
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETA2H:					//��������2Сʱ״̬
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	'a';					//��ֵ��ǰ������ʾ
					Num[1]	=	'2';					//��ֵ��ǰ������ʾ
					Num[2]	=	(Flash?' ':(S_SECOND?'0'+S_HOUR/10:' '));	//��ֵ��ǰ������ʾ
					Num[3]	=	(Flash?' ':(S_SECOND?'0'+S_HOUR%10:'0'));	//��ֵ��ǰ������ʾ
					Num[4]	=	(S_SECOND?'0'+S_MINUTE/10:(Flash?' ':'f'));	//��ֵ��ǰ������ʾ
					Num[5]	=	(S_SECOND?'0'+S_MINUTE%10:(Flash?' ':'f'));	//��ֵ��ǰ������ʾ
		
					NUM1s();

					F_250ms	=	0;
				}
				break;
			case	S_SETA2M:					//��������2����״̬
				if(F_250ms)
				{
					Flash	=	!Flash;

					Num[0]	=	'a';					//��ֵ��ǰ������ʾ
					Num[1]	=	'2';					//��ֵ��ǰ������ʾ
					Num[2]	=	(S_SECOND?'0'+S_HOUR/10:' ');				//��ֵ��ǰ������ʾ
					Num[3]	=	(S_SECOND?'0'+S_HOUR%10:(Flash?' ':'0'));	//��ֵ��ǰ������ʾ
					Num[4]	=	(Flash?' ':(S_SECOND?'0'+S_MINUTE/10:'f'));	//��ֵ��ǰ������ʾ
					Num[5]	=	(Flash?' ':(S_SECOND?'0'+S_MINUTE%10:'f'));	//��ֵ��ǰ������ʾ
		
					NUM1s();

					F_250ms	=	0;
				}
				break;
		}
	}					
}