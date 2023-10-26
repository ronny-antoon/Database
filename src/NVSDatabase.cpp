#include "NVSDatabase.hpp"
#include <string.h>

DatabaseError_t NVSDatabase::open(bool _readOnly)
{
    // Determine the NVS open mode based on the read-only flag
    nvs_open_mode_t mode = _readOnly ? NVS_READONLY : NVS_READWRITE;

    // Attempt to open the NVS database with the given namespace
    esp_err_t err = nvs_open(nvsNamespace, mode, &nvsHandle);

    // Check if the NVS database was successfully opened
    if (err != ESP_OK)
    {
        // Check if the namespace was not found
        if (err == ESP_ERR_NVS_NOT_FOUND)
            return DATABASE_NOT_FOUND;
        else
            return DATABASE_OPEN_ERROR;
    }

    return DATABASE_OK;
}

DatabaseError_t NVSDatabase::close()
{
    // Close the NVS database
    nvs_close(nvsHandle);

    return DATABASE_OK;
}

DatabaseError_t NVSDatabase::commit()
{
    // Commit any pending changes in the NVS database
    esp_err_t err = nvs_commit(nvsHandle);

    // Check if the commit operation was successful
    if (err != ESP_OK)
    {
        return DATABASE_ERROR;
    }

    return DATABASE_OK;
}

NVSDatabase::NVSDatabase(const char *_nvsNamespace)
{
    nvsNamespace = (char *)calloc(strlen(_nvsNamespace) + 1, sizeof(char));
    strcpy((char *)nvsNamespace, _nvsNamespace);
    nvsHandle = 0;
}

NVSDatabase::~NVSDatabase()
{
    // Free the namespace string
    free((char *)nvsNamespace);
}

DatabaseError_t NVSDatabase::isExist(const char *_key)
{
    // Check if the key is empty or null
    if (_key == nullptr || _key[0] == '\0' || strcmp(_key, "") == 0)
        return DATABASE_KEY_EMPTY;

    // Check if the key size exceeds the maximum allowed
    if (strlen(_key) > NVS_MAX_KEY_LENGTH)
        return DATABASE_KEY_INVALID_SIZE;

    // Open the NVS database in read-only mode
    DatabaseError_t errResult = open(true);
    if (errResult != DATABASE_OK)
    {
        return errResult;
    }

    size_t _requiredLength;
    // Check if the key exists in the NVS database
    esp_err_t err = nvs_get_str(nvsHandle, _key, NULL, &_requiredLength);

    // Check if the key was not found in the database
    if (err != ESP_OK)
    {
        if (err == ESP_ERR_NVS_NOT_FOUND)
            errResult = DATABASE_NOT_FOUND;
        else
            errResult = DATABASE_ERROR;
    }

    // Close the NVS database
    close();

    return errResult;
}

DatabaseError_t NVSDatabase::putPair(const char *_key, const char *_value)
{
    // Check if the key is empty or null
    if (_key == nullptr || _key[0] == '\0' || strcmp(_key, "") == 0)
        return DATABASE_KEY_EMPTY;

    // Check if the value is empty or null
    if (_value == nullptr || _value[0] == '\0' || strcmp(_value, "") == 0)
        return DATABASE_VALUE_EMPTY;

    // Check if the key size exceeds the maximum allowed
    if (strlen(_key) > NVS_MAX_KEY_LENGTH)
        return DATABASE_KEY_INVALID_SIZE;

    // Check if the value size exceeds the maximum allowed
    if (strlen(_value) > NVS_MAX_VALUE_LENGTH)
        return DATABASE_VALUE_INVALID_SIZE;

    // Open the NVS database in write mode
    DatabaseError_t errResult = open(false);
    if (errResult != DATABASE_OK)
    {
        return errResult;
    }

    // Set a string value in the NVS database
    esp_err_t err = nvs_set_str(nvsHandle, _key, _value);

    // Check the result of the nvs_set_str operation and map it to a DatabaseError_t
    if (err != ESP_OK)
    {
        errResult = DATABASE_ERROR;
    }
    else
    {
        errResult = commit();
    }

    // Close the NVS database
    close();

    return errResult;
}

