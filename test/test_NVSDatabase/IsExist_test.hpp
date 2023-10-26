#ifndef RELEASE
#ifndef IS_EXIST_TEST_H
#define IS_EXIST_TEST_H

#include <Arduino.h>
#include <gtest/gtest.h>
#include "NVSDatabase.hpp"

class IsExist_test : public ::testing::Test
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
// Test case: Attempt to check if a value exists with an empty or null key.
TEST_F(IsExist_test, Empty_Null_Key)
{
    // Call the method with an empty key
    DatabaseError_t result = nvsDb->isExist("");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_EMPTY);

    // Call the method with a null key
    result = nvsDb->isExist(nullptr);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_EMPTY);
}

// Test case: Attempt to check if a value exists with a key that exceeds the maximum allowed size.
TEST_F(IsExist_test, Invalid_Size_Key)
{
    // Create a key that exceeds the maximum size
    const char *key = "ThisIsAnInvalidKeyThatExceedsTheMaximumSize";

    // Call the method with an invalid key
    DatabaseError_t result = nvsDb->isExist(key);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID_SIZE);
}

// Test case: Attempt to check if a value exists for a non-existing key.
TEST_F(IsExist_test, NonExisting_Key)
{
    // Call the method to check if a value exists for a non-existing key
    DatabaseError_t result = nvsDb->isExist("non_existing_key");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_NOT_FOUND);
}

// Test case: Successfully check if a value exists with an existing key.
TEST_F(IsExist_test, Existing_Key)
{
    // Add a key-value pair
    nvsDb->putPair("existing_key", "existing_value");

    // Call the method to check if a value exists for an existing key
    DatabaseError_t result = nvsDb->isExist("existing_key");

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);
}

#endif
#endif
