#ifndef SET_TEST_H
#define SET_TEST_H

#include <Arduino.h>
#include <gtest/gtest.h>

#include "DatabaseDelegateInterface.hpp"
#include "NVSDelegate.hpp"
#include "DatabaseAPIInterface.hpp"
#include "DatabaseAPI.hpp"
#include "DatabaseError.hpp"

class Set_test : public ::testing::Test
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

// Test case: Attempt to store a key-value pair with an empty or null key.
TEST_F(Set_test, Empty_Null_Key)
{
    // Call the method with an empty key and a non-empty value
    DatabaseError_t result = myDatabase->set("", "non_empty_value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);

    // Call the method with a null key and a non-empty value
    result = myDatabase->set(nullptr, "non_empty_value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

// Test case: Attempt to store a key-value pair with a key that exceeds the maximum allowed size.
TEST_F(Set_test, Invalid_Size_Key)
{
    // Create a key that exceeds the maximum size
    const char *key = "ThisIsAnInvalidKeyThatExceedsTheMaximumSize";
    // Call the method with an invalid key and a non-empty value
    DatabaseError_t result = myDatabase->set(key, "non_empty_value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

// Test case: Attempt to store a key-value pair with an empty or null value.
TEST_F(Set_test, Empty_Null_Value)
{
    // Call the method with a non-empty key and an empty value
    DatabaseError_t result = myDatabase->set("non_empty_key", "");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);

    // Call the method with a non-empty key and a null value
    result = myDatabase->set("non_empty_key", nullptr);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);
}

// Test case: Attempt to store a key-value pair with a value that exceeds the maximum allowed size.
TEST_F(Set_test, Invalid_Size_Value)
{
    // Create a value that exceeds the maximum size
    char value[5000];
    for (int i = 0; i < 5000 - 1; i++)
    {
        value[i] = 'a';
    }
    value[5000 - 1] = '\0';
    // Call the method with a non-empty key and an invalid value
    DatabaseError_t result = myDatabase->set("non_empty_key", value);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);
}

// Test case: Successfully store a key-value pair with valid key and value.
TEST_F(Set_test, Valid_Key_Value)
{
    // Call the method to store a valid key-value pair
    DatabaseError_t result = myDatabase->set("valid_key", "valid_value");

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);
    // Check if the key-value pair was stored correctly
    char retrievedValue[100];
    size_t maxSize = 100;
    DatabaseError_t getResult = myDatabase->get("valid_key", retrievedValue, &maxSize);
    ASSERT_EQ(getResult, DATABASE_OK);
    ASSERT_STREQ(retrievedValue, "valid_value");
}

// Test case: Successfully store a key-value pair with existing key, and check if the value is updated.
TEST_F(Set_test, Existing_Key)
{
    // Call the method to store a valid key-value pair
    DatabaseError_t result = myDatabase->set("existing_key", "existing_value");

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);

    // Check if the key-value pair was stored correctly
    char retrievedValue[100];
    size_t maxSize = 100;
    DatabaseError_t getResult = myDatabase->get("existing_key", retrievedValue, &maxSize);
    ASSERT_EQ(getResult, DATABASE_OK);
    ASSERT_STREQ(retrievedValue, "existing_value");

    // Call the method to store a valid key-value pair with the same key
    result = myDatabase->set("existing_key", "updated_value");

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_KEY_ALREADY_EXISTS);

    // Check if the key-value pair was stored correctly
    getResult = myDatabase->get("existing_key", retrievedValue, &maxSize);
    ASSERT_EQ(getResult, DATABASE_OK);
    ASSERT_STREQ(retrievedValue, "existing_value");
}

#endif // SET_TEST_H
