#ifndef GET_LENGTH_TEST_H
#define GET_LENGTH_TEST_H

#include <gtest/gtest.h>
#include <Arduino.h>

#include "NVSDelegate.hpp"

class GetLength_test : public ::testing::Test
{
protected:
    NVSDelegate *nvsDelegate;
    uint32_t _startFreeHeap = 0;
    void SetUp() override
    {
        _startFreeHeap = ESP.getFreeHeap();
        nvsDelegate = new NVSDelegate("test_namespace");
    }

    void TearDown() override
    {
        nvsDelegate->eraseAll();
        delete nvsDelegate;
        nvsDelegate = nullptr;
        int heapDiff = ESP.getFreeHeap() - _startFreeHeap;
        if (heapDiff != 0)
            FAIL() << "Memory leak of " << heapDiff << " bytes"; // Fail the test if there is a memory leak
    }
};

TEST_F(GetLength_test, Empty_Null_Key)
{
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    // get a value from the database
    char value[valueLength];
    DatabaseError_t result = nvsDelegate->getLength("", &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);

    // get a value from the database
    result = nvsDelegate->getLength(nullptr, &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

TEST_F(GetLength_test, Key_Length_Too_Long)
{
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    // get a value from the database
    DatabaseError_t result = nvsDelegate->getLength("12345678901234567", &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

TEST_F(GetLength_test, Key_Not_Found)
{
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    // get a value from the database
    DatabaseError_t result = nvsDelegate->getLength("key", &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}

#endif // GET_LENGTH_TEST_H