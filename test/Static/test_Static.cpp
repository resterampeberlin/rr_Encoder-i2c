//!
//! @file test_Static.cpp
//! @author M. Nickels
//! @brief Static unit test
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
#include <unity.h>

//! own includes
#include "rr_Encoder-i2c.h"

EncoderI2C encoder;

//!
//! @brief retrieve Version number
//!
void test_Version(void) {
    String version = encoder.version();

    Serial.print("Version =");
    Serial.println(version);

    TEST_ASSERT_GREATER_THAN(0, version.length());
    TEST_ASSERT_LESS_THAN(sizeof(EncoderI2CVersion_t), version.length());
}

//!
//! @brief test button() method
//!
void test_Button(void) {
    TEST_ASSERT_FALSE(encoder.button());
}

//!
//! @brief test button() method
//!
void test_Config(void) {
    TEST_ASSERT_FALSE(encoder.button());

    // invert button behaviour
    EncoderI2Config_t config;

    config.invertSwitch = false;
    encoder.setConfig(config);

    TEST_ASSERT_TRUE(encoder.button());

    config.invertSwitch = true;
    encoder.setConfig(config);

    TEST_ASSERT_FALSE(encoder.button());
}

//!
//! @brief test direction() function
//!
void test_Direction(void) {
    TEST_ASSERT_EQUAL(None, encoder.direction());
}

//!
//! @brief test position() method
//!
void test_Position(void) {
    TEST_ASSERT_EQUAL(0, encoder.position());
}

//!
//! @brief test setPosition() method
//!
void test_SetPosition(void) {
    TEST_ASSERT_EQUAL(0, encoder.position());

    encoder.setPosition(10);

    // give encoder module time to digest command
    delay(500);

    TEST_ASSERT_EQUAL(10, encoder.position());
    TEST_ASSERT_EQUAL(None, encoder.direction());
}

//!
//! @brief test setIncrement() method
//!
void test_SetIncrement(void) {
    TEST_ASSERT_EQUAL(10, encoder.position());

    encoder.setIncrement(5);

    TEST_ASSERT_EQUAL(50, encoder.position());
    TEST_ASSERT_EQUAL(None, encoder.direction());
}

//!
//! @brief test setUpperLimit() method
//!
void test_SetUpperLimit(void) {
    TEST_ASSERT_EQUAL(50, encoder.position());

    encoder.setUpperLimit(40);

    TEST_ASSERT_EQUAL(40, encoder.position());
    TEST_ASSERT_EQUAL(None, encoder.direction());
}

//!
//! @brief test setLowerLimit() method
//!
void test_SetLowerLimit(void) {
    encoder.setPosition(-50);

    // give encoder module time to digest command
    delay(500);

    TEST_ASSERT_EQUAL(-50, encoder.position());

    encoder.setLowerLimit(-40);

    TEST_ASSERT_EQUAL(-40, encoder.position());
    TEST_ASSERT_EQUAL(None, encoder.direction());
}

//!
//! @brief test setAddress() method
//!
void test_SetAddress(void) {
    TEST_ASSERT_EQUAL(-40, encoder.position());

    // change i2c address
    encoder.setAddress(0x20);

    // wait for reconfiguration
    delay(1000);

    TEST_ASSERT_EQUAL(-40, encoder.position());

    // reset i2c address
    encoder.setAddress(0x10);

    // wait for reconfiguration
    delay(1000);

    TEST_ASSERT_EQUAL(-40, encoder.position());
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

    // start unit testing
    UNITY_BEGIN();

    RUN_TEST(test_Version);
    RUN_TEST(test_Button);
    RUN_TEST(test_Config);
    RUN_TEST(test_Position);
    RUN_TEST(test_Direction);
    RUN_TEST(test_SetPosition);
    RUN_TEST(test_SetIncrement);
    RUN_TEST(test_SetUpperLimit);
    RUN_TEST(test_SetLowerLimit);
    RUN_TEST(test_SetAddress);

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