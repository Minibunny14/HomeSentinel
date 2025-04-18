# PROJECT: HOME_SENTINEL

## Table of contents
1. [Abstract](#abstract)
2. [Features](#features)
3. [Hardware Requirements](#hardware-requirements)
4. [System Architecture](#system-architecture)

## Abstract
### Home-Security systems cost a fortune to invest in, why not build your own? Introducing, the `HOME_SENTINEL`.

A lightweight home security system that runs on the Espressif Systems ESP32. It controls sensors, locks, and alarms via Wi-Fi, and communicates using the MQTT protocol. Designed for privacy, performance, and easy DIY integration.

## Features

- **Motion Detection**: Utilizes either a PIR sensor or a custom laser-based detection unit to monitor movement in designated areas.
- **Wireless Communication**: Uses the ESP-NOW protocol for low-latency, peer-to-peer messaging between nodes, and MQTT for device-cloud interaction.
- **Remote Monitoring**: Receive real-time updates and control commands via a mobile phone, PC, or IoT dashboard.
- **Smart Lock Integration**: Supports magnetic or solenoid-based door locks that can be controlled remotely or triggered by detection events.
- **Alarm System**: Activates a buzzer or piezoelectric alarm when unauthorized motion is detected.
- **Status Indicators**: Optional LEDs indicate system status (e.g., armed/disarmed, sensor triggered).
- **Expandable Design**: Easily add more sensors or functionality using available GPIO pins and standard protocols.
- **DIY Friendly**: Designed with accessibility in mind – affordable components, minimal soldering, and detailed documentation.

## Hardware Requirements

1. **Detection unit** of some sort, you can use a 
`PIR motion sensor`, or, if you want to learn how to build your own detection unit, checkout this [`Laser_Based_Detection_System`](https://github.com/Minibunny14/Laser-Based-Obstruction-Detection-System) that I built, that goes well together with this project.

2. Raspberry Pi Pico W

3. Magnetic door locking mechanism, e.g `Solinoid`.

4. Buzzer / alarm or a Piezzo

5. **LEDs** (optional for status indicators), you can just use the `BUILT_IN` LED on the pico. 

6. Breadboard + jumper wires

7. Power supply or USB power

## System Architecture

The following diagram illustrates how devices in the PicoSecure system interact:

![alt text](./Images/HomeSentinelNetworkArchetechture.drawio.png)

1. **ESP32** connects to Wi-Fi and establishes a connection to the **MQTT Broker**. 
2. It publishes messages (e.g., motion detected) to topics like `home/motion/livingroom`.
3. The **MQTT Broker** relays messages to subscribed clients.
4. A **computer, phone, or dashboard app** receives updates and may send commands back (e.g., turn off alarm).
