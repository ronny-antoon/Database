#ifndef CONSTRUCTOR_TEST_H
#define CONSTRUCTOR_TEST_H

#include <gtest/gtest.h>

#include "NVSDelegate.hpp"

TEST(Constructor_test, Empty_Null_Namespace)
{
    // Create a new NVSDelegate with an empty namespace
    NVSDelegate *nvsDelegate = new NVSDelegate("");

    // Create a new NVSDelegate with a null namespace
    NVSDelegate *nvsDelegate2 = new NVSDelegate(nullptr);

    // Insert a key-value into the database1
    DatabaseError_t result = nvsDelegate->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // Insert the same key-value into the database2
    result = nvsDelegate2->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_ALREADY_EXISTS);

    // Erase all the data from the database1 and database2
    result = nvsDelegate->eraseAll();
    ASSERT_EQ(result, DATABASE_OK);
    result = nvsDelegate2->eraseAll();
    ASSERT_EQ(result, DATABASE_OK);

    // Delete all delegates
    delete nvsDelegate;
    delete nvsDelegate2;
}

TEST(Constructor_test, Two_With_The_Same_Namespace)
{
    // Create a new NVSDelegate with a namespace
    NVSDelegate *nvsDelegate = new NVSDelegate("test_namespace");

    // Create a new NVSDelegate with the same namespace
    NVSDelegate *nvsDelegate2 = new NVSDelegate("test_namespace");

    // Insert a key-value into the database1
    DatabaseError_t result = nvsDelegate->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // Insert the same key-value into the database2
    result = nvsDelegate2->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_KEY_ALREADY_EXISTS);

    // Erase all the data from the database1 and database2
    result = nvsDelegate->eraseAll();
    ASSERT_EQ(result, DATABASE_OK);
    result = nvsDelegate2->eraseAll();
    ASSERT_EQ(result, DATABASE_OK);

    // Delete all delegates
    delete nvsDelegate;
    delete nvsDelegate2;
}

TEST(Constructor_test, Max_Partition_Name_Length)
{
    // Create a new NVSDelegate with a namespace
    NVSDelegate *nvsDelegate = new NVSDelegate("12345678901234567");

    // Insert a key-value into the database
    DatabaseError_t result = nvsDelegate->insert("key", "value");

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // Get the value from the database
    size_t valueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    char value[valueLength];
    result = nvsDelegate->get("key", value, &valueLength);

    // Ensure the result matches the expected error code
    ASSERT_EQ(result, DATABASE_OK);

    // Ensure the value matches the expected value
    ASSERT_STREQ(value, "value");

    // Erase all the data from the database
    result = nvsDelegate->eraseAll();
    ASSERT_EQ(result, DATABASE_OK);

    // Delete all delegates
    delete nvsDelegate;
}
#endif // CONSTRUCTOR_TEST_H