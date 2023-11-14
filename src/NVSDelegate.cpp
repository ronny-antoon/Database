#include <string.h> // for strcpy and strlen

#include "NVSDelegate.hpp"

NVSDelegate::NVSDelegate(const char *partitionName)
{
    // Check if the partition name is valid
    size_t partitionNameLength;
    if (partitionName)
    {
        partitionNameLength = strlen(partitionName);
        if (partitionNameLength < NVS_DELEGATE_MAX_PARTITION_NAME_LENGTH && partitionNameLength > 0)
        {
            // Copy the partition name to the NVSDelegate object
            char *_partitionName = new char[strlen(partitionName) + 1];
            strcpy(_partitionName, partitionName);
            this->partitionName = _partitionName;
            return;
        }
    }
    // If the partition name is invalid, use the default partition name
    char *_partitionName = new char[strlen(NVS_DELEGATE_DEFAULT_PARTITION_NAME) + 1];
    strcpy(_partitionName, NVS_DELEGATE_DEFAULT_PARTITION_NAME);
    this->partitionName = _partitionName;
}

NVSDelegate::~NVSDelegate()
{
    // Delete the partition name
    if (partitionName)
    {
        delete[] partitionName;
        partitionName = nullptr;
    }
}

DatabaseError_t NVSDelegate::insert(const char *key, const char *value)
{
    // Check if the key is valid
    if (!isKeyValid(key))
    {
        return DATABASE_KEY_INVALID;
    }

    // Check if the value is valid
    if (!isValueValid(value))
    {
        return DATABASE_VALUE_INVALID;
    }

    // Check if the key already exists
    if (keyExists(key) == DATABASE_OK)
    {
        return DATABASE_KEY_ALREADY_EXISTS;
    }

    // Open the NVS database in read-write mode
    DatabaseError_t err = openDatabase(false);
    if (err != DATABASE_OK)
    {
        closeDatabase();
        return err;
    }

    // Set the key-value pair in the NVS database
    esp_err_t nvs_err = nvs_set_str(nvsHandle, key, value);

    // Check if the key-value pair was successfully set
    if (nvs_err != ESP_OK)
    {
        closeDatabase();
        return DATABASE_ERROR;
    }

    // Commit the changes to the NVS database
    err = commitDatabase();

    // Check if the commit operation was successful
    if (err != DATABASE_OK)
    {
        closeDatabase();
        return err;
    }

    // Close the NVS database
    closeDatabase();

    return DATABASE_OK;
}

DatabaseError_t NVSDelegate::update(const char *key, const char *value)
{
    // Check if the key is valid
    if (!isKeyValid(key))
    {
        return DATABASE_KEY_INVALID;
    }

    // Check if the value is valid
    if (!isValueValid(value))
    {
        return DATABASE_VALUE_INVALID;
    }

    // Check if the key already exists
    if (keyExists(key) != DATABASE_OK)
    {
        return DATABASE_KEY_NOT_FOUND;
    }

    // Open the NVS database in read-write mode
    DatabaseError_t err = openDatabase(false);
    if (err != DATABASE_OK)
    {
        closeDatabase();
        return err;
    }

    // Set the key-value pair in the NVS database
    esp_err_t nvs_err = nvs_set_str(nvsHandle, key, value);

    // Check if the key-value pair was successfully set
    if (nvs_err != ESP_OK)
    {
        closeDatabase();
        return DATABASE_ERROR;
    }

    // Commit the changes to the NVS database
    err = commitDatabase();

    // Check if the commit operation was successful
    if (err != DATABASE_OK)
    {
        closeDatabase();
        return err;
    }

    // Close the NVS database
    closeDatabase();

    return DATABASE_OK;
}

DatabaseError_t NVSDelegate::remove(const char *key)
{
    // Check if the key is valid
    if (!isKeyValid(key))
    {
        return DATABASE_KEY_INVALID;
    }

    // Check if the key already exists
    if (keyExists(key) != DATABASE_OK)
    {
        return DATABASE_KEY_NOT_FOUND;
    }

    // Open the NVS database in read-write mode
    DatabaseError_t err = openDatabase(false);
    if (err != DATABASE_OK)
    {
        closeDatabase();
        return err;
    }

    // Erase the key-value pair from the NVS database
    esp_err_t nvs_err = nvs_erase_key(nvsHandle, key);

    // Check if the key-value pair was successfully erased
    if (nvs_err != ESP_OK)
    {
        closeDatabase();
        return DATABASE_ERROR;
    }

    // Commit the changes to the NVS database
    err = commitDatabase();

    // Check if the commit operation was successful
    if (err != DATABASE_OK)
    {
        closeDatabase();
        return err;
    }

    // Close the NVS database
    closeDatabase();

    return DATABASE_OK;
}

