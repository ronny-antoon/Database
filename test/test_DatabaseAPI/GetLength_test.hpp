#ifndef GET_VALUE_LENGTH_OF_TEST_H
#define GET_VALUE_LENGTH_OF_TEST_H

#include <Arduino.h>
#include <gtest/gtest.h>

#include "DatabaseDelegateInterface.hpp"
#include "NVSDelegate.hpp"
#include "DatabaseAPIInterface.hpp"
#include "DatabaseAPI.hpp"
#include "DatabaseError.hpp"

class getLenght_API_test : public ::testing::Test
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
// Test case: Attempt to get the value length with an empty or null key.
TEST_F(getLenght_API_test, Empty_Null_Key)
{
    // Call the method with an empty key
    size_t requiredLength;
    DatabaseError_t result = myDatabase->getLength("", &requiredLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);

    // Call the method with a null key
    result = myDatabase->getLength(nullptr, &requiredLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

// Test case: Attempt to get the value length with a key that exceeds the maximum allowed size.
TEST_F(getLenght_API_test, Invalid_Size_Key)
{
    // Create a key that exceeds the maximum size
    const char *key = "ThisIsAnInvalidKeyThatExceedsTheMaximumSize";
    size_t requiredLength;

    // Call the method with an invalid key
    DatabaseError_t result = myDatabase->getLength(key, &requiredLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

// Test case: Attempt to get the value length for a non-existing key.
TEST_F(getLenght_API_test, NonExisting_Key)
{
    // Call the method to get the value length for a non-existing key
    size_t requiredLength;
    DatabaseError_t result = myDatabase->getLength("nonExistingKey", &requiredLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}

// Test case: Successfully get the value length with an existing key.
TEST_F(getLenght_API_test, Existing_Key)
{
    // Add a key-value pair
    myDatabase->set("existing_key", "existing_value");

    // Call the method to get the value length for an existing key
    size_t requiredLength;
    DatabaseError_t result = myDatabase->getLength("existing_key", &requiredLength);

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);
    // Ensure the retrieved value length matches the expected value length
    ASSERT_EQ(requiredLength, strlen("existing_value") + sizeof('\0')); // +1 for the null terminator
}

#endif
