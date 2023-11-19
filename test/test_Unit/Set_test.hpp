#ifndef UNIT_SET_TEST_HPP
#define UNIT_SET_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockingClass.hpp"
#include "DatabaseAPI.hpp"
#include "NVSDelegateInterface.hpp"

// setup test suite
class SetTest : public ::testing::Test
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

/** Testing set Method of DatabaseAPI class
 * @brief Set the value associated with the given key.
 *
 * @param key - Key to set, include the null terminator.
 * @param value - Value to set, include the null terminator.
 *
 * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
 * - DATABASE_OK if the key-value pair was successfully set.
 * - DATABASE_KEY_INVALID if the key is invalid.
 * - DATABASE_VALUE_INVALID if the value is invalid.
 * - DATABASE_ERROR for internal error.
 *
 *  test all possible return values
 */

TEST_F(SetTest, DATABASE_OK)
{
    // arrange
    const char *key = "key";
    const char *value = "value";

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READWRITE, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, set_str(::testing::_, testing::StrEq(key), testing::StrEq(value)))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_)).Times(1);

    // act
    DatabaseError_t err = databaseAPI->set(key, value);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);
}

TEST_F(SetTest, DATABASE_KEY_INVALID)
{
    // arrange
    const char *key = "";

    // act
    DatabaseError_t err1 = databaseAPI->set(key, "value");
    DatabaseError_t err2 = databaseAPI->set(nullptr, "value");
    // assert
    EXPECT_EQ(err1, DatabaseError_t::DATABASE_KEY_INVALID);
    EXPECT_EQ(err2, DatabaseError_t::DATABASE_KEY_INVALID);
}

TEST_F(SetTest, DATABASE_VALUE_INVALID)
{
    // arrange
    const char *key = "key";

    // act
    DatabaseError_t err = databaseAPI->set(key, nullptr);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_VALUE_INVALID);
}

TEST_F(SetTest, DATABASE_ERROR)
{
    // arrange
    const char *key = "key";
    const char *value = "value";

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READWRITE, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR));

    // act
    DatabaseError_t err = databaseAPI->set(key, value);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);

    // arrange
    DatabaseAPI databaseAPI(nullptr, "TEST_NVS");
    // act
    err = databaseAPI.set(key, value);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);
}

#endif // UNIT_SET_TEST_HPP
