#ifndef IS_EXIST_TEST_H
#define IS_EXIST_TEST_H

#include <Arduino.h>
#include <gtest/gtest.h>

#include "DatabaseDelegateInterface.hpp"
#include "NVSDelegate.hpp"
#include "DatabaseAPIInterface.hpp"
#include "DatabaseAPI.hpp"
#include "DatabaseError.hpp"

class IsExist_API_test : public ::testing::Test
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
        if (ESP.getFreeHeap() != _startFreeHeap)
            FAIL() << "Memory leak of " << _startFreeHeap - ESP.getFreeHeap() << " bytes"; // Fail the test if there is a memory leak
    }
};

// Test case: Attempt to check if a value exists with an empty or null key.
TEST_F(IsExist_API_test, Empty_Null_Key)
{
    // Call the method with an empty key
    DatabaseError_t result = myDatabase->isExist("");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);

    // Call the method with a null key
    result = myDatabase->isExist(nullptr);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

// Test case: Attempt to check if a value exists with a key that exceeds the maximum allowed size.
TEST_F(IsExist_API_test, Invalid_Size_Key)
{
    // Create a key that exceeds the maximum size
    const char *key = "ThisIsAnInvalidKeyThatExceedsTheMaximumSize";

    // Call the method with an invalid key
    DatabaseError_t result = myDatabase->isExist(key);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

// Test case: Attempt to check if a value exists for a non-existing key.
TEST_F(IsExist_API_test, NonExisting_Key)
{
    // Call the method to check if a value exists for a non-existing key
    DatabaseError_t result = myDatabase->isExist("nonExistingKey");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}

// Test case: Successfully check if a value exists with an existing key.
TEST_F(IsExist_API_test, Existing_Key)
{
    // Add a key-value pair
    myDatabase->set("existing_key", "existing_value");

    // Call the method to check if a value exists for an existing key
    DatabaseError_t result = myDatabase->isExist("existing_key");

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);
}

#endif
