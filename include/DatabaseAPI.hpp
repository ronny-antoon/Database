#ifndef DATABASE_API_H
#define DATABASE_API_H

/**
 * @file DatabaseAPI.hpp
 * @brief This file contains an implementation for the Database API class.
 * @author Ronny Antoon
 */

#include <stddef.h> // size_t

#include "DatabaseError.hpp" // DatabaseError_t
#include "DatabaseAPIInterface.hpp"
#include "DatabaseDelegateInterface.hpp"

/**
 * @brief DatabaseAPI - Implementation for the Database API class.
 *
 * The DatabaseAPI class provides an implementation for the Database API class.
 */
class DatabaseAPI : public DatabaseAPIInterface
{
public:
    DatabaseAPI(DatabaseDelegateInterface *delegate);

    /**
     * @brief Destroy the Database API object.
     */
    ~DatabaseAPI() override = default;

    /**
     * @brief Get the value associated with the given key.
     *
     * @param key - Key to search for, include the null terminator.
     * @param value - Value to return, include the null terminator.
     * @param maxLength - Maximum size of the value.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key-value pair was successfully found.
     * - DATABASE_KEY_INVALID if the key is invalid.
     * - DATABASE_VALUE_INVALID if the value is invalid.
     * - DATABASE_KEY_NOT_FOUND if the key was not found.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t get(const char *key, char *value, size_t *maxLength) override;

    /**
     * @brief Set the value associated with the given key.
     *
     * @param key - Key to set, include the null terminator.
     * @param value - Value to set, include the null terminator.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key-value pair was successfully set.
     * - DATABASE_KEY_INVALID if the key is invalid.
     * - DATABASE_VALUE_INVALID if the value is invalid.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t set(const char *key, const char *value) override;

    /**
     * @brief Remove the key-value pair associated with the given key.
     *
     * @param key - Key to remove, include the null terminator.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key-value pair was successfully removed.
     * - DATABASE_KEY_INVALID if the key is invalid.
     * - DATABASE_KEY_NOT_FOUND if the key was not found.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t remove(const char *key) override;

    /**
     * @brief Check if the given key exists in the database.
     *
     * @param key - Key to search for, include the null terminator.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key-value pair was successfully found.
     * - DATABASE_KEY_INVALID if the key is invalid.
     * - DATABASE_KEY_NOT_FOUND if the key was not found.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t isExist(const char *key) override;

    /**
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
     */
    DatabaseError_t getLength(const char *key, size_t *requiredLength) override;

    /**
     * @brief Erase all key-value pairs from the database.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the database was successfully erased.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t eraseAll() override;

private:
    DatabaseDelegateInterface *delegate; // Database delegate.
};

#endif // DATABASE_API_H