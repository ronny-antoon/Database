#include <Arduino.h>
#include <gtest/gtest.h>

#include "EraseAll_test.hpp"
#include "Get_test.hpp"
#include "IsExist_test.hpp"
#include "Set_test.hpp"
#include "Remove_test.hpp"
#include "GetLength_test.hpp"

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
