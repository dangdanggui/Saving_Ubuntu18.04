/*****************************************************************************
  Copyright: 2016-2021 www.corvin.cn ROS小课堂
  Description:为ROS三轮全向移动小车编写的基于Arduino DUE的下位机代码,上位机通过串口或
    IIC总线发送控制命令到下位机arduino上.下位机收到命令进行解析和执行相应的控制命令,这里
    下位机功能包括电压,电流测量,电机PID控制器的实现,传感器数据获取反馈,舵机的控制等.
  Author: corvin
  History:
    20190716:init code;
    20190927:增加蜂鸣器控制代码,电压,电流检测的代码;
    20200102:增加与上位机可以IIC通信控制的代码,现在arduino代码支持串口和IIC.
    20200209:增加红外测距传感器的代码,红外传感器型号SHARP GP2Y0A41SKOF 4-30cm.
    20200212:将蜂鸣器代码整合进sensors代码中,将电压电流检测代码整合在一个源码文件中.
    20200310:增加机械夹爪控制代码,夹爪安装时舵机初始角度为0度.
    20200509:增加IIC方式读取红外测距传感器数据方法.
    20200603:增加配置数字排针引脚输入输出模式功能.
    20200704:当接收控制移动命令超时后,首先判断电机是否在转动状态,只有在运动状态时才需要
      将电机停止,同时驱动芯片进入待机模式节能.若电机已经停止转动,那么就不再每次都停止电机.
    20200707:在loop循环中判断当超过指定未收到移动命令后,控制驱动芯片进入节能模式.当收到
      控制移动m命令后,重新使芯片进入到工作模式.进入节能模式时,编码器计数清零.
    20200910:增加可以控制pid调试信息输出的命令,命令字符y,参数1是打开调试信息,0是关闭.
    20200927:将电机超时停止转动从900ms->300ms,在运行时move_base时测出来的最大延时.
******************************************************************************/
#include <Servo.h>
#include <Wire.h>
#include <string.h>
#include "common.h"
#include "sensors.h"
#include "commands.h"
#include "serialI2CData.h"
#include "servos_control.h"
#include "pid_controller.h"


/********************** USER  AREAR *************************/
#define  VERSION   1.0  //当前代码版本号

//在该时间间隔内没有收到新的控制电机转动命令,则停止电机转动(单位ms)
#define  AUTO_STOP_INTERVAL  300

//可以改为串口Serial3,这样后面的串口输出都会跟着修改
#define  SERIAL_OUT  Serial

#define  BAUDRATE  57600  //SERIAL_OUT串口通信的波特率

#define  IIC_ADDR  0x08  //下位机默认的IIC地址,可以根据需要修改

//超过该时间未收到移动命令,使驱动芯片进入节能模式,单位ms
#define  ENERGE_MODE_TIME   4000
/********************** USER  END ****************************/

//电机进行PID控制的默认循环周期为40 Hz - 25ms
static const byte PID_INTERVAL = 25;

/* Track the next time we make a PID calculation */
static unsigned long nextPIDLoopCheck = PID_INTERVAL;
static unsigned long lastMotorCommand = AUTO_STOP_INTERVAL;

