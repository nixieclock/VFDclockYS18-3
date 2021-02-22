/****************************************Copyright (c)**************************************************
**				
**	��Ŀ����:			YS18-3 Clock
**	�ļ�����:			DS3231.c
**	ģ�鹦��:			ʵ��LPD6803��ȫ���ܿ���
**
**------------------------------------------------------------------------------------------------------

** 	�� �� ��:			����Զ
**	E-mail  :			yanzeyuan@163.com
**	QQ		:			6626209
**	Mobile	:			18602007878
**	����ʱ��:			2011-02-02
**	��    ��:			v1.0.0
**	��    ��:			��������
**
********************************************************************************************************/

#include <STC12C5620AD.H>				//����STC12C5620AD.Hͷ�ļ�
#include <Define.H>						//����Define.Hͷ�ļ�
#include <DS3231Variable.H>				//����DS3231Variable.Hͷ�ļ�

/********************************************************************************************************
** 	ȫ�ֳ�������
********************************************************************************************************/
bit	ack;							//Ӧ���־λ

/********************************************************************************************************
** 	��������:			BCD2HEX(uchar val)
**	��������:			BCDתHEX
**	��ڲ���:			val:BCD��
**	�� �� ��:			����Զ
**	����ʱ��:			2011-01-25 20:33
**	��    ��:			v1.0.0
********************************************************************************************************/
uchar	BCD2HEX(uchar val)
{
	return	((val>>4)*10)+(val&0x0f);
}

/********************************************************************************************************
** 	��������:			HEX2BCD(uchar val)
**	��������:			HEXתBCD
**	��ڲ���:			val:HAX��
**	�� �� ��:			����Զ
**	����ʱ��:			2011-01-25 20:34
**	��    ��:			v1.0.0
********************************************************************************************************/
uchar	HEX2BCD(uchar val)
{
	return	(((val%100)/10)<<4)|(val%10);
}

/********************************************************************************************************
** 	��������:			void	Delay5US() 
**	��������:			��ʱ5us
**	�� �� ��:			����Զ
**	����ʱ��:			2011-02-02 10:43
**	��    ��:			v1.0.0
********************************************************************************************************/
void	Delay5US() 
{
	uchar	i;
	for(i=0;i<10;i++)
    {_nop_();_nop_();}
}

void Start_I2C()
{
    SDA=1;                  //������ʼ�����������ź�
    SCL=1;
    Delay5US();             //��ʼ��������ʱ�����4.7us,��ʱ     
    SDA=0;                  //������ʼ�ź�
    Delay5US();             // ��ʼ��������ʱ�����4��s		        
    SCL=0;                  //ǯסI2C���ߣ�׼�����ͻ��������
}												   

void Stop_I2C()
{
    SDA=0;                  //���ͽ��������������ź�
    SCL=1;                  //������������ʱ�����4us
    Delay5US();											     
    SDA=1;                  //����I2C���߽����ź�
}												   

void SendByte(uchar c)
{
    uchar BitCnt;
   
    for(BitCnt=0;BitCnt<8;BitCnt++)         //Ҫ���͵����ݳ���Ϊ8λ
    {
        if((c<<BitCnt)&0x80)
            SDA=1;                          //�жϷ���λ
        else 
            SDA=0;     
        SCL=1;                            //��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ
        Delay5US();                       //��֤ʱ�Ӹߵ�ƽ���ڴ���4��s   
        SCL=0;
    }
    SDA=1;                                  //8λ��������ͷ������ߣ�׼������Ӧ��λ
    SCL=1;
    Delay5US();
    if(SDA==1)
        ack=0;    
    else
        ack=1;                              //�ж��Ƿ���յ�Ӧ���ź�
    SCL=0;
    Delay5US();
}	   

uchar RcvByte()
{
   uchar retc;
   uchar BitCnt;
 
   retc=0;
   SDA=1;                           //��������Ϊ���뷽ʽ
   for(BitCnt=0;BitCnt<8;BitCnt++)
   {
        SCL=0;                      //��ʱ����Ϊ�ͣ�׼����������λ       
        Delay5US();                 //ʱ�ӵ͵�ƽ���ڴ���4.7��s		       
        SCL=1;                      //��ʱ����Ϊ��ʹ��������������Ч
        Delay5US();
        retc=retc<<1;
        if(SDA==1)
            retc=retc+1;            //������λ,���յ�����λ����retc��
        Delay5US();
   }
   SCL=0;
   return(retc);
}			    

void Ack_I2C(bit a)
{
	SDA	=	a;  
    SCL=1;		      
    Delay5US();             //ʱ�ӵ͵�ƽ���ڴ���4��s    
    SCL=0;                  //��ʱ���ߣ�ǯסI2C�����Ա��������
    Delay5US();   
}					    

uchar write_byte(uchar addr, uchar write_data)
{
    Start_I2C();
    SendByte(DS3231_WriteAddress);
    if (ack == 0)
        return 0;
   
    SendByte(addr);   
    if (ack == 0)
        return 0;
   
    SendByte(write_data);
    if (ack == 0)
        return 0;
   
    Stop_I2C();
    Delay5US();      
    Delay5US();      
    return 1;
}					  

uchar read_current()
{
    uchar read_data;
    Start_I2C();
    SendByte(DS3231_ReadAddress);
    if(ack==0)
        return(0);	      
    read_data = RcvByte();
    Ack_I2C(1);
    Stop_I2C();
    return read_data;
}						 

uchar read_random(uchar random_addr)
{
    Start_I2C();
    SendByte(DS3231_WriteAddress);
    if(ack==0)
        return(0);	    
    SendByte(random_addr);
    if(ack==0)
        return(0);			    
    return(read_current());
}				  

void ModifyTime(uchar yea,uchar mon,uchar da,uchar hou,uchar min,uchar sec)
{
    uchar temp=0;
   
    temp=HEX2BCD(yea);
    write_byte(DS3231_YEAR,temp);   //�޸���
   
    temp=HEX2BCD(mon);
    write_byte(DS3231_MONTH,temp);  //�޸���
   
    temp=HEX2BCD(da);
    write_byte(DS3231_DAY,temp);    //�޸���
   
    temp=HEX2BCD(hou);
    write_byte(DS3231_HOUR,temp);   //�޸�ʱ
   
    temp=HEX2BCD(min);
    write_byte(DS3231_MINUTE,temp); //�޸ķ�
   
    temp=HEX2BCD(sec);
    write_byte(DS3231_SECOND,temp); //�޸���
}