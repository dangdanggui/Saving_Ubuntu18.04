/************************************************************************
  Copyright: 2016-2021 ROS小课堂 www.corvin.cn
  Description:ROS三轮小车的三个驱动电机的PID控制器的代码实现。
  Author: corvin
  History:
    20180209: init this file;
    20200903:更新各电机的kp参数为18,经测试可在75ms使电机进入目标转速.
    20200910：新增函数getDebugPIDFlag()设置pid调试标志,使用命令y 1就可以打开;
**************************************************************************/
#include "pid_controller.h"


static SetPointInfo AWheelPID, BWheelPID, CWheelPID;

//default motor PID Parameters
static byte AWheel_Kp = 18;
static byte AWheel_Kd = 15;
static byte AWheel_Ki = 0;
static byte AWheel_Ko = 10;

static byte BWheel_Kp = 18;
static byte BWheel_Kd = 15;
static byte BWheel_Ki = 0;
static byte BWheel_Ko = 10;

static byte CWheel_Kp = 18;
static byte CWheel_Kd = 15;
static byte CWheel_Ki = 0;
static byte CWheel_Ko = 10;

static boolean debugPID = DISABLE; //是否打开pid调试信息的标志

/******************************************************
  Description:通过命令参数来设置pid调试的标志.这里的参数如果
    为1,那就是打开调试信息,0就是关闭调试信息.
 *****************************************************/
void setDebugPIDFlag(byte flag)
{
  if (1 == flag)
  {
    debugPID = ENABLE;
  }
  else
  {
    debugPID = DISABLE;
  }
}

/**************************************************************
  Description:设置每个电机在每个PID周期内(25ms)应该得到的编码器计数,
   PID控制是通过该参数来计算得到需要驱动每个电机的pwm值的.这里的输入参数
   是int型,这是因为计数值可能为负数.
 *************************************************************/
void setPIDTargetTicks(int A_ticks, int B_ticks, int C_ticks)
{
  AWheelPID.TargetTicksPerFrame = A_ticks;
  BWheelPID.TargetTicksPerFrame = B_ticks;
  CWheelPID.TargetTicksPerFrame = C_ticks;
}

/***************************************************************************************************
  Initialize PID variables to zero to prevent startup spikes
  when turning PID on to start moving
  In particular, assign both Encoder and PrevEnc the current encoder value
  See http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-initialization/
  Note that the assumption here is that PID is only turned on
  when going from stop to moving, that's why we can init everything on zero.
****************************************************************************************************/
void resetPIDObjParam(void)
{
  AWheelPID.motorID   = A_WHEEL;
  AWheelPID.TargetTicksPerFrame = 0;
  AWheelPID.CurrEnc   = 0;
  AWheelPID.PrevEnc   = 0;
  AWheelPID.PrevFrameTicks = 0;
  AWheelPID.ITerm     = 0;
  AWheelPID.outputPWM = 0;

  BWheelPID.motorID   = B_WHEEL;
  BWheelPID.TargetTicksPerFrame = 0;
  BWheelPID.CurrEnc   = 0;
  BWheelPID.PrevEnc   = 0;
  BWheelPID.PrevFrameTicks = 0;
  BWheelPID.ITerm     = 0;
  BWheelPID.outputPWM = 0;

  CWheelPID.motorID   = C_WHEEL;
  CWheelPID.TargetTicksPerFrame = 0;
  CWheelPID.CurrEnc   = 0;
  CWheelPID.PrevEnc   = 0;
  CWheelPID.PrevFrameTicks = 0;
  CWheelPID.ITerm     = 0;
  CWheelPID.outputPWM = 0;
}