/***************************************************************
  Description:  当从串口或者IIC中解析出控制命令字符后，开始执行该命令.
    所有的命令字符定义都在commands.h文件中.
****************************************************************/
void runCommand(void)
{
  int i = 0;
  char *str = NULL;
  char *p = cmdDataObj.argv1;
  byte pid_args[12];

  if (cmdDataObj.cmd_chr != UPDATE_PID)
  {
    cmdDataObj.arg1 = atoi(cmdDataObj.argv1);
    cmdDataObj.arg2 = atoi(cmdDataObj.argv2);
    cmdDataObj.arg3 = atoi(cmdDataObj.argv3);
  }
  sendI2CObj.resetI2CData(); //将用于IIC数据发送的数据缓冲区清零

  switch (cmdDataObj.cmd_chr) //根据控制命令字符来判断需要执行的命令
  {
    case ANALOG_READ:  //'a'-读取模拟引脚数据
      SERIAL_OUT.println(analogRead(cmdDataObj.arg1));
      sendI2CObj.ackMsgStr.concat(analogRead(cmdDataObj.arg1));
      break;

    case GET_BAUDRATE: //'b'-获取串口通信波特率
      {
        SERIAL_OUT.println(BAUDRATE);
        int baud = BAUDRATE;
        sendI2CObj.ackMsgStr.concat(baud);
      }
      break;

    case CFG_DIG_MODE: //'c'-配置数字排针引脚输入输出模式
      cfgDigitalPin(cmdDataObj.arg1);
      SERIAL_OUT.println("OK");
      sendI2CObj.ackMsgStr.concat("OK");
      break;

    case DIGITAL_READ: //'d'-读取数字IO引脚上传感器的数据
      SERIAL_OUT.println(digitalRead(cmdDataObj.arg1));
      sendI2CObj.ackMsgStr.concat(digitalRead(cmdDataObj.arg1));
      break;

    case ANALOG_WRITE: //
      analogWrite(cmdDataObj.arg1, cmdDataObj.arg2);
      SERIAL_OUT.println("OK");
      sendI2CObj.ackMsgStr.concat("OK");
      break;

    case DIGITAL_WRITE: //'w'-数字引脚上写入高低电平
      if (cmdDataObj.arg2 == 0)
      {
        digitalWrite(cmdDataObj.arg1, LOW);
      }
      else if (cmdDataObj.arg2 == 1)
      {
        digitalWrite(cmdDataObj.arg1, HIGH);
      }
      SERIAL_OUT.println("OK");
      sendI2CObj.ackMsgStr.concat("OK");
      break;

    case READ_ENCODERS:  //'e'-获取三路电机的编码器中断计数值
      SERIAL_OUT.print(readEncoderCnt(A_WHEEL));
      sendI2CObj.ackMsgStr.concat(readEncoderCnt(A_WHEEL));
      SERIAL_OUT.print(" ");
      sendI2CObj.ackMsgStr.concat(" ");
      SERIAL_OUT.print(readEncoderCnt(B_WHEEL));
      sendI2CObj.ackMsgStr.concat(readEncoderCnt(B_WHEEL));
      SERIAL_OUT.print(" ");
      sendI2CObj.ackMsgStr.concat(" ");
      SERIAL_OUT.println(readEncoderCnt(C_WHEEL));
      sendI2CObj.ackMsgStr.concat(readEncoderCnt(C_WHEEL));
      break;

    case SERIAL_INFRARED:  //'h'-通过串口读取红外测距传感器值
      SERIAL_OUT.print(getDistance(F_INFRARED));
      SERIAL_OUT.print(" ");
      SERIAL_OUT.print(getDistance(L_INFRARED));
      SERIAL_OUT.print(" ");
      SERIAL_OUT.println(getDistance(R_INFRARED));
      break;

    case IIC_INFRARED: //'j'-IIC方式读取红外测距传感器数值
      sendI2CObj.ackMsgStr.concat(getDistance(F_INFRARED));
      sendI2CObj.ackMsgStr.concat(" ");
      sendI2CObj.ackMsgStr.concat(getDistance(L_INFRARED));
      sendI2CObj.ackMsgStr.concat(" ");
      sendI2CObj.ackMsgStr.concat(getDistance(R_INFRARED));
      break;

    case SERIAL_ULTRASONIC:  //'f'-通过串口读取超声波测距传感器值
      SERIAL_OUT.print(getUltrasonicDistance(ULTRASONIC_A_TRIG_PIN, ULTRASONIC_A_ECHO_PIN));
      SERIAL_OUT.print(" ");
      SERIAL_OUT.print(getUltrasonicDistance(ULTRASONIC_B_TRIG_PIN, ULTRASONIC_B_ECHO_PIN));
      SERIAL_OUT.print(" ");
      SERIAL_OUT.println(getUltrasonicDistance(ULTRASONIC_C_TRIG_PIN, ULTRASONIC_C_ECHO_PIN));
      break;

    case IIC_ULTRASONIC: //'k'-IIC方式读取超声波测距传感器数值
      sendI2CObj.ackMsgStr.concat(getUltrasonicDistance(ULTRASONIC_A_TRIG_PIN, ULTRASONIC_A_ECHO_PIN));
      sendI2CObj.ackMsgStr.concat(" ");
      sendI2CObj.ackMsgStr.concat(getUltrasonicDistance(ULTRASONIC_B_TRIG_PIN, ULTRASONIC_B_ECHO_PIN));
      sendI2CObj.ackMsgStr.concat(" ");
      sendI2CObj.ackMsgStr.concat(getUltrasonicDistance(ULTRASONIC_C_TRIG_PIN, ULTRASONIC_C_ECHO_PIN));
      break;

    case RESET_ENCODERS: //'r'-重置所有电机编码器计数为0
      resetEncodersCnt();
      resetPIDObjParam();
      SERIAL_OUT.println("OK");
      sendI2CObj.ackMsgStr.concat("OK");
      break;

    case MOTOR_CONTROL: //'m'-驱动电机转动,按照每个PID周期内的脉冲数来转动
      lastMotorCommand = millis();  /* Reset the auto stop timer */
      if (cmdDataObj.arg1 == 0 && cmdDataObj.arg2 == 0 && cmdDataObj.arg3 == 0)
      {
        setMoveStatus(STOP);
        resetPIDObjParam();
      }
      else //更改电机状态为正在转动
      {
        setMoveStatus(RUNNING);
        setPIDTargetTicks(cmdDataObj.arg1, cmdDataObj.arg2, cmdDataObj.arg3);
      }
      SERIAL_OUT.println("OK");
      sendI2CObj.ackMsgStr.concat("OK");
      break;

    case UPDATE_PID: //'u'-更新三个电机的PID参数,每个电机4个参数,共12个参数.
      memset(pid_args, 0, sizeof(pid_args));
      while ((str = strtok_r(p, ":", &p)) != '\0')
      {
        pid_args[i] = atoi(str);
        i++;
      }
      updatePIDParam(A_WHEEL, pid_args[0], pid_args[1], pid_args[2],  pid_args[3]);
      updatePIDParam(B_WHEEL, pid_args[4], pid_args[5], pid_args[6],  pid_args[7]);
      updatePIDParam(C_WHEEL, pid_args[8], pid_args[9], pid_args[10], pid_args[11]);
      SERIAL_OUT.println("OK");
      sendI2CObj.ackMsgStr.concat("OK");
      break;

    case BEEP_RING: //'p'-蜂鸣器提示音
      procBeepTip(cmdDataObj.arg1);
      SERIAL_OUT.println("OK");
      sendI2CObj.ackMsgStr.concat("OK");
      break;

    case GET_PID_TICKS: //'i'-得到在一个PID周期内的编码器的tick数
      SERIAL_OUT.print(getPidPrevFrameTicks(A_WHEEL));
      sendI2CObj.ackMsgStr.concat(getPidPrevFrameTicks(A_WHEEL));
      SERIAL_OUT.print(" ");
      sendI2CObj.ackMsgStr.concat(" ");
      SERIAL_OUT.print(getPidPrevFrameTicks(B_WHEEL));
      sendI2CObj.ackMsgStr.concat(getPidPrevFrameTicks(B_WHEEL));
      SERIAL_OUT.print(" ");
      sendI2CObj.ackMsgStr.concat(" ");
      SERIAL_OUT.println(getPidPrevFrameTicks(C_WHEEL));
      sendI2CObj.ackMsgStr.concat(getPidPrevFrameTicks(C_WHEEL));
      break;

    case GET_PIDOUT_PWM: //'o'-输出最终计算得到的各电机驱动PWM值
      SERIAL_OUT.print(getPidPWMOut(A_WHEEL));
      sendI2CObj.ackMsgStr.concat(getPidPWMOut(A_WHEEL));
      SERIAL_OUT.print(" ");
      sendI2CObj.ackMsgStr.concat(" ");
      SERIAL_OUT.print(getPidPWMOut(B_WHEEL));
      sendI2CObj.ackMsgStr.concat(getPidPWMOut(B_WHEEL));
      SERIAL_OUT.print(" ");
      sendI2CObj.ackMsgStr.concat(" ");
      SERIAL_OUT.println(getPidPWMOut(C_WHEEL));
      sendI2CObj.ackMsgStr.concat(getPidPWMOut(C_WHEEL));
      break;

    case GET_PERCENT: // 'g'-获取当前锂电池剩余电量百分比
      i = detectBatPercent();
      SERIAL_OUT.println(i);
      sendI2CObj.ackMsgStr.concat(i);
      break;

    case CODE_VERSION: // 'v'-获取当前固件代码的版本号
      SERIAL_OUT.println(VERSION);
      sendI2CObj.ackMsgStr.concat(VERSION);
      break;

    case SET_SERVO_POS:   // 's':设置舵机的转动角度
      myServos[cmdDataObj.arg1].setTargetPos(cmdDataObj.arg2, cmdDataObj.arg3);
      SERIAL_OUT.println("OK");
      sendI2CObj.ackMsgStr.concat("OK");
      break;

    case GET_SERVO_POS:   // 't':得到舵机的当前角度位置
      for (i = 0; i < SERVOS_CNT; i++)
      {
        SERIAL_OUT.print(myServos[i].getCurrentPos());
        sendI2CObj.ackMsgStr.concat(myServos[i].getCurrentPos());
        SERIAL_OUT.print(' ');
        sendI2CObj.ackMsgStr.concat(" ");
      }
      SERIAL_OUT.println("");
      break;

    case DEBUG_PID: // 'y':是否打开pid调试信息的命令
      setDebugPIDFlag(cmdDataObj.arg1);
      SERIAL_OUT.println("OK");
      sendI2CObj.ackMsgStr.concat("OK");
      break;
    
    case CONTROL_360SERVO:  // 'z':控制360°舵机转动
      if (cmdDataObj.arg2 == ANCLK_ROTATE)
      {
        myServos[cmdDataObj.arg1].setTargetPos(0, 0);
      }
      else if (cmdDataObj.arg2 == STOP_ROTATE)
      {
        myServos[cmdDataObj.arg1].setTargetPos(90, 0);
      }
      else if (cmdDataObj.arg2 == CLOCK_ROTATE)
      {
        myServos[cmdDataObj.arg1].setTargetPos(180, 0);
      }
      SERIAL_OUT.println("Done");
      break;

    default: //接收到错误的控制命令,该命令未在commands.h中定义
      SERIAL_OUT.println("Invalid Command");
      sendI2CObj.ackMsgStr.concat("Invalid Command");
      break;
  }
  sendI2CObj.ackMsgStr.concat("\r"); //在IIC数据末尾添加\r字符作为数据结束的标志
}

