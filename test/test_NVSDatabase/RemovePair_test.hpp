#ifndef RELEASE
#ifndef REMOVE_PAIR_TEST_H
#define REMOVE_PAIR_TEST_H

#include <Arduino.h>
#include <gtest/gtest.h>
#include "NVSDatabase.hpp"

class RemovePair_test : public ::testing::Test
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

// Test case: Attempt to remove a pair with an empty or null key.
TEST_F(RemovePair_test, Empty_Null_Key)
{
    // Call the method with an empty key
    DatabaseError_t result = nvsDb->removePair("");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_EMPTY);

    // Call the method with a null key
    result = nvsDb->removePair(nullptr);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_EMPTY);
}

// Test case: Attempt to remove a pair with a key that exceeds the maximum allowed size.
TEST_F(RemovePair_test, Invalid_Size_Key)
{
    // Create a key that exceeds the maximum size
    const char *key = "ThisIsAnInvalidKeyThatExceedsTheMaximumSize";

    // Call the method with an invalid key
    DatabaseError_t result = nvsDb->removePair(key);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID_SIZE);
}

// Test case: Attempt to remove a non-existing key-value pair.
TEST_F(RemovePair_test, NonExisting_Key)
{
    // Call the method to remove a non-existing key-value pair
    DatabaseError_t result = nvsDb->removePair("non_existing_key");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_NOT_FOUND);
}

// Test case: Successfully remove an existing key-value pair.
TEST_F(RemovePair_test, Existing_Key)
{
    // Add a key-value pair
    nvsDb->putPair("existing_key", "existing_value");

    // Call the method to remove an existing key-value pair
    DatabaseError_t result = nvsDb->removePair("existing_key");

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);

    // Call the method again to check if the key-value pair was removed
    result = nvsDb->isExist("existing_key");

    // Ensure the result matches the expected error code (key not found)
    ASSERT_EQ(result, DATABASE_NOT_FOUND);
}

#endif
#endif
