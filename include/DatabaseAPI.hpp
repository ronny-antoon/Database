#ifndef DATABASE_API_H
#define DATABASE_API_H

#include "DatabaseAPIInterface.hpp"
#include "NVSDelegateInterface.hpp"

class DatabaseAPI : public DatabaseAPIInterface
{
public:
    /**
     * @brief Construct a new Database API object
     *
     * @param nvsDelegate - NVS delegate to use.
     * @param nvsNamespace - Namespace for the NVS. The string should include the null terminator.
     */
    DatabaseAPI(NVSDelegateInterface *nvsDelegate, const char *nvsNamespace);

    /**
     * @brief Destroy the Database API Interface object.
     */
    ~DatabaseAPI() = default;

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
    DatabaseError_t get(const char *key, char *value, size_t maxValueLength) override;

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
     * - DATABASE_ERROR for internal error.
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
     * - DATABASE_ERROR for internal error.
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
    DatabaseError_t getValueLength(const char *key, size_t *requiredLength) override;

    /**
     * @brief Erase all key-value pairs from the database.
     *
     * @return DatabaseError_t - Error code indicating the result of the operation. Possible values:
     * - DATABASE_OK if the database was successfully erased.
     * - DATABASE_ERROR else.
     */
    DatabaseError_t eraseAll() override;

    /**
     * @brief Convert the given error code to a string.
     *
     * @param error - Error code to convert.
     * @param errorString - String to return.
     * @param maxLength - Maximum size of the string. 255 is the maximum size.
     *
     */
    void errorToString(DatabaseError_t error, char *errorString, uint8_t maxLength) override;

private:
    NVSDelegateInterface *_nvsDelegate; ///< NVS delegate to use.
    char _nvsNamespace[16];             ///< Namespace for the NVS;
};

#endif // DATABASE_API_H