/**************************************************************
  Description:程序启动后,最开始运行的配置代码,该初始化函数只执行一次.
    在这里配置串口,IIC通信,各传感器,电机,编码器,舵机等初始化信息.
 *************************************************************/
void setup()
{
  SERIAL_OUT.begin(BAUDRATE); //配置串口通信波特率
  cmdDataObj.resetCmdParam();

  initIIC();
  initSensors();
  initEncoders();
  initMotorsPinMode();
  initVoltageDetect();
  resetPIDObjParam();

  //when power on init all servos position
  for (byte i = 0; i < SERVOS_CNT; i++)
  {
    myServos[i].initServo(myServoPins[i], servoInitPosition[i], 0);
  }
}

/*********************************************************
  Description:初始化arduino的IIC通信,配置好IIC地址,发送数据和
    接收数据函数.
**********************************************************/
void initIIC(void)
{
  Wire.begin(IIC_ADDR); //配置设备的IIC地址
  Wire.onReceive(recIICData);  //配置IIC总线收到数据后的回调函数
  Wire.onRequest(sendIICData); //配置IIC总线上收到需反馈数据时的回调函数
}

/**********************************************************
  Description:向IIC设备master发送数据.
**********************************************************/
void sendIICData()
{
  Wire.write(sendI2CObj.ackMsgStr.c_str());
}

