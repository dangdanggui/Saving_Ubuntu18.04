/***************************************************************
  Copyright: 2016-2021 ROS小课堂 www.corvin.cn
  Author: corvin
  Description:
    定义舵机操作的类SweepServo,舵机连接的引脚和初始的角度.在这里连接
    的两个舵机分别连接5,6两个引脚,5号引脚连接上下移动的舵机,6号引脚连接
    控制夹爪的舵机.其中设置夹爪舵机初始角度为65度,上下转动舵机的初始
    角度为90度.对于连接在PWM7-PWM10的各舵机根据需要可以连接使用,代码上
    已经都初始化完成.
  History:
   20190928: initial this comment.
   20200901:将可以连接的舵机总数改为6个,pwm分别接在PWM5,PWM6,PWM7,
     PWM8,PWM9,PWM10共6个PWM引脚上.
***************************************************************/
#ifndef _SERVOS_CONTROL_H_
#define _SERVOS_CONTROL_H_

#define  SERVOS_CNT   6  //驱动板上共可以连接6个舵机

//定义所有舵机连接的PWM引脚
byte myServoPins[SERVOS_CNT] = {5, 6, 7, 8, 9, 10};

//定义各舵机的初始化角度
byte servoInitPosition[SERVOS_CNT] = {90, 65, 0, 0, 0, 0};

class SweepServo
{
  public:
    SweepServo();
    void initServo(byte servoPin, byte initPosition, byte stepDelayMs);
    void setTargetPos(byte targetPos, byte stepDelayMs);
    byte getCurrentPos(void);
    void moveServo(void);
    Servo getServoObj(void);

  private:
    Servo servo;
    byte stepDelayMs;           //舵机转动时的延时,越大舵机转动越慢,单位ms
    unsigned long lastMoveTime; //记录舵机上次转动的时间
    byte currentPosDegrees;     //记录舵机当前的角度位置
    byte targetPosDegrees;      //保存舵机需要转动的目标角度
};

SweepServo myServos[SERVOS_CNT]; //声明舵机对象数组

#endif
