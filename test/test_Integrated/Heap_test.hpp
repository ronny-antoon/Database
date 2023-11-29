#ifndef INTEGRATED_HEAP_TEST_HPP
#define INTEGRATED_HEAP_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <string>
#include "DatabaseAPI.hpp"
#include "NVSDelegate.hpp"

// Integrated test suite for DatabaseAPI::heap
class IntegratedHeapTest : public ::testing::Test
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

/**
 *  Integrated Testing set and eraseAll Methods, with check for memory leak
 */
TEST_F(IntegratedHeapTest, set_AND_eraseAll)
{
    // Arrange
    const char *key = "test_key";
    const char *value = "integrated_value";
    DatabaseError_t result;

    result = databaseAPI->set(key, value);
    EXPECT_EQ(result, DatabaseError_t::DATABASE_OK);

    result = databaseAPI->eraseAll();
    EXPECT_EQ(result, DatabaseError_t::DATABASE_OK);

    int heap = ESP.getMaxAllocHeap();
    for (int i = 0; i < 100; i++)
    {
        databaseAPI->set(key, value);
        databaseAPI->eraseAll();
    }
    EXPECT_EQ(heap, ESP.getMaxAllocHeap());
}

/**
 *  Integrated Testing set and get and remove Methods, with check for memory leak
 */
TEST_F(IntegratedHeapTest, set_AND_get_AND_remove)
{
    // Arrange
    const char *key = "test_key";
    const char *value = "integrated_value";
    char actualValue[NVS_DELEGATE_MAX_VALUE_LENGTH];
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;

    databaseAPI->set(key, value);

    databaseAPI->get(key, actualValue, 100);
    EXPECT_STREQ(actualValue, value);

    databaseAPI->remove(key);

    int heap = ESP.getMaxAllocHeap();
    for (int i = 0; i < 100; i++)
    {
        databaseAPI->set(key, value);
        databaseAPI->get(key, actualValue, 100);
        databaseAPI->remove(key);
    }
    EXPECT_EQ(heap, ESP.getMaxAllocHeap());
}

/**
 *  Integrated Testing getValueLength and isExist Methods, with check for memory leak
 */
TEST_F(IntegratedHeapTest, getValueLength_AND_isExist)
{
    // Arrange
    const char *key = "test_key";
    const char *value = "integrated_value";
    size_t valueLength = 0;
    DatabaseError_t result;

    result = databaseAPI->set(key, value);
    EXPECT_EQ(result, DatabaseError_t::DATABASE_OK);

    result = databaseAPI->isExist(key);
    EXPECT_EQ(result, DatabaseError_t::DATABASE_OK);

    result = databaseAPI->getValueLength(key, &valueLength);
    EXPECT_EQ(result, DatabaseError_t::DATABASE_OK);
    EXPECT_EQ(valueLength, (strlen(value)) + 1);

    result = databaseAPI->eraseAll();
    EXPECT_EQ(result, DatabaseError_t::DATABASE_OK);
}

#endif // INTEGRATED_HEAP_TEST_HPP