#include "NVSDelegate.hpp"
#include <nvs.h> // for nvs handle
#include <string.h>

// Implementation of NVSDelegate class methods

// Step 1: Open a namespace in the NVS.
NVSDelegateError_t NVSDelegate::open(const char *name, NVSDelegateOpenMode_t open_mode, NVSDelegateHandle_t *out_handle)
{
    // Step 1.1: Check if the namespace name is valid.
    if (name == nullptr || strlen(name) >= NVS_DELEGATE_MAX_NAMESPACE_LENGTH || strlen(name) == 0)
    {
        return NVSDelegateError_t::NVS_DELEGATE_NAMESPACE_INVALID;
    }

    //////////////////////////////////////
    // // Step 1.2: Check if the out_handle is nullptr, allocate memory if needed.
    // out_handle = new NVSDelegateHandle_t();
    // // out_handle->internalHandle = new nvs_handle_t();

    // // Step 1.3: Map open_mode to nvs_open_mode_t.
    // nvs_handle_t *_handle = static_cast<nvs_handle_t *>(out_handle->internalHandle);
    //////////////////////////////////////////
    nvs_open_mode_t _open_mode = open_mode == NVSDelegateOpenMode_t::NVSDelegate_READONLY ? NVS_READONLY : NVS_READWRITE;

    // Step 1.4: Call nvs_open to open the namespace.
    esp_err_t err = nvs_open(name, _open_mode, out_handle);

    // Step 1.5: Check the result of nvs_open and return the corresponding NVSDelegateError_t.
    switch (err)
    {
    case ESP_OK:
        return NVSDelegateError_t::NVS_DELEGATE_OK;
    case ESP_ERR_NVS_NOT_FOUND:
        return NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND;
    case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
        return NVSDelegateError_t::NVS_DELEGATE_NOT_ENOUGH_SPACE;
    default:
        break;
    }

    return NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR;
}

// Step 2: Close a namespace in the NVS.
void NVSDelegate::close(NVSDelegateHandle_t handle)
{
    // if (handle.internalHandle == nullptr)
    // {
    //     return;
    // }

    // Step 2.1: Retrieve the internal NVS handle.
    // nvs_handle_t *_handle = static_cast<nvs_handle_t *>(handle.internalHandle);

    // Step 2.2: Call nvs_close to close the namespace.
    nvs_close(handle);
}

// Step 3: Set a string value in the NVS.
NVSDelegateError_t NVSDelegate::set_str(NVSDelegateHandle_t handle, const char *key, const char *value)
{
    // Step 3.1: Check if the key is valid.
    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
    {
        return NVSDelegateError_t::NVS_DELEGATE_KEY_INVALID;
    }

    // Step 3.2: Check if the value is valid.
    if (value == nullptr || strlen(value) >= NVS_DELEGATE_MAX_VALUE_LENGTH || strlen(value) == 0)
    {
        return NVSDelegateError_t::NVS_DELEGATE_VALUE_INVALID;
    }

    // if (handle.internalHandle == nullptr)
    //     return NVSDelegateError_t::NVS_DELEGATE_HANDLE_INVALID;

    // // Step 3.3: Retrieve the internal NVS handle.
    // nvs_handle_t *_handle = static_cast<nvs_handle_t *>(handle.internalHandle);

    // Step 3.4: Call nvs_set_str to set the string value.
    esp_err_t err = nvs_set_str(handle, key, value);

    // Step 3.5: Check the result of nvs_set_str and return the corresponding NVSDelegateError_t.
    switch (err)
    {
    case ESP_OK:
        return NVSDelegateError_t::NVS_DELEGATE_OK;
    case ESP_ERR_NVS_NOT_FOUND:
        return NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND;
    case ESP_ERR_NVS_READ_ONLY:
        return NVSDelegateError_t::NVS_DELEGATE_READONLY;
    case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
        return NVSDelegateError_t::NVS_DELEGATE_NOT_ENOUGH_SPACE;
    case ESP_ERR_NVS_INVALID_HANDLE:
        return NVSDelegateError_t::NVS_DELEGATE_HANDLE_INVALID;
    default:
        break;
    }

    return NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR;
}

