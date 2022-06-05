//!

//! @author M. Nickels
//! @brief Dynamic unit test

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
#include <unity.h>

//! own includes
#include "rr_Encoder-i2c.h"

EncoderI2C encoder;

//! pin definition for hardware encoder simulation
#define BUTTON_PIN    4
#define ENCA_PIN      2
#define ENCB_PIN      3

//! delay after each signal change
#define ENCODER_DELAY 100

//!
//! @brief simulate press of the button
//!
//! @param state button state
//!
void encoderButton(boolean state) {
    // usually a hardware encoder has got pullup resistors. Therefore the output is inverted
    digitalWrite(BUTTON_PIN, !state);
    delay(ENCODER_DELAY);
}

//!
//! @brief simulate a clockwise rotation of the encoder
//!
void encoderCCW(void) {

    // Simulate this sequence
    //
    // ENCA  --+  +----
    //         |  |
    //         +--+
    // ENCB  ----+  +--
    //           |  |
    //           +--+

    digitalWrite(ENCA_PIN, LOW);
    delay(ENCODER_DELAY);
    digitalWrite(ENCB_PIN, LOW);
    delay(ENCODER_DELAY);
    digitalWrite(ENCA_PIN, HIGH);
    delay(ENCODER_DELAY);
    digitalWrite(ENCB_PIN, HIGH);

    // give encoder module time to digest signals
    delay(8 * ENCODER_DELAY);
}

//!
//! @brief simulate a counterclockwise rotation of the encoder
//!
void encoderCW(void) {

    // Simulate this sequence
    //
    // ENCA  ----+  +--
    //           |  |
    //           +--+
    // ENCB  --+  +----
    //         |  |
    //         +--+

    digitalWrite(ENCB_PIN, LOW);
    delay(ENCODER_DELAY);
    digitalWrite(ENCA_PIN, LOW);
    delay(ENCODER_DELAY);
    digitalWrite(ENCB_PIN, HIGH);
    delay(ENCODER_DELAY);
    digitalWrite(ENCA_PIN, HIGH);

    // give encoder module time to digest signals
    delay(8 * ENCODER_DELAY);
}

//!
//! @brief test button() method
//!
void test_Button(void) {
    TEST_ASSERT_FALSE(encoder.button());

    encoderButton(true);

    TEST_ASSERT_TRUE(encoder.button());

    encoderButton(false);

    TEST_ASSERT_FALSE(encoder.button());
}

//!
//! @brief test button() method
//!
void test_Config(void) {
    // invert button behaviour
    EncoderI2Config_t config;

    config.invertSwitch = false;
    encoder.setConfig(config);

    encoderButton(false);

    TEST_ASSERT_TRUE(encoder.button());

    encoderButton(true);

    TEST_ASSERT_FALSE(encoder.button());

    // restore previous configuration
    config.invertSwitch = true;
    encoder.setConfig(config);
    encoderButton(false);

    TEST_ASSERT_FALSE(encoder.button());
}

//!
//! @brief test direction() function CW
//!
void test_DirectionCW(void) {
    encoder.setPosition(0);

    encoderCW();

    TEST_ASSERT_EQUAL(Forward, encoder.direction());
    TEST_ASSERT_EQUAL(4, encoder.position());
    TEST_ASSERT_EQUAL(None, encoder.direction());
}

//!
//! @brief test direction() function CCW
//!
void test_DirectionCCW(void) {
    encoder.setPosition(0);

    encoderCCW();

    TEST_ASSERT_EQUAL(Backward, encoder.direction());
    TEST_ASSERT_EQUAL(-4, encoder.position());
    TEST_ASSERT_EQUAL(None, encoder.direction());
}

//!
//! @brief test setUpperLimit() method
//!
void test_SetUpperLimit(void) {
    encoder.setPosition(0);
    encoder.setUpperLimit(10);

    for (unsigned loop = 1; loop <= 5; loop++) {
        encoderCW();
    }

    TEST_ASSERT_EQUAL(10, encoder.position());
}

//!
//! @brief test setLowerLimit() method
//!
void test_SetLowerLimit(void) {
    encoder.setPosition(0);
    encoder.setLowerLimit(-7);

    for (unsigned loop = 1; loop <= 4; loop++) {
        encoderCCW();
    }

    TEST_ASSERT_EQUAL(-7, encoder.position());
}

//!
//! @brief Setup routine
//!
void setup() {
    // NOTE!!! Wait for >2 secs
    // if board doesn't support software reset via Serial.DTR/RTS
    delay(2000);

    // connect i2c
    Wire.begin();

    // configure hardware encoder simulation
    pinMode(BUTTON_PIN, OUTPUT);
    digitalWrite(BUTTON_PIN, HIGH);

    pinMode(ENCA_PIN, OUTPUT);
    digitalWrite(ENCA_PIN, HIGH);

    pinMode(ENCB_PIN, OUTPUT);
    digitalWrite(ENCB_PIN, HIGH);

    // start unit testing
    UNITY_BEGIN();

    RUN_TEST(test_Button);
    RUN_TEST(test_Config);
    RUN_TEST(test_DirectionCW);
    RUN_TEST(test_DirectionCCW);
    RUN_TEST(test_SetUpperLimit);
    RUN_TEST(test_SetLowerLimit);

    // stop unit testing
    UNITY_END();

    // reset module
    encoder.reset();
}

//!
//! @brief Main loop
//!
void loop() {
}