//!
//! @file rr_Encoder-i2c-common.h
//! @author M. Nickels
//! @brief Master and slave common functions for ATtiny85 based encoder wth i2c
//! @version 0.1
//! @date 2021-12-20
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
#include "rr_Encoder-i2c-common.h"

//!
//! @brief send data over i2c interface
//!
//! @param data point to the data buffer
//! @param count number of bytes to be sent. Note that count cannot be greater than 32
//!
void sendData(byte* data, byte count) {
    for (byte loop = 0; loop < count; loop++) {
        Wire.write(data[loop]);
    }
}

//!
//! @brief receive data over i2c interface
//!
//! @param data point to the data buffer
//! @param count number of bytes to be received
//!
void receiveData(byte* data, byte count) {
    for (byte loop = 0; loop < count; loop++) {
        if (dataAvailable()) {
            data[loop] = Wire.read();
        }
        else {
            PRINT_ERROR("Data not available, expected %d, received %d", count, loop);
            break;
        }
    }

    while (dataAvailable()) {
        byte b = Wire.read();

        PRINT_ERROR("Surplus data received %x", b);
    }

#ifndef ARDUINO_AVR_ATTINYX5
    if (Wire.getWireTimeoutFlag()) {
        PRINT_ERROR("I2C timeout occured", NULL);

        Wire.clearWireTimeoutFlag();
    }
#endif
}

//!
//! @brief check if data is availabe
//!
//! @return boolean true if data is available on bus, false otherwise
//!
boolean dataAvailable(void) {
#ifndef ARDUINO_AVR_ATTINYX5
    return Wire.available() && !Wire.getWireTimeoutFlag();
#else
    return Wire.available();
#endif
}
