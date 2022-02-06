//!
//! @file rr_Encoder-i2c.cpp
//! @author M. Nickels
//! @brief class to connect an ATtiny85 based encoder wth i2c
//! @version 0.1
//! @date 14.1.2022
//!
//! @copyright Copyright (c) 2022
//!
//! This file is part of the Library "RREncoderI2C".
//!
//!      Creative Commons Attribution-ShareAlike 4.0 International License.
//!
//! To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/
//! or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
//!

#include <Arduino.h>
#include <Wire.h>

#include "rr_Encoder-i2c.h"

//!
//! @brief Construct a new EncoderI2C object with default address
//!
//!
EncoderI2C::EncoderI2C() : EncoderI2C(ENCODER_I2C_ADDRESS) {
}

//!
//! @brief Construct a new EncoderI2C object
//!
//! @param newAddress i2c address on bus
//!
EncoderI2C::EncoderI2C(int newAddress) {
    address = newAddress;
}

//!
//! @brief read the current position
//!
//! @return EncoderI2CPosition_t
//!
EncoderI2CPosition_t EncoderI2C::position(void) {
    sendCommand(Get_Position);

    return receivePosition();
}

//!
//! @brief set a new position
//!
//! @param position
//!
void EncoderI2C::setPosition(EncoderI2CPosition_t position) {
    sendCommand(Set_Position);

    sendPosition(position);
}

//!
//! @brief set the increment for each rotation
//!
//! @param increment
//!
void EncoderI2C::setIncremenent(EncoderI2CPosition_t increment) {
    sendCommand(Set_Increment);

    sendPosition(increment);
}

//!
//! @brief read the last direction
//!
//! @return EncoderI2CDirection_t
//!
EncoderI2CDirection_t EncoderI2C::direction(void) {
    sendCommand(Get_Direction);

    return receiveDirection();
}

//!
//! @brief read the button state
//!
//! @return boolean
//!
boolean EncoderI2C::button(void) {
    sendCommand(Get_Button);

    return receiveBoolean();
}

//!
//! @brief read the version of the module
//!
//! @return String
//!
String EncoderI2C::version(void) {
    sendCommand(Get_Version);

    Wire.requestFrom(address, sizeof(EncoderI2CVersion_t));

    // initialize string
    EncoderI2CVersion_t versionString;

    memset(versionString, 0, sizeof(EncoderI2CVersion_t));

    byte loop = 0;

    while (Wire.available() && loop <= sizeof(EncoderI2CVersion_t)) {
        versionString[loop++] = Wire.read();
    }

    // skip remaining data (should not happen)
    if (Wire.available()) {
        Wire.read();
    }

    return String(versionString);
}

//!
//! @brief send a command to the module
//!
//! @param cmd
//!
void EncoderI2C::sendCommand(EncoderI2CCommands_t cmd) {
    Wire.beginTransmission(address);
    Wire.write(cmd);
    Wire.endTransmission();
}

//!
//! @brief sends an EncoderI2CPosition_t value to the module
//!
//! @param value value to be send
//!
void EncoderI2C::sendPosition(EncoderI2CPosition_t value) {
    Wire.beginTransmission(address);

    EncoderI2CPosition_t data = value;

    for (byte loop = 1; loop <= sizeof(EncoderI2CPosition_t); loop++) {
        Wire.write(data & 0xff);
        data = data >> 8;
    }

    Wire.endTransmission();
}

//!
//! @brief reads a boolean from the module
//!
//! @return boolean
//!
boolean EncoderI2C::receiveBoolean(void) {
    Wire.requestFrom(address, sizeof(boolean));

    int  data  = 0;
    byte count = 0;

    while (Wire.available()) {
        data |= Wire.read();
        data = data << 8;
        count++;
    }

    return (boolean)data;
}

//!
//! @brief reads an EncoderI2CPosition_t from the module
//!
//! @return EncoderI2CPosition_t the read value
//!
EncoderI2CPosition_t EncoderI2C::receivePosition(void) {
    Wire.requestFrom(address, sizeof(EncoderI2CPosition_t));

    EncoderI2CPosition_t data  = 0;
    byte                 count = 0;

    while (Wire.available()) {
        data |= Wire.read();
        data = data << 8;
        count++;
    }

    return data;
}

//!
//! @brief reads a EncoderI2CDirection_t value from the module
//!
//! @return EncoderI2CDirection_t the read value
//!
EncoderI2CDirection_t EncoderI2C::receiveDirection(void) {
    Wire.requestFrom(address, sizeof(EncoderI2CDirection_t));

    int  data  = 0;
    byte count = 0;

    while (Wire.available()) {
        data |= Wire.read();
        data = data << 8;
        count++;
    }

    return (EncoderI2CDirection_t)data;
}