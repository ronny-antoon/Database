#ifndef UNIT_HEAP_TEST_HPP
#define UNIT_HEAP_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockingClass.hpp"
#include "DatabaseAPI.hpp"
#include "NVSDelegateInterface.hpp"

// setup test suite
class HeapTest : public ::testing::Test
{
protected:
    int _startFreeHeap;
    int _endFreeHeap;
    void SetUp() override
    {
        delay(10);
        _startFreeHeap = ESP.getFreeHeap();
        delay(10);
        // setup mock
        mockNVSDelegate = new MockNVSDelegate();
        databaseAPI = new DatabaseAPI(mockNVSDelegate, "TEST_NVS");
    }

    void TearDown() override
    {
        delete databaseAPI;
        delete mockNVSDelegate;

        delay(10);
        _endFreeHeap = ESP.getFreeHeap();
        delay(10);
        if (_startFreeHeap != _endFreeHeap)
            FAIL() << "Memory leak of " << (_startFreeHeap - _endFreeHeap) << " bytes"; // Fail the test if there is a memory leak
    }

    DatabaseAPI *databaseAPI;
    MockNVSDelegate *mockNVSDelegate;
};

/**
 * @brief Test get method, and check the max alloc heap size
 */
TEST_F(HeapTest, get_MaxHeapSize)
{
    // arrange
    const char *key = "key";
    const char *expectedValue = "value";
    char actualValue[NVS_DELEGATE_MAX_VALUE_LENGTH];
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;
    DatabaseError_t err;

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READONLY, ::testing::_))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, get_str(::testing::_, testing::StrEq(key), nullptr, ::testing::NotNull()))
        .WillRepeatedly(::testing::DoAll(::testing::SetArgPointee<3>(strlen(expectedValue)), ::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK)));

    EXPECT_CALL(*mockNVSDelegate, get_str(::testing::_, testing::StrEq(key), ::testing::NotNull(), ::testing::NotNull()))
        .WillRepeatedly(::testing::DoAll(::testing::SetArrayArgument<2>(expectedValue, expectedValue + strlen(expectedValue) + 1), ::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK)));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_))
        .WillRepeatedly(::testing::Return());

    // act
    err = databaseAPI->get(key, actualValue, maxValueLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);
    EXPECT_STREQ(actualValue, expectedValue);

    int maxHeapSize = ESP.getMaxAllocHeap();
    for (int i = 0; i < 100; i++)
    {
        // act
        err = databaseAPI->get(key, actualValue, maxValueLength);
        // assert
        ASSERT_EQ(err, DatabaseError_t::DATABASE_OK);
        ASSERT_STREQ(actualValue, expectedValue);
    }
    EXPECT_EQ(maxHeapSize, ESP.getMaxAllocHeap());
}

/**
 * @brief Test set method, and check the max alloc heap size
 */
TEST_F(HeapTest, set_MaxHeapSize)
{
    // arrange
    const char *key = "key";
    const char *value = "value";
    DatabaseError_t err;

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READWRITE, ::testing::_))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, set_str(::testing::_, testing::StrEq(key), testing::StrEq(value)))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_))
        .WillRepeatedly(::testing::Return());

    EXPECT_CALL(*mockNVSDelegate, commit(::testing::_))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    // act
    err = databaseAPI->set(key, value);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);

    int maxHeapSize = ESP.getMaxAllocHeap();
    for (int i = 0; i < 100; i++)
    {
        // act
        err = databaseAPI->set(key, value);
        // assert
        ASSERT_EQ(err, DatabaseError_t::DATABASE_OK);
    }
    EXPECT_EQ(maxHeapSize, ESP.getMaxAllocHeap());
}

/**
 * @brief Test remove method, and check the max alloc heap size
 */
TEST_F(HeapTest, remove_MaxHeapSize)
{
    // arrange
    const char *key = "key";
    DatabaseError_t err;

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READWRITE, ::testing::_))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, erase_key(::testing::_, testing::StrEq(key)))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_))
        .WillRepeatedly(::testing::Return());

    EXPECT_CALL(*mockNVSDelegate, commit(::testing::_))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    // act
    err = databaseAPI->remove(key);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);

    int maxHeapSize = ESP.getMaxAllocHeap();
    for (int i = 0; i < 100; i++)
    {
        // act
        err = databaseAPI->remove(key);
        // assert
        ASSERT_EQ(err, DatabaseError_t::DATABASE_OK);
    }
    EXPECT_EQ(maxHeapSize, ESP.getMaxAllocHeap());
}

// isExist
TEST_F(HeapTest, isExist_MaxHeapSize)
{
    // arrange
    const char *key = "key";
    DatabaseError_t err;

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READONLY, ::testing::_))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, get_str(::testing::_, testing::StrEq(key), nullptr, ::testing::NotNull()))
        .WillRepeatedly(::testing::DoAll(::testing::SetArgPointee<3>(5), ::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK)));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_))
        .WillRepeatedly(::testing::Return());

    // act
    err = databaseAPI->isExist(key);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);

    int maxHeapSize = ESP.getMaxAllocHeap();
    for (int i = 0; i < 100; i++)
    {
        // act
        err = databaseAPI->isExist(key);
        // assert
        ASSERT_EQ(err, DatabaseError_t::DATABASE_OK);
    }
    EXPECT_EQ(maxHeapSize, ESP.getMaxAllocHeap());
}

// getValueLength
TEST_F(HeapTest, getValueLength_MaxHeapSize)
{
    // arrange
    const char *key = "key";
    size_t requiredLength = 0;
    DatabaseError_t err;

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READONLY, ::testing::_))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, get_str(::testing::_, testing::StrEq(key), nullptr, ::testing::NotNull()))
        .WillRepeatedly(::testing::DoAll(::testing::SetArgPointee<3>(strlen(key)), ::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK)));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_))
        .WillRepeatedly(::testing::Return());

    // act
    err = databaseAPI->getValueLength(key, &requiredLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);
    EXPECT_EQ(requiredLength, strlen(key));

    int maxHeapSize = ESP.getMaxAllocHeap();
    for (int i = 0; i < 100; i++)
    {
        // act
        err = databaseAPI->getValueLength(key, &requiredLength);
        // assert
        ASSERT_EQ(err, DatabaseError_t::DATABASE_OK);
        ASSERT_EQ(requiredLength, strlen(key));
    }
    EXPECT_EQ(maxHeapSize, ESP.getMaxAllocHeap());
}

// eraseAll
TEST_F(HeapTest, eraseAll_MaxHeapSize)
{
    // arrange
    DatabaseError_t err;

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READWRITE, ::testing::_))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, erase_all(::testing::_))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_))
        .WillRepeatedly(::testing::Return());

    EXPECT_CALL(*mockNVSDelegate, commit(::testing::_))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    // act
    err = databaseAPI->eraseAll();
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);

    int maxHeapSize = ESP.getMaxAllocHeap();
    for (int i = 0; i < 100; i++)
    {
        // act
        err = databaseAPI->eraseAll();
        // assert
        ASSERT_EQ(err, DatabaseError_t::DATABASE_OK);
    }
    EXPECT_EQ(maxHeapSize, ESP.getMaxAllocHeap());
}

#endif // UNIT_HEAP_TEST_HPP