/*******************************************************************
  Description:处理串口或IIC总线上获取到的字符数据,并从中解析出命令字符和有
  效参数. 所有的控制命令字符串结尾都需要以'\r'结尾,根据该字符  判断是否收到
  一条完整命令.当收到该结束字符后,就开始执行接收到的指令.
*******************************************************************/
void procRecChar(char tmp_chr)
{
  if (tmp_chr == 13)  //判断是否收到了命令最后的结束字符'\r'
  {
    runCommand();
    cmdDataObj.resetCmdParam(); //在执行完命令后,可以将串口接收到的缓冲区参数重置清零
  }
  else if (tmp_chr == ' ') //使用空格来区分命令字符后面的各参数,遇到空格说明后面有新参数
  {
    cmdDataObj.argCnt++;
    cmdDataObj.argIndex = 0;
  }
  else //process single-letter command
  {
    if (cmdDataObj.argCnt == 0) //第一个字符都是命令字符
    {
      cmdDataObj.cmd_chr = tmp_chr;
    }
    else if (cmdDataObj.argCnt == 1) //开始保存第一个参数中所有字符到argv1缓冲区
    {
      cmdDataObj.argv1[cmdDataObj.argIndex] = tmp_chr;
      cmdDataObj.argIndex++;
    }
    else if (cmdDataObj.argCnt == 2)  //开始保存第二个参数中所有字符到argv2缓冲区
    {
      cmdDataObj.argv2[cmdDataObj.argIndex] = tmp_chr;
      cmdDataObj.argIndex++;
    }
    else if (cmdDataObj.argCnt == 3) //开始保存第三个参数中所有字符到argv3缓冲区
    {
      cmdDataObj.argv3[cmdDataObj.argIndex] = tmp_chr;
      cmdDataObj.argIndex++;
    }
  }
}

