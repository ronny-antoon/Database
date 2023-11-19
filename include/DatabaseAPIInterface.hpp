#ifndef DATABASE_API_INTERFACE_H
#define DATABASE_API_INTERFACE_H

/**
 * @file DatabaseAPIInterface.hpp
 * @brief This file contains an Interface for the Database API class.
 * @author Ronny Antoon
 */

#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

// Error codes for database
enum DatabaseError_t : uint8_t
{
    DATABASE_OK,                 // No error
    DATABASE_KEY_INVALID,        // Key is invalid
    DATABASE_VALUE_INVALID,      // Invalid value
    DATABASE_KEY_NOT_FOUND,      // Key not found
    DATABASE_KEY_ALREADY_EXISTS, // Key already exists
    DATABASE_NAMESPACE_INVALID,  // Namespace is invalid
    DATABASE_NOT_ENOUGH_SPACE,   // Not enough space
    DATABASE_ERROR,              // internal database error
};

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
     * @param maxValueLength - Maximum size of the value.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key-value pair was successfully found.
     * - DATABASE_KEY_INVALID if the key is invalid.
     * - DATABASE_VALUE_INVALID if the value is invalid.
     * - DATABASE_KEY_NOT_FOUND if the key was not found.
     * - DATABASE_ERROR for internal error.
     */
    virtual DatabaseError_t get(const char *key, char *value, size_t maxValueLength) = 0;

    /**
     * @brief Set the value associated with the given key or update value if key Exist.
     *
     * @param key - Key to set, include the null terminator.
     * @param value - Value to set, include the null terminator.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key-value pair was successfully set.
     * - DATABASE_KEY_INVALID if the key is invalid.
     * - DATABASE_VALUE_INVALID if the value is invalid.
     * - DATABASE_ERROR for internal error.
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
     * - DATABASE_ERROR for internal error.
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
     * - DATABASE_ERROR for internal error.
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
    virtual DatabaseError_t getValueLength(const char *key, size_t *requiredLength) = 0;

    /**
     * @brief Erase all key-value pairs from the database.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the database was successfully erased.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t eraseAll() = 0;

    /**
     * @brief Convert the given error code to a string.
     *
     * @param error - Error code to convert.
     * @param errorString - String to return.
     * @param maxLength - Maximum size of the string. 255 is the maximum size.
     *
     */
    virtual void errorToString(DatabaseError_t error, char *errorString, uint8_t maxLength) = 0;
};

#endif // DATABASE_API_INTERFACE_H