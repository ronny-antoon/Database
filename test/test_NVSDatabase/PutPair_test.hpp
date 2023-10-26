#ifndef RELEASE
#ifndef PUT_PAIR_TEST_H
#define PUT_PAIR_TEST_H

#include <Arduino.h>
#include <gtest/gtest.h>
#include "NVSDatabase.hpp"

class PutPair_test : public ::testing::Test
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

// Test case: Attempt to store a key-value pair with an empty or null key.
TEST_F(PutPair_test, Empty_Null_Key)
{
    // Call the method with an empty key and a non-empty value
    DatabaseError_t result = nvsDb->putPair("", "non_empty_value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_EMPTY);

    // Call the method with a null key and a non-empty value
    result = nvsDb->putPair(nullptr, "non_empty_value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_EMPTY);
}

// Test case: Attempt to store a key-value pair with a key that exceeds the maximum allowed size.
TEST_F(PutPair_test, Invalid_Size_Key)
{
    // Create a key that exceeds the maximum size
    const char *key = "ThisIsAnInvalidKeyThatExceedsTheMaximumSize";
    // Call the method with an invalid key and a non-empty value
    DatabaseError_t result = nvsDb->putPair(key, "non_empty_value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID_SIZE);
}

// Test case: Attempt to store a key-value pair with an empty or null value.
TEST_F(PutPair_test, Empty_Null_Value)
{
    // Call the method with a non-empty key and an empty value
    DatabaseError_t result = nvsDb->putPair("non_empty_key", "");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_EMPTY);

    // Call the method with a non-empty key and a null value
    result = nvsDb->putPair("non_empty_key", nullptr);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_EMPTY);
}

// Test case: Attempt to store a key-value pair with a value that exceeds the maximum allowed size.
TEST_F(PutPair_test, Invalid_Size_Value)
{
    // Create a value that exceeds the maximum size
    char value[5000];
    for (int i = 0; i < 5000 - 1; i++)
    {
        value[i] = 'a';
    }
    value[5000 - 1] = '\0';
    // Call the method with a non-empty key and an invalid value
    DatabaseError_t result = nvsDb->putPair("non_empty_key", value);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID_SIZE);
}

// Test case: Successfully store a key-value pair with valid key and value.
TEST_F(PutPair_test, Valid_Key_Value)
{
    // Call the method to store a valid key-value pair
    DatabaseError_t result = nvsDb->putPair("valid_key", "valid_value");

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);
    // Check if the key-value pair was stored correctly
    char retrievedValue[100];
    size_t maxSize = 100;
    DatabaseError_t getResult = nvsDb->getValueOf("valid_key", retrievedValue, &maxSize);
    ASSERT_EQ(getResult, DATABASE_OK);
    ASSERT_STREQ(retrievedValue, "valid_value");
}

// Test case: Successfully store a key-value pair with existing key, and check if the value is updated.
TEST_F(PutPair_test, Existing_Key)
{
    // Call the method to store a valid key-value pair
    DatabaseError_t result = nvsDb->putPair("existing_key", "existing_value");

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);
    // Check if the key-value pair was stored correctly
    char retrievedValue[100];
    size_t maxSize = 100;
    DatabaseError_t getResult = nvsDb->getValueOf("existing_key", retrievedValue, &maxSize);
    ASSERT_EQ(getResult, DATABASE_OK);
    ASSERT_STREQ(retrievedValue, "existing_value");

    // Call the method to store a valid key-value pair with the same key
    result = nvsDb->putPair("existing_key", "updated_value");

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);
    // Check if the key-value pair was stored correctly
    getResult = nvsDb->getValueOf("existing_key", retrievedValue, &maxSize);
    ASSERT_EQ(getResult, DATABASE_OK);
    ASSERT_STREQ(retrievedValue, "updated_value");
}

#endif
#endif