/************************************************************
   Description:用来从IIC总线上接收数据并进行处理,这里处理数据仍然
     调用的是procRecChar()函数,跟串口数据解析函数是一样的.因为无论
     串口还是IIC发送过来的控制命令格式都必须是一样的.
 ***********************************************************/
void recIICData(int byteCount)
{
  while (Wire.available())//判断IIC总线上是否还有数据,若有数据则不断的处理
  {
    char tmp_chr = Wire.read();  //从IIC总线上读取一个字符进行处理
    //SERIAL_OUT.println(char(tmp_chr));
    procRecChar(tmp_chr); //处理该字符
  }
}

/**********************************************************************
  Description:该loop函数会不断的循环执行,除非arduino设备断电,否则该loop程序
    就会一直运行.在此函数内可以增加需要不断循环处理检测的函数.
 *********************************************************************/
void loop()
{
  while (SERIAL_OUT.available()) //判断串口中是否收到控制命令
  {
    char tmp_chr = SERIAL_OUT.read();  //读取串口缓冲区的一个字符
    procRecChar(tmp_chr); //处理该字符
  }

  //判断是否到了下一次的PID循环周期,这里是将电机的驱动控制给分割在每个PID周期内进行
  if (millis() > nextPIDLoopCheck)
  {
    updateMotorPWM();
    nextPIDLoopCheck += PID_INTERVAL;
  }

  //判断接收移动命令是否超时,超时后若电机还在转动将立刻停止
  if ((getMoveStatus()) && ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL))
  {
    setMoveStatus(STOP);  //设置电机停止转动状态
  }

  //检查每个舵机是否需要转动
  for (byte i = 0; i < SERVOS_CNT; i++)
  {
    myServos[i].moveServo();
  }

  //当未收到控制电机转动命令的超时时间大于指定时间,使驱动芯片进入节能模式
  if ((!getMoveStatus()) && ((millis() - lastMotorCommand) > ENERGE_MODE_TIME))
  {
    setMotorStandby(ENABLE); //驱动芯片进入节能模式
    resetEncodersCnt(); //清除编码器计数为0
  }
}
