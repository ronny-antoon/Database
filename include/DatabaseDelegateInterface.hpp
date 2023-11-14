#ifndef DATABASE_DELEGATE_INTERFACE_H
#define DATABASE_DELEGATE_INTERFACE_H

/**
 * @file DatabaseDelegateInterface.hpp
 * @brief This file contains an Interface for the Database Delegate class.
 * @author Ronny Antoon
 */

#include "DatabaseError.hpp" // DatabaseError_t

/**
 * @brief DatabaseDelegateInterface - Interface for the Database Delegate class.
 *
 * The DatabaseDelegateInterface class provides an interface for the Database Delegate class.
 */
class DatabaseDelegateInterface
{
public:
    /**
     * @brief Destroy the Database Delegate Interface object.
     */
    virtual ~DatabaseDelegateInterface() = default;

    /**
     * @brief Insert a new key-value pair into the database.
     *
     * @param key - Key to insert, include the null terminator.
     * @param value - Value to insert, include the null terminator.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key-value pair was successfully inserted.
     * - DATABASE_KEY_INVALID if the key is invalid.
     * - DATABASE_VALUE_INVALID if the value is invalid.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t insert(const char *key, const char *value) = 0;

    /**
     * @brief Update a key-value pair in the database.
     *
     * @param key - Key to update, include the null terminator.
     * @param value - Value to update, include the null terminator.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key-value pair was successfully updated.
     * - DATABASE_KEY_INVALID if the key is invalid.
     * - DATABASE_VALUE_INVALID if the value is invalid.
     * - DATABASE_KEY_NOT_FOUND if the key was not found.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t update(const char *key, const char *value) = 0;

    /**
     * @brief Remove a key-value pair from the database.
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
     * @brief Get the value associated with a key from the database.
     *
     * @param key - Key to get the value for, include the null terminator.
     * @param value - Buffer to store the value, include the null terminator.
     * @param maxLength - Maximum size of the value buffer.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the value was successfully retrieved.
     * - DATABASE_KEY_INVALID if the key is invalid.
     * - DATABASE_VALUE_INVALID if the value is invalid.
     * - DATABASE_KEY_NOT_FOUND if the key was not found.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t get(const char *key, char *value, size_t *maxLength) = 0;

    /**
     * @brief Get the length of the value associated with a key from the database.
     *
     * @param key - Key to get the value length for, include the null terminator.
     * @param requiredLength - Pointer to store the required length of the value.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the value length was successfully retrieved.
     * - DATABASE_KEY_INVALID if the key is invalid.
     * - DATABASE_KEY_NOT_FOUND if the key was not found.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t getLength(const char *key, size_t *requiredLength) = 0;

    /**
     * @brief Erase all key-value pairs from the database.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if all key-value pairs were successfully erased.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t eraseAll() = 0;
};

#endif // DATABASE_DELEGATE_INTERFACE_H