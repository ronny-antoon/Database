#ifndef INSERT_TEST_H
#define INSERT_TEST_H

#include <gtest/gtest.h>

#include "NVSDelegate.hpp"

class Insert_test : public ::testing::Test
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
        if (ESP.getFreeHeap() != _startFreeHeap)
            FAIL() << "Memory leak of " << _startFreeHeap - ESP.getFreeHeap() << " bytes"; // Fail the test if there is a memory leak
    }
};

TEST_F(Insert_test, Empty_Null_Key)
{
    // Insert a key-value into the database
    DatabaseError_t result = nvsDelegate->insert("", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);

    // Insert a key-value into the database
    result = nvsDelegate->insert(nullptr, "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

TEST_F(Insert_test, Empty_Null_Value)
{
    // Insert a key-value into the database
    DatabaseError_t result = nvsDelegate->insert("key", "");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);

    // Insert a key-value into the database
    result = nvsDelegate->insert("key", nullptr);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);
}

TEST_F(Insert_test, Key_Length_Too_Long)
{
    // Insert a key-value into the database
    DatabaseError_t result = nvsDelegate->insert("12345678901234567", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

TEST_F(Insert_test, Value_Length_Too_Long)
{
    char myValue[1024];         // 1024 is the maximum value length
    memset(myValue, 'a', 1023); // Fill the value with 'a'
    myValue[1023] = '\0';       // Add the null terminator

    // Insert a key-value into the database
    DatabaseError_t result = nvsDelegate->insert("key", myValue);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    char myValue2[1025];         // 1025 is one more than the maximum value length
    memset(myValue2, 'a', 1024); // Fill the value with 'a'
    myValue2[1024] = '\0';       // Add the null terminator

    // Insert a key-value into the database
    result = nvsDelegate->insert("key2", myValue2);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);
}

TEST_F(Insert_test, Key_Already_Exist)
{
    // Insert a key-value into the database
    DatabaseError_t result = nvsDelegate->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // Insert the same key-value into the database
    result = nvsDelegate->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_ALREADY_EXISTS);
}

#endif // INSERT_TEST_H