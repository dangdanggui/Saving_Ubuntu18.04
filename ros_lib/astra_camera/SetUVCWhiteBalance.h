#ifndef _ROS_SERVICE_SetUVCWhiteBalance_h
#define _ROS_SERVICE_SetUVCWhiteBalance_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace astra_camera
{

static const char SETUVCWHITEBALANCE[] = "astra_camera/SetUVCWhiteBalance";

  class SetUVCWhiteBalanceRequest : public ros::Msg
  {
    public:
      typedef int32_t _white_balance_type;
      _white_balance_type white_balance;

    SetUVCWhiteBalanceRequest():
      white_balance(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_white_balance;
      u_white_balance.real = this->white_balance;
      *(outbuffer + offset + 0) = (u_white_balance.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_white_balance.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_white_balance.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_white_balance.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->white_balance);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      union {
        int32_t real;
        uint32_t base;
      } u_white_balance;
      u_white_balance.base = 0;
      u_white_balance.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_white_balance.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_white_balance.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_white_balance.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->white_balance = u_white_balance.real;
      offset += sizeof(this->white_balance);
     return offset;
    }

    virtual const char * getType() override { return SETUVCWHITEBALANCE; };
    virtual const char * getMD5() override { return "fc2837b62008590625057801873acc63"; };

  };

  class SetUVCWhiteBalanceResponse : public ros::Msg
  {
    public:

    SetUVCWhiteBalanceResponse()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
     return offset;
    }

    virtual const char * getType() override { return SETUVCWHITEBALANCE; };
    virtual const char * getMD5() override { return "d41d8cd98f00b204e9800998ecf8427e"; };

  };

  class SetUVCWhiteBalance {
    public:
    typedef SetUVCWhiteBalanceRequest Request;
    typedef SetUVCWhiteBalanceResponse Response;
  };

}
#endif
