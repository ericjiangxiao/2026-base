// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// A global instance of competition
// This object is used to register callbacks for the autonomous and driver control periods.
competition Competition;

// ----------------------------------------------------------------------------
//                                Controller Callbacks
// ----------------------------------------------------------------------------

// This function is called when the L1 button is pressed.
// It spins the intake motor forward while the button is held down.
void buttonL1_action() {
  intake.spin(forward, 12, volt);
  while(controller(primary).ButtonL1.pressing()) {
    wait (20, msec);
  }
  intake.stop(coast);
}

// This function is called when the R2 button is pressed.
// It holds the drivetrain in place until the button is released.
void buttonR2_action() {
  chassis.stop(hold);
  controller(primary).rumble(".");
  waitUntil(!controller(primary).ButtonR2.pressing());
  chassis.stop(coast);
}

// This function is called when the Right button is pressed.
void buttonRight_action()
{
  // activate test mode if the button is pressed immediately after running the program
  if ((Brain.Timer.time(sec) < 5) && !auton_test_mode) {
    controller(primary).rumble("-");
    auton_test_mode = true;
    return;
  }
  // if in test mode, scroll through the auton menu
  if (auton_test_mode)
  {
    current_auton_selection = (current_auton_selection + 1) % auton_num;
    show_auton_menu();
    return;
  }

  // otherwise run macro function 
  chassis.driver_control_disabled = true;
  // insert code below

  chassis.driver_control_disabled = false;
}

// This function is called when the B button is pressed.
void buttonB_action()
{
  // toggle team color if the button is pressed when intaking is running
  if (intake.isSpinning()) {
    controller(primary).rumble("-");
    is_red_team = !is_red_team;
    if (is_red_team)  controller(primary).Screen.print("red      ");
    else  controller(primary).Screen.print("blue      ");
    return;
  }

  // otherwise run macro function
  chassis.driver_control_disabled = true;
  // insert code below

  chassis.driver_control_disabled = false;
}

// This function is called when the A button is pressed.
void buttonA_action()
{
  // run auton test if in test mode
  if (auton_test_mode)
  {
    run_auton_test();
  }
}

// ----------------------------------------------------------------------------
//                                Main
// ----------------------------------------------------------------------------
//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Register the autonomous and driver control functions.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Register the controller button callbacks.
  controller(primary).ButtonRight.pressed(buttonRight_action);
  controller(primary).ButtonA.pressed(buttonA_action);
  controller(primary).ButtonB.pressed(buttonB_action);

  controller(primary).ButtonR2.pressed(buttonR2_action);
  controller(primary).ButtonL1.pressed(buttonL1_action);
  controller(primary).ButtonR1.pressed(toggle_lift);

  // Run the pre-autonomous function.
  pre_auton();

  // additional setup for other subsystems of the robot
  liftRotation.setPosition(liftRotation.angle(deg), degrees);
  arm_distance.changed(detect_arm);

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
