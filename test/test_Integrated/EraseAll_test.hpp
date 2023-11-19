#ifndef INTEGRATED_ERASEALL_TEST_HPP
#define INTEGRATED_ERASEALL_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <string>
#include "DatabaseAPI.hpp"
#include "NVSDelegate.hpp"

// Integrated test suite for DatabaseAPI::eraseAll
class IntegratedEraseAllTest : public ::testing::Test
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

/** Integrated Testing of eraseAll Method of DatabaseAPI class
 * @brief Erase all key-value pairs in the database using the actual NVS implementation.
 *
 * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
 * - DATABASE_OK if all key-value pairs are successfully erased.
 * - DATABASE_ERROR for internal error.
 */

TEST_F(IntegratedEraseAllTest, DATABASE_OK)
{
    // Arrange
    const char *key = "test_key";
    const char *value = "integrated_value";

    // Store a key-value pair in the actual NVS implementation
    databaseAPI->set(key, value);

    // Act
    DatabaseError_t err = databaseAPI->eraseAll();

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);

    // Verify that the database is empty by attempting to get the key
    char actualValue[NVS_DELEGATE_MAX_VALUE_LENGTH];
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    err = databaseAPI->get(key, actualValue, maxValueLength);

    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_NOT_FOUND);
}

TEST_F(IntegratedEraseAllTest, DATABASE_ERROR)
{
    // Arrange
    DatabaseAPI databaseAPI(nullptr, "testNamespace");

    // Act
    DatabaseError_t err = databaseAPI.eraseAll();

    // Assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);
}

#endif // INTEGRATED_ERASEALL_TEST_HPP
