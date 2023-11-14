#ifndef UPDATE_TEST_H
#define UPDATE_TEST_H

#include <gtest/gtest.h>

#include "NVSDelegate.hpp"

class Update_test : public ::testing::Test
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

TEST_F(Update_test, Empty_Null_Key)
{
    // update a key-value into the database
    DatabaseError_t result = nvsDelegate->update("", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);

    // update a key-value into the database
    result = nvsDelegate->update(nullptr, "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

TEST_F(Update_test, Empty_Null_Value)
{
    // update a key-value into the database
    DatabaseError_t result = nvsDelegate->update("key", "");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);

    // update a key-value into the database
    result = nvsDelegate->update("key", nullptr);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);
}

TEST_F(Update_test, Key_Length_Too_Long)
{
    // update a key-value into the database
    DatabaseError_t result = nvsDelegate->update("12345678901234567", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);

    // update a key-value into the database
    result = nvsDelegate->update("12345678901234567890123456789012345678901234567890123456789012345678901234567890", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

TEST_F(Update_test, Value_Length_Too_Long)
{
    // insert a key-value into the database
    DatabaseError_t result = nvsDelegate->insert("key", "myValue");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    int maxNumber = NVS_DELEGATE_MAX_VALUE_LENGTH;
    char myValue[maxNumber];             // 1024 is the maximum value length
    memset(myValue, 'a', maxNumber - 1); // Fill the value with 'a'
    myValue[maxNumber - 1] = '\0';       // Add the null terminator

    // update a key-value into the database
    result = nvsDelegate->update("key", myValue);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    char myValue2[maxNumber + 1];     // 1025 is one more than the maximum value length
    memset(myValue2, 'a', maxNumber); // Fill the value with 'a'
    myValue2[maxNumber] = '\0';       // Add the null terminator

    // update a key-value into the database
    result = nvsDelegate->update("key2", myValue2);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);
}

TEST_F(Update_test, Key_Not_Exist)
{
    // Insert a key-value into the database
    DatabaseError_t result = nvsDelegate->update("key1234", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}

TEST_F(Update_test, Key_Exists)
{
    // Insert a key-value into the database
    DatabaseError_t result = nvsDelegate->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // update a key-value into the database
    result = nvsDelegate->update("key", "value12");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);
}

TEST_F(Update_test, Update_Existing_Key_new_Value)
{
    // Insert a key-value into the database
    DatabaseError_t result = nvsDelegate->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // update a key-value into the database
    result = nvsDelegate->update("key", "value12");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // Get the value associated with the key
    char value[1024];
    size_t maxLength = 1024;
    result = nvsDelegate->get("key", value, &maxLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // Ensure the value matches the expected value
    ASSERT_STREQ(value, "value12");
}
#endif // UPDATE_TEST_H