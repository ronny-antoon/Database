#ifndef REMOVE_TEST_H
#define REMOVE_TEST_H

#include <gtest/gtest.h>

#include "NVSDelegate.hpp"

class Remove_test : public ::testing::Test
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

TEST_F(Remove_test, Empty_Null_Key)
{
    // remove a key from the database
    DatabaseError_t result = nvsDelegate->remove("");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);

    // remove a key from the database
    result = nvsDelegate->remove(nullptr);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

TEST_F(Remove_test, Key_Length_Too_Long)
{
    // remove a key from the database
    DatabaseError_t result = nvsDelegate->remove("12345678901234567");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_INVALID);
}

TEST_F(Remove_test, Key_Exist)
{
    // Create a key-value pair in the database
    DatabaseError_t result = nvsDelegate->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // remove a key from the database
    result = nvsDelegate->remove("key");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);
}

TEST_F(Remove_test, Key_Not_Exist)
{
    // Remove a key pair from the database
    DatabaseError_t result = nvsDelegate->remove("key");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_NOT_FOUND);
}
#endif // REMOVE_TEST_H