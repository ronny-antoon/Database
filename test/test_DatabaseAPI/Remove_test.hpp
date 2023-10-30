#ifndef REMOVE_TEST_H
#define REMOVE_TEST_H

#include <Arduino.h>
#include <gtest/gtest.h>

#include "DatabaseDelegateInterface.hpp"
#include "NVSDelegate.hpp"
#include "DatabaseAPIInterface.hpp"
#include "DatabaseAPI.hpp"
#include "DatabaseError.hpp"

class Remove_API_test : public ::testing::Test
{
protected:
    DatabaseDelegateInterface *nvsDelegate;
    DatabaseAPIInterface *myDatabase;
    void SetUp() override
    {
        nvsDelegate = new NVSDelegate("test_namespace");
        myDatabase = new DatabaseAPI(nvsDelegate);
    }

    void TearDown() override
    {
        myDatabase->eraseAll();
        delete nvsDelegate;
        delete myDatabase;
    }
};

// Test case: Attempt to remove a pair with an empty or null key.
TEST_F(Remove_API_test, Empty_Null_Key)
{
    // Call the method with an empty key
    DatabaseError_t result = myDatabase->remove("");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);

    // Call the method with a null key
    result = myDatabase->remove(nullptr);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

// Test case: Attempt to remove a pair with a key that exceeds the maximum allowed size.
TEST_F(Remove_API_test, Invalid_Size_Key)
{
    // Create a key that exceeds the maximum size
    const char *key = "ThisIsAnInvalidKeyThatExceedsTheMaximumSize";

    // Call the method with an invalid key
    DatabaseError_t result = myDatabase->remove(key);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

// Test case: Attempt to remove a non-existing key-value pair.
TEST_F(Remove_API_test, NonExisting_Key)
{
    // Call the method to remove a non-existing key-value pair
    DatabaseError_t result = myDatabase->remove("nonExistingKey");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}

// Test case: Successfully remove an existing key-value pair.
TEST_F(Remove_API_test, Existing_Key)
{
    // Add a key-value pair
    myDatabase->set("existing_key", "existing_value");

    // Call the method to remove an existing key-value pair
    DatabaseError_t result = myDatabase->remove("existing_key");

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);

    // Call the method again to check if the key-value pair was removed
    result = myDatabase->isExist("existing_key");

    // Ensure the result matches the expected error code (key not found)
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}

#endif // REMOVE_TEST_H