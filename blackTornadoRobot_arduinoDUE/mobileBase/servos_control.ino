/**********************************************************************
  Copyright: 2016-2021 ROS小课堂 www.corvin.cn
  Description:控制驱动板上连接的舵机进行转动的实现代码.
  Author: corvin
  History:
    20181121: initial this code.
    20200901:优化代码中类型变量,增加注释信息,使代码更健壮稳定.
    20200912:优化moveServo()函数,只有舵机目标角度和当前角度不同才控制舵机.
**********************************************************************/

/*********************************************************************
   Description:舵机控制的构造函数,初始化舵机的各参数为0.
 ********************************************************************/
SweepServo::SweepServo()
{
  this->currentPosDegrees = 0;
  this->targetPosDegrees  = 0;
  this->lastMoveTime      = 0L;
}

/**********************************************************************
   Description:初始化各舵机的pwm引脚,初始化角度,舵机转动时的速度延时.
 *********************************************************************/
void SweepServo::initServo(byte servoPin, byte initPosition, byte stepDelayMs)
{
  this->servo.attach(servoPin);
  this->stepDelayMs = stepDelayMs;
  this->currentPosDegrees = initPosition;
  this->targetPosDegrees  = initPosition;
  this->lastMoveTime = millis();

  //when power on, move all servos to init Position
  this->servo.write(initPosition);
}

/******************************************************************
   Description:检测舵机是否需要转动,控制舵机转动时,我们增加了一个延时参数来
     控制舵机转动速度.在每个控制循环周期内,只有大于指定的延时才会控制舵机转动.
 ******************************************************************/
void SweepServo::moveServo(void)
{
  //计算当前到上次移动舵机时流逝的时间
  unsigned long delta = millis() - this->lastMoveTime;

  //如果流逝的时间大于需要舵机移动的时间,而且同时当前舵机的角度不是目标角度,那就需要控制舵机转动
  if (((this->targetPosDegrees) != (this->currentPosDegrees)) && (delta > (this->stepDelayMs)))
  {
    if (this->targetPosDegrees > this->currentPosDegrees)
    {
      this->currentPosDegrees++;
      this->servo.write(this->currentPosDegrees);
    }
    else
    {
      this->currentPosDegrees--;
      this->servo.write(this->currentPosDegrees);
    }

    this->lastMoveTime = millis(); //保存当前时间为了下次判断舵机是否需要转动
  }
}

/***************************************************************
  Description:设置舵机需要以指定的延时转动到的目的角度.这里的byte取值范围
   是0-255.所以控制舵机转动时的最大延时为255ms.
 ***************************************************************/
void SweepServo::setTargetPos(byte targetPos, byte stepDelayMs)
{
  this->targetPosDegrees = targetPos;
  this->stepDelayMs      = stepDelayMs;
}

/***************************************************************
   Description:获取舵机的当前角度.
 ***************************************************************/
byte SweepServo::getCurrentPos(void)
{
  return this->currentPosDegrees;
}

/***************************************************************
   Description:由于驱动板共可以连接6个舵机,所以舵机对象也有6个.这里就是
     获取指定的舵机对象,这样就可以根据该对象来对舵机进行相应操作了.
 ***************************************************************/
Servo SweepServo::getServoObj(void)
{
  return this->servo;
}
