#ifndef RELEASE
#ifndef GET_VALUE_LENGTH_OF_TEST_H
#define GET_VALUE_LENGTH_OF_TEST_H

#include <Arduino.h>
#include <gtest/gtest.h>
#include "NVSDatabase.hpp"

class GetValueLengthOf_test : public ::testing::Test
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

// Test case: Attempt to get the value length with an empty or null key.
TEST_F(GetValueLengthOf_test, Empty_Null_Key)
{
    // Call the method with an empty key
    size_t requiredLength;
    DatabaseError_t result = nvsDb->getValueLengthOf("", &requiredLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_EMPTY);

    // Call the method with a null key
    result = nvsDb->getValueLengthOf(nullptr, &requiredLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_EMPTY);
}

// Test case: Attempt to get the value length with a key that exceeds the maximum allowed size.
TEST_F(GetValueLengthOf_test, Invalid_Size_Key)
{
    // Create a key that exceeds the maximum size
    const char *key = "ThisIsAnInvalidKeyThatExceedsTheMaximumSize";
    size_t requiredLength;

    // Call the method with an invalid key
    DatabaseError_t result = nvsDb->getValueLengthOf(key, &requiredLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID_SIZE);
}

// Test case: Attempt to get the value length for a non-existing key.
TEST_F(GetValueLengthOf_test, NonExisting_Key)
{
    // Call the method to get the value length for a non-existing key
    size_t requiredLength;
    DatabaseError_t result = nvsDb->getValueLengthOf("non_existing_key", &requiredLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_NOT_FOUND);
}

// Test case: Successfully get the value length with an existing key.
TEST_F(GetValueLengthOf_test, Existing_Key)
{
    // Add a key-value pair
    nvsDb->putPair("existing_key", "existing_value");

    // Call the method to get the value length for an existing key
    size_t requiredLength;
    DatabaseError_t result = nvsDb->getValueLengthOf("existing_key", &requiredLength);

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);
    // Ensure the retrieved value length matches the expected value length
    ASSERT_EQ(requiredLength, strlen("existing_value") + sizeof('\0')); // +1 for the null terminator
}

#endif
#endif
