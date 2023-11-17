#ifndef ERASE_ALL_TEST_H
#define ERASE_ALL_TEST_H

#include <Arduino.h>
#include <gtest/gtest.h>

#include "DatabaseDelegateInterface.hpp"
#include "NVSDelegate.hpp"
#include "DatabaseAPIInterface.hpp"
#include "DatabaseAPI.hpp"
#include "DatabaseError.hpp"

class EraseAll_API_test : public ::testing::Test
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

// Test case: Successfully erase all key-value pairs within the namespace.
TEST_F(EraseAll_API_test, Erase_All_Keys)
{
    // Add some key-value pairs
    myDatabase->set("key1", "value1");
    myDatabase->set("key2", "value2");

    // Call the method to erase all key-value pairs within the namespace
    DatabaseError_t result = myDatabase->eraseAll();

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);

    // Attempt to check if key1 exists (should not exist after erasing all)
    result = myDatabase->isExist("key1");
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);

    // Attempt to check if key2 exists (should not exist after erasing all)
    result = myDatabase->isExist("key2");
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}

#endif
