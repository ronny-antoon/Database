#include <Arduino.h>
#include <gtest/gtest.h>

#include "test_NVSDatabase/PutPair_test.hpp"
#include "test_NVSDatabase/GetValueOf_test.hpp"
#include "test_NVSDatabase/IsExist_test.hpp"
#include "test_NVSDatabase/GetValueLengthOf_test.hpp"
#include "test_NVSDatabase/RemovePair_test.hpp"
#include "test_NVSDatabase/EraseAll_test.hpp"

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

    Serial.println("Finished all tests!");

    delay(10000);
}
