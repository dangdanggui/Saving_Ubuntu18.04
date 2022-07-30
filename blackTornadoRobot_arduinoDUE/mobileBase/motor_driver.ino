/*****************************************************************
  Copyright:2016-2021 www.corvin.cn ROS小课堂
  Description:驱动电机进行转动的代码,这里使用的驱动芯片是TB6612FNG.
    当PWM引脚置低电平时,则对应路电机就处于刹车状态.通过setMotorStandby()
    函数可以设置驱动芯片进入使能状态或待机状态,只有在使能状态才能驱动电机.
    在待机状态,驱动芯片进入低功耗模式.这里需要注意电机停止时需要用刹车模式,
    自由停车模式在使用ROS上层代码控制时会导致电机抖动,从而烧坏电机.
  Author: corvin
  Hisotry: 20180209: init this file;
    20200102:为各函数增加注释信息,优化各常量命名.
    20200217:增加电机驱动芯片设置standby状态的函数.
    20200704:当电机停止转动时,配置芯片进入待机模式省电,需要驱动电机转动时,
      先使能驱动芯片进入正常工作模式.
*****************************************************************/

//两个驱动芯片的standby引脚定义,高电平是芯片使能状态,低电平是待机状态
static const byte stby_A_Pin  = 22;
static const byte stby_B_Pin  = 23;

static const byte MOTOR_A_IN1  = 24;
static const byte MOTOR_A_IN2  = 25;
static const byte A_PWM_PIN    = 2;   //A wheel pwm pin

static const byte MOTOR_B_IN1  = 28;
static const byte MOTOR_B_IN2  = 29;
static const byte B_PWM_PIN    = 3;   //B wheel pwm pin

static const byte MOTOR_C_IN1  = 32;
static const byte MOTOR_C_IN2  = 33;
static const byte C_PWM_PIN    = 4;   //C wheel pwm pin

static boolean moving = STOP; //标记是否有电机在转动

//三路电机开始默认的转动方向标记为正向转动
static boolean direcA = FORWARDS;
static boolean direcB = FORWARDS;
static boolean direcC = FORWARDS;

/*****************************************************
  Description:初始化配置各电机引脚模式.配置控制三个电机
   正反转的IN脚为输出模式,驱动IC的standby引脚为输出模式.
   默认使能驱动IC,使其进入待机工作状态.等到控制电机转动时,再使
   驱动IC进入工作模式.
 ****************************************************/
void initMotorsPinMode(void)
{
  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);
  pinMode(MOTOR_B_IN1, OUTPUT);
  pinMode(MOTOR_B_IN2, OUTPUT);
  pinMode(MOTOR_C_IN1, OUTPUT);
  pinMode(MOTOR_C_IN2, OUTPUT);

  pinMode(stby_A_Pin, OUTPUT);
  pinMode(stby_B_Pin, OUTPUT);

  setMotorStandby(ENABLE); //默认驱动芯片进入待机模式
}

/******************************************************************
  Description:驱动电机转动的底层函数,这里需要的参数是控制电机转动的方向,
   电机的两个输入引脚,电机连接的pwm引脚,需要驱动的pwm值.这里的pwm值越大,
   则电机转动的速度越快.这里的参数类型都设置为byte,取值范围0-255,没必要使用
   int浪费存储空间.
 ******************************************************************/
static void driveMotor(boolean direc, byte IN1_Pin, byte IN2_Pin, byte PWM_Pin, byte value)
{
  if (direc == FORWARDS)
  {
    digitalWrite(IN1_Pin, HIGH);
    digitalWrite(IN2_Pin, LOW);
  }
  else
  {
    digitalWrite(IN1_Pin, LOW);
    digitalWrite(IN2_Pin, HIGH);
  }
  analogWrite(PWM_Pin, value);
}

/***********************************************************************
  Description:设置三路电机转动的pwm值,这里只需三路电机的pwm值,通过pwm值的正负,
   来区分电机的正反转.当输入的pwm值为0,则表明需要该路电机需刹车制动.这里需要注意
   为了防止出现穿透电流,在驱动芯片中各模式切换时需要加上死区时间.例如：
   ON->OFF->短路制动,从ON状态切换到短路制动模式中间,必须加上OFF状态转换.
   短路制动->OFF->ON:从短路制动模式到ON模式中,必须加上OFF状态.
 **********************************************************************/
