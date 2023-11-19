#ifndef INTEGRATED_GETVALUELENGTH_TEST_HPP
#define INTEGRATED_GETVALUELENGTH_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <string>
#include "DatabaseAPI.hpp"
#include "NVSDelegate.hpp"

// Integrated test suite for DatabaseAPI::getValueLength
class IntegratedGetValueLengthTest : public ::testing::Test
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

/** Integrated Testing of getValueLength Method of DatabaseAPI class
 * @brief Get the length of the value associated with the given key using the actual NVS implementation.
 *
 * @param key - Key to search for, include the null terminator.
 *
 * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
 * - DATABASE_OK if the length is successfully obtained.
 * - DATABASE_KEY_INVALID if the key is invalid.
 * - DATABASE_KEY_NOT_FOUND if the key was not found.
 * - DATABASE_ERROR for internal error.
 */

TEST_F(IntegratedGetValueLengthTest, DATABASE_OK)
{
    // Arrange
    const char *key = "test_key";
    const char *value = "integrated_value";

    // Store the key-value pair in the actual NVS implementation
    databaseAPI->set(key, value);

    // Act
    size_t valueLength = 0;
    DatabaseError_t err = databaseAPI->getValueLength(key, &valueLength);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);
    EXPECT_EQ(valueLength, (strlen(value)) + 1);
}

TEST_F(IntegratedGetValueLengthTest, DATABASE_KEY_INVALID)
{
    // Arrange
    const char *key = "";

    // Act
    size_t valueLength = 0;
    DatabaseError_t err = databaseAPI->getValueLength(key, &valueLength);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_INVALID);

    // Act
    err = databaseAPI->getValueLength(nullptr, &valueLength);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_INVALID);

    // Act
    err = databaseAPI->getValueLength("ToLongKeyInvalid", &valueLength);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_INVALID);
}

TEST_F(IntegratedGetValueLengthTest, DATABASE_KEY_NOT_FOUND)
{
    // Arrange
    const char *key = "nonexistent_key";

    // Act
    size_t valueLength = 0;
    DatabaseError_t err = databaseAPI->getValueLength(key, &valueLength);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_NOT_FOUND);
}

TEST_F(IntegratedGetValueLengthTest, DATABASE_ERROR)
{
    // Arrange
    DatabaseAPI databaseAPI(nullptr, "testNamespace");
    const char *key = "test_key";

    // Act
    size_t valueLength = 0;
    DatabaseError_t err = databaseAPI.getValueLength(key, &valueLength);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);
}

#endif // INTEGRATED_GETVALUELENGTH_TEST_HPP
