#ifndef UNIT_GET_VALUE_LENGTH_TEST_HPP
#define UNIT_GET_VALUE_LENGTH_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockingClass.hpp"
#include "DatabaseAPI.hpp"
#include "NVSDelegateInterface.hpp"

// setup test suite
class GetValueLengthTest : public ::testing::Test
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

/** Testing getValueLength Method of DatabaseAPI class
 * @brief Get the length of the value associated with the given key.
 *
 * @param key - Key to search for, include the null terminator.
 * @param requiredLength - the return Length of the value associated with the given key.
 *
 * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
 * - DATABASE_OK if the key-value pair was successfully found.
 * - DATABASE_KEY_INVALID if the key is invalid.
 * - DATABASE_KEY_NOT_FOUND if the key was not found.
 * - DATABASE_ERROR else.
 *
 *  test all possible return values
 */

TEST_F(GetValueLengthTest, DATABASE_OK)
{
    // arrange
    const char *key = "key";
    const char *value = "value";
    size_t requiredLength = 0;

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READONLY, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, get_str(::testing::_, testing::StrEq(key), nullptr, testing::Pointee(requiredLength)))
        .WillOnce(::testing::DoAll(::testing::SetArgPointee<3>(strlen(value)), ::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK)));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_)).Times(1);

    // act
    DatabaseError_t err = databaseAPI->getValueLength(key, &requiredLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);
    EXPECT_EQ(requiredLength, strlen(value));
}

TEST_F(GetValueLengthTest, DATABASE_KEY_INVALID)
{
    // arrange
    const char *key = "";
    const char *InvalidKey = "1234567891234567891";
    size_t requiredLength = 0;

    // act
    DatabaseError_t err1 = databaseAPI->getValueLength(key, &requiredLength);
    DatabaseError_t err2 = databaseAPI->getValueLength(nullptr, &requiredLength);
    DatabaseError_t err3 = databaseAPI->getValueLength(InvalidKey, &requiredLength);
    // assert
    EXPECT_EQ(err1, DatabaseError_t::DATABASE_KEY_INVALID);
    EXPECT_EQ(err2, DatabaseError_t::DATABASE_KEY_INVALID);
    EXPECT_EQ(err3, DatabaseError_t::DATABASE_KEY_INVALID);
}

TEST_F(GetValueLengthTest, DATABASE_KEY_NOT_FOUND)
{
    // arrange
    const char *key = "key";
    size_t requiredLength = 0;

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READONLY, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, get_str(::testing::_, testing::StrEq(key), nullptr, testing::Pointee(requiredLength)))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_)).Times(1);

    // act
    DatabaseError_t err = databaseAPI->getValueLength(key, &requiredLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_NOT_FOUND);
}

TEST_F(GetValueLengthTest, DATABASE_ERROR)
{
    // arrange
    const char *key = "key";
    size_t requiredLength = 0;
    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READONLY, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR));
    // act
    DatabaseError_t err = databaseAPI->getValueLength(key, &requiredLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);

    // arrange
    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READONLY, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));
    EXPECT_CALL(*mockNVSDelegate, get_str(::testing::_, testing::StrEq(key), nullptr, testing::Pointee(requiredLength)))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR));
    EXPECT_CALL(*mockNVSDelegate, close(::testing::_)).Times(1);
    // act
    err = databaseAPI->getValueLength(key, &requiredLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);

    // arrange
    DatabaseAPI databaseAPI(nullptr, "TEST_NVS");
    // act
    err = databaseAPI.getValueLength(key, &requiredLength);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);
}

#endif // UNIT_GET_VALUE_LENGTH_TEST_HPP