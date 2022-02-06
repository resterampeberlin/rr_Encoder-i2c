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

//! own includes
#include "rr_Encoder-i2c.h"

EncoderI2C encoder;

//!
//! @brief Setup routine
//!
void setup() {
    Serial.begin(115200);

    while (!Serial)
        ;

    Serial.print("Encoder module version:");
    Serial.println(encoder.version());
}

//!
//! @brief Main loop
//!
//! Print continously the status of the encoder
//!
void loop() {
    while (true) {
        Serial.print("Position:");
        Serial.print(encoder.position());
        Serial.print("\t");
        Serial.print(encoder.button());
        Serial.print("\t");
        Serial.print(encoder.direction());
        Serial.println();

        delay(200);
    }
}