// Step 4: Get a string value from the NVS.
NVSDelegateError_t NVSDelegate::get_str(NVSDelegateHandle_t handle, const char *key, char *out_value, size_t *length)
{
    // Step 4.1: Check if the key is valid.
    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
    {
        return NVSDelegateError_t::NVS_DELEGATE_KEY_INVALID;
    }

    // Step 4.2: Check if out_value and length are valid.
    if ( length == nullptr)
    {
        return NVSDelegateError_t::NVS_DELEGATE_VALUE_INVALID;
    }

    // if (handle.internalHandle == nullptr)
    //     return NVSDelegateError_t::NVS_DELEGATE_HANDLE_INVALID;

    // // Step 4.3: Retrieve the internal NVS handle.
    // nvs_handle_t *_handle = static_cast<nvs_handle_t *>(handle.internalHandle);

    // Step 4.4: Call nvs_get_str to get the string value.
    esp_err_t err = nvs_get_str(handle, key, out_value, length);

    // Step 4.5: Check the result of nvs_get_str and return the corresponding NVSDelegateError_t.
    switch (err)
    {
    case ESP_OK:
        return NVSDelegateError_t::NVS_DELEGATE_OK;
    case ESP_ERR_NVS_INVALID_HANDLE:
        return NVSDelegateError_t::NVS_DELEGATE_HANDLE_INVALID;
    case ESP_ERR_NVS_NOT_FOUND:
        return NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND;
    default:
        break;
    }

    return NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR;
}

// Step 5: Erase a key from the NVS.
NVSDelegateError_t NVSDelegate::erase_key(NVSDelegateHandle_t handle, const char *key)
{
    // Step 5.1: Check if the key is valid.
    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
    {
        return NVSDelegateError_t::NVS_DELEGATE_KEY_INVALID;
    }

    // if (handle.internalHandle == nullptr)
    //     return NVSDelegateError_t::NVS_DELEGATE_HANDLE_INVALID;

    // // Step 5.2: Retrieve the internal NVS handle.
    // nvs_handle_t *_handle = static_cast<nvs_handle_t *>(handle.internalHandle);

    // Step 5.3: Call nvs_erase_key to erase the key.
    esp_err_t err = nvs_erase_key(handle, key);

    // Step 5.4: Check the result of nvs_erase_key and return the corresponding NVSDelegateError_t.
    switch (err)
    {
    case ESP_OK:
        return NVSDelegateError_t::NVS_DELEGATE_OK;
    case ESP_ERR_NVS_INVALID_HANDLE:
        return NVSDelegateError_t::NVS_DELEGATE_HANDLE_INVALID;
    case ESP_ERR_NVS_NOT_FOUND:
        return NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND;
    case ESP_ERR_NVS_READ_ONLY:
        return NVSDelegateError_t::NVS_DELEGATE_READONLY;
    default:
        break;
    }

    return NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR;
}

// Step 6: Erase all keys from the NVS.
NVSDelegateError_t NVSDelegate::erase_all(NVSDelegateHandle_t handle)
{

    // if (handle.internalHandle == nullptr)
    //     return NVSDelegateError_t::NVS_DELEGATE_HANDLE_INVALID;

    // // Step 6.1: Retrieve the internal NVS handle.
    // nvs_handle_t *_handle = static_cast<nvs_handle_t *>(handle.internalHandle);

    // Step 6.2: Call nvs_erase_all to erase all keys.
    esp_err_t err = nvs_erase_all(handle);

    // Step 6.3: Check the result of nvs_erase_all and return the corresponding NVSDelegateError_t.
    switch (err)
    {
    case ESP_OK:
        return NVSDelegateError_t::NVS_DELEGATE_OK;
    case ESP_ERR_NVS_INVALID_HANDLE:
        return NVSDelegateError_t::NVS_DELEGATE_HANDLE_INVALID;
    case ESP_ERR_NVS_READ_ONLY:
        return NVSDelegateError_t::NVS_DELEGATE_READONLY;
    default:
        break;
    }

    return NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR;
}
