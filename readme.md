RGB BOTS TEMPLATE IS INSPIRED BY JARS-TEMPLATE

# Install
## Download the source code
go to [github](https://github.com/jun-xiao-mainlinestem/2026base), download the source code as zip file.

![source](./images/install.JPG)

## Open the project in VS code
- unzip the downloaded file to a folder on local drive
- in vscdoe, select menu 'file' -> 'open folder' 

# Edit the code
## Library Code
Under the `rgb-template` folder, there are three files, PID, drive, and utility.The most one is the `drive.cpp` which controls the chassis. 

### util.cpp
Within the `util.cpp`, there are some miscellaneous functions for convenience, used within the library.

### PID.cpp
There are two interfaces to create a PID object. The first is the simpler PID control which uses `error`, `kp`, and `kd`. This PID is used to correct headings while driving because it does not use `ki`. The more complex PID is the that uses all the parameters, `error`, `kp`, `ki`, `kd`, `starti`, `settle_error`,  `settle_time`, and `timeout`. This PID is used for all situations not just the drivetrain, for example an arm or claw. If you don't want to use a coefficent, then you can just set the value as "0". The `ki` parameter is what gives the PID the extra push it needs to reach the target and the `starti` is the parameter that tells the PID when to start intagrating errors. For example `starti` is when the error is less the `settle_error` and then it starts intagrating.

### drive.cpp
There are three types of drive API (Application programming interface): `drive_with_voltage`, `turn_to_heading`, and `drive_distance`. `drive_with_voltage` is a time-based drive method where the left and right sides of the drivetrain are controled separately. To make somthing drive you must set the power level of both sides, then wait for however long, and then stop it by setting both sides' power levels to zero. The second drive API has three diffent variations and is the `turn_to_heading` API, where it turns to the heading acording to the bird-eye view of the field using PID. There are two other variations of the simplest version of this API which add more variables. The first variation adds a `turn_max_voltage`, which limits the max voltage and therfore makes it slower, but far more accurate. However the second variation does the exact opposite, making it turn faster, at the cost of reduced accuraccy. It does this by also adding three more variables, `nonstop`, `settle_error`, and `settle_time`. The `nonstop` varible makes it so that it does not wait for the turn to fully complete before the next step. The `settle_error` parameter adds wiggle room for the PID turn and the `settle_time` helps the PID complete faster. The final API is `drive_distance`, which drives the specified distance using PID and has four different variations. Positive is forward, negative is backward. The first and second variations are simple, they drive the specified distance and drive a specified distance with a `drive_max_voltage` respectively. They next two variations are more complex as they are curve functions, they drive and turn at the same time. The third variation introduces a `heading` and a `heading_max_voltage` while the fourth variation adds a `nonstop`, `drive_settle_error`, and `drive_settle_time`, which is similar to the `turn_to_heading` API.

## Sample Project
This sample project showcases how to use the RGB-template to program your robot.

### chassis.cpp
In this file, you define all the drivetrain motors as well as the inertial sensor. For example the port numbers, gear ratio, etc. The constants of the drive PID is set in the function `reset_chassis`. You can change any parameter.

Also in this file, you can define the drive mode by setting the constant boolean `DRIVE_TANK_MODE` to true or false. The constants of `TURN_FACTOR` and `STEER_BIAS` determine how fast you turn with the turn joystick only, and how much you curve when both joysticks are push simultaneously, respectively. The constant `NUMBER_OF_MOTORS` checks if the specified number of motors are connected, and if lower number is reported, then it tells you that one motor must be disconnected. All other function are internal, which means that they are only used in `chassis.cpp`, except for the `pre-auton` function which must be called at the beginning of you `main` function.

### robot-config.cpp
In this file, you configure all your other motors and sensors for other sub-systems. There is already some sample code to demonstrate how to PID control to lift the arm and such.

### autons.cpp
In this file, you write your own auton functions with drivetrain (chassis) APIs:
```cpp
chassis.turn_to_heading(90);
chassis.drive_distance(24);
chassis.drive_with_voltage(6, 6);
chassis.stop(coast);
```
You can also configure the auton menus and can add test functions.

### main.cpp
In this file, you write additional functions for game plays and change the game controler buttons.

In the `main` function, you need to link all the functions to the corresponding the button controls, BEFORE the `pre_auton` function. Add the initialization of other sub-systems after `pre_auton`. There is some sample code to demonstrate writing a function for a button. The exampels show how to dual purpose your functions depending on the context.