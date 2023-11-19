#ifndef NVS_DELEGATE_H
#define NVS_DELEGATE_H

#include "NVSDelegateInterface.hpp"

class NVSDelegate : public NVSDelegateInterface
{
public:
    /**
     * @brief Construct a new NVSDelegate object
     */
    NVSDelegate() = default;

    /**
     * @brief Destroy the NVSDelegate object
     */
    ~NVSDelegate() = default;

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
    NVSDelegateError_t open(const char *name, NVSDelegateOpenMode_t open_mode, NVSDelegateHandle_t *out_handle) override;

    /**
     * @brief Close a namespace in the NVS.
     *
     * @param handle Handle to the namespace
     */
    void close(NVSDelegateHandle_t handle) override;

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
    NVSDelegateError_t set_str(NVSDelegateHandle_t handle, const char *key, const char *value) override;

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
    NVSDelegateError_t get_str(NVSDelegateHandle_t handle, const char *key, char *out_value, size_t *length) override;

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
    NVSDelegateError_t erase_key(NVSDelegateHandle_t handle, const char *key) override;

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
    NVSDelegateError_t erase_all(NVSDelegateHandle_t handle) override;
};

#endif // NVS_DELEGATE_H