/****************************************************************************
  Description:PID routine to compute the next motor commands
*****************************************************************************/
static void doWheelPID(SetPointInfo *p, byte kp, byte ki, byte kd, byte ko)
{
  int Perror = 0;
  int outputPWM = 0; //保存PID计算得到的PWM
  int currentFrameTicks = 0;

  p->CurrEnc = readEncoderCnt(p->motorID);  //获取当前电机编码器的计数值
  currentFrameTicks = p->CurrEnc - p->PrevEnc; //计算当前PID周期内得到的ticks数
  p->PrevEnc = p->CurrEnc;  //将当前读取的编码器值保存到PrevEnc中,为下次计算做准备

  Perror = p->TargetTicksPerFrame - currentFrameTicks;

  if (debugPID)
  {
    Serial.print("ID:");
    Serial.print(p->motorID);
    Serial.print(" ");

    Serial.print("Time:");
    Serial.print(millis());
    Serial.print(" ");

    Serial.print("Tar:");
    Serial.print(p->TargetTicksPerFrame);
    Serial.print(" ");

    Serial.print("Total:");
    Serial.print(p->CurrEnc);
    Serial.print(" ");

    Serial.print("Tick:");
    Serial.print(currentFrameTicks);
    Serial.print(" ");

    Serial.print("Diff:");
    Serial.print(Perror);
    Serial.print(" ");
  }

  //Avoid derivative kick and allow tuning changes,
  //see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-derivative-kick/
  //see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
  outputPWM = (kp * Perror - kd * (currentFrameTicks - p->PrevFrameTicks) + p->ITerm) / ko;
  p->PrevFrameTicks = currentFrameTicks;  //保存当前的input值,为下次计算做准备

  // Accumulate Integral error *or* Limit output. Stop accumulating when output saturates
  outputPWM += p->outputPWM;
  if (outputPWM >= MAX_PWM)
  {
    outputPWM = MAX_PWM;
  }
  else if (outputPWM <= -MAX_PWM)
  {
    outputPWM = -MAX_PWM;
  }
  else
  {
    //allow turning changes,
    //see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-tuning-changes/
    p->ITerm += ki * Perror;
  }

  p->outputPWM = outputPWM;  //将计算得到最终的pwm值保存起来,准备驱动电机转动

  if (debugPID)
  {
    Serial.print("PWM:");
    Serial.println(p->outputPWM);
    if(p->motorID == 3)
    {
        Serial.println();
    }
  }
}

/*********************************************************************
  Description:在每个PID控制周期内来根据最新的目标tick数来计算需要的pwm值,最终
   使用计算得到的各电机pwm值来驱动电机的转动,从而完成电机指定转动速度的控制。
 *********************************************************************/
void updateMotorPWM(void)
{
  if (!getMoveStatus()) //若电机现在没有在转动状态,则将所有电机PID参数重新初始化
  {
    //Reset PIDs once, to prevent startup spikes,
    //see http://brettbeauregard.com/blog/2011/04/improving-the-beginner%E2%80%99s-pid-initialization/
    //PrevInput is considered a good proxy to detect whether reset has already happened
    if (AWheelPID.PrevFrameTicks != 0 || BWheelPID.PrevFrameTicks != 0 || CWheelPID.PrevFrameTicks != 0)
    {
      resetPIDObjParam();
    }
    AWheelPID.CurrEnc = readEncoderCnt(A_WHEEL);
    AWheelPID.PrevEnc = AWheelPID.CurrEnc;

    BWheelPID.CurrEnc = readEncoderCnt(B_WHEEL);
    BWheelPID.PrevEnc = BWheelPID.CurrEnc;

    CWheelPID.CurrEnc = readEncoderCnt(C_WHEEL);
    CWheelPID.PrevEnc = CWheelPID.CurrEnc;
  }
  else //检测到当前需要有电机转动
  {
    /* Compute PID update for each motor */
    if (AWheelPID.TargetTicksPerFrame != 0) //开始使用PID控制计算PWM值
    {
      doWheelPID(&AWheelPID, AWheel_Kp, AWheel_Ki, AWheel_Kd, AWheel_Ko);
    }
    else //目标tick数为0,则停止该电机的转动,将上次计算输出的PWM值清零
    {
      AWheelPID.outputPWM = 0;
    }

    if (BWheelPID.TargetTicksPerFrame != 0)
    {
      doWheelPID(&BWheelPID, BWheel_Kp, BWheel_Ki, BWheel_Kd, BWheel_Ko);
    }
    else //目标tick数为0,则停止该电机的转动,将上次计算输出的PWM值清零
    {
      BWheelPID.outputPWM = 0;
    }

    if (CWheelPID.TargetTicksPerFrame != 0)
    {
      doWheelPID(&CWheelPID, CWheel_Kp, CWheel_Ki, CWheel_Kd, CWheel_Ko);
    }
    else //目标tick数为0,则停止该电机的转动,将上次计算输出的PWM值清零
    {
      CWheelPID.outputPWM = 0;
    }

    /* 根据PID控制器最终计算得出的PWM值来控制三台电机的转动速度 */
    setMotorsPWM(AWheelPID.outputPWM, BWheelPID.outputPWM, CWheelPID.outputPWM);
  }
}

