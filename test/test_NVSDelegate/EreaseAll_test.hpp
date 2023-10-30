#ifndef ERASE_ALL_TEST_H
#define ERASE_ALL_TEST_H

#include <gtest/gtest.h>

#include "NVSDelegate.hpp"

class EraseAll_test : public ::testing::Test
{
protected:
    NVSDelegate *nvsDelegate;
    void SetUp() override
    {
        nvsDelegate = new NVSDelegate("test_namespace");
    }

    void TearDown() override
    {
        nvsDelegate->eraseAll();
        delete nvsDelegate;
    }
};

TEST_F(EraseAll_test, Check_If_Erease_Done)
{
    // Insert a key-value pair into the database
    DatabaseError_t result = nvsDelegate->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // Erase all the data from the database
    result = nvsDelegate->eraseAll();

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // get a value from the database
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    char value[valueLength];
    result = nvsDelegate->get("key", value, &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}

#endif // ERASE_ALL_TEST_H