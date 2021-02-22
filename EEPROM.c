#include <STC12C5620AD.H>				//加载STC12C5620AD.H头文件
#include <intrins.H>
typedef unsigned char INT8U;//替换为#define uchar unsigned char
typedef unsigned int   INT16U;//替换为#define uint unsigned int

sfr IAP_DATA    = 0xE2;
sfr IAP_ADDRH   = 0xE3;
sfr IAP_ADDRL   = 0xE4;
sfr IAP_CMD     = 0xE5;
sfr IAP_TRIG    = 0xE6;
sfr IAP_CONTR   = 0xE7;//使用内部EEPROM时需要用到的寄存器，宏晶的头文件也有定义

//定义Flash 操作等待时间及允许IAP/ISP/EEPROM 操作的常数
//#define ENABLE_ISP 0x80 //系统工作时钟<30MHz 时，对IAP_CONTR 寄存器设置此值
#define ENABLE_ISP 0x81 //系统工作时钟<24MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x82 //系统工作时钟<20MHz 时，对IAP_CONTR 寄存器设置此值
//我用的11.0592晶振，按照<20 MHz设置，没问题
//#define ENABLE_ISP 0x83 //系统工作时钟<12MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x84 //系统工作时钟<6MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x85 //系统工作时钟<3MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x86 //系统工作时钟<2MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x87 //系统工作时钟<1MHz 时，对IAP_CONTR 寄存器设置此值

union union_temp16
{
    INT16U un_temp16;
    INT8U un_temp8[2];
}my_unTemp16;
//为了分离出EEPROM16位地址的高位和低位，用除法和求余运算也能实现这个目的
/*
union 共用体名{ 
　　数据类型 成员名; 
　　数据类型 成员名; 
　　... 
　　} 变量名;//公用体，百度一下，你就知道。
*/

INT8U Byte_Read(INT16U add);              //读一字节，调用前需打开IAP 功能
void Byte_Program(INT16U add, INT8U ch); //字节编程，调用前需打开IAP 功能
void Sector_Erase(INT16U add);            //擦除扇区
void IAP_Disable();                       //关闭IAP 功能
void Delay();
//函数声明

/*
void main (void)
{
    INT16U eeprom_address;//uint eeprom_address
    INT8U read_eeprom;//uchar read_eeprom 
    P1 = 0xF0;                            //演示程序开始，让 P1[3:0] 控制的灯亮
    Delay();                              //延时
    P1 = 0x0F;                            //演示程序开始，让 P1[7:4] 控制的灯亮
    Delay()    ;                          //延时
    //将EEPROM 测试起始地址单元的内容读出
    eeprom_address = DATA_FLASH_START_ADDRESS; 
//将测试起始地址送给eeprom_address

    read_eeprom = Byte_Read(eeprom_address);    //读EEPROM的值,存到read_eeprom
    if (DEBUG_DATA == read_eeprom)
{   //数据是对的，亮 P1.7 控制的灯，然后在 P1 口上将 EEPROM 的数据显示出来
        P1 = ~0x80; //”~”按位取反
        Delay()    ;                            //延时
        P1 = ~read_eeprom;
    }
    else
    {   //数据是错的，亮 P1.3 控制的灯，然后在 P1 口上将 EEPROM 的数据显示出来
        //再将该EEPROM所在的扇区整个擦除，将正确的数据写入后，亮 P1.5 控制的灯
        P1 = ~0x08;
        Delay()    ;                            //延时
        P1 = ~read_eeprom;
        Delay()    ;                            //延时
        Sector_Erase(eeprom_address);           //擦除整个扇区
        Byte_Program(eeprom_address, DEBUG_DATA);//将 DEBUG_DATA 写入 EEPROM
        P1 = ~0x20;                 //熄灭 P1.3 控制的灯，亮 P1.5 控制的灯
    }
    while (1);                      //CPU 在此无限循环执行此句
}
*/

//读一字节，调用前需打开IAP 功能，入口:DPTR = 字节地址，返回:A = 读出字节
INT8U Byte_Read(INT16U add)
{
    IAP_DATA = 0x00;
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x01;                 //IAP/ISP/EEPROM 字节读命令
    my_unTemp16.un_temp16 = add;//把add赋给共用体
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址
    //EA = 0;
    IAP_TRIG = 0x46;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xB9;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable(); //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    return (IAP_DATA);
}

//字节编程，调用前需打开IAP 功能，入口:DPTR = 字节地址, A= 须编程字节的数据
void Byte_Program(INT16U add, INT8U ch)
{
    IAP_CONTR = ENABLE_ISP;         //打开 IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x02;                 //IAP/ISP/EEPROM 字节编程命令
    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址
    IAP_DATA = ch;                  //要编程的数据先送进IAP_DATA 寄存器
    //EA = 0;
    IAP_TRIG = 0x46;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xB9;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable(); //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

//擦除扇区, 入口:DPTR = 扇区地址
void Sector_Erase(INT16U add)
{
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x03;                 //IAP/ISP/EEPROM 扇区擦除命令
    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址
    //EA = 0;
    IAP_TRIG = 0x46;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xB9;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable(); //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

void IAP_Disable()
{
    //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    IAP_CONTR = 0;      //关闭IAP 功能
    IAP_CMD   = 0;      //清命令寄存器,使命令寄存器无命令,此句可不用
    IAP_TRIG = 0;      //清命令触发寄存器,使命令触发寄存器无触发,此句可不用
    IAP_ADDRH = 0;
    IAP_ADDRL = 0;
}