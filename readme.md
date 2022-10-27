## Welcome to Chris Buchanan's Digitize repo

## Project 1 Overview:
Project 1 will have you set up your project laptop and Arduino development environment, as well as begin on the core of your RTU - a command line interpreter. You will be required to take in a string from the keyboard, parse it, and execute different functions based on the contents. In addition to the laptop and Arduino board, you will also be given a bi-color LED to control.

### RTU Application overview:
The thousand foot view is that the program interprets commands and implements them. Currently, it can control led's and show information through the serial connection. It does this by continuously looping and checking for input, then controling the led's. If input is entered, it will print the character back to screen. If return is pressed, it will send the command off to be processed. If there's no input to process, it will continue on to perform the blink function.

### General Setup Info:
My laptop is running Ubuntu 22.04.1 LTS and it has arduino version 1.8.19 installed. I'm using picocom to connect to arduino via serial, and using vim as the text editor. Doxygen is used to maintain a working documentation of the code. Git & github (https://github.com/cbuchanan919/Digitize) are being used to source control my code.

### Notes:
Getting arduino installed & the IDE talking to the arduino board was relatively easy. Getting the makefile installed & configured properly was very challenging for me. I had to configure the ~/.bashrc file to add the necessary make paths, google error messages, and add the include arduino.h in the main file, (etc).