DatabaseError_t NVSDelegate::get(const char *key, char *value, size_t *maxLength)
{
    // Check if the key is valid
    if (!isKeyValid(key))
    {
        return DATABASE_KEY_INVALID;
    }

    // Check if the value is valid
    if (!value)
    {
        return DATABASE_VALUE_INVALID;
    }

    // Check if the key already exists
    if (keyExists(key) != DATABASE_OK)
    {
        return DATABASE_KEY_NOT_FOUND;
    }

    // Open the NVS database in read-only mode
    DatabaseError_t err = openDatabase(true);
    if (err != DATABASE_OK)
    {
        closeDatabase();
        return err;
    }

    size_t requiredLength;
    // Get the length of the value associated with the key
    esp_err_t err_nvs = nvs_get_str(nvsHandle, key, NULL, &requiredLength);

    // Check if the value buffer length is sufficient
    if (requiredLength > *maxLength)
    {
        closeDatabase();
        return DATABASE_VALUE_INVALID;
    }

    // Get the value associated with the key from the NVS database
    err_nvs = nvs_get_str(nvsHandle, key, value, maxLength);

    // Check if the key-value pair was successfully retrieved
    if (err_nvs != ESP_OK)
    {
        closeDatabase();
        return DATABASE_ERROR;
    }

    // Close the NVS database
    closeDatabase();

    return DATABASE_OK;
}

DatabaseError_t NVSDelegate::getLength(const char *key, size_t *requiredLength)
{
    // Check if the key is valid
    if (!isKeyValid(key))
    {
        return DATABASE_KEY_INVALID;
    }

    // Check if the key already exists
    if (keyExists(key) != DATABASE_OK)
    {
        return DATABASE_KEY_NOT_FOUND;
    }

    // Open the NVS database in read-only mode
    DatabaseError_t err = openDatabase(true);
    if (err != DATABASE_OK)
    {
        closeDatabase();
        return err;
    }

    // Get the length of the value associated with the key
    esp_err_t nvs_err = nvs_get_str(nvsHandle, key, NULL, requiredLength);

    // Check if the key-value pair was successfully retrieved
    if (nvs_err != ESP_OK)
    {
        closeDatabase();
        return DATABASE_ERROR;
    }

    // Close the NVS database
    closeDatabase();

    return DATABASE_OK;
}

DatabaseError_t NVSDelegate::eraseAll()
{
    // Open the NVS database in read-write mode
    DatabaseError_t err = openDatabase(false);
    if (err != DATABASE_OK)
    {
        closeDatabase();
        return err;
    }

    // Erase all key-value pairs from the NVS database
    esp_err_t nvs_err = nvs_erase_all(nvsHandle);

    // Check if all key-value pairs were successfully erased
    if (nvs_err != ESP_OK)
    {
        closeDatabase();
        return DATABASE_ERROR;
    }

    // Commit the changes to the NVS database
    err = commitDatabase();

    // Check if the commit operation was successful
    if (err != DATABASE_OK)
    {
        closeDatabase();
        return err;
    }

    // Close the NVS database
    closeDatabase();

    return DATABASE_OK;
}

bool NVSDelegate::isKeyValid(const char *key)
{
    if (key)
    {
        size_t keyLength = strlen(key);
        if (keyLength < NVS_DELEGATE_MAX_KEY_LENGTH && keyLength > 0)
        {
            return true;
        }
    }
    return false;
}

bool NVSDelegate::isValueValid(const char *value)
{
    if (value)
    {
        size_t valueLength = strlen(value);
        if (valueLength < NVS_DELEGATE_MAX_VALUE_LENGTH && valueLength > 0)
        {
            return true;
        }
    }
    return false;
}

DatabaseError_t NVSDelegate::openDatabase(bool _readOnly)
{
    // Determine the NVS open mode based on the read-only flag
    nvs_open_mode_t mode = _readOnly ? NVS_READONLY : NVS_READWRITE;

    // Attempt to open the NVS database with the given namespace
    esp_err_t err = nvs_open(partitionName, mode, &nvsHandle);

    // Check if the NVS database was successfully opened
    if (err != ESP_OK)
    {
        if (err == ESP_ERR_NVS_NOT_FOUND)
            return DATABASE_KEY_NOT_FOUND;
        else
            return DATABASE_ERROR;
    }
    return DATABASE_OK;
}

void NVSDelegate::closeDatabase()
{
    // Close the NVS database
    nvs_close(nvsHandle);
}

DatabaseError_t NVSDelegate::commitDatabase()
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

DatabaseError_t NVSDelegate::keyExists(const char *key)
{
    // Check if the key is valid
    if (!isKeyValid(key))
    {
        return DATABASE_KEY_INVALID;
    }

    // Open the NVS database in read-only mode, and check if the open operation was successful
    DatabaseError_t err = openDatabase(true);
    if (err != DATABASE_OK)
    {
        closeDatabase();
        return err;
    }

    size_t requiredLength;
    // Check if the key exists in the NVS database
    esp_err_t nvs_err = nvs_get_str(nvsHandle, key, NULL, &requiredLength);

    // Check if the key exists
    if (nvs_err == ESP_OK)
    {
        closeDatabase();
        return DATABASE_OK;
    }
    else if (err == ESP_ERR_NVS_NOT_FOUND)
    {
        closeDatabase();
        return DATABASE_KEY_NOT_FOUND;
    }
    else
    {
        closeDatabase();
        return DATABASE_ERROR;
    }
}
