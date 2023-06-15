# Embedded_Smart_Home

## Software modules:

### 1.Client: a desktop client application based on Qt Creator, with functions including:

(1). To connect to the server in the LAN, you need to provide ipv4 and port

(2). Call the weather API to realize the function of weather query in different cities

(3). Call the header file library to display the real time

(4). Write a protocol to realize the LAN communication between the client and the server, and transmit the signals of the six functions of light, buzzer, and fan on and off from the client to the server

### 2.Server: LAN server file written in C language, the functions include:

(1). Receive signals to control light, buzzer, and fan to turn on and off the six functions transmitted through the written protocol

(2). Realize the serial port communication between the server and the development board through UART, and send the signals for the six functions of light, buzzer, and fan on and off to the development board

## Hardware module:

Through the programming of STM32 single-chip microcomputer, the signal from the server to control the light, buzzer, fan on and off six functions is received. Realize the six actual functions of controlling light, buzzer, and fan to be turned on and off by three actual hardware

## Result photo:

### 1.Weather search function:

![weather_search.png](Picture/weather_search.png)

### 2.The effect display of the main interface of the client and the server terminal:

![result.png](Picture/result.png)

### 3.Development board effect display:

![hardware.jpg](Picture/hardware.jpg)

### Finally,realize the effect of embedded smart home
