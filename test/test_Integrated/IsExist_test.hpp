#ifndef INTEGRATED_ISEXIST_TEST_HPP
#define INTEGRATED_ISEXIST_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <string>
#include "DatabaseAPI.hpp"
#include "NVSDelegate.hpp"

// Integrated test suite for DatabaseAPI::isExist
class IntegratedIsExistTest : public ::testing::Test
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

/** Integrated Testing of isExist Method of DatabaseAPI class
 * @brief Check if the key exists in the database using the actual NVS implementation.
 *
 * @param key - Key to search for, include the null terminator.
 *
 * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
 * - DATABASE_OK if the key exists.
 * - DATABASE_KEY_INVALID if the key is invalid.
 * - DATABASE_KEY_NOT_FOUND if the key was not found.
 * - DATABASE_ERROR for internal error.
 */

TEST_F(IntegratedIsExistTest, DATABASE_OK)
{
    // Arrange
    const char *key = "test_key";
    const char *value = "integrated_value";

    // Store the key-value pair in the actual NVS implementation
    databaseAPI->set(key, value);

    // Act
    DatabaseError_t err = databaseAPI->isExist(key);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);
}

TEST_F(IntegratedIsExistTest, DATABASE_KEY_INVALID)
{
    // Arrange
    const char *key = "";

    // Act
    DatabaseError_t err = databaseAPI->isExist(key);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_INVALID);

    // Act
    err = databaseAPI->isExist(nullptr);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_INVALID);

    // Act
    err = databaseAPI->isExist("ToLongKeyInvalid");

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_INVALID);
}

TEST_F(IntegratedIsExistTest, DATABASE_KEY_NOT_FOUND)
{
    // Arrange
    const char *key = "nonexistent_key";

    // Act
    DatabaseError_t err = databaseAPI->isExist(key);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_NOT_FOUND);
}

TEST_F(IntegratedIsExistTest, DATABASE_ERROR)
{
    // Arrange
    DatabaseAPI databaseAPI(nullptr, "testNamespace");
    const char *key = "test_key";

    // Act
    DatabaseError_t err = databaseAPI.isExist(key);

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);
}

#endif // INTEGRATED_ISEXIST_TEST_HPP
