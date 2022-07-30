/*************************************************************************
  Copyright: 2016-2021 ROS小课堂 www.corvin.cn
  Description:各传感器代码的实现部分,注意这里的蜂鸣器在发声音的时候,使用了delay()
      该函数使用timer0来进行延时,会导致整个系统的loop循环运行被暂停.
  Author: corvin
  History:
      20200209:增加三个红外测距传感器,型号SHARP GP2Y0A41SK0F 4-30cm.
      20200212:将蜂鸣器代码整合在这里,删除蜂鸣器原来的源码文件.
      20200218:完善红外测距函数,限定输出距离范围在4-30cm内.
      20200512：增加蜂鸣器一直响和关闭的功能.
      20200516:增加超声波传感器测距的功能.
      20200603：增加数字排针输入输出控制函数.
**************************************************************************/

/************************************************
  Description:初始化各传感器的引脚输入输出模式.
 ***********************************************/
void initSensors()
{
  //初始化红外测距传感器的引脚模式
  pinMode(F_INFRARED, INPUT);
  pinMode(L_INFRARED, INPUT);
  pinMode(R_INFRARED, INPUT);

  //初始化蜂鸣器引脚模式
  pinMode(BEEP_PIN, OUTPUT);
  devicePowerOnTip(); //蜂鸣器发出,设备加电时的提示音,两声短促嘀嘀声音.

  //初始化超声波传感器引脚模式
  pinMode(ULTRASONIC_A_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_B_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_C_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_A_ECHO_PIN, INPUT);
  pinMode(ULTRASONIC_B_ECHO_PIN, INPUT);
  pinMode(ULTRASONIC_C_ECHO_PIN, INPUT);

  //初始化配置数字引脚模式-输入模式
  pinMode(OE_PIN,  OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(OE_PIN,  LOW); //配置电平转换芯片使能
  cfgDigitalPin(INPUT_MODE);  //默认配置数字引脚为输入模式
}

/*****************************************************
 * Description: 初始化数字排针引脚模式,注意这里使用了
 *   电平转换芯片SN74LVC4245A,所以在配置IO引脚输入输出时,
 *   需要配置好芯片的DIR引脚.这里在使用时需要注意四根排针的
 *   模式是同步的,输入都输入,输出都输出模式.
 ****************************************************/
void cfgDigitalPin(int mode)
{
  if (INPUT_MODE == mode) //输入模式
  {
    digitalWrite(DIR_PIN, HIGH);

    pinMode(D49_PIN, INPUT);
    pinMode(D50_PIN, INPUT);
    pinMode(D51_PIN, INPUT);
    pinMode(D52_PIN, INPUT);
  }
  else //输出模式
  {
    digitalWrite(DIR_PIN, LOW);

    pinMode(D49_PIN, OUTPUT);
    pinMode(D50_PIN, OUTPUT);
    pinMode(D51_PIN, OUTPUT);
    pinMode(D52_PIN, OUTPUT);
  }
}

/**********************************************************************
  Description:获取指定的模拟引脚上红外测距传感器的值，输出单位是cm.这里需要注意
    输出的距离值范围需要在规定的4-30cm范围内,所有其他大于或小于的数据都需要限制.
    经过实际传感器插在板子上测试,发现读取的模拟引脚值比实际值小10左右,
    因此需要在计算电压的时候,读取值增加10后再计算电压.当得到正确电压后就
    可以计算距离了,这里的公式参考github上得到的,网址如下：
  https://github.com/guillaume-rico/SharpIR/blob/master/SharpIR.cpp
**********************************************************************/
double getDistance(int sensor_pin)
{
  double distance = 0.0;
  uint16_t value = analogRead(sensor_pin);
  double voltage = (value + 10) * VOL_SENSE;

  distance = 12.08 * pow(voltage, -1.058); //计算距离公式
  if (distance > MAX_DISTANCE)
  {
    distance = MAX_DISTANCE;
  }
  else if (distance < MIN_DISTANCE)
  {
    distance = MIN_DISTANCE;
  }

  return distance;
}

/**********************************************************************
  Description:获取指定的模拟引脚上超声波测距传感器的值，输出单位是cm.这里需要注意
    输出的距离值范围需要在规定的2-400cm范围内,所有其他大于或小于的数据都需要限制.
    注意，两次测试间隔要大于150ms.
**********************************************************************/
double getUltrasonicDistance(int TrigPin, int EchoPin)
{
  double distance = 0.0;

  digitalWrite(TrigPin, LOW);
  delayMicroseconds(1);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  distance = pulseIn(EchoPin, HIGH) / (1000000.0 / 340.0 / 100.0 * 2.0); //将回波时间换算成cm, pulse(us)/1000000(s)*340(m)*100(cm)/2
  distance = (int(distance * 100.0)) / 100.0; //保留两位小数
  if (distance > MAX_Ultrasonic_DISTANCE)
  {
    distance = MAX_Ultrasonic_DISTANCE;
  }
  else if (distance < MIN_Ultrasonic_DISTANCE)
  {
    distance = MIN_Ultrasonic_DISTANCE;
  }

  return distance;
}

/************************************************
  Description:操作蜂鸣器发出各种提示声音.
 ************************************************/
void procBeepTip(byte flag)
{
  switch (flag)
  {
    case CONNECTING_BEEP: //arduino与上位机正在建立连接的提示音
      startConnectingTip();
      break;

    case DISCONNECTING_BEEP: //arduino与上位机正在断开连接的提示音
      disConnectingTip();
      break;

    case ALWAYS_BEEP: //蜂鸣器一直响
      digitalWrite(BEEP_PIN, BEEP_ON);
      break;

    case CLOSE_BEEP: //关闭蜂鸣器声音
      digitalWrite(BEEP_PIN, BEEP_OFF);
      break;

    default:
      break;
  }
}

/**********************************************
   下位机arduino刚加电启动时提示音,间隔40ms的两声短
   促提示音,只在加电时运行一次.
**********************************************/
static void devicePowerOnTip(void)
{
  for (int i = 0; i < 2; i++)
  {
    delay(30);
    digitalWrite(BEEP_PIN, BEEP_ON);
    delay(30);
    digitalWrite(BEEP_PIN, BEEP_OFF);
  }
}

/*****************************************
   上位机与下位机开始建立通信时的提示音,
   间隔80ms的两声短促提示音.
*****************************************/
void startConnectingTip(void)
{
  for (int i = 0; i < 2; i++)
  {
    delay(80);
    digitalWrite(BEEP_PIN, BEEP_ON);
    delay(80);
    digitalWrite(BEEP_PIN, BEEP_OFF);
  }
}

/******************************************
   上位机与下位机断开通信时的提示音,
   间隔500ms的两声长提示音.
*******************************************/
void disConnectingTip(void)
{
  for (int i = 0; i < 2; i++)
  {
    delay(500);
    digitalWrite(BEEP_PIN, BEEP_ON);
    delay(500);
    digitalWrite(BEEP_PIN, BEEP_OFF);
  }
}
