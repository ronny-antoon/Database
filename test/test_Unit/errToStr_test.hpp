#ifndef ERR_TO_STR_TEST_HPP
#define ERR_TO_STR_TEST_HPP

#include <Arduino.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "MockingClass.hpp"
#include "DatabaseAPI.hpp"
#include "NVSDelegateInterface.hpp"

// setup test suite
class ErrToStrTest : public ::testing::Test
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

/** Testing errToStr Method of DatabaseAPI class
 * @brief Convert error code to string.
 *
 * @param error - Error code to convert.
 * @return const char* - String representation of the error code.
 *
 *  test all possible return values
 */

TEST_F(ErrToStrTest, DATABASE_OK)
{
    char resault[50];
    databaseAPI->errorToString(DatabaseError_t::DATABASE_OK, resault, 50);
    databaseAPI->errorToString(DatabaseError_t::DATABASE_KEY_INVALID, resault, 50);
    databaseAPI->errorToString(DatabaseError_t::DATABASE_VALUE_INVALID, resault, 50);
    databaseAPI->errorToString(DatabaseError_t::DATABASE_KEY_NOT_FOUND, resault, 50);
    databaseAPI->errorToString(DatabaseError_t::DATABASE_KEY_ALREADY_EXISTS, resault, 50);
    databaseAPI->errorToString(DatabaseError_t::DATABASE_NAMESPACE_INVALID, resault, 50);
    databaseAPI->errorToString(DatabaseError_t::DATABASE_NOT_ENOUGH_SPACE, resault, 50);
    databaseAPI->errorToString(DatabaseError_t::DATABASE_ERROR, resault, 50);
}

#endif // ERR_TO_STR_TEST_HPP