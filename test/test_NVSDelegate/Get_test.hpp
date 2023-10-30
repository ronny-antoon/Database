#ifndef GET_TEST_H
#define GET_TEST_H

#include <gtest/gtest.h>

#include "NVSDelegate.hpp"

class Get_test : public ::testing::Test
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

TEST_F(Get_test, Empty_Null_Key)
{
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    // get a value from the database
    char value[valueLength];
    DatabaseError_t result = nvsDelegate->get("", value, &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);

    // get a value from the database
    result = nvsDelegate->get(nullptr, value, &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

TEST_F(Get_test, Null_Value)
{
    // Insert a key-value pair into the database
    DatabaseError_t result = nvsDelegate->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    // get a value from the database
    result = nvsDelegate->get("key", nullptr, &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_VALUE_INVALID);
}

TEST_F(Get_test, Key_Length_Too_Long)
{
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    // get a value from the database
    char value[valueLength];
    DatabaseError_t result = nvsDelegate->get("12345678901234567", value, &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

TEST_F(Get_test, Key_Exist)
{
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    // Create a key-value pair in the database
    DatabaseError_t result = nvsDelegate->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // get a value from the database
    char value[valueLength];
    result = nvsDelegate->get("key", value, &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // Ensure the value matches the expected value
    ASSERT_STREQ(value, "value");
}

TEST_F(Get_test, Key_Not_Exist)
{
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    // get a value from the database
    char value[valueLength];
    DatabaseError_t result = nvsDelegate->get("key", value, &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}

#endif // GET_TEST_H