#ifndef UNIT_REMOVE_TEST_HPP
#define UNIT_REMOVE_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockingClass.hpp"
#include "DatabaseAPI.hpp"
#include "NVSDelegateInterface.hpp"

// setup test suite
class RemoveTest : public ::testing::Test
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

/** Testing remove Method of DatabaseAPI class
 * @brief Remove the key-value pair from the database.
 *
 * @param key - Key to remove, include the null terminator.
 *
 * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
 * - DATABASE_OK if the key was successfully removed.
 * - DATABASE_KEY_INVALID if the key is invalid.
 * - DATABASE_KEY_NOT_FOUND if the key was not found.
 * - DATABASE_ERROR for internal error.
 *
 *  test all possible return values
 */

TEST_F(RemoveTest, DATABASE_OK)
{
    // arrange
    const char *key = "key";

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READWRITE, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, erase_key(::testing::_, testing::StrEq(key)))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_)).Times(1);

    // act
    DatabaseError_t err = databaseAPI->remove(key);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);
}

TEST_F(RemoveTest, DATABASE_KEY_INVALID)
{
    // arrange
    const char *key = "";

    // act
    DatabaseError_t err1 = databaseAPI->remove(key);
    DatabaseError_t err2 = databaseAPI->remove(nullptr);
    // assert
    EXPECT_EQ(err1, DatabaseError_t::DATABASE_KEY_INVALID);
    EXPECT_EQ(err2, DatabaseError_t::DATABASE_KEY_INVALID);
}

TEST_F(RemoveTest, DATABASE_KEY_NOT_FOUND)
{
    // arrange
    const char *key = "key";

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READWRITE, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, erase_key(::testing::_, testing::StrEq(key)))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_)).Times(1);

    // act
    DatabaseError_t err = databaseAPI->remove(key);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_NOT_FOUND);

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READWRITE, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND));
    err = databaseAPI->remove(key);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_KEY_NOT_FOUND);
}

TEST_F(RemoveTest, DATABASE_ERROR)
{
    // arrange
    const char *key = "key";

    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READWRITE, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR));

    // act
    DatabaseError_t err = databaseAPI->remove(key);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);

    // arrange
    DatabaseAPI databaseAPI(nullptr, "TEST_NVS");
    // act
    err = databaseAPI.remove(key);
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);
}

#endif // UNIT_REMOVE_TEST_HPP
