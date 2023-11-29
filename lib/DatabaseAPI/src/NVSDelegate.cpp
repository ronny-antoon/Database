#include "NVSDelegate.hpp"

NVSDelegate::NVSDelegate(MultiPrinterLoggerInterface *const logger) : m_logger(logger)
{
    Log_Debug(m_logger, "NVSDelegate created");
}

NVSDelegate::~NVSDelegate()
{
    Log_Debug(m_logger, "NVSDelegate destroyed");
}

NVSDelegateError_t NVSDelegate::open(
    char const *const name, NVSDelegateOpenMode_t const open_mode,
    NVSDelegateHandle_t *out_handle) const
{
    // Check if the namespace name is valid
    if (!isNamespaceValid(name))
    {
        Log_Error(m_logger, "NVSDelegate open error: Invalid namespace name");
        return NVSDelegateError_t::NVS_DELEGATE_NAMESPACE_INVALID;
    }

    // Attempt to open the namespace
    esp_err_t err = nvs_open(name, mapOpenMode(open_mode), out_handle);

    // Map ESP-IDF errors to NVSDelegateError_t
    return mapErrorAndPrint(err);
}

void NVSDelegate::close(NVSDelegateHandle_t handle) const
{
    Log_Verbose(m_logger, "NVSDelegate closing namespace");
    // Close the specified namespace
    nvs_close(handle);
}

NVSDelegateError_t NVSDelegate::set_str(
    NVSDelegateHandle_t handle, char const *const key,
    char const *const value) const
{
    // Check if the key and value are valid
    if (!isKeyValid(key))
    {
        Log_Error(m_logger, "NVSDelegate set_str error: Invalid key");
        return NVSDelegateError_t::NVS_DELEGATE_KEY_INVALID;
    }

    if (!isValueValid(value))
    {
        Log_Error(m_logger, "NVSDelegate set_str error: Invalid value");
        return NVSDelegateError_t::NVS_DELEGATE_VALUE_INVALID;
    }

    Log_Verbose(m_logger, "NVSDelegate setting key '%s' to value '%s'", key, value);
    // Attempt to set the string value for the specified key
    esp_err_t err = nvs_set_str(handle, key, value);

    // Map ESP-IDF errors to NVSDelegateError_t
    return mapErrorAndPrint(err);
}

NVSDelegateError_t NVSDelegate::get_str(
    NVSDelegateHandle_t handle, char const *const key,
    char *out_value, size_t *length) const
{
    // Check if the key is valid
    if (!isKeyValid(key))
    {
        Log_Error(m_logger, "NVSDelegate get_str error: Invalid key");
        return NVSDelegateError_t::NVS_DELEGATE_KEY_INVALID;
    }

    // Check if the length pointer is valid
    if (length == nullptr)
    {
        Log_Error(m_logger, "NVSDelegate get_str error: Invalid length pointer");
        return NVSDelegateError_t::NVS_DELEGATE_VALUE_INVALID;
    }

    Log_Verbose(m_logger, "NVSDelegate getting value for key '%s'", key);
    // Attempt to get the string value for the specified key
    esp_err_t err = nvs_get_str(handle, key, out_value, length);

    // Map ESP-IDF errors to NVSDelegateError_t
    return mapErrorAndPrint(err);
}

NVSDelegateError_t NVSDelegate::erase_key(
    NVSDelegateHandle_t handle, char const *const key) const
{
    // Check if the key is valid
    if (!isKeyValid(key))
    {
        Log_Error(m_logger, "NVSDelegate erase_key error: Invalid key");
        return NVSDelegateError_t::NVS_DELEGATE_KEY_INVALID;
    }

    Log_Verbose(m_logger, "NVSDelegate erasing key '%s'", key);
    // Attempt to erase the key and its associated value
    esp_err_t err = nvs_erase_key(handle, key);

    // Map ESP-IDF errors to NVSDelegateError_t
    return mapErrorAndPrint(err);
}

NVSDelegateError_t NVSDelegate::erase_all(NVSDelegateHandle_t handle) const
{
    Log_Verbose(m_logger, "Erasing all keys and values from defualt");
    // Attempt to erase all keys and values in the specified namespace
    esp_err_t err = nvs_erase_all(handle);

    // Map ESP-IDF errors to NVSDelegateError_t
    return mapErrorAndPrint(err);
}

NVSDelegateError_t NVSDelegate::erase_flash_all() const
{
    Log_Verbose(m_logger, "Erasing all keys and values from all namespaces");
    // Attempt to erase all keys and values in all namespaces
    esp_err_t err = nvs_flash_erase();

    // Map ESP-IDF errors to NVSDelegateError_t
    return mapErrorAndPrint(err);
}

nvs_open_mode_t const NVSDelegate::mapOpenMode(NVSDelegateOpenMode_t const open_mode) const
{
    return (open_mode == NVSDelegateOpenMode_t::NVSDelegate_READONLY)
               ? NVS_READONLY
               : NVS_READWRITE;
}

NVSDelegateError_t const NVSDelegate::mapErrorAndPrint(esp_err_t const err) const
{
    switch (err)
    {
    case ESP_OK:
        return NVSDelegateError_t::NVS_DELEGATE_OK;
    case ESP_ERR_NVS_NOT_FOUND:
        Log_Error(m_logger, "Key not found");
        return NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND;
    case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
        Log_Error(m_logger, "Not enough space");
        return NVSDelegateError_t::NVS_DELEGATE_NOT_ENOUGH_SPACE;
    case ESP_ERR_NVS_INVALID_HANDLE:
        Log_Error(m_logger, "Invalid namespace handle");
        return NVSDelegateError_t::NVS_DELEGATE_HANDLE_INVALID;
    case ESP_ERR_NVS_READ_ONLY:
        Log_Error(m_logger, "Attempt to write in READONLY mode");
        return NVSDelegateError_t::NVS_DELEGATE_READONLY;
    default:
        break;
    }

    Log_Error(m_logger, "Unknown error");
    return NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR;
}

bool NVSDelegate::isNamespaceValid(const char *const name) const
{
    return name && strlen(name) > 0 && strlen(name) < NVS_DELEGATE_MAX_NAMESPACE_LENGTH;
}

bool NVSDelegate::isKeyValid(const char *const key) const
{
    return key && strlen(key) > 0 && strlen(key) < NVS_DELEGATE_MAX_KEY_LENGTH;
}

bool NVSDelegate::isValueValid(const char *const value) const
{
    return value && strlen(value) > 0 && strlen(value) < NVS_DELEGATE_MAX_VALUE_LENGTH;
}