/***************************************************************
  Description:更新指定编号电机的PID参数.
 ***************************************************************/
void updatePIDParam(byte index, byte kp, byte kd, byte ki, byte ko)
{
  switch (index)
  {
    case A_WHEEL:
      AWheel_Kp = kp;
      AWheel_Kd = kd;
      AWheel_Ki = ki;
      AWheel_Ko = ko;
      if (debugPID)
      {
        Serial.print("A PID:");
        Serial.print(AWheel_Kp);
        Serial.print(" ");
        Serial.print(AWheel_Kd);
        Serial.print(" ");
        Serial.print(AWheel_Ki);
        Serial.print(" ");
        Serial.print(AWheel_Ko);
        Serial.println();
      }
      break;

    case B_WHEEL:
      BWheel_Kp = kp;
      BWheel_Kd = kd;
      BWheel_Ki = ki;
      BWheel_Ko = ko;
      if (debugPID)
      {
        Serial.print("B PID:");
        Serial.print(BWheel_Kp);
        Serial.print(" ");
        Serial.print(BWheel_Kd);
        Serial.print(" ");
        Serial.print(BWheel_Ki);
        Serial.print(" ");
        Serial.print(BWheel_Ko);
        Serial.println();
      }
      break;

    case C_WHEEL:
      CWheel_Kp = kp;
      CWheel_Kd = kd;
      CWheel_Ki = ki;
      CWheel_Ko = ko;
      if (debugPID)
      {
        Serial.print("C PID:");
        Serial.print(CWheel_Kp);
        Serial.print(" ");
        Serial.print(CWheel_Kd);
        Serial.print(" ");
        Serial.print(CWheel_Ki);
        Serial.print(" ");
        Serial.print(CWheel_Ko);
        Serial.println();
      }
      break;

    default:
      break;
  }
}

/****************************************************************
  Description:得到在上一个PID循环周期内计算得到的编码器tick数,该函数都是
      在调试PID参数的时候才需要用到.
 ****************************************************************/
int getPidPrevFrameTicks(byte wheel)
{
  int prevFrameTicks = 0;

  if (wheel == A_WHEEL)
  {
    prevFrameTicks = AWheelPID.PrevFrameTicks;
  }
  else if (wheel == B_WHEEL)
  {
    prevFrameTicks = BWheelPID.PrevFrameTicks;
  }
  else
  {
    prevFrameTicks = CWheelPID.PrevFrameTicks;
  }

  return prevFrameTicks;
}

/*****************************************************
  Description:输出最终PID计算得出的各电机驱动PWM值,该值输出
  主要是为了调试各电机的PID参数时使用的.注意该pwm值可能为负值,
  正负表明驱动电机转动的方向.
******************************************************/
int getPidPWMOut(byte wheel)
{
  int pidout = 0;

  if (wheel == A_WHEEL)
  {
    pidout = AWheelPID.outputPWM;
  }
  else if (wheel == B_WHEEL)
  {
    pidout = BWheelPID.outputPWM;
  }
  else
  {
    pidout = CWheelPID.outputPWM;
  }

  return pidout;
}
