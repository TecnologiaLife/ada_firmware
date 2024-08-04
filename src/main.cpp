#include <Arduino.h>
#include <FaceAnimation.hpp>




Face::FaceAnimation robot_face;




void setup()
{
  robot_face.begin();

  delay(1000);
}


void loop()
{
  uint32_t now_ms = millis();
  static uint32_t prev_ms = 0;

  robot_face.update();

  if((now_ms - prev_ms) > 2000)
  {
    Face::FacialExpression now_fe = robot_face.getFacialExpression();

    if(now_fe == Face::FacialExpression::None)
    {
      robot_face.setFacialExpression(Face::FacialExpression::Normal);
    }
    else if(now_fe == Face::FacialExpression::Normal)
    {
      robot_face.setFacialExpression(Face::FacialExpression::Angry);
    }
    else if(now_fe == Face::FacialExpression::Angry)
    {
      robot_face.setFacialExpression(Face::FacialExpression::Sad);
    }
    else if(now_fe == Face::FacialExpression::Sad)
    {
      robot_face.setFacialExpression(Face::FacialExpression::Happy);
    }
    else if(now_fe == Face::FacialExpression::Happy)
    {
      robot_face.setFacialExpression(Face::FacialExpression::Normal);
    }

    prev_ms = now_ms;
  }
}