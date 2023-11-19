#ifndef NVS_DELEGATE_INTERFACE_H
#define NVS_DELEGATE_INTERFACE_H

#include <stdint.h> // uint8_t
#include <stddef.h> // size_t

#define NVS_DELEGATE_MAX_KEY_LENGTH 16
#define NVS_DELEGATE_MAX_VALUE_LENGTH 1024
#define NVS_DELEGATE_MAX_NAMESPACE_LENGTH 16

// ERROR CODES FOR NVS DELEGATE
enum class NVSDelegateError_t : uint8_t
{
    NVS_DELEGATE_OK,                // No error
    NVS_DELEGATE_KEY_INVALID,       // Key is invalid
    NVS_DELEGATE_NOT_ENOUGH_SPACE,  // Not enough space
    NVS_DELEGATE_NAMESPACE_INVALID, // Namespace is invalid
    NVS_DELEGATE_HANDLE_INVALID,    // Handle is invalid
    NVS_DELEGATE_READONLY,          // Read only

    NVS_DELEGATE_VALUE_INVALID,      // Invalid value
    NVS_DELEGATE_KEY_NOT_FOUND,      // Key not found
    NVS_DELEGATE_KEY_ALREADY_EXISTS, // Key already exists
    NVS_DELEGATE_UNKOWN_ERROR,       // Internal  error
};

// OPEN MODE FOR NVS DELEGATE
enum class NVSDelegateOpenMode_t : uint8_t
{
    NVSDelegate_READWRITE,
    NVSDelegate_READONLY,
};

// Forward declaration for nvs handle
typedef uint32_t NVSDelegateHandle_t;

// Forward declaration Abstract class
class NVSDelegateInterface
{
public:
    /**
     * @brief Destroy the NVSDelegateInterface object
     */
    virtual ~NVSDelegateInterface() = default;

    /**
     * @brief Open a namespace in the NVS.
     *
     * @param name Namespace name
     * @param open_mode Open mode
     * @param out_handle Handle to the namespace
     *
     * @details
     *
     * @return NVSDelegateError_t:-
     *      NVS_DELEGATE_OK: No error.
     *      NVS_DELEGATE_NAMESPACE_INVALID: Namespace is invalid.
     *      NVS_DELEGATE_KEY_NOT_FOUND: Namespace does not exist.
     *      NVS_DELEGATE_NOT_ENOUGH_SPACE: Not enough space.
     *      NVS_DELEGATE_UNKOWN_ERROR: Internal error.
     */
    virtual NVSDelegateError_t open(const char *name, NVSDelegateOpenMode_t open_mode, NVSDelegateHandle_t *out_handle) = 0;

    /**
     * @brief Close a namespace in the NVS.
     *
     * @param handle Handle to the namespace
     */
    virtual void close(NVSDelegateHandle_t handle) = 0;

    /**
     * @brief Set a string value in the NVS.
     *
     * @param handle Handle to the namespace
     * @param key Key
     * @param value Value
     *
     * @details
     *
     * @return NVSDelegateError_t:-
     *      NVS_DELEGATE_OK: No error.
     *      NVS_DELEGATE_KEY_INVALID: Key is invalid.
     *      NVS_DELEGATE_VALUE_INVALID: Value is invalid.
     *      NVS_DELEGATE_HANDLE_INVALID: Handle is invalid.
     *      NVS_DELEGATE_READONLY: Read only.
     *      NVS_DELEGATE_KEY_NOT_FOUND: Key does not exist.
     *      NVS_DELEGATE_NOT_ENOUGH_SPACE: Not enough space.
     *      NVS_DELEGATE_UNKOWN_ERROR: Internal error.
     */
    virtual NVSDelegateError_t set_str(NVSDelegateHandle_t handle, const char *key, const char *value) = 0;

    /**
     * @brief Get a string value from the NVS.
     *
     * @param handle Handle to the namespace
     * @param key Key
     * @param out_value Value
     * @param length Length of the value
     *
     * @details
     *
     * @return NVSDelegateError_t:-
     *      NVS_DELEGATE_OK: No error.
     *      NVS_DELEGATE_KEY_INVALID: Key is invalid.
     *      NVS_DELEGATE_VALUE_INVALID: Value is invalid.
     *      NVS_DELEGATE_HANDLE_INVALID: Handle is invalid.
     *      NVS_DELEGATE_KEY_NOT_FOUND: Key does not exist.
     *      NVS_DELEGATE_UNKOWN_ERROR: Internal error.
     */
    virtual NVSDelegateError_t get_str(NVSDelegateHandle_t handle, const char *key, char *out_value, size_t *length) = 0;

    /**
     * @brief Erase a key from the NVS.
     *
     * @param handle Handle to the namespace
     * @param key Key
     *
     * @details
     *
     * @return NVSDelegateError_t:-
     *      NVS_DELEGATE_OK: No error.
     *      NVS_DELEGATE_KEY_INVALID: Key is invalid.
     *      NVS_DELEGATE_HANDLE_INVALID: Handle is invalid.
     *      NVS_DELEGATE_READONLY: Read only.
     *      NVS_DELEGATE_KEY_NOT_FOUND: Key does not exist.
     *      NVS_DELEGATE_UNKOWN_ERROR: Internal error.
     */
    virtual NVSDelegateError_t erase_key(NVSDelegateHandle_t handle, const char *key) = 0;

    /**
     * @brief Erase all keys from the NVS.
     *
     * @param handle Handle to the namespace
     *
     * @details
     *
     * @return NVSDelegateError_t:-
     *      NVS_DELEGATE_OK: No error.
     *      NVS_DELEGATE_HANDLE_INVALID: Handle is invalid.
     *      NVS_DELEGATE_READONLY: Read only.
     *      NVS_DELEGATE_UNKOWN_ERROR: Internal error.
     */
    virtual NVSDelegateError_t erase_all(NVSDelegateHandle_t handle) = 0;
};

#endif // NVS_DELEGATE_INTERFACE_H