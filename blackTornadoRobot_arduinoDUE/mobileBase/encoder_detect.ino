/*******************************************************************
  Copyright: 2016-2021  www.corvin.cn ROS小课堂
  Description:  Encoder definitions需要连接到arduino DUE的正确引脚上,
    对于arduino due来说,任何数字引脚都可以作为中断引脚来使用,所以做如下定义:
    Encoder A: connect interrupt [pin 26, 27]
    Encoder B: connect interrupt [pin 30, 31]
    Encoder C: connect intterupt [pin 34, 35]
    这些引脚都已经在驱动板上定义好了,连接电机时只需要将电机线插在插座上即可.
    若电机减速比是1:78,编码器一圈是11个脉冲,那么车轮转动一圈一路信号输出的脉冲
    计数为78*11=858,那么两路同时记录中断,输出就是1716个中断计数。
  Author: corvin
  History:
    20190909:init this file;
    20200212:增加完善代码注释信息.
    20200903:将电机编码器连接引脚的数据类型定义从int改为byte.
    20210120:配置各电机编码器引脚模式为INPUT_PULLUP,中断检测下降沿信号;
*******************************************************************/

//A,B,C三个电机的编码器连接的中断引脚,这些引脚驱动板已经定好了,这里代码禁止修改
static const byte ENC_A_PIN_A = 26;
static const byte ENC_A_PIN_B = 27;
static const byte ENC_B_PIN_A = 30;
static const byte ENC_B_PIN_B = 31;
static const byte ENC_C_PIN_A = 34;
static const byte ENC_C_PIN_B = 35;

//保存三个电机编码器的中断计数值,使用易失性变量(volatile)保存
static volatile long A_enc_cnt = 0L;
static volatile long B_enc_cnt = 0L;
static volatile long C_enc_cnt = 0L;

/***********************************************************
  Description:初始化各电机编码器所连接引脚的模式,然后就是配置各中断
    函数.这里的中断检测使用的是下降沿电平触发模式,如果改为CHANGE那就
    成为了四倍频模式了,但是我们电机没必要这么高的检测精度.
************************************************************/
void initEncoders(void)
{
  pinMode(ENC_A_PIN_A, INPUT_PULLUP);
  pinMode(ENC_A_PIN_B, INPUT_PULLUP);
  attachInterrupt(ENC_A_PIN_A, encoderA_ISR, FALLING);
  attachInterrupt(ENC_A_PIN_B, encoderA_ISR, FALLING);

  pinMode(ENC_B_PIN_A, INPUT_PULLUP);
  pinMode(ENC_B_PIN_B, INPUT_PULLUP);
  attachInterrupt(ENC_B_PIN_A, encoderB_ISR, FALLING);
  attachInterrupt(ENC_B_PIN_B, encoderB_ISR, FALLING);

  pinMode(ENC_C_PIN_A, INPUT_PULLUP);
  pinMode(ENC_C_PIN_B, INPUT_PULLUP);
  attachInterrupt(ENC_C_PIN_A, encoderC_ISR, FALLING);
  attachInterrupt(ENC_C_PIN_B, encoderC_ISR, FALLING);
}

/*******************************************************
  Description:A电机的编码器的中断处理函数,编码器的AB相输出都是
   调用该中断处理函数.
 ******************************************************/
void encoderA_ISR (void)
{
  if (directionWheel(A_WHEEL) == BACKWARDS)
  {
    A_enc_cnt--;
  }
  else
  {
    A_enc_cnt++;
  }
}

/*******************************************************
  Description:B电机的编码器的中断处理函数,编码器的AB相输出都是
   调用该中断处理函数.
 ******************************************************/
void encoderB_ISR (void)
{
  if (directionWheel(B_WHEEL) == BACKWARDS)
  {
    B_enc_cnt--;
  }
  else
  {
    B_enc_cnt++;
  }
}

/*******************************************************
  Description:C电机的编码器的中断处理函数,编码器的AB相输出都是
   调用该中断处理函数.
 ******************************************************/
void encoderC_ISR (void)
{
  if (directionWheel(C_WHEEL) == BACKWARDS)
  {
    C_enc_cnt--;
  }
  else
  {
    C_enc_cnt++;
  }
}

/********************************************
  Description:根据输入参数来读取对应电机的编码器
   读数,注意这里的读数是有可能为负数.
 *******************************************/
long readEncoderCnt(byte motor)
{
  if (motor == A_WHEEL)
  {
    return A_enc_cnt;
  }
  else if (motor == B_WHEEL)
  {
    return B_enc_cnt;
  }
  else
  {
    return C_enc_cnt;
  }
}

/********************************************
  Description:将所有编码器的中断计数值清零.
 *******************************************/
void resetEncodersCnt(void)
{
  A_enc_cnt = 0L;
  B_enc_cnt = 0L;
  C_enc_cnt = 0L;
}
