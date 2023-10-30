#ifndef GET_LENGTH_TEST_H
#define GET_LENGTH_TEST_H

#include <gtest/gtest.h>

#include "NVSDelegate.hpp"

class GetLength_test : public ::testing::Test
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

TEST_F(GetLength_test, Empty_Null_Key)
{
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    // get a value from the database
    char value[valueLength];
    DatabaseError_t result = nvsDelegate->getLength("", &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);

    // get a value from the database
    result = nvsDelegate->getLength(nullptr, &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

TEST_F(GetLength_test, Key_Length_Too_Long)
{
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    // get a value from the database
    DatabaseError_t result = nvsDelegate->getLength("12345678901234567", &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

TEST_F(GetLength_test, Key_Not_Found)
{
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    // get a value from the database
    DatabaseError_t result = nvsDelegate->getLength("key", &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}

#endif // GET_LENGTH_TEST_H