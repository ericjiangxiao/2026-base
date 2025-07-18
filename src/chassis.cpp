// vex.h includes all of the headers for the VEX V5 library
#include "vex.h"

// All vex classes and functions are in the vex namespace
using namespace vex;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// ----------------------------------------------------------------------------
//                                Drivetrain
// ----------------------------------------------------------------------------
// motor definitions
// The first parameter is the port number.
// The second parameter is the gear ratio.
// The third parameter is whether the motor is reversed or not.
motor leftMotor1 = motor(PORT1, ratio6_1, true);
motor leftMotor2 = motor(PORT2, ratio6_1, true);
motor leftMotor3 = motor(PORT3, ratio18_1, false);

motor rightMotor1 = motor(PORT4, ratio6_1, false);
motor rightMotor2 = motor(PORT5, ratio6_1, false);
motor rightMotor3 = motor(PORT6, ratio18_1, true);

// inertial sensor for turning and heading
inertial inertial1 = inertial(PORT9);

// if you want the drive mode to be changeable in you code, remove the "const"
// true for tank drive, false for arcade drive
const bool DRIVE_TANK_MODE = false;

// constant definitions for driver control
// TURN_FACTOR reduces the sensitivity of the turn stick
const float TURN_FACTOR = 0.85;
// STEER_BIAS allows for a non-proportional steering response
const float STEER_BIAS = 0.5;

// total number of motors, including drivetrain
const int NUMBER_OF_MOTORS = 8;

// end game reminder will start at 85 seconds into the match
const int END_GAME_SECONDS = 85;


// The Drive class is a wrapper for the VEX motor_group class.
// It provides a more intuitive interface for controlling a drivetrain.
Drive chassis(
  //Left Motors:
  motor_group(leftMotor1, leftMotor2, leftMotor3),
  //Right Motors:
  motor_group(rightMotor1, rightMotor2, rightMotor3),
  //Inertial Sensor:
  inertial1,
  //wheel diameter:
  2.75,
  //Gear ratio of motor to wheel: if your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
  0.75
);

// Resets the chassis to a known state.
// This function should be called before any autonomous routines.
void reset_chassis() {
  // Resets the heading of the inertial sensor to the current heading.
  chassis.set_heading(inertial1.heading());
  // Stops the chassis motors.
  chassis.stop(coast);

  // Sets the drive constants for the chassis.
  // These constants are used to control the acceleration and deceleration of the chassis.
  chassis.set_drive_constants(10, 1.5, 0, 10, 0);
  // Sets the heading constants for the chassis.
  // These constants are used to control the turning of the chassis.
  chassis.set_heading_constants(6, .4, 1);
  // Sets the turn constants for the chassis.
  // These constants are used to control the turning of the chassis.
  chassis.set_turn_constants(10, 0.2, .015, 1.5, 7.5);

  // Sets the exit conditions for the drive functions.
  // These conditions are used to determine when the drive function should exit.
  chassis.set_drive_exit_conditions(1, 200, 2000);
  // Sets the exit conditions for the turn functions.
  // These conditions are used to determine when the turn function should exit.
  chassis.set_turn_exit_conditions(1.5, 200, 1500);
}

// This function is a thread that runs in the background to remind the driver of the end game.
int endgame_timer() {
  // Clears the brain timer.
  Brain.Timer.clear();
  // Waits until the end game starts.
  while (Brain.Timer.time(sec) < END_GAME_SECONDS) {
    wait(200, msec);
  }
  // Prints a message to the controller screen.
  controller(primary).Screen.print("end game ...");
  // Rumbles the controller.
  controller(primary).rumble("-");
  // Checks the motors every 60 seconds.
  while(true)
  {
    wait(60, seconds);
    check_motors(NUMBER_OF_MOTORS);
  }
  return 1;
}

// This variable is used to exit the autonomous menu.
bool exit_auton_menu = false;

