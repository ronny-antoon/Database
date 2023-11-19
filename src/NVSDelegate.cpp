#include "NVSDelegate.hpp"
#include <nvs.h>
#include <string.h>

NVSDelegateError_t NVSDelegate::open(const char *name, NVSDelegateOpenMode_t open_mode, NVSDelegateHandle_t *out_handle)
{
    // Check if the namespace name is valid
    if (name == nullptr || strlen(name) >= NVS_DELEGATE_MAX_NAMESPACE_LENGTH || strlen(name) == 0)
    {
        return NVSDelegateError_t::NVS_DELEGATE_NAMESPACE_INVALID;
    }

    // Convert the NVSDelegateOpenMode_t to nvs_open_mode_t
    nvs_open_mode_t _open_mode = open_mode == NVSDelegateOpenMode_t::NVSDelegate_READONLY ? NVS_READONLY : NVS_READWRITE;

    // Attempt to open the namespace
    esp_err_t err = nvs_open(name, _open_mode, out_handle);

    // Map ESP-IDF errors to NVSDelegateError_t
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

void NVSDelegate::close(NVSDelegateHandle_t handle)
{
    // Close the specified namespace
    nvs_close(handle);
}

NVSDelegateError_t NVSDelegate::set_str(NVSDelegateHandle_t handle, const char *key, const char *value)
{
    // Check if the key and value are valid
    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
    {
        return NVSDelegateError_t::NVS_DELEGATE_KEY_INVALID;
    }

    if (value == nullptr || strlen(value) >= NVS_DELEGATE_MAX_VALUE_LENGTH || strlen(value) == 0)
    {
        return NVSDelegateError_t::NVS_DELEGATE_VALUE_INVALID;
    }

    // Attempt to set the string value for the specified key
    esp_err_t err = nvs_set_str(handle, key, value);

    // Map ESP-IDF errors to NVSDelegateError_t
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

NVSDelegateError_t NVSDelegate::get_str(NVSDelegateHandle_t handle, const char *key, char *out_value, size_t *length)
{
    // Check if the key is valid
    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
    {
        return NVSDelegateError_t::NVS_DELEGATE_KEY_INVALID;
    }

    // Check if the length pointer is valid
    if (length == nullptr)
    {
        return NVSDelegateError_t::NVS_DELEGATE_VALUE_INVALID;
    }

    // Attempt to get the string value for the specified key
    esp_err_t err = nvs_get_str(handle, key, out_value, length);

    // Map ESP-IDF errors to NVSDelegateError_t
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

NVSDelegateError_t NVSDelegate::erase_key(NVSDelegateHandle_t handle, const char *key)
{
    // Check if the key is valid
    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
    {
        return NVSDelegateError_t::NVS_DELEGATE_KEY_INVALID;
    }

    // Attempt to erase the key and its associated value
    esp_err_t err = nvs_erase_key(handle, key);

    // Map ESP-IDF errors to NVSDelegateError_t
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

NVSDelegateError_t NVSDelegate::erase_all(NVSDelegateHandle_t handle)
{
    // Attempt to erase all keys and values in the specified namespace
    esp_err_t err = nvs_erase_all(handle);

    // Map ESP-IDF errors to NVSDelegateError_t
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
