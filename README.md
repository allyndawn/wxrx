# WX-RX (Indoor Unit / Receiver)

![Indoor Unit / Receiver](/Media/wx-rx.png)

## Overview

You’ve found the GitHub repository for the indoor unit / receiver part of my wireless weather station, a.k.a. WX-RX.

A STM32 Nucleo-F429ZI ARM Cortex M4 development board running FreeRTOS serves as the heart of the indoor unit. An RFM69HCW ISM radio operating at 915 MHz receives the time, location and weather data transmitted periodically by the outdoor unit. A SparkFun LCD-14073 two line LCD display displays that data.

A future iteration of the receiver firmware may use the Nucleo’s on-board Ethernet port to upload the time and location stamped weather data to the cloud.

All code is written in C, with separate modules and tasks for each peripheral and a “core” task managing messaging between the peripherals.

More detailed information is available in the [WX-RX Project Wiki](https://github.com/allendav/wx-rx/wiki)

## Related Projects

* [WX-TX (Outdoor Unit / Transmitter) Project](https://github.com/allendav/wx-tx)
