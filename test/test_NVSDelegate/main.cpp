#ifndef MY_TEST_SUITE_RUNNING
#define MY_TEST_SUITE_RUNNING

#include <Arduino.h>
#include <gtest/gtest.h>

#include "IncludeAll_test.hpp"

void setup()
{
    Serial.begin(115200);
    ::testing::InitGoogleTest();
}

void loop()
{
    if (RUN_ALL_TESTS())
        ;

    delay(1000);

    Serial.println("-----------------------------------Finished all tests!-----------------------------------");

    delay(10000);
}

#endif // MY_TEST_SUITE_RUNNING