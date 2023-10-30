#ifndef NVS_DELEGATE_H
#define NVS_DELEGATE_H

/**
 * @file NVSDelegate.hpp
 * @brief This file contains the NVSDelegate class which implements the DatabaseDelegateInterface for the ESP32/Arduino NVS database.
 * @author Ronny Antoon
 */

#include <stddef.h> // size_t
#include <nvs.h>    // for nvs handle

#include "DatabaseError.hpp"             // DatabaseError_t
#include "DatabaseDelegateInterface.hpp" // DatabaseDelegateInterface

#define NVS_DELEGATE_MAX_PARTITION_NAME_LENGTH 16        // Maximum length of a partition name in NVS including the null terminator.
#define NVS_DELEGATE_MAX_KEY_LENGTH 16                   // Maximum length of a key in NVS including the null terminator.
#define NVS_DELEGATE_MAX_VALUE_LENGTH 1024               // Maximum length of a value in NVS including the null terminator.
#define NVS_DELEGATE_DEFAULT_PARTITION_NAME "NVSDefault" // Default partition name to use if the provided partition name is invalid.

/**
 * @brief NVSDelegate - Implements the DatabaseDelegateInterface for the ESP32/Arduino NVS database.
 */
class NVSDelegate : public DatabaseDelegateInterface
{
public:
    /**
     * @brief Construct a new NVSDelegate object.
     *
     * @param partitionName - The name of the partition to use.
     *
     * @note If the partition name is invalid, the default partition name will be used.
     */
    NVSDelegate(const char *partitionName);

    /**
     * @brief Destroy the NVSDelegate object.
     */
    ~NVSDelegate() override;

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
     * - DATABASE_KEY_ALREADY_EXISTS if the key already exists.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t insert(const char *key, const char *value) override;

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
    DatabaseError_t update(const char *key, const char *value) override;

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
    DatabaseError_t remove(const char *key) override;

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
    DatabaseError_t get(const char *key, char *value, size_t *maxLength) override;

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
    DatabaseError_t getLength(const char *key, size_t *requiredLength) override;

    /**
     * @brief Erase all key-value pairs from the database.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if all key-value pairs were successfully erased.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t eraseAll() override;

private:
    const char *partitionName; // The name of the partition to use
    nvs_handle_t nvsHandle;    // Handle for NVS

    /**
     * @brief Check if a key is valid.
     *
     * @param key - Key to check, include the null terminator.
     *
     * @return true if the key is valid.
     * @return false if the key is invalid.
     */
    bool isKeyValid(const char *key);

    /**
     * @brief Check if a value is valid.
     *
     * @param value - Value to check, include the null terminator.
     *
     * @return true if the value is valid.
     * @return false if the value is invalid.
     */
    bool isValueValid(const char *value);

    /**
     * @brief Open the database.
     *
     * @param _readOnly - Flag indicating if the database should be opened in read-only mode.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the database was successfully opened.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t openDatabase(bool _readOnly);

    /**
     * @brief Close the database.
     */
    void closeDatabase();

    /**
     * @brief Commit the database.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the database was successfully committed.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t commitDatabase();

    /**
     * @brief Check if a key exists in the database.
     *
     * @param key - Key to check, include the null terminator.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key exists in the database.
     * - DATABASE_KEY_INVALID if the key is invalid.
     * - DATABASE_KEY_NOT_FOUND if the key was not found.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t keyExists(const char *key);
};

#endif // NVS_DELEGATE_H