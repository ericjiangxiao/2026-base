#include "vex.h"
using namespace vex;

motor intake = motor(PORT8, ratio18_1, true);
motor liftMotor = motor(PORT7, ratio36_1, false);
rotation liftRotation = rotation(PORT10, true);
distance arm_distance = distance(PORT11);

bool lift_is_up = true;
void detect_arm(){
  if (arm_distance.objectDistance(inches) < 1 && lift_is_up){
    lift_arm();
  }
}

void rotate_lift_to(float angle)
{
  PID liftPID(angle - liftRotation.position(deg), 0.4, 0.2, 0, 15, 3, 200, 1000);
  while (liftPID.is_done() == false) {
    float error = (angle - liftRotation.position(deg));
    float output = liftPID.compute(error);
    output = threshold(output, -10, 10);
    liftMotor.spin(forward, output, volt);
    wait(10, msec);
  }
  liftMotor.stop(hold);
}

void lift_arm(){
  rotate_lift_to(240);
}

void release_arm()
{
  rotate_lift_to(40);
}

void toggle_lift() {
  if (lift_is_up == true) {
   lift_arm();
   lift_is_up = false;
  }
  else {
   release_arm();
   lift_is_up = true;
  }
  wait (200, msec);
}