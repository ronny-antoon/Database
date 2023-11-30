#ifndef NVS_DELEGATE_H
#define NVS_DELEGATE_H

#include <nvs.h>
#include <nvs_flash.h>
#include <string.h>
#include <MultiPrinterLoggerInterface.hpp>

#include "NVSDelegateInterface.hpp"

/**
 * @brief Implementation of NVSDelegateInterface for handling non-volatile storage operations.
 */
class NVSDelegate : public NVSDelegateInterface
{
public:
    /**
     * @brief Default constructor for NVSDelegate.
     *
     * @param logger Pointer to the logger interface.
     */
    NVSDelegate(MultiPrinterLoggerInterface *const logger = nullptr);

    /**
     * @brief Default destructor for NVSDelegate.
     */
    ~NVSDelegate();

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
    NVSDelegateError_t open(
        char const *const name, NVSDelegateOpenMode_t const open_mode,
        NVSDelegateHandle_t *out_handle) const override;

    /**
     * @brief Closes the specified non-volatile storage namespace.
     *
     * @param handle The handle of the namespace to close.
     */
    void close(NVSDelegateHandle_t handle) const override;

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
    NVSDelegateError_t set_str(
        NVSDelegateHandle_t handle, char const *const key,
        char const *const value) const override;

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
    NVSDelegateError_t get_str(
        NVSDelegateHandle_t handle, char const *const key,
        char *out_value, size_t *length) const override;

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
    NVSDelegateError_t erase_key(
        NVSDelegateHandle_t handle, char const *const key) const override;

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
    NVSDelegateError_t erase_all(NVSDelegateHandle_t handle) const override;

    /**
     * @brief Erases all keys and values from all non-volatile storage namespaces.
     *
     * @return NVSDelegateError_t indicating the success or failure of the operation.
     *         - NVS_DELEGATE_OK: Operation successful.
     *         - NVS_DELEGATE_UNKOWN_ERROR: Unknown error.
     */
    NVSDelegateError_t erase_flash_all() const override;

    /**
     * @brief Commits any pending changes to the specified non-volatile storage namespace.
     *
     * @param handle The handle of the namespace.
     * @return NVSDelegateError_t indicating the success or failure of the operation.
     *         - NVS_DELEGATE_OK: Operation successful.
     *         - NVS_DELEGATE_HANDLE_INVALID: Invalid namespace handle.
     *         - NVS_DELEGATE_READONLY: Attempt to commit in READONLY mode.
     *         - NVS_DELEGATE_UNKOWN_ERROR: Unknown error.
     */
    NVSDelegateError_t commit(NVSDelegateHandle_t handle) const override;

private:
    /**
     * @brief Pointer to the logger interface.
     */
    MultiPrinterLoggerInterface *const m_logger;

    /**
     * @brief Maps NVSDelegateOpenMode_t to nvs_open_mode.
     *
     * @param open_mode The NVSDelegateOpenMode_t to map.
     * @return nvs_open_mode mapped from the given NVSDelegateOpenMode_t.
     */
    nvs_open_mode_t const mapOpenMode(NVSDelegateOpenMode_t const open_mode) const;

    /**
     * @brief Maps esp_err_t to NVSDelegateError_t.
     *
     * @param err The esp_err_t to map.
     * @return NVSDelegateError_t mapped from the given esp_err_t.
     */
    NVSDelegateError_t mapErrorAndPrint(esp_err_t const err) const;

    /**
     * @brief Prints the given error and returns it.
     *
     * @param error The error to print and return.
     * @return The given error.
     */
    NVSDelegateError_t printAndReturnError(NVSDelegateError_t const error) const;

    /**
     * @brief Checks if the given namespace name is valid.
     *
     * @param name The namespace name to check.
     * @return true if the namespace name is valid, false otherwise.
     */
    bool isNamespaceValid(char const *const name) const;

    /**
     * @brief Checks if the given key is valid.
     *
     * @param key The key to check.
     * @return true if the key is valid, false otherwise.
     */
    bool isKeyValid(char const *const key) const;

    /**
     * @brief Checks if the given value is valid.
     *
     * @param value The value to check.
     * @return true if the value is valid, false otherwise.
     */
    bool isValueValid(char const *const value) const;
};

#endif // NVS_DELEGATE_H
