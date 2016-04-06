# Automatic Orchestra

## Library for Arduino

This library contains the source code for the sound installation *Automatic Orchestra* which was first presented at the [Resonate](http://resonate.io/) festival in 2015. Originally conceived as a collaboration between the [University of the Arts Bremen](http://www.hfk-bremen.de/en) and the [Copenhagen Institute of Interaction Design (CIID)](http://ciid.dk/) the project continued to be exhibited and extended in a series of workshops.

This library contains only the source code of the framework used to create the installation presented at Resonate. The actual compositions and setup files live in a separate repository.

### Installation

[Download](https://github.com/automatic-orchestra/library-for-arduino/archive/master.zip) the lastest version of the master branch or optionally fork and clone the repository to your local computer.

If you download the zip archive unzip it, rename the folder to ```automatic-orchestra``` and move into your Arduino ```libraries``` folder. The ```libraries``` folder is inside your Arduino Sketchbook folder – on OS X it usually is located in your user's ```Documents``` folder. Your can see and change your Arduino Sketchbook folder in the Arduino preferences.

In case you cloned the repository the nicest way to add the library to Arduino is to create a **symbolic link** of the repository folder and place it inside your Arduino ```libraries``` folder – see above. To create a symbolic link on OS X use the ln command: ```ln -s path/to/the/repository path/to/your/arduino/libraries/automatic-orchestra```.

In case Arduino was already open you'll have to close and open it again in order to make the library available in Arduino.

### Examples

Once you installed the library you'll find some basic examples on how to use the library in the Arduino application menu under ```File › Examples › Examples from Custom Libraries › Automatic Orchestra```.