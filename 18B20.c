/**************************************
;WWW.STCMCU.COM
;Mobile:13922805190
;0755-82948412
�¶ȴ�����DS18B20���Գ���
��оƬ  : STC12C5A60S2 (1T)
����Ƶ��: 12.000MHz
**************************************/

#include <STC12C5620AD.H>				//����STC12C5620AD.Hͷ�ļ�
#include "INTRINS.H"

typedef unsigned char BYTE;		

sbit DQ = P1^6;                     //DS18B20�����ݿ�λP1.6
BYTE TPH;                           //����¶�ֵ�ĸ��ֽ�
BYTE TPL;                           //����¶�ֵ�ĵ��ֽ�

void DelayXus(BYTE n);
void DS18B20_Reset();
void DS18B20_WriteByte(BYTE dat);
BYTE DS18B20_ReadByte();

unsigned int	GetTemp()			//��ȡ�¶�ֵ 0:C 1:F
{
    DS18B20_Reset();                //�豸��λ
    DS18B20_WriteByte(0xCC);        //����ROM����
    DS18B20_WriteByte(0x44);        //��ʼת������
    while (!DQ);                    //�ȴ�ת�����

    DS18B20_Reset();                //�豸��λ
    DS18B20_WriteByte(0xCC);        //����ROM����
    DS18B20_WriteByte(0xBE);        //���ݴ�洢������
    TPL = DS18B20_ReadByte();       //���¶ȵ��ֽ�
    TPH = DS18B20_ReadByte();       //���¶ȸ��ֽ�
							 
	return	(TPH*0x100+TPL)/16;
}

/**************************************
��ʱX΢��(STC12C5A60S2@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
����ʱ������ʹ��1T��ָ�����ڽ��м���,�봫ͳ��12T��MCU��ͬ
**************************************/
void DelayXus(BYTE n)
{
    while (n--)
    {
        _nop_();
        _nop_();
    }
}

/**************************************
��λDS18B20,������豸�Ƿ����
**************************************/
void DS18B20_Reset()
{
    CY = 1;
    while (CY)
    {
        DQ = 0;                     //�ͳ��͵�ƽ��λ�ź�
        DelayXus(240);              //��ʱ����480us
        DelayXus(240);
        DQ = 1;                     //�ͷ�������
        DelayXus(60);               //�ȴ�60us
        CY = DQ;                    //����������
        DelayXus(240);              //�ȴ��豸�ͷ�������
        DelayXus(180);
    }
}

/**************************************
��DS18B20��1�ֽ�����
**************************************/
BYTE DS18B20_ReadByte()
{
    BYTE i;
    BYTE dat = 0;

    for (i=0; i<8; i++)             //8λ������
    {
        dat >>= 1;
        DQ = 0;                     //��ʼʱ��Ƭ
        DelayXus(1);                //��ʱ�ȴ�
        DQ = 1;                     //׼������
        DelayXus(1);                //������ʱ
        if (DQ) dat |= 0x80;        //��ȡ����
        DelayXus(60);               //�ȴ�ʱ��Ƭ����
    }

    return dat;
}

/**************************************
��DS18B20д1�ֽ�����
**************************************/
void DS18B20_WriteByte(BYTE dat)
{
    char i;

    for (i=0; i<8; i++)             //8λ������
    {
        DQ = 0;                     //��ʼʱ��Ƭ
        DelayXus(1);                //��ʱ�ȴ�
        dat >>= 1;                  //�ͳ�����
        DQ = CY;
        DelayXus(60);               //�ȴ�ʱ��Ƭ����
        DQ = 1;                     //�ָ�������
        DelayXus(1);                //�ָ���ʱ
    }
}
