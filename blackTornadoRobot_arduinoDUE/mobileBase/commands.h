/***********************************************************************************
  Copyright: 2016-2021 ROS小课堂 www.corvin.cn
  Description:Define single-letter commands that will be sent by the PC over the
     serial link or IIC bus.
  Author: corvin
  History: 
    20180209:init this file,add CODE_VERSION command;
    20200214:增加各命令字符的注释信息.
    20200310:增加夹爪控制信息.
    20200509:增加IIC方式读取红外测距传感器命令'j',与串口读取红外测距命令区分开.
    20200516:去掉电流检测命令'f',因为现在已经去掉了电流检测传感器.
    20200603:将c命令更新为配置数字排针的引脚模式,板上4个引脚输入输出模式是同步配置的.
    20200910:新增pid调试信息打开关闭命令'y',这样就可以随时控制pid调试信息输出了.
************************************************************************************/
#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#define  ANALOG_READ       'a' //读取模拟引脚值命令
#define  GET_BAUDRATE      'b' //获取串口通信波特率
#define  CFG_DIG_MODE      'c' //配置数字排针的引脚模式
#define  DIGITAL_READ      'd' //读取数字排针引脚电平状态

#define  READ_ENCODERS     'e' //读取编码器输出值
#define  SERIAL_ULTRASONIC 'f' //使用串口读取超声波测距传感器数值
#define  GET_PERCENT       'g' //获取当前锂电池剩余电量百分比
#define  SERIAL_INFRARED   'h' //使用串口读取红外测距传感器值

#define  GET_PID_TICKS     'i' //得到在一个PID周期内的编码器tick数
#define  IIC_INFRARED      'j' //IIC方式读取红外测距传感器数值
#define  IIC_ULTRASONIC    'k' //IIC方式读取超声波测距传感器数值
#define  MOTOR_CONTROL     'm' //驱动电机转动指定的脉冲

#define  GET_PIDOUT_PWM    'o' //输出最终计算得到的PWM值
#define  BEEP_RING         'p' //蜂鸣器报警提示音控制命令

#define  RESET_ENCODERS    'r' //将所有电机编码器计数清零
#define  SET_SERVO_POS     's' //设置舵机的转动角度
#define  GET_SERVO_POS     't' //得到舵机的当前角度位置
#define  UPDATE_PID        'u' //更新所有电机的PID参数
#define  CODE_VERSION      'v' //返回当前固件代码的版本号

#define  DIGITAL_WRITE     'w' //往数字IO引脚写入高低电平
#define  ANALOG_WRITE      'x' //往模拟IO引脚写入高低电平
#define  DEBUG_PID         'y' //控制pid调试信息输出的命令
#define  CONTROL_360SERVO  'z' //控制360°舵机转动的命令

#endif
