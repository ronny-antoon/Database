#ifndef UNIT_ERASE_ALL_TEST_HPP
#define UNIT_ERASE_ALL_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockingClass.hpp"
#include "DatabaseAPI.hpp"
#include "NVSDelegateInterface.hpp"

// setup test suite
class EraseAllTest : public ::testing::Test
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

/** Testing eraseAll Method of DatabaseAPI class
 * @brief Erase all key-value pairs in the database.
 *
 * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
 * - DATABASE_OK if all key-value pairs were successfully erased.
 * - DATABASE_ERROR for internal error.
 *
 *  test all possible return values
 */

TEST_F(EraseAllTest, DATABASE_OK)
{
    // arrange
    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READWRITE, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, erase_all(::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    EXPECT_CALL(*mockNVSDelegate, close(::testing::_)).Times(1);

    EXPECT_CALL(*mockNVSDelegate, commit(::testing::_))
        .WillRepeatedly(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_OK));

    // act
    DatabaseError_t err = databaseAPI->eraseAll();
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_OK);
}

TEST_F(EraseAllTest, DATABASE_ERROR)
{
    // arrange
    EXPECT_CALL(*mockNVSDelegate, open(testing::StrEq("TEST_NVS"), NVSDelegateOpenMode_t::NVSDelegate_READWRITE, ::testing::_))
        .WillOnce(::testing::Return(NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR));

    // act
    DatabaseError_t err = databaseAPI->eraseAll();
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);

    // arrange
    DatabaseAPI databaseAPI(nullptr, "TEST_NVS");
    // act
    err = databaseAPI.eraseAll();
    // assert
    EXPECT_EQ(err, DatabaseError_t::DATABASE_ERROR);
}

#endif // UNIT_ERASE_ALL_TEST_HPP
