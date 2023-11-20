#include "DatabaseAPI.hpp"

// Constructor for DatabaseAPI
DatabaseAPI::DatabaseAPI(NVSDelegateInterface *nvsDelegate, const char *nvsNamespace, MultiPrinterLoggerInterface *logger) : _nvsDelegate(nvsDelegate), _logger(logger)
{
    Log_Info(_logger, "DatabaseAPI constructor called.");
    // If the provided namespace is invalid, use the default namespace "DEFAULT_NVS"
    if (nvsNamespace == nullptr || strlen(nvsNamespace) >= NVS_DELEGATE_MAX_NAMESPACE_LENGTH || strlen(nvsNamespace) == 0)
        strcpy(_nvsNamespace, "DEFAULT_NVS");
    else
        strcpy(_nvsNamespace, nvsNamespace);
}

// Destructor for DatabaseAPI
DatabaseAPI::~DatabaseAPI()
{
    Log_Info(_logger, "DatabaseAPI destructor called.");
}

// Retrieves the value associated with the specified key from the database
DatabaseError_t DatabaseAPI::get(const char *key, char *value, size_t maxValueLength)
{
    Log_Debug(_logger, "DatabaseAPI::get called.");
    // Ensure that the NVSDelegate is initialized
    if (_nvsDelegate == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    // Validate input parameters
    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
        return DatabaseError_t::DATABASE_KEY_INVALID;
    if (value == nullptr || maxValueLength == 0)
        return DatabaseError_t::DATABASE_VALUE_INVALID;

    // Open the NVS namespace in READONLY mode
    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READONLY, &handle);
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
    {
        // Handle specific errors, return appropriate DatabaseError_t value
        if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
            return DatabaseError_t::DATABASE_KEY_NOT_FOUND;
        else
            return DatabaseError_t::DATABASE_ERROR;
    }

    // Check the length of the value associated with the key
    err = _nvsDelegate->get_str(handle, key, nullptr, &maxValueLength);

    // Handle specific errors, close the NVS namespace, and return appropriate DatabaseError_t value
    if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
    {
        _nvsDelegate->close(handle);
        return DatabaseError_t::DATABASE_KEY_NOT_FOUND;
    }

    if (err == NVSDelegateError_t::NVS_DELEGATE_UNKOWN_ERROR)
    {
        _nvsDelegate->close(handle);
        return DatabaseError_t::DATABASE_ERROR;
    }

    // Retrieve the value associated with the key
    err = _nvsDelegate->get_str(handle, key, value, &maxValueLength);

    // Close the NVS namespace
    _nvsDelegate->close(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    return DatabaseError_t::DATABASE_OK;
}

// Sets the value for the specified key in the database
DatabaseError_t DatabaseAPI::set(const char *key, const char *value)
{
    Log_Debug(_logger, "DatabaseAPI::set called.");

    // Ensure that the NVSDelegate is initialized
    if (_nvsDelegate == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    // Validate input parameters
    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
        return DatabaseError_t::DATABASE_KEY_INVALID;
    if (value == nullptr || strlen(value) >= NVS_DELEGATE_MAX_VALUE_LENGTH || strlen(value) == 0)
        return DatabaseError_t::DATABASE_VALUE_INVALID;

    // Open the NVS namespace in READWRITE mode
    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READWRITE, &handle);
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    // Set the value for the specified key
    err = _nvsDelegate->set_str(handle, key, value);

    // Close the NVS namespace
    _nvsDelegate->close(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    return DatabaseError_t::DATABASE_OK;
}

// Removes the specified key and its associated value from the database
DatabaseError_t DatabaseAPI::remove(const char *key)
{
    Log_Debug(_logger, "DatabaseAPI::remove called.");
    // Ensure that the NVSDelegate is initialized
    if (_nvsDelegate == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    // Validate input parameters
    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
        return DatabaseError_t::DATABASE_KEY_INVALID;

    // Open the NVS namespace in READWRITE mode
    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READWRITE, &handle);
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
    {
        // Handle specific errors, return appropriate DatabaseError_t value
        if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
            return DatabaseError_t::DATABASE_KEY_NOT_FOUND;
        else
            return DatabaseError_t::DATABASE_ERROR;
    }

    // Erase the key and its associated value
    err = _nvsDelegate->erase_key(handle, key);

    // Close the NVS namespace
    _nvsDelegate->close(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
        return DatabaseError_t::DATABASE_KEY_NOT_FOUND;

    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    return DatabaseError_t::DATABASE_OK;
}

// Checks if the specified key exists in the database
DatabaseError_t DatabaseAPI::isExist(const char *key)
{
    Log_Debug(_logger, "DatabaseAPI::isExist called.");
    // Ensure that the NVSDelegate is initialized
    if (_nvsDelegate == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    // Validate input parameters
    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
        return DatabaseError_t::DATABASE_KEY_INVALID;

    // Open the NVS namespace in READONLY mode
    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READONLY, &handle);
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
    {
        // Handle specific errors, return appropriate DatabaseError_t value
        if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
            return DatabaseError_t::DATABASE_KEY_NOT_FOUND;
        else
            return DatabaseError_t::DATABASE_ERROR;
    }

    size_t length = 0;

    // Check the length of the value associated with the key
    err = _nvsDelegate->get_str(handle, key, nullptr, &length);

    // Close the NVS namespace
    _nvsDelegate->close(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
        return DatabaseError_t::DATABASE_KEY_NOT_FOUND;

    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    // If the length is zero, the key is considered not found
    if (length == 0)
        return DatabaseError_t::DATABASE_KEY_NOT_FOUND;

    return DatabaseError_t::DATABASE_OK;
}

// Retrieves the length of the value associated with the specified key
DatabaseError_t DatabaseAPI::getValueLength(const char *key, size_t *length)
{
    Log_Debug(_logger, "DatabaseAPI::getValueLength called.");
    // Ensure that the NVSDelegate is initialized
    if (_nvsDelegate == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    // Validate input parameters
    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
        return DatabaseError_t::DATABASE_KEY_INVALID;
    if (length == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    // Open the NVS namespace in READONLY mode
    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READONLY, &handle);
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    // Get the length of the value associated with the key
    err = _nvsDelegate->get_str(handle, key, nullptr, length);

    // Close the NVS namespace
    _nvsDelegate->close(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
        return DatabaseError_t::DATABASE_KEY_NOT_FOUND;

    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    return DatabaseError_t::DATABASE_OK;
}

// Removes all keys and values from the database
DatabaseError_t DatabaseAPI::eraseAll()
{
    Log_Debug(_logger, "DatabaseAPI::eraseAll called.");
    // Ensure that the NVSDelegate is initialized
    if (_nvsDelegate == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    // Open the NVS namespace in READWRITE mode
    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READWRITE, &handle);
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    // Erase all keys and values in the NVS namespace
    err = _nvsDelegate->erase_all(handle);

    // Close the NVS namespace
    _nvsDelegate->close(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    return DatabaseError_t::DATABASE_OK;
}

// Converts a DatabaseError_t value to a human-readable error string
void DatabaseAPI::errorToString(DatabaseError_t error, char *errorString, uint8_t maxLength)
{
    // Validate input parameters
    if (errorString == nullptr || maxLength < 50)
        return;

    // Convert the DatabaseError_t value to a human-readable error string
    switch (error)
    {
    case DatabaseError_t::DATABASE_OK:
        strncpy(errorString, "No Error.", maxLength);
        break;
    case DatabaseError_t::DATABASE_KEY_INVALID:
        strncpy(errorString, "Invalid Key.", maxLength);
        break;
    case DatabaseError_t::DATABASE_VALUE_INVALID:
        strncpy(errorString, "Invalid Value.", maxLength);
        break;
    case DatabaseError_t::DATABASE_KEY_NOT_FOUND:
        strncpy(errorString, "Key Not Found.", maxLength);
        break;
    case DatabaseError_t::DATABASE_KEY_ALREADY_EXISTS:
        strncpy(errorString, "Key Already Exists.", maxLength);
        break;
    case DatabaseError_t::DATABASE_NAMESPACE_INVALID:
        strncpy(errorString, "Invalid Namespace.", maxLength);
        break;
    case DatabaseError_t::DATABASE_NOT_ENOUGH_SPACE:
        strncpy(errorString, "Not Enough Space.", maxLength);
        break;
    case DatabaseError_t::DATABASE_ERROR:
        strncpy(errorString, "Internal Error.", maxLength);
        break;
    default:
        strncpy(errorString, "Unknown Error.", maxLength);
        break;
    }
}
