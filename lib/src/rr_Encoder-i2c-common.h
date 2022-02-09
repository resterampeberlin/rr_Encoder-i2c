//!
//! @file rr_Encoder-i2c-common.h
//! @author M. Nickels
//! @brief Master and slave common definitions for ATtiny85 based encoder wth i2c
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

//! default i2c slave address
#define ENCODER_I2C_ADDRESS 0x10

//! I2C commands. Use fixed bit size to prevent problems with other platforms
typedef uint8_t EncoderI2CCommands_t;

//! define valid commands
enum {
    Get_Position   = 0x10, //!< get current encoder value
    Get_Direction  = 0x20, //!< get last direction
    Get_Button     = 0x30, //!< get push button status
    Set_Position   = 0x40, //!< set encoder value
    Set_Increment  = 0x50, //!< set +/- increment
    Set_LowerLimit = 0x51, //!< set lower limit
    Set_UpperLimit = 0x52, //!< set upper limit
    Set_Address    = 0x60, //!< set i2c address
    Get_Version    = 0x70  //!< get version of slave firmware
};

//! encoder position type. Use fixed bit size to prevent problems with other platforms
typedef int32_t EncoderI2CPosition_t;

//! type for the version string
//!
//! the version string could not be longer than xxx.yyy.zzz => 12 chars including trailing "0"
typedef char EncoderI2CVersion_t[12];

//! encoder direction
typedef enum {
    None     = 0x00, //!< No movement
    Forward  = 0x10, //!< Forward movement
    Backward = 0x20  //!< backwar movement
} EncoderI2CDirection_t;
