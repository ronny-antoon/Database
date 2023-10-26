#ifndef DATABASESTORAGE_H
#define DATABASESTORAGE_H

/**
 * @file DatabaseStorage.hpp
 * @brief This file contains abstract class for database storage.
 *
 * This class is used for creating different types of database storages.
 * For example: NVS, SD, file, etc.
 * This class is used in Database class.
 *
 * @see NVSDatabase as an example of database storage.
 */

#include <stddef.h>

// Error codes for database
enum DatabaseError_t
{
    DATABASE_OK,                 // No error
    DATABASE_ERROR,              // General database error
    DATABASE_NOT_FOUND,          // Key not found
    DATABASE_FILE_NOT_FOUND,     // File not found
    DATABASE_OPEN_ERROR,         // Error while opening the database
    DATABASE_KEY_INVALID_SIZE,   // Invalid key size
    DATABASE_VALUE_INVALID_SIZE, // Invalid buffer size
    DATABASE_TIMEOUT,            // Timeout error
    DATABASE_KEY_EMPTY,          // Key is empty
    DATABASE_VALUE_EMPTY,        // Value is empty
};

/**
 * @brief DatabaseStorage - abstract class for database storage.
 *
 * This class is used for creating different types of database storages.
 * For example: NVS, SD, file, etc.
 */
class DatabaseStorage
{
public:
    /**
     * @brief Check if a key exists in the database.
     *
     * @param _key - The key to check.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key exists in the database.
     * - DATABASE_KEY_EMPTY if the key is empty.
     * - DATABASE_KEY_INVALID_SIZE if the key size exceeds the maximum allowed.
     * - DATABASE_OPEN_ERROR if the database opening failed.
     * - DATABASE_NOT_FOUND if the specified key was not found in the database.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t isExist(const char *_key) = 0;

    /**
     * @brief Store a key-value pair in the database.
     *
     * @param _key - The key to store.
     * @param _value - The value to associate with the key.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key-value pair was successfully stored.
     * - DATABASE_KEY_EMPTY if the key is empty.
     * - DATABASE_VALUE_EMPTY if the value is empty.
     * - DATABASE_KEY_INVALID_SIZE if the key size exceeds the maximum allowed.
     * - DATABASE_VALUE_INVALID_SIZE if the value size exceeds the maximum allowed.
     * - DATABASE_OPEN_ERROR if the database opening failed.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t putPair(const char *_key, const char *_value) = 0;

    /**
     * @brief Retrieve the value associated with a key from the database.
     *
     * @param _key - The key to retrieve the value for.
     * @param _value - A buffer to store the retrieved value.
     * @param _maxSize - The maximum size of the _value buffer.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the value was successfully retrieved.
     * - DATABASE_KEY_EMPTY if the key is empty.
     * - DATABASE_KEY_INVALID_SIZE if the key size exceeds the maximum allowed.
     * - DATABASE_OPEN_ERROR if the database opening failed.
     * - DATABASE_NOT_FOUND if the specified key was not found in the database.
     * - DATABASE_INVALID_SIZE if the buffer size is insufficient.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t getValueOf(const char *_key, char *_value, size_t *_maxSize) = 0;

    /**
     * @brief Get the length of the value associated with a key in the database.
     *
     * @param _key - The key to retrieve the value length for.
     * @param _requiredLength - A pointer to store the required length of the value.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the value length was successfully retrieved.
     * - DATABASE_KEY_EMPTY if the key is empty.
     * - DATABASE_KEY_INVALID_SIZE if the key size exceeds the maximum allowed.
     * - DATABASE_OPEN_ERROR if the database opening failed.
     * - DATABASE_NOT_FOUND if the specified key was not found in the database.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t getValueLengthOf(const char *_key, size_t *_requiredLength) = 0;

    /**
     * @brief Remove a key-value pair from the database.
     *
     * @param _key - The key to remove.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key-value pair was successfully removed.
     * - DATABASE_KEY_EMPTY if the key is empty.
     * - DATABASE_KEY_INVALID_SIZE if the key size exceeds the maximum allowed.
     * - DATABASE_OPEN_ERROR if the database opening failed.
     * - DATABASE_NOT_FOUND if the specified key was not found in the database.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t removePair(const char *_key) = 0;

    /**
     * @brief Erase all key-value pairs from the database.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if all key-value pairs were successfully removed.
     * - DATABASE_OPEN_ERROR if the database opening failed.
     * - DATABASE_ERROR else.
     */
    virtual DatabaseError_t eraseAll() = 0;

    /**
     * @brief Destructor for DatabaseStorage.
     */
    virtual ~DatabaseStorage() {}
};

#endif // DATABASESTORAGE_H