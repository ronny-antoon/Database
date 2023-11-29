#ifndef UNIT_GET_TEST_HPP
#define UNIT_GET_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockingClass.hpp"
#include "DatabaseAPI.hpp"
#include "NVSDelegateInterface.hpp"

// setup test suite
class GetTest : public ::testing::Test
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

/** Testing get Method of DatabaseAPI class
 * @brief Get the value associated with the given key.
 *
 * @param key - Key to search for, include the null terminator.
 * @param value - Value to return, include the null terminator.
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

TEST_F(GetTest, DATABASE_OK)
{
    // arrange
    const char *key = "key";
    const char *expectedValue = "value";
    char actualValue[NVS_DELEGATE_MAX_VALUE_LENGTH];
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READONLY, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, get_str(::testing::_, testing::StrEq(key), nullptr, ::testing::NotNull()))
        .WillOnce(::testing::DoAll(::testing::SetArgPointee<3>(strlen(expectedValue)), ::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK)));

    EXPECT_CALL(*mockNVSDelegate, get_str(::testing::_, testing::StrEq(key), ::testing::NotNull(), ::testing::NotNull()))
        .WillOnce(::testing::DoAll(::testing::SetArrayArgument<2>(expectedValue, expectedValue + strlen(expectedValue) + 1), ::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK)));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_)).Times(1);

    

    // act
    DatabaseError_t err = databaseAPI->get(key, actualValue, maxValueLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);
    EXPECT_STREQ(actualValue, expectedValue);
}

TEST_F(GetTest, DATABASE_KEY_INVALID)
{
    // arrange
    const char *key = "";
    const char *InvalidKey = "1234567891234567891";
    char actualValue[NVS_DELEGATE_MAX_VALUE_LENGTH];
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;

    // act
    DatabaseError_t err1 = databaseAPI->get(key, actualValue, maxValueLength);
    DatabaseError_t err2 = databaseAPI->get(nullptr, actualValue, maxValueLength);
    DatabaseError_t err3 = databaseAPI->get(InvalidKey, actualValue, maxValueLength);
    // assert
    EXPECT_EQ(err1, DatabaseError_t::DATABASE_KEY_INVALID);
    EXPECT_EQ(err2, DatabaseError_t::DATABASE_KEY_INVALID);
    EXPECT_EQ(err3, DatabaseError_t::DATABASE_KEY_INVALID);
}

TEST_F(GetTest, DATABASE_VALUE_INVALID)
{
    // arrange
    const char *key = "key";
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;

    // act
    DatabaseError_t err = databaseAPI->get(key, nullptr, maxValueLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_VALUE_INVALID);
}

TEST_F(GetTest, DATABASE_KEY_NOT_FOUND)
{
    // arrange
    const char *key = "key";
    char actualValue[NVS_DELEGATE_MAX_VALUE_LENGTH];
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READONLY, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, get_str(::testing::_, testing::StrEq(key), nullptr, ::testing::NotNull()))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_)).Times(1);

    // act
    DatabaseError_t err = databaseAPI->get(key, actualValue, maxValueLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_NOT_FOUND);

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READONLY, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND));
    err = databaseAPI->get(key, actualValue, maxValueLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_NOT_FOUND);
}

TEST_F(GetTest, DATABASE_ERROR)
{
    // arrange
    const char *key = "key";
    char actualValue[NVS_DELEGATE_MAX_VALUE_LENGTH];
    size_t maxValueLength = NVS_DELEGATE_MAX_VALUE_LENGTH;

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READONLY, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR));
    // act
    DatabaseError_t err = databaseAPI->get(key, actualValue, maxValueLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);

    // arrange
    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READONLY, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR));
    // act
    err = databaseAPI->get(key, actualValue, maxValueLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);

    // arrange
    DatabaseAPI databaseAPI(nullptr, "TEST_NVS");
    // act
    err = databaseAPI.get(key, actualValue, maxValueLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);
}

#endif // UNIT_GET_TEST_HPP
