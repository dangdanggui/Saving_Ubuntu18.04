/****************************************************************
  Copyright: 2016-2021 ROS小课堂 www.corvin.cn
  Author: corvin
  Description: 用于从arduino的串口或者IIC总线上获得上位机发送过来的命令
    的类，包含各种成员变量和函数声明.
  History:
    20181207: initial this file.
    20200215：增加保存往IIC中发送数据的类定义sendI2CData.
****************************************************************/
#ifndef _SERIAL_I2C_DATA_H_
#define _SERIAL_I2C_DATA_H_

#define  ENTER_CHAR  '\r'

/************************************************************
  Description:用于保存从串口或IIC总线上获取到的数据的类.
************************************************************/
class serialIICData
{
  public:
    void resetCmdParam(); //重置保存串口的类对象的各个数据

    byte argCnt;   //用来保存有几个参数
    byte argIndex; //用来记录参数保存到第几个字符

    char cmd_chr; //保存从串口接收到的单命令字符

    //保存命令字符后面的各字符,第一个缓冲区大小为36,为了可以保存下
    //三个电机的PID参数(一个电机4个参数,三个电机共12个,每个参数2字符,
    //这里还要加上12个参数中间还有11个冒号:用来分割各参数.所以存储空间
    //12*2+11=35
    char argv1[36];
    char argv2[4];
    char argv3[4];

    //将命令字符后的三个参数转换成整数后各保存在此三个变量中
    int arg1;
    int arg2;
    int arg3;
};

/***************************************************************
  Description:用于往IIC总线上发送反馈数据,当上位机的IIC master需要
      下位机的IIC总线反馈数据时的类.
****************************************************************/
class sendI2CData
{
  public:
    void resetI2CData();

    String ackMsgStr;
    int byteCnt;
};

serialIICData cmdDataObj;
sendI2CData sendI2CObj;
#endif
