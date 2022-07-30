/***************************************************************************
  Copyright: 2016-2021 ROS小课堂 www.corvin.cn
  Description:定义一些通用的信息,这样可以在多个源码中共享.
  Author: corvin
  History:
    20200328:init this file.
    20200603:增加二自由度机械臂上下停止运动的宏定义.
    20200912:将控制360度舵机转动的宏定义修改为更易读的.
****************************************************************************/
#ifndef _COMMON_H_
#define _COMMON_H_

#define   A_WHEEL     1
#define   B_WHEEL     2
#define   C_WHEEL     3

//控制360度舵机转动时需要的
#define   CLOCK_ROTATE  1  //顺时针转动舵机
#define   STOP_ROTATE   2  //停止舵机转动
#define   ANCLK_ROTATE  3  //逆时针转动舵机

//表明是否有电机在转动状态
#define  RUNNING     true
#define  STOP        false

//表明电机的转动方向
#define  FORWARDS    false
#define  BACKWARDS   true

//表明驱动芯片是否在待机状态
#define  ENABLE      true
#define  DISABLE     false

#endif
