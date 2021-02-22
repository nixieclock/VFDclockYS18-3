#include <STC12C5620AD.H>				//����STC12C5620AD.Hͷ�ļ�
#include <intrins.H>
typedef unsigned char INT8U;//�滻Ϊ#define uchar unsigned char
typedef unsigned int   INT16U;//�滻Ϊ#define uint unsigned int

sfr IAP_DATA    = 0xE2;
sfr IAP_ADDRH   = 0xE3;
sfr IAP_ADDRL   = 0xE4;
sfr IAP_CMD     = 0xE5;
sfr IAP_TRIG    = 0xE6;
sfr IAP_CONTR   = 0xE7;//ʹ���ڲ�EEPROMʱ��Ҫ�õ��ļĴ������꾧��ͷ�ļ�Ҳ�ж���

//����Flash �����ȴ�ʱ�估����IAP/ISP/EEPROM �����ĳ���
//#define ENABLE_ISP 0x80 //ϵͳ����ʱ��<30MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
#define ENABLE_ISP 0x81 //ϵͳ����ʱ��<24MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x82 //ϵͳ����ʱ��<20MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//���õ�11.0592���񣬰���<20 MHz���ã�û����
//#define ENABLE_ISP 0x83 //ϵͳ����ʱ��<12MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x84 //ϵͳ����ʱ��<6MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x85 //ϵͳ����ʱ��<3MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x86 //ϵͳ����ʱ��<2MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ
//#define ENABLE_ISP 0x87 //ϵͳ����ʱ��<1MHz ʱ����IAP_CONTR �Ĵ������ô�ֵ

union union_temp16
{
    INT16U un_temp16;
    INT8U un_temp8[2];
}my_unTemp16;
//Ϊ�˷����EEPROM16λ��ַ�ĸ�λ�͵�λ���ó�������������Ҳ��ʵ�����Ŀ��
/*
union ��������{ 
������������ ��Ա��; 
������������ ��Ա��; 
����... 
����} ������;//�����壬�ٶ�һ�£����֪����
*/

INT8U Byte_Read(INT16U add);              //��һ�ֽڣ�����ǰ���IAP ����
void Byte_Program(INT16U add, INT8U ch); //�ֽڱ�̣�����ǰ���IAP ����
void Sector_Erase(INT16U add);            //��������
void IAP_Disable();                       //�ر�IAP ����
void Delay();
//��������

/*
void main (void)
{
    INT16U eeprom_address;//uint eeprom_address
    INT8U read_eeprom;//uchar read_eeprom 
    P1 = 0xF0;                            //��ʾ����ʼ���� P1[3:0] ���Ƶĵ���
    Delay();                              //��ʱ
    P1 = 0x0F;                            //��ʾ����ʼ���� P1[7:4] ���Ƶĵ���
    Delay()    ;                          //��ʱ
    //��EEPROM ������ʼ��ַ��Ԫ�����ݶ���
    eeprom_address = DATA_FLASH_START_ADDRESS; 
//��������ʼ��ַ�͸�eeprom_address

    read_eeprom = Byte_Read(eeprom_address);    //��EEPROM��ֵ,�浽read_eeprom
    if (DEBUG_DATA == read_eeprom)
{   //�����ǶԵģ��� P1.7 ���Ƶĵƣ�Ȼ���� P1 ���Ͻ� EEPROM ��������ʾ����
        P1 = ~0x80; //��~����λȡ��
        Delay()    ;                            //��ʱ
        P1 = ~read_eeprom;
    }
    else
    {   //�����Ǵ�ģ��� P1.3 ���Ƶĵƣ�Ȼ���� P1 ���Ͻ� EEPROM ��������ʾ����
        //�ٽ���EEPROM���ڵ�������������������ȷ������д����� P1.5 ���Ƶĵ�
        P1 = ~0x08;
        Delay()    ;                            //��ʱ
        P1 = ~read_eeprom;
        Delay()    ;                            //��ʱ
        Sector_Erase(eeprom_address);           //������������
        Byte_Program(eeprom_address, DEBUG_DATA);//�� DEBUG_DATA д�� EEPROM
        P1 = ~0x20;                 //Ϩ�� P1.3 ���Ƶĵƣ��� P1.5 ���Ƶĵ�
    }
    while (1);                      //CPU �ڴ�����ѭ��ִ�д˾�
}
*/

//��һ�ֽڣ�����ǰ���IAP ���ܣ����:DPTR = �ֽڵ�ַ������:A = �����ֽ�
INT8U Byte_Read(INT16U add)
{
    IAP_DATA = 0x00;
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = 0x01;                 //IAP/ISP/EEPROM �ֽڶ�����
    my_unTemp16.un_temp16 = add;//��add����������
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ
    //EA = 0;
    IAP_TRIG = 0x46;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = 0xB9;   //����A5h ��ISP/IAP ����������������
    _nop_();
    //EA = 1;
    IAP_Disable(); //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
    return (IAP_DATA);
}

//�ֽڱ�̣�����ǰ���IAP ���ܣ����:DPTR = �ֽڵ�ַ, A= �����ֽڵ�����
void Byte_Program(INT16U add, INT8U ch)
{
    IAP_CONTR = ENABLE_ISP;         //�� IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = 0x02;                 //IAP/ISP/EEPROM �ֽڱ������
    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ
    IAP_DATA = ch;                  //Ҫ��̵��������ͽ�IAP_DATA �Ĵ���
    //EA = 0;
    IAP_TRIG = 0x46;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = 0xB9;   //����A5h ��ISP/IAP ����������������
    _nop_();
    //EA = 1;
    IAP_Disable(); //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}

//��������, ���:DPTR = ������ַ
void Sector_Erase(INT16U add)
{
    IAP_CONTR = ENABLE_ISP;         //��IAP ����, ����Flash �����ȴ�ʱ��
    IAP_CMD = 0x03;                 //IAP/ISP/EEPROM ������������
    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //����Ŀ�굥Ԫ��ַ�ĸ�8 λ��ַ
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //����Ŀ�굥Ԫ��ַ�ĵ�8 λ��ַ
    //EA = 0;
    IAP_TRIG = 0x46;   //���� 5Ah,����A5h ��ISP/IAP �����Ĵ���,ÿ�ζ������
    IAP_TRIG = 0xB9;   //����A5h ��ISP/IAP ����������������
    _nop_();
    //EA = 1;
    IAP_Disable(); //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
                    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
}

void IAP_Disable()
{
    //�ر�IAP ����, ����ص����⹦�ܼĴ���,ʹCPU ���ڰ�ȫ״̬,
    //һ��������IAP �������֮����ر�IAP ����,����Ҫÿ�ζ���
    IAP_CONTR = 0;      //�ر�IAP ����
    IAP_CMD   = 0;      //������Ĵ���,ʹ����Ĵ���������,�˾�ɲ���
    IAP_TRIG = 0;      //��������Ĵ���,ʹ������Ĵ����޴���,�˾�ɲ���
    IAP_ADDRH = 0;
    IAP_ADDRL = 0;
}