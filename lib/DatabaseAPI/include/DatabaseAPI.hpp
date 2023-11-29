#ifndef DATABASE_API_H
#define DATABASE_API_H

#include <MultiPrinterLoggerInterface.hpp>
#include <string.h>

#include "NVSDelegateInterface.hpp"
#include "DatabaseAPIInterface.hpp"

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
    DatabaseAPI(
        NVSDelegateInterface *const nvsDelegate, char const *const nvsNamespace,
        MultiPrinterLoggerInterface *const logger = nullptr);

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
    DatabaseError_t get(
        char const *const key, char *value, size_t maxValueLength) const override;

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
    DatabaseError_t set(char const *const key, char const *const value) override;

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
    DatabaseError_t remove(char const *const key) override;

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
    DatabaseError_t isExist(char const *const key) const override;

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
    DatabaseError_t getValueLength(
        char const *const key, size_t *requiredLength) const override;

    /**
     * @brief Removes all keys and values from the database.
     *
     * @return DatabaseError_t indicating the success or failure of the operation.
     *         - DATABASE_OK: Operation successful.
     *         - DATABASE_ERROR: General database error.
     */
    DatabaseError_t eraseAll() override;

    /**
     * @brief eraseFlashAll - Format the Flash partition.
     *
     * @return DatabaseError_t indicating the success or failure of the operation.
     *         - DATABASE_OK: Operation successful.
     *         - DATABASE_ERROR: General database error.
     */
    DatabaseError_t eraseFlashAll() override;

private:
    NVSDelegateInterface *const _nvsDelegate;              /**< Pointer to the NVSDelegateInterface instance. */
    char _nvsNamespace[NVS_DELEGATE_MAX_NAMESPACE_LENGTH]; /**< The namespace to use in non-volatile storage. */
    MultiPrinterLoggerInterface *const _logger;            /**< Pointer to the MultiPrinterLoggerInterface instance. */

    /**
     * @brief Maps the given NVSDelegateError_t value to a DatabaseError_t value.
     *
     * @param err The NVSDelegateError_t value to map.
     * @return DatabaseError_t The mapped DatabaseError_t value.
     */
    DatabaseError_t const mapErrorAndPrint(NVSDelegateError_t const err) const;

    /**
     * @brief Checks if the given key is valid.
     *
     * @param key The key to check.
     * @return true if the key is valid, false otherwise.
     */
    bool isKeyValid(char const *const key) const;
};

#endif // DATABASE_API_H