DatabaseError_t NVSDatabase::getValueOf(const char *_key, char *_value, size_t *_maxSize)
{
    // Check if the key is empty or null
    if (_key == nullptr || _key[0] == '\0' || strcmp(_key, "") == 0)
        return DATABASE_KEY_EMPTY;

    // Check if the key size exceeds the maximum allowed
    if (strlen(_key) > NVS_MAX_KEY_LENGTH)
        return DATABASE_KEY_INVALID_SIZE;

    // Check if the value is  null
    if (_value == nullptr)
        return DATABASE_ERROR;

    // Check if the maxSize pointer is null
    if (_maxSize == nullptr)
        return DATABASE_ERROR;

    // Open the NVS database in read-only mode
    DatabaseError_t errResult = open(true);

    size_t _requiredLength;
    esp_err_t err = nvs_get_str(nvsHandle, _key, NULL, &_requiredLength);

    // Check if the key was not found in the database
    if (err != ESP_OK)
    {
        if (err == ESP_ERR_NVS_NOT_FOUND)
            errResult = DATABASE_NOT_FOUND;
        else
            errResult = DATABASE_ERROR;
    }
    else
    {
        // Check if the provided buffer size is insufficient for the value
        if (*_maxSize < _requiredLength)
        {
            errResult = DATABASE_VALUE_INVALID_SIZE;
        }
        else
        {
            err = nvs_get_str(nvsHandle, _key, _value, _maxSize);
            // Check if there was an error in retrieving the value
            if (err != ESP_OK)
            {
                errResult = DATABASE_ERROR;
            }
        }
    }

    // Close the NVS database
    close();

    return errResult;
}

DatabaseError_t NVSDatabase::getValueLengthOf(const char *_key, size_t *_requiredLength)
{
    // Check if the key is empty or null
    if (_key == nullptr || _key[0] == '\0' || strcmp(_key, "") == 0)
        return DATABASE_KEY_EMPTY;

    // Check if the key size exceeds the maximum allowed
    if (strlen(_key) > NVS_MAX_KEY_LENGTH)
        return DATABASE_KEY_INVALID_SIZE;

    // Check if the requiredLength pointer is null
    if (_requiredLength == nullptr)
        return DATABASE_ERROR;

    // Open the NVS database in read-only mode
    DatabaseError_t errResult = open(true);
    if (errResult != DATABASE_OK)
    {
        return errResult;
    }

    esp_err_t err = nvs_get_str(nvsHandle, _key, NULL, _requiredLength);

    // Check if the key was not found in the database
    if (err != ESP_OK)
    {
        if (err == ESP_ERR_NVS_NOT_FOUND)
            errResult = DATABASE_NOT_FOUND;
        else
            errResult = DATABASE_ERROR;
    }

    // Close the NVS database
    close();

    return errResult;
}

DatabaseError_t NVSDatabase::removePair(const char *_key)
{
    // Check if the key is empty or null
    if (_key == nullptr || _key[0] == '\0' || strcmp(_key, "") == 0)
        return DATABASE_KEY_EMPTY;

    // Check if the key size exceeds the maximum allowed
    if (strlen(_key) > NVS_MAX_KEY_LENGTH)
        return DATABASE_KEY_INVALID_SIZE;

    // Open the NVS database in write mode
    DatabaseError_t errResult = open(false);
    if (errResult != DATABASE_OK)
    {
        return errResult;
    }

    // Remove a key from the NVS database
    esp_err_t err = nvs_erase_key(nvsHandle, _key);

    // Check if the key was not found in the database
    if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        errResult = DATABASE_NOT_FOUND;
    }
    // Check if there was an error in erasing the key
    else if (err != ESP_OK)
    {
        errResult = DATABASE_ERROR;
    }
    else
    {
        errResult = commit();
    }

    // Close the NVS database
    close();

    return errResult;
}

DatabaseError_t NVSDatabase::eraseAll()
{
    // Open the NVS database in write mode
    DatabaseError_t errResult = open(false);
    if (errResult != DATABASE_OK)
    {
        return errResult;
    }

    // Erase all keys and values from the NVS database
    esp_err_t err = nvs_erase_all(nvsHandle);

    // Check if there was an error in erasing all keys and values
    if (err != ESP_OK)
    {
        errResult = DATABASE_ERROR;
    }
    else
    {
        errResult = commit();
    }

    // Close the NVS database
    close();

    return errResult;
}
