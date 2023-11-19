#ifndef DATABASE_API_H
#define DATABASE_API_H

#include <MultiPrinterLoggerInterface.hpp>

#include "DatabaseAPIInterface.hpp"
#include "NVSDelegateInterface.hpp"

/**
 * @brief Implementation of DatabaseAPIInterface for interacting with non-volatile storage using NVSDelegate.
 */
class DatabaseAPI : public DatabaseAPIInterface
{
public:
    /**
     * @brief Constructor for DatabaseAPI.
     *
     * @param nvsDelegate Pointer to the NVSDelegateInterface instance.
     * @param nvsNamespace The namespace to use in non-volatile storage.
     */
    DatabaseAPI(NVSDelegateInterface *nvsDelegate, const char *nvsNamespace, MultiPrinterLoggerInterface *logger = nullptr);

    /**
     * @brief Destructor for DatabaseAPI.
     */
    ~DatabaseAPI();

    /**
     * @brief Retrieves the value associated with the specified key from the database.
     *
     * @param key The key for the value.
     * @param value Buffer to store the retrieved value.
     * @param maxValueLength The maximum length of the buffer.
     * @return DatabaseError_t indicating the success or failure of the operation.
     *         - DATABASE_OK: Operation successful.
     *         - DATABASE_KEY_INVALID: Invalid key.
     *         - DATABASE_VALUE_INVALID: Invalid value buffer.
     *         - DATABASE_KEY_NOT_FOUND: Key not found.
     *         - DATABASE_ERROR: General database error.
     */
    DatabaseError_t get(const char *key, char *value, size_t maxValueLength) override;

    /**
     * @brief Sets the value for the specified key in the database.
     *
     * @param key The key for the value.
     * @param value The value to set.
     * @return DatabaseError_t indicating the success or failure of the operation.
     *         - DATABASE_OK: Operation successful.
     *         - DATABASE_KEY_INVALID: Invalid key.
     *         - DATABASE_VALUE_INVALID: Invalid value.
     *         - DATABASE_ERROR: General database error.
     */
    DatabaseError_t set(const char *key, const char *value) override;

    /**
     * @brief Removes the specified key and its associated value from the database.
     *
     * @param key The key to remove.
     * @return DatabaseError_t indicating the success or failure of the operation.
     *         - DATABASE_OK: Operation successful.
     *         - DATABASE_KEY_INVALID: Invalid key.
     *         - DATABASE_KEY_NOT_FOUND: Key not found.
     *         - DATABASE_ERROR: General database error.
     */
    DatabaseError_t remove(const char *key) override;

    /**
     * @brief Checks if the specified key exists in the database.
     *
     * @param key The key to check for existence.
     * @return DatabaseError_t indicating the success or failure of the operation.
     *         - DATABASE_OK: Key exists.
     *         - DATABASE_KEY_INVALID: Invalid key.
     *         - DATABASE_KEY_NOT_FOUND: Key not found.
     *         - DATABASE_ERROR: General database error.
     */
    DatabaseError_t isExist(const char *key) override;

    /**
     * @brief Retrieves the length of the value associated with the specified key.
     *
     * @param key The key for the value.
     * @param requiredLength Pointer to store the required length of the value.
     * @return DatabaseError_t indicating the success or failure of the operation.
     *         - DATABASE_OK: Operation successful.
     *         - DATABASE_KEY_INVALID: Invalid key.
     *         - DATABASE_ERROR: General database error.
     */
    DatabaseError_t getValueLength(const char *key, size_t *requiredLength) override;

    /**
     * @brief Removes all keys and values from the database.
     *
     * @return DatabaseError_t indicating the success or failure of the operation.
     *         - DATABASE_OK: Operation successful.
     *         - DATABASE_ERROR: General database error.
     */
    DatabaseError_t eraseAll() override;

    /**
     * @brief Converts a DatabaseError_t value to a human-readable error string.
     *
     * @param error The DatabaseError_t value to convert.
     * @param errorString Buffer to store the error string.
     * @param maxLength The maximum length of the error string buffer.
     */
    void errorToString(DatabaseError_t error, char *errorString, uint8_t maxLength) override;

private:
    NVSDelegateInterface *_nvsDelegate;   /**< Pointer to the NVSDelegateInterface instance. */
    char _nvsNamespace[16];               /**< The namespace to use in non-volatile storage. */
    MultiPrinterLoggerInterface *_logger; /**< Pointer to the MultiPrinterLoggerInterface instance. */
};

#endif // DATABASE_API_H
