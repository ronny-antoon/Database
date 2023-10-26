#ifndef RELEASE
#ifndef ERASE_ALL_TEST_H
#define ERASE_ALL_TEST_H

#include <Arduino.h>
#include <gtest/gtest.h>
#include "NVSDatabase.hpp"

class EraseAll_test : public ::testing::Test
{
protected:
    NVSDatabase *nvsDb;
    void SetUp() override
    {
        nvsDb = new NVSDatabase("test_namespace");
    }

    void TearDown() override
    {
        delete nvsDb;
    }
};

// Test case: Successfully erase all key-value pairs within the namespace.
TEST_F(EraseAll_test, Erase_All_Keys)
{
    // Add some key-value pairs
    nvsDb->putPair("key1", "value1");
    nvsDb->putPair("key2", "value2");

    // Call the method to erase all key-value pairs within the namespace
    DatabaseError_t result = nvsDb->eraseAll();

    // Ensure the result matches the expected success code
    ASSERT_EQ(result, DATABASE_OK);

    // Attempt to check if key1 exists (should not exist after erasing all)
    result = nvsDb->isExist("key1");
    ASSERT_EQ(result, DATABASE_NOT_FOUND);

    // Attempt to check if key2 exists (should not exist after erasing all)
    result = nvsDb->isExist("key2");
    ASSERT_EQ(result, DATABASE_NOT_FOUND);
}

#endif
#endif
