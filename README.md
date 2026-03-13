# ESP32-Oil-Level-Alarm-System

This project presents an ESP32-based oil tank monitoring system designed to measure and supervise the oil level in an industrial installation. An analog level sensor provides a voltage signal 
proportional to the oil level, which is read by the ESP32 ADC. The signal is processed using an exponential moving average (EMA) filter to reduce measurement noise, and the oil volume is 
estimated through linear interpolation between experimentally calibrated voltage points. 

When the measured oil volume exceeds a predefined threshold, a visual and acoustic alarm is activated using an LED and an active buzzer to alert the operator. In addition to local signaling, 
the system publishes the measured volume via MQTT to a Node-RED dashboard for real-time remote monitoring. This architecture combines embedded signal processing, hardware integration, and IoT 
communication for reliable tank level supervision.

## Features

- Oil level measurement using analog sensor
- Linear interpolation calibration
- Exponential moving average (EMA) filtering
- LED and buzzer alarm signaling
- MQTT communication
- Real-time monitoring with Node-RED
