//!
//! @file rr_Encoder-i2c.cpp
//! @author M. Nickels
//! @brief class to connect an ATtiny85 based encoder wth i2c
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

#pragma once

#include "rr_Encoder-i2c-common.h"

//!
//! @brief abstraction class for the protocol to the i2c module
//!
//!
class EncoderI2C {

  public:
    EncoderI2C();
    EncoderI2C(int newAddress);

    // get/set encoder position
    EncoderI2CPosition_t position(void);
    void                 setPosition(EncoderI2CPosition_t position);

    // set increment
    void setIncremenent(EncoderI2CPosition_t increment);

    // set limits
    void setLowerLimit(EncoderI2CPosition_t limit);
    void setUpperLimit(EncoderI2CPosition_t limit);

    // last direction
    EncoderI2CDirection_t direction(void);

    // current button status
    boolean button(void);

    // set new i2c address for module
    void setAddress(byte newAddress);

    // firmware version of module
    String version(void);

  protected:
    // check transmission
    void checkTransmission(unsigned received, unsigned expected);

    // send data
    void sendCommand(EncoderI2CCommands_t cmd);
    void sendPosition(EncoderI2CPosition_t value);
    void sendAddress(byte newAddress);

    // receive data
    boolean               receiveBoolean(void);
    EncoderI2CPosition_t  receivePosition(void);
    EncoderI2CDirection_t receiveDirection(void);

    //! i2c slave address
    int address;
};