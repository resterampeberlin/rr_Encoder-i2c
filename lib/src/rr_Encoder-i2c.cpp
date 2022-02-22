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

#include "rr_DebugUtils.h"
#include "rr_Encoder-i2c.h"

//!
//! @brief Construct a new EncoderI2C object with default address
//!
//!
EncoderI2C::EncoderI2C() {
    address = ENCODER_I2C_ADDRESS;
}

//!
//! @brief Construct a new EncoderI2C object
//!
//! @param newAddress i2c address on bus
//!
EncoderI2C::EncoderI2C(int newAddress) : EncoderI2C() {
    address = newAddress;
}

//!
//! @brief read the current position
//!
//! The reported position is scaled with the increment and constrained by
//! upper and lower limit
//!
//! @return EncoderI2CPosition_t the encoder position
//!
EncoderI2CPosition_t EncoderI2C::position(void) {
    sendCommand(Get_Position);

    return receivePosition();
}

//!
//! @brief set the new position
//!
//! Since the raw encoder position is always multiplied by the increment value and
//! constrained by the lower and upper limit, the position shall be within those
//! limits and a multiple of the increment
//!
//! @param position new position
//!
void EncoderI2C::setPosition(EncoderI2CPosition_t position) {
    sendCommand(Set_Position);

    sendPosition(position);

    // give the encoder the chance to reach the main loop to transfer the new value
    delay(200);
}

//!
//! @brief set the increment for each position
//!
//! @param increment new increment
//!
void EncoderI2C::setIncrement(EncoderI2CPosition_t increment) {
    sendCommand(Set_Increment);

    sendPosition(increment);
}

//!
//! @brief set the lower limit for the position
//!
//! @param limit new lower limit
//!
void EncoderI2C::setLowerLimit(EncoderI2CPosition_t limit) {
    sendCommand(Set_LowerLimit);

    sendPosition(limit);
}

//!
//! @brief set the upper limit for the position
//!
//! @param limit new upper limit
//!
void EncoderI2C::setUpperLimit(EncoderI2CPosition_t limit) {
    sendCommand(Set_UpperLimit);

    sendPosition(limit);
}

//!
//! @brief read the last direction
//!
//!
//! @return EncoderI2CDirection_t last direction of the encoder or None if no change
//!         occurred
//!
EncoderI2CDirection_t EncoderI2C::direction(void) {
    sendCommand(Get_Direction);

    return receiveDirection();
}

//!
//! @brief read the button state
//!
//! @return boolean true if button pressed or false otherwise
//!
boolean EncoderI2C::button(void) {
    sendCommand(Get_Button);

    return receiveBoolean();
}

//!
//! @brief set new i2c address for module
//!
//! @param newAddress the new i2c address
//!
void EncoderI2C::setAddress(byte newAddress) {
    sendCommand(Set_Address);

    sendAddress(newAddress);
    address = newAddress;
}

//!
//! @brief read the version of the module
//!
//! @return String the version of the module firmware
//!
String EncoderI2C::version(void) {
    EncoderI2CVersion_t versionString;

    // initialize string
    memset(versionString, 0, sizeof(EncoderI2CVersion_t));

    sendCommand(Get_Version);
    Wire.requestFrom(address, sizeof(EncoderI2CVersion_t));
    receiveData((byte*)versionString, sizeof(versionString));

    return String(versionString);
}

//!
//! @brief Set configuration of encoder module
//!
//! @param config the new configuration
//!
void EncoderI2C::setConfig(EncoderI2Config_t config) {
    sendCommand(Set_Config);

    sendConfig(config);
}

//!
//! @brief reset the module
//!
//!
void EncoderI2C::reset(void) {
    // send command twice to ensure a kind of failsafe handling
    sendCommand(Reset_Module);
    sendCommand(Reset_Module);
}

//!
//! @brief send a command to the module
//!
//! @param cmd the command to be sent
//!
void EncoderI2C::sendCommand(EncoderI2CCommands_t cmd) {
#ifndef ARDUINO_AVR_ATTINYX5
    Wire.setWireTimeout();
#endif

    Wire.beginTransmission(address);
    sendData((byte*)&cmd, sizeof(cmd));
    Wire.endTransmission();
}

//!
//! @brief sends an EncoderI2CPosition_t value to the module
//!
//! @param value value to be sent
//!
void EncoderI2C::sendPosition(EncoderI2CPosition_t value) {
    Wire.beginTransmission(address);
    sendData((byte*)&value, sizeof(value));
    Wire.endTransmission();
}

//!
//! @brief send an i2c address to the module
//!
//! @param newAddress the i2c address
//!
void EncoderI2C::sendAddress(byte newAddress) {
    Wire.beginTransmission(address);
    sendData((byte*)&newAddress, sizeof(newAddress));
    Wire.endTransmission();
}

//!
//! @brief send new config to the module
//!
//! @param config the new configuration
//!
void EncoderI2C::sendConfig(EncoderI2Config_t config) {
    Wire.beginTransmission(address);
    sendData((byte*)&config, sizeof(config));
    Wire.endTransmission();
}

//!
//! @brief reads a boolean from the module
//!
//! @return boolean read value
//!
boolean EncoderI2C::receiveBoolean(void) {
    boolean data;

    Wire.requestFrom(address, sizeof(data));
    receiveData((byte*)&data, sizeof(data));

    return data;
}

//!
//! @brief reads an EncoderI2CPosition_t from the module
//!
//! @return EncoderI2CPosition_t the read value
//!
EncoderI2CPosition_t EncoderI2C::receivePosition(void) {
    EncoderI2CPosition_t data = 0;

    Wire.requestFrom(address, sizeof(data));
    receiveData((byte*)&data, sizeof(data));

    return data;
}

//!
//! @brief reads a EncoderI2CDirection_t value from the module
//!
//! @return EncoderI2CDirection_t the read value
//!
EncoderI2CDirection_t EncoderI2C::receiveDirection(void) {
    EncoderI2CDirection_t data;

    Wire.requestFrom(address, sizeof(data));
    receiveData((byte*)&data, sizeof(data));

    return data;
}