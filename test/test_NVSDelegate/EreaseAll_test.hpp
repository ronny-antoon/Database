#ifndef ERASE_ALL_TEST_H
#define ERASE_ALL_TEST_H

#include <gtest/gtest.h>
#include <Arduino.h>

#include "NVSDelegate.hpp"

class EraseAll_test : public ::testing::Test
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

TEST_F(EraseAll_test, Check_If_Erease_Done)
{
    // Insert a key-value pair into the database
    DatabaseError_t result = nvsDelegate->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // Erase all the data from the database
    result = nvsDelegate->eraseAll();

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // get a value from the database
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    char value[valueLength];
    result = nvsDelegate->get("key", value, &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}

#endif // ERASE_ALL_TEST_H