// This is the user control function.
// It is called when the driver control period starts.
void usercontrol(void) {
  // Exits the autonomous menu.
  exit_auton_menu = true;
  // Resets the chassis.
  reset_chassis();
  // Starts the end game timer thread.
  thread endgame_timer_thread = thread(endgame_timer);

  // This loop runs forever, controlling the robot during the driver control period.
  while (1) {
    // This is the tank drive code.
    if (DRIVE_TANK_MODE) chassis.control_tank(controller(primary).Axis3.position(), controller(primary).Axis2.position());
    // This is the arcade drive code.
    else {
      // This code reduces the sensitivity of the turn stick when it is near the center.
      if (abs(controller(primary).Axis4.position()) < 100)
        chassis.control_arcade(controller(primary).Axis2.position(), controller(primary).Axis4.position() * TURN_FACTOR, STEER_BIAS);
      // This code uses the full sensitivity of the turn stick when it is far from the center.
      else
        chassis.control_arcade(controller(primary).Axis2.position(), controller(primary).Axis4.position(), STEER_BIAS);
    }
    // This wait prevents the loop from using too much CPU time.
    wait(20, msec); 
  }
}

// This is the autonomous function.
// It is called when the autonomous period starts.
void autonomous(void) {
  // Exits the autonomous menu.
  exit_auton_menu = true;
  // Runs the selected autonomous routine.
  run_auton_item();
}

// This function prints the selected autonomous routine to the brain and controller screens.
void print_menu_item(char const * txt[]) {
  // If the custom test is selected, print "custom test" to the controller screen.
  if (current_auton_selection < 0) {
    controller(primary).Screen.print("custom test");
    return;
  }    
  // Clears the brain screen.
  Brain.Screen.clearScreen();
  // Sets the cursor to the third row, first column.
  Brain.Screen.setCursor(3, 1);
  // Prints the selected autonomous routine to the brain screen.
  Brain.Screen.print("%s", txt[current_auton_selection]);
  // Prints the selected autonomous routine to the controller screen.
  controller(primary).Screen.print("%s", txt[current_auton_selection]);
}

// This function displays the autonomous menu on the brain screen.
void print_menu(char const * txt[]) {
  // Sets the font to mono30.
  Brain.Screen.setFont(mono30);
  // Prints the selected autonomous routine.
  print_menu_item(txt);

  // This loop runs until the autonomous menu is exited.
  while (!exit_auton_menu) {
    // If the brain screen is pressed, cycle through the autonomous routines.
    if (Brain.Screen.pressing()) {
      // Waits until the finger is lifted up from the screen.
      while (Brain.Screen.pressing()) {
        wait(20, msec);
      }
      // Cycles through the autonomous routines.
      current_auton_selection = (current_auton_selection + 1) % auton_num;
      // Prints the selected autonomous routine.
      print_menu_item(txt);
      // Rumbles the controller.
      controller(primary).rumble(".");
    }
    // This wait prevents the loop from using too much CPU time.
    wait(50, msec);
  }
  // Sets the font to mono20.
  Brain.Screen.setFont(mono20);
}

// This function sets up the gyro.
void setup_gyro() {
  // Waits until the inertial sensor is calibrated.
  while (inertial1.isCalibrating()) {
    wait(25, msec);
  }
  // Rumbles the controller to indicate that the gyro is calibrated.
  controller(primary).rumble("-");
  // If the inertial sensor is not installed, print an error message to the controller screen.
  if (!inertial1.installed()) {
    controller(primary).Screen.print("inertial sensor failure");
    controller(primary).rumble("----");
  }
}

// This function is called before the autonomous period starts.
void pre_auton() {
  // Sets up the gyro.
  setup_gyro();
  // Checks the motors.
  check_motors(NUMBER_OF_MOTORS);
  // Resets the chassis.
  reset_chassis();
  // Shows the autonomous menu.
  show_auton_menu();
}