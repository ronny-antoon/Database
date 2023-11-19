#ifndef INTEGRATED_SET_TEST_HPP
#define INTEGRATED_SET_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <string>
#include "DatabaseAPI.hpp"
#include "NVSDelegate.hpp"

// Integrated test suite for DatabaseAPI::set
class IntegratedSetTest : public ::testing::Test
{
protected:
    int startFreeHeap = 0;
    int memoryLeak = 0;

    void SetUp() override
    {
        // Get the free heap before each test
        delay(10);
        startFreeHeap = ESP.getFreeHeap();
        delay(10);

        // Initialize the database API with the actual NVS implementation
        nvsDelegate = new NVSDelegate();
        databaseAPI = new DatabaseAPI(nvsDelegate, "testNamespace");
    }

    void TearDown() override
    {
        // Delete the database API
        NVSDelegateHandle_t handle;
        nvsDelegate->open("testNamespace", NVSDelegateOpenMode_t::NVSDelegate_READWRITE, &handle);
        nvsDelegate->erase_all(handle);
        nvsDelegate->close(handle);

        delete databaseAPI;
        delete nvsDelegate;

        // Calculate the memory leak
        delay(10);
        memoryLeak = ESP.getFreeHeap() - startFreeHeap;
        delay(10);

        if (memoryLeak != 0)
            FAIL() << "Memory leak of " << memoryLeak << " bytes"; // Fail the test if there is a memory leak
    }

    DatabaseAPI *databaseAPI;
    NVSDelegate *nvsDelegate;
};

/** Integrated Testing of set Method of DatabaseAPI class
 * @brief Set the value associated with the given key using the actual NVS implementation.
 *
 * @param key - Key to set, include the null terminator.
 * @param value - Value to set, include the null terminator.
 *
 * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
 * - DATABASE_OK if the key-value pair was successfully set.
 * - DATABASE_KEY_INVALID if the key is invalid.
 * - DATABASE_VALUE_INVALID if the value is invalid.
 * - DATABASE_ERROR for internal error.
 */

TEST_F(IntegratedSetTest, DATABASE_OK)
{
    // Arrange
    const char *key = "test_key";
    const char *value = "integrated_value";

    // Act
    DatabaseError_t err = databaseAPI->set(key, value);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);

    // Verify that the value is set correctly by using the get method
    char actualValue[NVS_DELEGATE_MAX_VALUE_LENGTH];
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    err = databaseAPI->get(key, actualValue, maxValueLength);

    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);
    EXPECT_STREQ(actualValue, value);
}

TEST_F(IntegratedSetTest, DATABASE_KEY_INVALID)
{
    // Arrange
    const char *key = "";
    const char *value = "integrated_value";

    // Act
    DatabaseError_t err = databaseAPI->set(key, value);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_INVALID);

    // Act
    err = databaseAPI->set(nullptr, value);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_INVALID);

    // Act
    err = databaseAPI->set("ToLongKeyInvalid", value);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_INVALID);
}

TEST_F(IntegratedSetTest, DATABASE_VALUE_INVALID)
{
    // Arrange
    const char *key = "test_key";
    const char *value = nullptr;

    // Act
    DatabaseError_t err = databaseAPI->set(key, value);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_VALUE_INVALID);

    // Act
    err = databaseAPI->set(key, "");

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_VALUE_INVALID);
}

TEST_F(IntegratedSetTest, DATABASE_ERROR)
{
    // Arrange
    DatabaseAPI databaseAPI(nullptr, "testNamespace");
    const char *key = "test_key";
    const char *value = "integrated_value";

    // Act
    DatabaseError_t err = databaseAPI.set(key, value);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);
}

#endif // INTEGRATED_SET_TEST_HPP
