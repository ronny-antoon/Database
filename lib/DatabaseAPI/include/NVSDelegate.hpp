#ifndef NVS_DELEGATE_H
#define NVS_DELEGATE_H

#include <nvs.h>
#include <string.h>

#include "NVSDelegateInterface.hpp"

/**
 * @brief Implementation of NVSDelegateInterface for handling non-volatile storage operations.
 */
class NVSDelegate : public NVSDelegateInterface
{
public:
    /**
     * @brief Default constructor for NVSDelegate.
     */
    NVSDelegate() = default;

    /**
     * @brief Default destructor for NVSDelegate.
     */
    ~NVSDelegate() = default;

    /**
     * @brief Opens a namespace in non-volatile storage with the specified name and mode.
     *
     * @param name The name of the namespace to open.
     * @param open_mode The mode in which to open the namespace (READWRITE or READONLY).
     * @param out_handle Pointer to receive the handle for the opened namespace.
     * @return NVSDelegateError_t indicating the success or failure of the operation.
     *         - NVS_DELEGATE_OK: Operation successful.
     *         - NVS_DELEGATE_NAMESPACE_INVALID: Invalid namespace name.
     *         - NVS_DELEGATE_NOT_ENOUGH_SPACE: Not enough space in the storage.
     *         - NVS_DELEGATE_KEY_NOT_FOUND: Namespace not found.
     *         - NVS_DELEGATE_UNKOWN_ERROR: Unknown error.
     */
    NVSDelegateError_t open(const char *name, NVSDelegateOpenMode_t open_mode, NVSDelegateHandle_t *out_handle) override;

    /**
     * @brief Closes the specified non-volatile storage namespace.
     *
     * @param handle The handle of the namespace to close.
     */
    void close(NVSDelegateHandle_t handle) override;

    /**
     * @brief Sets a string value for the specified key in the given non-volatile storage namespace.
     *
     * @param handle The handle of the namespace.
     * @param key The key for the string value.
     * @param value The string value to set.
     * @return NVSDelegateError_t indicating the success or failure of the operation.
     *         - NVS_DELEGATE_OK: Operation successful.
     *         - NVS_DELEGATE_KEY_INVALID: Invalid key.
     *         - NVS_DELEGATE_VALUE_INVALID: Invalid value.
     *         - NVS_DELEGATE_READONLY: Attempt to write in READONLY mode.
     *         - NVS_DELEGATE_NOT_ENOUGH_SPACE: Not enough space in the storage.
     *         - NVS_DELEGATE_HANDLE_INVALID: Invalid namespace handle.
     *         - NVS_DELEGATE_UNKOWN_ERROR: Unknown error.
     */
    NVSDelegateError_t set_str(NVSDelegateHandle_t handle, const char *key, const char *value) override;

    /**
     * @brief Gets the string value for the specified key from the given non-volatile storage namespace.
     *
     * @param handle The handle of the namespace.
     * @param key The key for the string value.
     * @param out_value Buffer to store the retrieved string value.
     * @param length Pointer to the length of the buffer; updated with the actual length of the string.
     * @return NVSDelegateError_t indicating the success or failure of the operation.
     *         - NVS_DELEGATE_OK: Operation successful.
     *         - NVS_DELEGATE_KEY_INVALID: Invalid key.
     *         - NVS_DELEGATE_VALUE_INVALID: Invalid value.
     *         - NVS_DELEGATE_HANDLE_INVALID: Invalid namespace handle.
     *         - NVS_DELEGATE_KEY_NOT_FOUND: Key not found.
     *         - NVS_DELEGATE_UNKOWN_ERROR: Unknown error.
     */
    NVSDelegateError_t get_str(NVSDelegateHandle_t handle, const char *key, char *out_value, size_t *length) override;

    /**
     * @brief Erases the key and its associated value from the specified non-volatile storage namespace.
     *
     * @param handle The handle of the namespace.
     * @param key The key to erase.
     * @return NVSDelegateError_t indicating the success or failure of the operation.
     *         - NVS_DELEGATE_OK: Operation successful.
     *         - NVS_DELEGATE_KEY_INVALID: Invalid key.
     *         - NVS_DELEGATE_HANDLE_INVALID: Invalid namespace handle.
     *         - NVS_DELEGATE_KEY_NOT_FOUND: Key not found.
     *         - NVS_DELEGATE_READONLY: Attempt to erase in READONLY mode.
     *         - NVS_DELEGATE_UNKOWN_ERROR: Unknown error.
     */
    NVSDelegateError_t erase_key(NVSDelegateHandle_t handle, const char *key) override;

    /**
     * @brief Erases all keys and values from the specified non-volatile storage namespace.
     *
     * @param handle The handle of the namespace.
     * @return NVSDelegateError_t indicating the success or failure of the operation.
     *         - NVS_DELEGATE_OK: Operation successful.
     *         - NVS_DELEGATE_HANDLE_INVALID: Invalid namespace handle.
     *         - NVS_DELEGATE_READONLY: Attempt to erase in READONLY mode.
     *         - NVS_DELEGATE_UNKOWN_ERROR: Unknown error.
     */
    NVSDelegateError_t erase_all(NVSDelegateHandle_t handle) override;
};

#endif // NVS_DELEGATE_H
