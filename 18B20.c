/**************************************
;WWW.STCMCU.COM
;Mobile:13922805190
;0755-82948412
温度传感器DS18B20测试程序
主芯片  : STC12C5A60S2 (1T)
工作频率: 12.000MHz
**************************************/

#include <STC12C5620AD.H>				//加载STC12C5620AD.H头文件
#include "INTRINS.H"

typedef unsigned char BYTE;		

sbit DQ = P1^6;                     //DS18B20的数据口位P1.6
BYTE TPH;                           //存放温度值的高字节
BYTE TPL;                           //存放温度值的低字节

void DelayXus(BYTE n);
void DS18B20_Reset();
void DS18B20_WriteByte(BYTE dat);
BYTE DS18B20_ReadByte();

unsigned int	GetTemp()			//获取温度值 0:C 1:F
{
    DS18B20_Reset();                //设备复位
    DS18B20_WriteByte(0xCC);        //跳过ROM命令
    DS18B20_WriteByte(0x44);        //开始转换命令
    while (!DQ);                    //等待转换完成

    DS18B20_Reset();                //设备复位
    DS18B20_WriteByte(0xCC);        //跳过ROM命令
    DS18B20_WriteByte(0xBE);        //读暂存存储器命令
    TPL = DS18B20_ReadByte();       //读温度低字节
    TPH = DS18B20_ReadByte();       //读温度高字节
							 
	return	(TPH*0x100+TPL)/16;
}

/**************************************
延时X微秒(STC12C5A60S2@12M)
不同的工作环境,需要调整此函数
此延时函数是使用1T的指令周期进行计算,与传统的12T的MCU不同
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
复位DS18B20,并检测设备是否存在
**************************************/
void DS18B20_Reset()
{
    CY = 1;
    while (CY)
    {
        DQ = 0;                     //送出低电平复位信号
        DelayXus(240);              //延时至少480us
        DelayXus(240);
        DQ = 1;                     //释放数据线
        DelayXus(60);               //等待60us
        CY = DQ;                    //检测存在脉冲
        DelayXus(240);              //等待设备释放数据线
        DelayXus(180);
    }
}

/**************************************
从DS18B20读1字节数据
**************************************/
BYTE DS18B20_ReadByte()
{
    BYTE i;
    BYTE dat = 0;

    for (i=0; i<8; i++)             //8位计数器
    {
        dat >>= 1;
        DQ = 0;                     //开始时间片
        DelayXus(1);                //延时等待
        DQ = 1;                     //准备接收
        DelayXus(1);                //接收延时
        if (DQ) dat |= 0x80;        //读取数据
        DelayXus(60);               //等待时间片结束
    }

    return dat;
}

/**************************************
向DS18B20写1字节数据
**************************************/
void DS18B20_WriteByte(BYTE dat)
{
    char i;

    for (i=0; i<8; i++)             //8位计数器
    {
        DQ = 0;                     //开始时间片
        DelayXus(1);                //延时等待
        dat >>= 1;                  //送出数据
        DQ = CY;
        DelayXus(60);               //等待时间片结束
        DQ = 1;                     //恢复数据线
        DelayXus(1);                //恢复延时
    }
}
