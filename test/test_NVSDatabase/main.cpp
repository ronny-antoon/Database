#ifndef RELEASE
#include <Arduino.h>
#include <gtest/gtest.h>

// #include "PutPair_test.hpp"
// #include "GetValueOf_test.hpp"
// #include "IsExist_test.hpp"
// #include "GetValueLengthOf_test.hpp"
// #include "RemovePair_test.hpp"
// #include "EraseAll_test.hpp"

void setup()
{
    // should be the same value as for the `test_speed` option in "platformio.ini"
    // default value is test_speed=115200
    Serial.begin(115200);

    ::testing::InitGoogleTest();
    // if you plan to use GMock, replace the line above with
    // ::testing::InitGoogleMock();
}

TEST(Example, EmptyTest)
{
    ASSERT_TRUE(true);
}

void loop()
{
    // Run tests
    if (RUN_ALL_TESTS())
        ;

    // sleep for 1 sec
    delay(1000);
}
#endif