#ifndef RELEASE
#ifdef EMBEDED
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <gtest/gtest.h>

#include "../test/test_NVSDatabase/PutPair_test.hpp"
#include "../test/test_NVSDatabase/GetValueOf_test.hpp"
#include "../test/test_NVSDatabase/IsExist_test.hpp"
#include "../test/test_NVSDatabase/GetValueLengthOf_test.hpp"
#include "../test/test_NVSDatabase/RemovePair_test.hpp"
#include "../test/test_NVSDatabase/EraseAll_test.hpp"

TEST(Example, EmptyTest)
{
    ASSERT_TRUE(false);
}

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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#else
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>

TEST(Example, EmptyTest)
{
    ASSERT_TRUE(false);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
#endif