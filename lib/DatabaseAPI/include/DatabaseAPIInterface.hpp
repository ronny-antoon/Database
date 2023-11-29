#ifndef DATABASE_API_INTERFACE_H
#define DATABASE_API_INTERFACE_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Enumeration representing possible errors in the Database API.
 */
enum DatabaseError_t : uint8_t
{
    DATABASE_OK,                 /**< No error. */
    DATABASE_KEY_INVALID,        /**< Invalid key. */
    DATABASE_VALUE_INVALID,      /**< Invalid value. */
    DATABASE_KEY_NOT_FOUND,      /**< Key not found. */
    DATABASE_KEY_ALREADY_EXISTS, /**< Key already exists. */
    DATABASE_NAMESPACE_INVALID,  /**< Invalid namespace. */
    DATABASE_NOT_ENOUGH_SPACE,   /**< Not enough space in the storage. */
    DATABASE_ERROR,              /**< General database error. */
};

/**
 * @brief Interface for a database API providing basic CRUD operations.
 */
class DatabaseAPIInterface
{
public:
    /**
     * @brief Virtual destructor for DatabaseAPIInterface.
     */
    virtual ~DatabaseAPIInterface() = default;

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
    virtual DatabaseError_t get(
        char const *const key, char *value, size_t maxValueLength) const = 0;

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
    virtual DatabaseError_t set(char const *const key, char const *const value) = 0;

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
    virtual DatabaseError_t remove(char const *const key) = 0;

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
    virtual DatabaseError_t isExist(char const *const key) const = 0;

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
    virtual DatabaseError_t getValueLength(
        char const *const key, size_t *requiredLength) const = 0;

    /**
     * @brief Removes all keys and values from the database.
     *
     * @return DatabaseError_t indicating the success or failure of the operation.
     *         - DATABASE_OK: Operation successful.
     *         - DATABASE_ERROR: General database error.
     */
    virtual DatabaseError_t eraseAll() = 0;

    /**
     * @brief eraseFlashAll - Format the Flash partition.
     *
     * @return DatabaseError_t indicating the success or failure of the operation.
     *         - DATABASE_OK: Operation successful.
     *         - DATABASE_ERROR: General database error.
     */
    virtual DatabaseError_t eraseFlashAll() = 0;

    /**
     * @brief Converts a DatabaseError_t value to a human-readable error string.
     *
     * @param error The DatabaseError_t value to convert.
     * @param errorString Buffer to store the error string.
     * @param maxLength The maximum length of the error string buffer.
     */
    virtual void errorToString(
        DatabaseError_t const error, char *const errorString,
        uint8_t const maxLength) const = 0;
};

#endif // DATABASE_API_INTERFACE_H
