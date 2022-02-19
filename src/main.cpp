//!
//! @file main.cpp
//! @author M. Nickels
//! @brief Main file for the Library "rr_ArduinoUtils"
//! @version 0.1
//! @date 2021-12-20
//!
//! @copyright Copyright (c) 2021
//!
//! This work is licensed under the
//!
//!      Creative Commons Attribution-NonCommercial 4.0 International License.
//!
//! To view a copy of this license, visit http://creativecommons.org/licenses/by-nc/4.0/
//! or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
//!

#include <Arduino.h>
#include <Wire.h>

//! own includes
#include "rr_DebugUtils.h"
#include "rr_Encoder-i2c.h"

EncoderI2C encoder;

//!
//! @brief Setup routine
//!
void setup() {
#ifdef __PLATFORMIO_BUILD_DEBUG__
    Debug.beginSerial();

    // set tabs
    unsigned tabs[] = {20, 30, 40, 50};

    Debug.setTabs(tabs, sizeof(tabs) / sizeof(tabs[0]));
#else
    #pragma GCC error "set 'build_type = debug' in platformio.ini"
#endif

    Wire.begin();

    PRINT_INFO("Encoder module version: %s", encoder.version().c_str());
}

//!
//! @brief Main loop
//!
//! Print continously the status of the encoder
//!
void loop() {
    while (true) {
        PRINT_DEBUG("Position: %ld\tSw: %d\tDir: %d", encoder.position(), encoder.button(), encoder.direction());

        delay(200);
    }
}