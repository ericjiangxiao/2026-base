# RGB BOTS template

This project provides a lightweight c++ template for VEX V5 robotics competition, featuring a modular architecture and a library for autonomous driving control.

## Installation
*   **Prerequisites:** Before you begin, make sure:
    - You have [VScode with VEX extension](https://kb.vex.com/hc/en-us/categories/360002333191-V5?sc=vs-code-extension) installed.
    - (Optional) You have the [GitHub](https://github.com/) extension in VS Code installed.
    - (Optional) If you are using Mac with Apple silicon: [install Rosetta](https://support.apple.com/en-us/102527) to simulate intel x86 chip if errors show up with VEX extension.
*   **Download and Open the Project:** 
    *   Download [the source code](https://github.com/ericjiangxiao/2026-base/archive/refs/heads/main.zip) as a ZIP file. Unzip and extract the downloaded file to a local folder.
    *   In VScode, go to `File > Open Folder` and select the extracted folder.
    *  (Optional) Or follow the instructions to [pull the repositry](doc/how_to_clone_and_pull_with_vscode.md) using VScode.
*   **(Optional) If Using Mac with Apple Silicon**: 
    *   Copy `.vscode` folder from a new project to overwrite the settings.

## Project Structure

The project is organized into the following directories:

*   `src/`: Main source code
    *   `main.cpp`: Entry point, competition control, and button mappings.
    *   `chassis.cpp`: Drivetrain configuration and control.
    *   `robot-config.cpp`: Configuration for other motors and sensors.
    *   `autons.cpp`: Autonomous routines.
    *   `rgb-template/`: Library code.
*   `include/`: Header files
*   `vex/`: VEX-specific makefiles.
*   `doc/`: Additional documentation.

## Configuration

### Drivetrain ([chassis.cpp](src/chassis.cpp))

*   **Motors and Sensors:** Define your drivetrain motors and inertial sensor, including ports, gear ratios, and motor direction. By default, the port numbers are 1, 2, 3 for the left side, 4, 5, 6 for the right side and 10 for the inertial sensor. 
*   **Drive Mode:** Set `DRIVE_TANK_MODE` to `true` for tank control or `false` for arcade control.
*   **Driver Control Constants:**
    *   `TURN_FACTOR`: Slows down the turning speed.
    *   `STEER_BIAS`: Controls the curve of the robot when both joysticks are used.
*   **(Optional) PID Constants:** If needed, adjust the PID constants for driving and turning in the `reset_chassis()` function. 

### Other Subsystems ([robot-config.cpp](src/robot-config.cpp))

*   **Motor Configuration:** Define your motors and sensors for other subsystems like intake or lift.
*   **Number of Motors:** Set `NUMBER_OF_MOTORS` to total number of motors to allow the program to automatically check for disconnected or overheated motors.
*   **Help functions:** Write helper functions to control the subsystems and declare those functions in [robot-config.h](include/robot-config.h) so that your other cpp files can call them.

### Driver Control ([main.cpp](src/main.cpp))

*   **Button Functions:** Create button functions by using the helper functions in  [robot-config.cpp](src/robot-config.cpp)
*   **Button Bindings:** After the bottom of the file, in the `main()` function, map controller buttons to the functions.
*   **(Optional) Context-Aware Button Actions:** Create functions that allow a single button to perform different actions based on the robot's state. For example, a button could perform one action normally, but a different action if another button is held down simultaneously or if the robot is in a specific mode (like `auton_test_mode`).

### Autonomous Routines ([autons.cpp](src/autons.cpp))

*   **Auton Functions:** Write your autonomous routines as separate functions.
*   **Auton Mappings:** Map auton functions to menu items in the `run_auton_item()` function 
*   **Auton Menu Text:** Add the names of your autonomous functions to the `auton_menu_text` array to make them shown on the brain's screen.
*   **(Optional) Auton Parameters:** Create array variables for the auton functions at the top of the file for easy tuning the auton parameters during tournaments.

## Run sample program
- Build and download the program to the brain and using game controller to select the program slot and run the program.
- For arcade driving, use the `left stick` to turn and `right stick` to drive forward and backward. After you release the stick, the controller screen shows the current heading. If you press the `B button`, the controller shows the distance drived and reset the motor encoder. For sample button mappings and actions, see [button controls](doc/button_control.md).
- (Optional) Test autons using the game controller with the following steps:
    - Press the controller's `Right button` within 5 seconds of program startup to enter test mode.
    - When in test mode, press the `Right button` to cycle through the list of autonomous routines on the controller screen.
    - Press the `A button` to run the routine shown on the screen. Hold the button to continue pass stops.

&nbsp;
---
# More Information of the [Library](src/rgb-template/)
## Drive APIs ([drive.h](include/rgb-template/drive.h))

The `Drive` class provides a set of APIs to control the robot's movement.

### `drive_with_voltage(float leftVoltage, float rightVoltage)`

This is the most basic drive function. It sets the voltage for the left and right sides of the drivetrain directly.

**Example:**

```cpp
// Drive forward at half speed for 1 second
chassis.drive_with_voltage(6, 6);
wait(1000, msec);
chassis.stop(coast);
```

### `turn_to_heading(...)`

This API turns the robot to a specific heading using a PID controller.

**Variations:**

1.  `turn_to_heading(float heading)`: Turns to the specified heading with default parameters.
2.  `turn_to_heading(float heading, float turn_max_voltage)`: Limits the maximum voltage for a slower, more accurate turn.
3.  `turn_to_heading(float heading, float turn_max_voltage, bool nonstop, float settle_error = 5, float settle_time = 50)`: Advanced version for faster turns with optional non-blocking execution.

**Examples:**

```cpp
// Simple turn to 90 degrees
chassis.turn_to_heading(90);

// Slower, more accurate turn
chassis.turn_to_heading(90, 6);

// Fast turn without waiting for completion
chassis.turn_to_heading(90, 12, true);
```

### `drive_distance(...)`

This API drives the robot a specific distance using a PID controller, with options for maintaining a heading.

**Variations:**

1.  `drive_distance(float distance)`: Drives the specified distance with default parameters.
2.  `drive_distance(float distance, float drive_max_voltage)`: Limits the maximum voltage for driving.
3.  `drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage)`: Drives while maintaining a specific heading (curved drive).
4.  `drive_distance(float distance, float drive_max_voltage, float heading, float heading_max_voltage, bool nonstop, float drive_settle_error=2, float drive_settle_time=50)`: Advanced version for curved drives with optional non-blocking execution.

**Examples:**

```cpp
// Drive forward 24 inches
chassis.drive_distance(24);

// Drive forward 24 inches with a maximum voltage of 8
chassis.drive_distance(24, 8);

// Drive forward 24 inches while turning to a heading of 45 degrees
chassis.drive_distance(24, 10, 45, 6);

// Curved drive without waiting for completion
chassis.drive_distance(24, 10, 45, 6, true);
```

## PID Control ([PID.h](include/rgb-template/PID.h))

This class provides two PID controller constructors:

1.  `PID(float error, float kp, float kd)`: A simple PID controller for applications like heading correction, where integral control is not necessary.
2.  `PID(float error, float kp, float ki, float kd, float starti, float settle_error, float settle_time, float timeout)`: A full PID controller for more complex systems like arms or lifts, where precise control is required.
