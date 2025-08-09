// Copyright (c) 2025 David Such
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#pragma once

// Defines high-level symbolic roles for peripherals
// Used by PeripheralMap, HardwareInterface, and PrimalLayers logic

enum PeripheralRole {
    // SENSORS
    SENSOR_OBSTACLE_FRONT,
    SENSOR_OBSTACLE_REAR,
    SENSOR_LIGHT,
    SENSOR_TEMPERATURE,
    SENSOR_IR_LEFT,
    SENSOR_IR_MIDDLE,
    SENSOR_IR_RIGHT,
    SENSOR_IR_REMOTE,
    SENSOR_AUDIO,

    // ACTUATORS
    ACTUATOR_LEFT_WHEEL,
    ACTUATOR_RIGHT_WHEEL,
    ACTUATOR_REAR_LEFT_WHEEL,
    ACTUATOR_REAR_RIGHT_WHEEL,
    ACTUATOR_SERVO_PAN,
    ACTUATOR_SERVO_TILT,
    ACTUATOR_LED_LEFT,
    ACTUATOR_LED_RIGHT,
    ACTUATOR_LED_ONBOARD,
    ACTUATOR_BUZZER,

    // COMMUNICATION
    COMM_UART,
    COMM_I2C,
    COMM_SPI,

    // GENERAL
    PERIPHERAL_UNKNOWN
};