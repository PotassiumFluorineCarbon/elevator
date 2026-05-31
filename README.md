**Elevator Project**

**Overview**
This repository contains the software, protocols, and documentation for the **CAN (Controller Area Network) Elevator project**, developed for **Engineering Project VI**.

The goal of this project is to build a **distributed elevator control** system using a CAN bus architecture. This system is integrated with a **Local Area Network (LAN)** to provide internet-based remote access, real-time diagnostics, and system logging.

**System Architecture**
The project utilizes **a distributed node architecture** to manage low level hardware control and high level remote monitoring:

**Elevator Controller (Arduino)**: Responsible for controlling the elevator motor from the servo and tracking positioning using a laser distance sensor.

**Car and Floor Controllers**: These nodes manage the physical floor selection buttons, up and down switches, door states, and LED indicators.

**Supervisory Controller (Raspberry Pi)**: This serves as the project's hub, bridging the CAN network to the LAN via a USB-to-CAN adapter. It also implements the system's finite state machine.

**Data Repository and Web Server**: This component hosts the MySQL database and web application. It logs all elevator events, generates diagnostic reports, and enables remote operation via a web interface.

**Development Phases**
The project is organized into three major phases:

**Phase 1: CAN Network**
  *Design and implementation of the common CAN messaging protocol.
  *Establishment of communication between embedded controllers.

**Phase 2: LAN and Remote Control**
  *Development of the data server and diagnostics repository.
  *Creation of the web-based user interface and database integration.

**Phase 3: System Integration**
  *Full integration of the CAN and LAN communication modules.
  *Implementation of the final finite state machine and system-specific features.

**Tech Stack**
**Hardware and Sensors**
  *Microcontrollers: STM32 Nucleo, Arduino, Raspberry Pi
  *Adapter: PEAK USB/CAN Adapter
  *Components: Laser Rangefinder, Servo Motor

**Back-End and Database**
Embedded Control: C/C++
Server-side Logic: PHP
Data Logging: MySQL

**Front-End**
  *HTML5
  *CSS3
  *JavaScript

**Team**
Eric

Blaine

Dallas