void setMotorsPWM(int motor_a_pwm, int motor_b_pwm, int motor_c_pwm)
{
  stopMotorMove(MOTOR_A_IN1, MOTOR_A_IN2, A_PWM_PIN); //motor A停止模式
  if (motor_a_pwm > 0) //drive motor A
  {
    direcA = FORWARDS;
    driveMotor(direcA, MOTOR_A_IN1, MOTOR_A_IN2, A_PWM_PIN, motor_a_pwm);
  }
  else if (motor_a_pwm < 0)
  {
    direcA = BACKWARDS;
    driveMotor(direcA, MOTOR_A_IN1, MOTOR_A_IN2, A_PWM_PIN, -motor_a_pwm);
  }
  else //pwm = 0, motor A stop
  {
    analogWrite(A_PWM_PIN, LOW); //短路制动模式
  }

  stopMotorMove(MOTOR_B_IN1, MOTOR_B_IN2, B_PWM_PIN); //Motor B停止模式
  if (motor_b_pwm > 0) //drive motor B
  {
    direcB = FORWARDS;
    driveMotor(direcB, MOTOR_B_IN1, MOTOR_B_IN2, B_PWM_PIN, motor_b_pwm);
  }
  else if (motor_b_pwm < 0)
  {
    direcB = BACKWARDS;
    driveMotor(direcB, MOTOR_B_IN1, MOTOR_B_IN2, B_PWM_PIN, -motor_b_pwm);
  }
  else //pwm = 0, motor B stop
  {
    analogWrite(B_PWM_PIN, LOW); //短路制动模式
  }

  stopMotorMove(MOTOR_C_IN1, MOTOR_C_IN2, C_PWM_PIN); //Motor C停止模式
  if (motor_c_pwm > 0) //drive motor C
  {
    direcC = FORWARDS;
    driveMotor(direcC, MOTOR_C_IN1, MOTOR_C_IN2, C_PWM_PIN, motor_c_pwm);
  }
  else if (motor_c_pwm < 0)
  {
    direcC = BACKWARDS;
    driveMotor(direcC, MOTOR_C_IN1, MOTOR_C_IN2, C_PWM_PIN, -motor_c_pwm);
  }
  else //pwm = 0, motor C stop
  {
    analogWrite(C_PWM_PIN, LOW); //短路制动模式
  }
}

/**************************************************************
  Description:设置并记录下当前是否有电机转动的状态.当控制电机进入停止
    状态时,顺便一起让驱动芯片进入待机模式,可以节省电能消耗.但是进入待机
    模式后,就没办法立刻将电机刹车停止转动了.
 *************************************************************/
void setMoveStatus(boolean state)
{
  moving = state;
  if (moving == STOP) //电机在停止转动状态
  {
    setMotorsPWM(0, 0, 0); //立即停止电机转动,直接给各电机pwm赋值0
  }
  else //电机需要转动
  {
    setMotorStandby(DISABLE); //使驱动芯片进入工作状态
  }
}

/***************************************************
  Description:获取当前是否有电机正在转动的标志,如果
   有电机在转动那么反馈1,没有电机转动反馈0.
 **************************************************/
boolean getMoveStatus(void)
{
  return moving;
}

/***************************************************
   Description:获取各电机的转动方向.
 **************************************************/
boolean directionWheel(byte wheel)
{
  if (wheel == A_WHEEL)
  {
    return direcA;
  }
  else if (wheel == B_WHEEL)
  {
    return direcB;
  }
  else
  {
    return direcC;
  }
}

/*******************************************************
  Description:设置电机进入使能状态或待机状态,只有驱动芯片
   在使能状态时,才能驱动电机转动.当在待机状态时,驱动芯片处
   于待机状态,驱动芯片进入低功耗模式,无法驱动电机再转动了.
 *******************************************************/
static void setMotorStandby(boolean state)
{
  if (state == DISABLE) //进入使能工作状态
  {
    digitalWrite(stby_A_Pin, HIGH);
    digitalWrite(stby_B_Pin, HIGH);
  }
  else //进入待机节能状态 ENABLE
  {
    digitalWrite(stby_A_Pin, LOW);
    digitalWrite(stby_B_Pin, LOW);
  }
}

/****************************************************************
  Description:当收到PWM=0时,就是停止相应的电机停止转动,这里的实现方式是
    自由停车,不是立即刹车模式.立即刹车只需要对应PWM引脚置LOW即可.但是刹车
    模式对电机损伤比较大,还是自由停车模式更好一点.
 ****************************************************************/
static void stopMotorMove(byte IN1_Pin, byte IN2_Pin, byte PWM_Pin)
{
  digitalWrite(IN1_Pin, LOW);
  digitalWrite(IN2_Pin, LOW);

  analogWrite(PWM_Pin, HIGH);
}
