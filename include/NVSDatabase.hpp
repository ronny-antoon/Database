#ifndef NVSDATABASE_H
#define NVSDATABASE_H

/**
 * @file NVSDatabase.hpp
 * @brief This file contains a class for NVS (Non-volatile Storage) database storage.
 * @author Ronny Antoon
 *
 * @todo Add logging.
 *
 * @warning This class is not thread safe. To ensure thread safety, external synchronization mechanisms.
 */

#include "DatabaseStorage.hpp"
#include <nvs.h>

#define NVS_MAX_KEY_LENGTH 16     // Maximum length of a key in NVS including the null terminator.
#define NVS_MAX_VALUE_LENGTH 4000 // Maximum length of a value in NVS

/**
 * @brief NVSDatabase - class for NVS database storage.
 *
 * The NVSDatabase class provides an interface to interact with the ESP-IDF's Non-volatile Storage (NVS) API.
 * NVS is used for storing configuration data and other non-volatile information.
 *
 * @warning This class is not thread safe. To ensure thread safety, external synchronization mechanisms
 * should be used when accessing the NVSDatabase instance from multiple threads.
 */
class NVSDatabase : public DatabaseStorage
{
private:
    nvs_handle_t nvsHandle;   // Handle for NVS
    const char *nvsNamespace; // Namespace for NVS

    /**
     * @brief Open NVS database.
     *
     * @param _readOnly - open database in read-only mode if true, read-write mode if false.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the database was successfully opened.
     * - DATABASE_OPEN_ERROR if the database opening failed.
     * - DATABASE_NOT_FOUND if the specified namespace was not found.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t open(bool _readOnly);

    /**
     * @brief Close NVS database.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the database was successfully closed.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t close();

    /**
     * @brief Commit changes to NVS database.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if changes were successfully committed.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t commit();

public:
    /**
     * @brief Construct a new NVSDatabase object.
     *
     * @param _nvsNamespace - Namespace for NVS, used to group and organize data.
     */
    NVSDatabase(const char *_nvsNamespace);

    /**
     * @brief Destructor for the NVSDatabase object.
     */
    ~NVSDatabase() override;

    /**
     * @brief Check if a key exists in the NVS database.
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
    DatabaseError_t isExist(const char *_key) override;

    /**
     * @brief Store a key-value pair in the NVS database.
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
    DatabaseError_t putPair(const char *_key, const char *_value) override;

    /**
     * @brief Retrieve the value associated with a key from the NVS database.
     *
     * @param _key - The key to retrieve the value for.
     * @param _value - A buffer to store the retrieved value.
     * @param _maxSize - The maximum size of the _value buffer.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the value was successfully retrieved.
     * - DATABASE_KEY_EMPTY if the key is empty.
     * - DATABASE_KEY_INVALID_SIZE if the key size exceeds the maximum allowed.
     * - DATABASE_ERROR if there was an issue with the database.
     * - DATABASE_NOT_FOUND if the specified key was not found in the database.
     * - DATABASE_VALUE_INVALID_SIZE if the _maxSize is not sufficient for the value.
     */
    DatabaseError_t getValueOf(const char *_key, char *_value, size_t *_maxSize) override;

    /**
     * @brief Get the length of the value associated with a key in the NVS database.
     *
     * @param _key - The key to get the value length for.
     * @param _requiredLength - A pointer to store the required length of the value.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the value length was successfully retrieved.
     * - DATABASE_KEY_EMPTY if the key is empty.
     * - DATABASE_KEY_INVALID_SIZE if the key size exceeds the maximum allowed.
     * - DATABASE_ERROR if there was an issue with the database.
     * - DATABASE_NOT_FOUND if the specified key was not found in the database.
     */
    DatabaseError_t getValueLengthOf(const char *_key, size_t *_requiredLength) override;

    /**
     * @brief Remove a key-value pair from the NVS database.
     *
     * @param _key - The key to remove.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the key-value pair was successfully removed.
     * - DATABASE_KEY_EMPTY if the key is empty.
     * - DATABASE_KEY_INVALID_SIZE if the key size exceeds the maximum allowed.
     * - DATABASE_NOT_FOUND if the specified key was not found in the database.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t removePair(const char *_key) override;

    /**
     * @brief Erase all values from the NVS database within the specified namespace.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the values were successfully erased.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t eraseAll() override;
};

#endif // NVSDATABASE_H
