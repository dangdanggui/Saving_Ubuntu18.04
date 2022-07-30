/*************************************************************
  Copyright: 2016-2021 ROS小课堂 www.corvin.cn
  Description:
    用于arduino从串口或IIC总线获得命令类中各函数的实现.
  Author: corvin
  History:
   20181207: initial this file.
   20200217：增加重置I2C发送数据缓冲区的函数实现并增加注释信息.
*************************************************************/

/************************************************
 Description:将接收串口和I2C控制命令的缓冲区给清零,
   开始准备存储新的控制命令数据.
 ***********************************************/
void serialIICData::resetCmdParam()
{
  this->cmd_chr = ENTER_CHAR;

  memset(this->argv1, ENTER_CHAR, sizeof(this->argv1));
  memset(this->argv2, ENTER_CHAR, sizeof(this->argv2));
  memset(this->argv3, ENTER_CHAR, sizeof(this->argv3));

  this->argCnt   = 0;
  this->argIndex = 0;
}

/*************************************************
 Description:将在IIC总线上发送数据的缓冲区给清零,准备
   发送新的IIC数据.
 *************************************************/
void sendI2CData::resetI2CData()
{
  ackMsgStr = "";
  this->byteCnt = 0;
}
