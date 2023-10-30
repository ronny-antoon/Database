#ifndef DATABASE_API_INTERFACE_H
#define DATABASE_API_INTERFACE_H

/**
 * @file DatabaseAPIInterface.hpp
 * @brief This file contains an Interface for the Database API class.
 * @author Ronny Antoon
 */

#include <stddef.h> // size_t

#include "DatabaseError.hpp" // DatabaseError_t

/**
 * @brief DatabaseAPIInterface - Interface for the Database API class.
 *
 * The DatabaseAPIInterface class provides an interface for the Database API class.
 */
class DatabaseAPIInterface
{
public:
    /**
     * @brief Destroy the Database API Interface object.
     */
    virtual ~DatabaseAPIInterface() = default;

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
    virtual DatabaseError_t get(const char *key, char *value, size_t *maxLength) = 0;

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
    virtual DatabaseError_t set(const char *key, const char *value) = 0;

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
    virtual DatabaseError_t remove(const char *key) = 0;

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
    virtual DatabaseError_t isExist(const char *key) = 0;

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
    virtual DatabaseError_t getLength(const char *key, size_t *requiredLength) = 0;

    /**
     * @brief Erase all key-value pairs from the database.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the database was successfully erased.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t eraseAll() = 0;
};

#endif // DATABASE_API_INTERFACE_H