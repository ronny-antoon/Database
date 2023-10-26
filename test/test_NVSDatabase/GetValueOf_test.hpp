#ifndef RELEASE
#ifndef GET_VALUE_OF_TEST_H
#define GET_VALUE_OF_TEST_H

#include <Arduino.h>
#include <gtest/gtest.h>
#include "NVSDatabase.hpp"

class GetValueOf_test : public ::testing::Test
{
protected:
    NVSDatabase *nvsDb;
    void SetUp() override
    {
        nvsDb = new NVSDatabase("test_namespace");
    }

    void TearDown() override
    {
        nvsDb->eraseAll();
        delete nvsDb;
    }
};

// Test case: Attempt to retrieve a value with an empty or null key.
TEST_F(GetValueOf_test, Empty_Null_Key)
{
    char value[100];
    size_t maxSize = 100;
    // Call the method with an empty key
    DatabaseError_t result = nvsDb->getValueOf("", value, &maxSize);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_EMPTY);

    // Call the method with an null key
    result = nvsDb->getValueOf(nullptr, value, &maxSize);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_EMPTY);
}

// Test case: Attempt to retrieve a value with a key that exceeds the maximum allowed size.
TEST_F(GetValueOf_test, Invalid_Size_Key)
{
    char value[100];
    size_t maxSize = 100;
    // Create a key that exceeds the maximum size
    const char *key = "ThisIsAnInvalidKeyThatExceedsTheMaximumSize";
    // Call the method with an invalid key
    DatabaseError_t result = nvsDb->getValueOf(key, value, &maxSize);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID_SIZE);
}

// Test case: Attempt to retrieve a value with a buffer size that is too small.
TEST_F(GetValueOf_test, Value_Buffer_Too_Small)
{
    // Add a key-value pair
    nvsDb->putPair("small_buffer", "small_buffer_value");

    char value[10]; // Buffer size is too small
    size_t maxSize = 10;
    // Call the method with a value buffer that is too small
    DatabaseError_t result = nvsDb->getValueOf("small_buffer", value, &maxSize);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID_SIZE);
}

// Test case: Attempt to retrieve a value with a null value buffer.
TEST_F(GetValueOf_test, Null_Value_Buffer)
{
    size_t maxSize = 100;
    // Call the method with a null value buffer
    DatabaseError_t result = nvsDb->getValueOf("null_value", nullptr, &maxSize);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_ERROR);
}

// Test case: Attempt to retrieve a value with a null maxSize.
TEST_F(GetValueOf_test, Null_MaxSize)
{
    char value[100];
    size_t maxSize = 100;
    // Call the method with a null maxSize
    DatabaseError_t result = nvsDb->getValueOf("null_maxsize", value, nullptr);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_ERROR);
}

// Test case: Successfully retrieve a value with an existing key.
TEST_F(GetValueOf_test, Existing_Key)
{
    // Add a key-value pair
    nvsDb->putPair("existing_key", "existing_value");

    char value[100];
    size_t maxSize = 100;
    // Call the method to retrieve a value for an existing key
    DatabaseError_t result = nvsDb->getValueOf("existing_key", value, &maxSize);

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);
    // Ensure the retrieved value matches the expected value
    ASSERT_STREQ(value, "existing_value");
}

// Test case: Attempt to retrieve a value for a non-existing key.
TEST_F(GetValueOf_test, NonExisting_Key)
{
    char value[100];
    size_t maxSize = 100;
    // Call the method to retrieve a value for a non-existing key
    DatabaseError_t result = nvsDb->getValueOf("non_existing_key", value, &maxSize);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_NOT_FOUND);
}

// Test case: Successfully retrieve a value and ensure it matches the expected value.
TEST_F(GetValueOf_test, Value_Retrieved_Correctly)
{
    // Add a key-value pair
    nvsDb->putPair("key_to_retrieve", "value_to_retrieve");

    char value[100];
    size_t maxSize = 100;
    // Call the method to retrieve a value and check if it matches the expected value
    DatabaseError_t result = nvsDb->getValueOf("key_to_retrieve", value, &maxSize);

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);
    // Ensure the retrieved value matches the expected value
    ASSERT_STREQ(value, "value_to_retrieve");
}

#endif
#endif
