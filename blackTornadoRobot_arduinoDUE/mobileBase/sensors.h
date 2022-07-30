/*******************************************************************
 Copyright:2016-2021 ROS小课堂 www.corvin.cn
Description:Functions for various sensor types.
Author: corvin
History: 20180209: init this file;
    20200209:增加三个红外测距传感器.
    20200512:增加蜂鸣器一直响和关闭的功能.
    20200516:增加超声波传感器和数字排针引脚的输入输出模式定义.
    20200603:增加数字引脚控制芯片的功能引脚定义,DIR_PIN和OE_PIN.
    20210120:驱动板上更新三个红外测距传感器的接口为A1,A2,A3,这里需做同步修改;
********************************************************************/
#ifndef __SENSORS_H__
#define __SENSORS_H__

#define VOL_SENSE 0.004975124  //5除1005

//红外传感器的最远和最近的距离定义,输出的距离不能大于最大值和小于最小值
#define MAX_DISTANCE  30.0
#define MIN_DISTANCE   4.0

//超声传感器的最远和最近的距离定义,输出的距离不能大于最大值和小于最小值
#define MAX_Ultrasonic_DISTANCE  400.0
#define MIN_Ultrasonic_DISTANCE   3.0

//前左右三路红外测距传感器连接的模拟引脚定义
#define  F_INFRARED   A1
#define  L_INFRARED   A2
#define  R_INFRARED   A3

#define  BEEP_PIN    53 //蜂鸣器的数字引脚定义

#define  DISCONNECTING_BEEP  0 //断开连接时的提示音
#define  CONNECTING_BEEP     1 //建立连接时的提示音
#define  ALWAYS_BEEP         2 //蜂鸣器一直响
#define  CLOSE_BEEP          3 //关闭蜂鸣器声音

#define  BEEP_ON   LOW
#define  BEEP_OFF  HIGH

//定义超声波传感器引脚
#define ULTRASONIC_A_TRIG_PIN  11
#define ULTRASONIC_B_TRIG_PIN  12
#define ULTRASONIC_C_TRIG_PIN  13

#define ULTRASONIC_A_ECHO_PIN  46
#define ULTRASONIC_B_ECHO_PIN  47
#define ULTRASONIC_C_ECHO_PIN  48

//定义数字排针引脚
#define D49_PIN  49
#define D50_PIN  50
#define D51_PIN  51
#define D52_PIN  52

//定义数字排针控制芯片引脚
#define OE_PIN   44
#define DIR_PIN  45

#define INPUT_MODE   0
#define OUTPUT_MODE  1

void initSensors();
double getDistance(int sensor_pin);
double getUltrasonicDistance(int TrigPin, int EchoPin);
void procBeepTip(byte flag);
void cfgDigitalPin(int mode);
#endif
