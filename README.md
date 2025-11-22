// Written by Joakim Drageset, Ingrid Sand & Thomas Nagelgaard - MAS246
# Group 4 - Elevator Project
This Git includes most of the work we did to learn, develop and implement C++ code from the beginning of the project.

Initially, it is worth noting that we have modified the code in the library <ArduinoSTL.h> based on the advice of earlier students. The modification is as follows: 

In the file libraries\ArduinoSTL\src\new_handler.cpp on line 22, we commented out 
const std::nothrow_t std::nothrow = { };
This fix should be included in the libraries in the Git itself, but if it is not the code may not run as intended.

The other libraries used are TimerOne and Array, these should both also be in the Git, but may need to be installed seperately.

## To Run the Code
With this fix in place, the code will run in its entirety when the sketch elevator_grp4\src\main\main.ino is opened and built to an Arduino Mega 2560. This requires that the Arduino is connected to the host computer in a way that allows for serial communication. Additionally, this should be done from Arduino IDE for consistency. 


## Other contents
The folder contains the libraries we used in the elevator_grp4\libraries folder, all of the research and development code in elevator_grp4\Test code, the experiment video and the written report. All of the code written to save and process the data in Matlab is also in Test code, along with the data from the experiment itself, named video_data.mat. 
