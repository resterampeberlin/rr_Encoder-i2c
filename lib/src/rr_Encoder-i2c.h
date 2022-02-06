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

class EncoderI2C {

  public:
    EncoderI2C();
    EncoderI2C(int newAddress);

    EncoderI2CPosition_t  position(void);
    void                  setPosition(EncoderI2CPosition_t position);

    void                  setIncremenent(EncoderI2CPosition_t increment);

    EncoderI2CDirection_t direction(void);
    boolean               button(void);

    String                version(void);

  private:
    void                  sendCommand(EncoderI2CCommands_t cmd);

    void                  sendPosition(EncoderI2CPosition_t value);

    boolean               receiveBoolean(void);
    EncoderI2CPosition_t  receivePosition(void);
    EncoderI2CDirection_t receiveDirection(void);

    int                   address;
};