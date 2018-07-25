# Camscii
OpenCV utility that converts your camera feed to ascii characters in real-time, with recording capabilities.

This applicaton assumes a unix-like environment, but could be adapted with little effort to compile and run on windows. It has been tested only on OSx.

![alt text](https://raw.githubusercontent.com/tj10200/Camscii/master/myvid.gif)

# Prerequisits
OpenCV
CMake
boost_program_options

#Building
1 - cd into directory

`cd ~/Camscii`

2 - Make a build directory and cd into it

`mkdir ~/Camscii/build`
`cd ~/Camscii/build`

3 - Run CMake

`cmake ../`

4 - Make

`make`

5 - run Executable

`cd ../; ./Camscii`
