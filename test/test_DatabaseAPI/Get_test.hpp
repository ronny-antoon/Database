#ifndef GET_VALUE_OF_TEST_H
#define GET_VALUE_OF_TEST_H

#include <Arduino.h>
#include <gtest/gtest.h>

#include "DatabaseDelegateInterface.hpp"
#include "NVSDelegate.hpp"
#include "DatabaseAPIInterface.hpp"
#include "DatabaseAPI.hpp"
#include "DatabaseError.hpp"

class Get_API_test : public ::testing::Test
{
protected:
    DatabaseDelegateInterface *nvsDelegate;
    DatabaseAPIInterface *myDatabase;
    uint32_t _startFreeHeap = 0;
    void SetUp() override
    {
        _startFreeHeap = ESP.getFreeHeap();
        nvsDelegate = new NVSDelegate("test_namespace");
        myDatabase = new DatabaseAPI(nvsDelegate);
    }

    void TearDown() override
    {
        myDatabase->eraseAll();
        delete nvsDelegate;
        delete myDatabase;
        nvsDelegate = nullptr;
        myDatabase = nullptr;
        int heapDiff = ESP.getFreeHeap() - _startFreeHeap;
        if (heapDiff != 0)
            FAIL() << "Memory leak of " << heapDiff << " bytes"; // Fail the test if there is a memory leak
    }
};

// Test case: Attempt to retrieve a value with an empty or null key.
TEST_F(Get_API_test, Empty_Null_Key)
{
    char value[100];
    size_t maxSize = 100;
    // Call the method with an empty key
    DatabaseError_t result = myDatabase->get("", value, &maxSize);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);

    // Call the method with an null key
    result = myDatabase->get(nullptr, value, &maxSize);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

// Test case: Attempt to retrieve a value with a key that exceeds the maximum allowed size.
TEST_F(Get_API_test, Invalid_Size_Key)
{
    char value[100];
    size_t maxSize = 100;
    // Create a key that exceeds the maximum size
    const char *key = "ThisIsAnInvalidKeyThatExceedsTheMaximumSize";
    // Call the method with an invalid key
    DatabaseError_t result = myDatabase->get(key, value, &maxSize);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

// Test case: Attempt to retrieve a value with a buffer size that is too small.
TEST_F(Get_API_test, Value_Buffer_Too_Small)
{
    // Add a key-value pair
    myDatabase->set("small_buffer", "small_buffer_value");

    char value[10]; // Buffer size is too small
    size_t maxSize = 10;
    // Call the method with a value buffer that is too small
    DatabaseError_t result = myDatabase->get("small_buffer", value, &maxSize);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);
}

// Test case: Attempt to retrieve a value with a null value buffer.
TEST_F(Get_API_test, Null_Value_Buffer)
{
    // Insert a key-value pair
    DatabaseError_t result = myDatabase->set("null_value", "null_value");

    // Check if the insertion was successful
    ASSERT_EQ(result, DATABASE_OK);

    size_t maxSize = 100;
    // Call the method with a null value buffer
    result = myDatabase->get("null_value", nullptr, &maxSize);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);
}

// Test case: Attempt to retrieve a value with a null maxSize.
TEST_F(Get_API_test, Null_MaxSize)
{
    // Insert a key-value pair
    DatabaseError_t result = myDatabase->set("null_maxsize", "null_maxsize");

    // Check if the insertion was successful
    ASSERT_EQ(result, DATABASE_OK);

    char value[100];
    size_t maxSize = 100;
    // Call the method with a null maxSize
    result = myDatabase->get("null_maxsize", value, nullptr);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);
}

// Test case: Successfully retrieve a value with an existing key.
TEST_F(Get_API_test, Existing_Key)
{
    // Add a key-value pair
    myDatabase->set("existing_key", "existing_value");

    char value[100];
    size_t maxSize = 100;
    // Call the method to retrieve a value for an existing key
    DatabaseError_t result = myDatabase->get("existing_key", value, &maxSize);

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);
    // Ensure the retrieved value matches the expected value
    ASSERT_STREQ(value, "existing_value");
}

// Test case: Attempt to retrieve a value for a non-existing key.
TEST_F(Get_API_test, NonExisting_Key)
{
    char value[100];
    size_t maxSize = 100;
    // Call the method to retrieve a value for a non-existing key
    DatabaseError_t result = myDatabase->get("nonExistingKey", value, &maxSize);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}

// Test case: Successfully retrieve a value and ensure it matches the expected value.
TEST_F(Get_API_test, Value_Retrieved_Correctly)
{
    // Add a key-value pair
    myDatabase->set("key_to_retrieve", "value_to_retrieve");

    char value[100];
    size_t maxSize = 100;
    // Call the method to retrieve a value and check if it matches the expected value
    DatabaseError_t result = myDatabase->get("key_to_retrieve", value, &maxSize);

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);
    // Ensure the retrieved value matches the expected value
    ASSERT_STREQ(value, "value_to_retrieve");
}

#endif