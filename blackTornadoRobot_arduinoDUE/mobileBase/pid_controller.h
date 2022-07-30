/************************************************************************
  Copyright: 2016-2021 ROS小课堂 www.corvin.cn
  Description:Functions and type-defs for PID control.
  Author: corvin
  History: 
    20180209: init this file;
    20200910:删除DEBUG_PID宏定义,使其可以通过命令打开关闭pid调试信息;
**************************************************************************/
#ifndef _PID_CONTROLLER_H_
#define _PID_CONTROLLER_H_

/* Maximum PWM signal */
#define  MAX_PWM    255

/* PID setpoint info For a Motor */
typedef struct {
  byte motorID;  //电机编号,这里总共有三个电机需要控制

  int TargetTicksPerFrame;  //在每一次的PID循环中(25ms)应该得到的脉冲数
  long CurrEnc;  //保存当前PID循环时电机编码器的记录值
  long PrevEnc;  //保存前一次PID循环时编码器的记录值

  //Using previous Frame Ticks instead of PrevError to avoid derivative kick,
  //see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-derivative-kick/
  int PrevFrameTicks; //保存上一个PID循环内编码器的ticks数

  //Using integrated term (ITerm) instead of integrated error (Ierror), to allow tuning changes,
  //see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
  int ITerm;    //integrated term
  int outputPWM;  //最后计算得到需要驱动电机转动的PWM值
} SetPointInfo;

void resetPIDObjParam(void);
void updateMotorPWM(void);
void setPIDTargetTicks(int A_ticks, int B_ticks, int C_ticks);
void updatePIDParam(byte index, byte kp, byte kd, byte ki, byte ko);

int getPidPrevFrameTicks(byte wheel);
void setDebugPIDFlag(byte flag);
int getPidPWMOut(byte wheel);
#endif
