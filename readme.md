# RGB BOTS template

This project provides a template for VEX V5 competition robotics, featuring a modular architecture and a library for advanced control.

## Installation

1.  **Download:** Download the project from [GitHub](https://github.com/ericjiangxiao/2026-base) as a ZIP file.
2.  **Unzip:** Extract the downloaded file to a local folder.
3.  **Open in VS Code:** In Visual Studio Code, go to `File > Open Folder` and select the extracted folder.

## Project Structure

The project is organized into the following directories:

*   `src/`: Main source code
    *   `main.cpp`: Entry point, competition control, and button mappings.
    *   `chassis.cpp`: Drivetrain configuration and control.
    *   `autons.cpp`: Autonomous routines.
    *   `robot-config.cpp`: Configuration for other motors and sensors.
    *   `rgb-template/`: Library code.
*   `include/`: Header files
*   `vex/`: VEX-specific makefiles.

## Configuration

### Drivetrain (`chassis.cpp`)

*   **Motors and Sensors:** Define your drivetrain motors and inertial sensor, including ports, gear ratios, and motor direction.
*   **Drive Mode:** Set `DRIVE_TANK_MODE` to `true` for tank control or `false` for arcade control.
*   **Driver Control Constants:**
    *   `TURN_FACTOR`: Adjusts the turning speed.
    *   `STEER_BIAS`: Controls the curve of the robot when both joysticks are used.
*   **PID Constants:** Tune the PID constants for driving and turning in the `reset_chassis()` function.

### Other Subsystems (`robot-config.cpp`)

Configure motors and sensors for other subsystems like arms, claws, or intakes in this file.

## Drive APIs (`drive.h`)

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

## Autonomous Routines (`autons.cpp`)

*   **Create Functions:** Write your autonomous routines as separate functions.
*   **Auton Menu:** Add the names of your autonomous functions to the `auton_menu_text` array to make them selectable on the brain's screen.
*   **Select and Run:** Use the `run_auton_item()` function to execute the selected autonomous routine.

## Driver Control (`main.cpp`)

*   **Button Bindings:** In the `main()` function, map controller buttons to specific actions or functions.
*   **Context-Aware Button Actions:** Create functions that allow a single button to perform different actions based on the robot's state. For example, a button could perform one action normally, but a different action if another button is held down simultaneously or if the robot is in a specific mode (like `auton_test_mode`).

## PID Control (`PID.h`)

The library provides two PID controller constructors:

1.  `PID(float error, float kp, float kd)`: A simple PID controller for applications like heading correction, where integral control is not necessary.
2.  `PID(float error, float kp, float ki, float kd, float starti, float settle_error, float settle_time, float timeout)`: A full PID controller for more complex systems like arms or lifts, where precise control is required.

## Utility Functions (`util.h`)

The `util.h` file contains several helper functions, including:

*   `reduce_0_to_360()`: Normalizes an angle to the range [0, 360).
*   `reduce_negative_180_to_180()`: Normalizes an angle to the range [-180, 180).
*   `to_deg()`: Converts radians to degrees.
*   `threshold()`: Constrains a value within a specified range.
*   `to_volt()`: Converts a percentage to voltage.
*   `deadband()`: Creates a dead zone for joystick input.
*   `check_motors()`: Checks for disconnected or overheated motors.
