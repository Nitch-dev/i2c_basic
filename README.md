⚡ESP32 Dual-Core RTOS Stress Test & Sensor Integration

Project Overview

This project utilizes the Espressif IoT Development Framework (ESP-IDF) and FreeRTOS to perform a stress test on the ESP32's dual-core architecture. The primary goal is to push the limits of parallel execution


🚀 Quick Setup

NOTE: Please Use Vscode and ESPIDF Extenstion it makes the Whole Process alot more Easier and Dev Compatible 
in the future 

Clone the Repository:

git clone [Your Repository URL Here]
cd [Your Repository Directory]


Configure the Project:
Run the configuration menu to select your target chip, flash settings, and any required sensor driver configurations.

idf.py menuconfig


Build the Project:
Compile the source code and generate the binary files.

idf.py build


Flash the Device:
Connect your ESP32 board and flash the compiled binaries.
