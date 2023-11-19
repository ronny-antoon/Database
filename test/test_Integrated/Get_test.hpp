#ifndef INTEGRATED_GET_TEST_HPP
#define INTEGRATED_GET_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <string>
#include "DatabaseAPI.hpp"
#include "NVSDelegate.hpp"

// Integrated test suite for DatabaseAPI::get
class IntegratedGetTest : public ::testing::Test
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

/** Integrated Testing of get Method of DatabaseAPI class
 * @brief Get the value associated with the given key using the actual NVS implementation.
 *
 * @param key - Key to search for, include the null terminator.
 * @param expectedValue - Expected value associated with the given key.
 * @param maxValueLength - Maximum size of the value.
 *
 * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
 * - DATABASE_OK if the key-value pair was successfully found.
 * - DATABASE_KEY_INVALID if the key is invalid.
 * - DATABASE_VALUE_INVALID if the value is invalid.
 * - DATABASE_KEY_NOT_FOUND if the key was not found.
 * - DATABASE_ERROR for internal error.
 *
 *  test all possible return values
 */

TEST_F(IntegratedGetTest, DATABASE_OK)
{
    // Arrange
    const char *key = "test_key";
    const char *expectedValue = "integrated_value";
    char actualValue[NVS_DELEGATE_MAX_VALUE_LENGTH];
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;

    // Store the key-value pair in the actual NVS implementation
    databaseAPI->set(key, expectedValue);

    // Act
    DatabaseError_t err = databaseAPI->get(key, actualValue, maxValueLength);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);
    EXPECT_STREQ(actualValue, expectedValue);
}

TEST_F(IntegratedGetTest, DATABASE_KEY_INVALID)
{
    // Arrange
    const char *key = "";
    char actualValue[NVS_DELEGATE_MAX_VALUE_LENGTH];
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;

    // Act
    DatabaseError_t err = databaseAPI->get(key, actualValue, 0);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_INVALID);
}

TEST_F(IntegratedGetTest, DATABASE_VALUE_INVALID)
{
    // Arrange
    const char *key = "test_key";
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;

    // Act
    DatabaseError_t err = databaseAPI->get(key, nullptr, maxValueLength);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_VALUE_INVALID);
}

TEST_F(IntegratedGetTest, DATABASE_KEY_NOT_FOUND)
{
    // Arrange
    const char *key = "test_key";
    char actualValue[NVS_DELEGATE_MAX_VALUE_LENGTH];
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;

    // Act
    DatabaseError_t err = databaseAPI->get(key, actualValue, maxValueLength);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_NOT_FOUND);
}

TEST_F(IntegratedGetTest, DATABASE_ERROR)
{
    // Arrange
    DatabaseAPI databaseAPI(nullptr, "testNamespace");
    const char *key = "test_key";
    char actualValue[NVS_DELEGATE_MAX_VALUE_LENGTH];
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;

    // Act
    DatabaseError_t err = databaseAPI.get(key, actualValue, maxValueLength);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);
}

#endif // INTEGRATED_GET_TEST_HPP
