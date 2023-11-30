#include "DatabaseAPI.hpp"

// Constructor for DatabaseAPI
DatabaseAPI::DatabaseAPI(
    NVSDelegateInterface *const nvsDelegate, char const *const nvsNamespace,
    MultiPrinterLoggerInterface *const logger)
    : _nvsDelegate(nvsDelegate), _logger(logger)
{
    // If the provided namespace is invalid, use the default namespace "DEFAULT_NVS"
    if (nvsNamespace == nullptr || strlen(nvsNamespace) >= NVS_DELEGATE_MAX_NAMESPACE_LENGTH || strlen(nvsNamespace) == 0)
        strcpy(_nvsNamespace, "DEFAULT_NVS");
    else
        strcpy(_nvsNamespace, nvsNamespace);

    Log_Debug(_logger, "DatabaseAPI created for namespace '%s'", _nvsNamespace);
}

// Destructor for DatabaseAPI
DatabaseAPI::~DatabaseAPI()
{
    Log_Debug(_logger, "DatabaseAPI destroyed");
}

// Retrieves the value associated with the specified key from the database
DatabaseError_t DatabaseAPI::get(
    char const *const key, char *value, size_t maxValueLength) const
{
    // Ensure that the Delegate is initialized
    if (!_nvsDelegate)
        return mapErrorAndPrint(NVS_DELEGATE_UNKOWN_ERROR);

    // Validate input parameters
    if (!isKeyValid(key))
        return mapErrorAndPrint(NVS_DELEGATE_KEY_INVALID);
    if (value == nullptr || maxValueLength == 0)
        return mapErrorAndPrint(NVS_DELEGATE_VALUE_INVALID);

    // Open the NVS namespace in READONLY mode
    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READONLY, &handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    // Check the length of the value associated with the key
    err = _nvsDelegate->get_str(handle, key, nullptr, &maxValueLength);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
    {
        _nvsDelegate->close(handle);
        return mapErrorAndPrint(err);
    }

    // Retrieve the value associated with the key
    err = _nvsDelegate->get_str(handle, key, value, &maxValueLength);

    // Close the NVS namespace
    _nvsDelegate->close(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    Log_Verbose(_logger, "Key '%s' retrieved successfully", key);
    return DATABASE_OK;
}

// Sets the value for the specified key in the database
DatabaseError_t DatabaseAPI::set(char const *const key, char const *const value)
{
    // Ensure that the Delegate is initialized
    if (!_nvsDelegate)
        return mapErrorAndPrint(NVS_DELEGATE_UNKOWN_ERROR);

    // Validate input parameters
    if (!isKeyValid(key))
        return mapErrorAndPrint(NVS_DELEGATE_KEY_INVALID);
    if (value == nullptr || strlen(value) >= NVS_DELEGATE_MAX_VALUE_LENGTH || strlen(value) == 0)
        return mapErrorAndPrint(NVS_DELEGATE_VALUE_INVALID);

    // Open the NVS namespace in READWRITE mode
    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READWRITE, &handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    // Set the value for the specified key
    err = _nvsDelegate->set_str(handle, key, value);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
    {
        _nvsDelegate->close(handle);
        return mapErrorAndPrint(err);
    }

    err = _nvsDelegate->commit(handle);

    // Close the NVS namespace
    _nvsDelegate->close(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    Log_Verbose(_logger, "Key '%s' set successfully", key);
    return DATABASE_OK;
}

// Removes the specified key and its associated value from the database
DatabaseError_t DatabaseAPI::remove(char const *const key)
{
    // Ensure that the Delegate is initialized
    if (!_nvsDelegate)
        return mapErrorAndPrint(NVS_DELEGATE_UNKOWN_ERROR);

    // Validate input parameters
    if (!isKeyValid(key))
        return mapErrorAndPrint(NVS_DELEGATE_KEY_INVALID);

    // Open the NVS namespace in READWRITE mode
    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READWRITE, &handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    // Erase the key and its associated value
    err = _nvsDelegate->erase_key(handle, key);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
    {
        _nvsDelegate->close(handle);
        return mapErrorAndPrint(err);
    }

    err = _nvsDelegate->commit(handle);

    // Close the NVS namespace
    _nvsDelegate->close(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    Log_Verbose(_logger, "Key '%s' removed successfully", key);
    return DATABASE_OK;
}

// Checks if the specified key exists in the database
DatabaseError_t DatabaseAPI::isExist(char const *const key) const
{
    // Ensure that the Delegate is initialized
    if (!_nvsDelegate)
        return mapErrorAndPrint(NVS_DELEGATE_UNKOWN_ERROR);

    // Validate input parameters
    if (!isKeyValid(key))
        return mapErrorAndPrint(NVS_DELEGATE_KEY_INVALID);

    // Open the NVS namespace in READONLY mode
    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READONLY, &handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    size_t length = 0;

    // Check the length of the value associated with the key
    err = _nvsDelegate->get_str(handle, key, nullptr, &length);

    // Close the NVS namespace
    _nvsDelegate->close(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    // If the length is zero, the key is considered not found
    if (length == 0)
        return mapErrorAndPrint(NVS_DELEGATE_KEY_NOT_FOUND);

    Log_Verbose(_logger, "Key '%s' exists", key);
    return DATABASE_OK;
}

// Retrieves the length of the value associated with the specified key
DatabaseError_t DatabaseAPI::getValueLength(
    char const *const key, size_t *requiredLength) const
{
    // Ensure that the Delegate is initialized
    if (!_nvsDelegate)
        return mapErrorAndPrint(NVS_DELEGATE_UNKOWN_ERROR);

    // Validate input parameters
    if (!isKeyValid(key))
        return mapErrorAndPrint(NVS_DELEGATE_KEY_INVALID);
    if (requiredLength == nullptr)
        return mapErrorAndPrint(NVS_DELEGATE_VALUE_INVALID);

    // Open the NVS namespace in READONLY mode
    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READONLY, &handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    // Get the length of the value associated with the key
    err = _nvsDelegate->get_str(handle, key, nullptr, requiredLength);

    // Close the NVS namespace
    _nvsDelegate->close(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    Log_Verbose(_logger, "Length of value for key '%s' is %zu", key, *requiredLength);
    return DATABASE_OK;
}

// Removes all keys and values from the database
DatabaseError_t DatabaseAPI::eraseAll()
{
    // Ensure that the Delegate is initialized
    if (!_nvsDelegate)
        return mapErrorAndPrint(NVS_DELEGATE_UNKOWN_ERROR);

    // Open the NVS namespace in READWRITE mode
    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READWRITE, &handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    // Erase all keys and values in the NVS namespace
    err = _nvsDelegate->erase_all(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
    {
        _nvsDelegate->close(handle);
        return mapErrorAndPrint(err);
    }

    err = _nvsDelegate->commit(handle);

    // Close the NVS
    _nvsDelegate->close(handle);

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    Log_Verbose(_logger, "All keys and values erased successfully");
    return DATABASE_OK;
}

// Format Flash partition
DatabaseError_t DatabaseAPI::eraseFlashAll()
{
    // Ensure that the Delegate is initialized
    if (!_nvsDelegate)
        return mapErrorAndPrint(NVS_DELEGATE_UNKOWN_ERROR);

    // Erase the entire Flash partition
    NVSDelegateError_t err = _nvsDelegate->erase_flash_all();

    // Handle specific errors and return appropriate DatabaseError_t value
    if (err != NVS_DELEGATE_OK)
        return mapErrorAndPrint(err);

    Log_Verbose(_logger, "Flash partition erased successfully");
    return DATABASE_OK;
}

DatabaseError_t const DatabaseAPI::mapErrorAndPrint(NVSDelegateError_t const err) const
{
    switch (err)
    {
    case NVS_DELEGATE_OK:
        return DATABASE_OK;
    case NVS_DELEGATE_KEY_INVALID:
        Log_Error(_logger, "Invalid key");
        return DATABASE_KEY_INVALID;
    case NVS_DELEGATE_NOT_ENOUGH_SPACE:
        Log_Error(_logger, "Not enough space");
        return DATABASE_NOT_ENOUGH_SPACE;
    case NVS_DELEGATE_NAMESPACE_INVALID:
        Log_Error(_logger, "Invalid namespace name");
        return DATABASE_NAMESPACE_INVALID;
    case NVS_DELEGATE_HANDLE_INVALID:
        Log_Error(_logger, "Invalid namespace handle");
        return DATABASE_ERROR;
    case NVS_DELEGATE_READONLY:
        Log_Error(_logger, "Attempt to modify in READONLY mode");
        return DATABASE_ERROR;
    case NVS_DELEGATE_VALUE_INVALID:
        Log_Error(_logger, "Invalid value");
        return DATABASE_VALUE_INVALID;
    case NVS_DELEGATE_KEY_NOT_FOUND:
        Log_Error(_logger, "Key not found");
        return DATABASE_KEY_NOT_FOUND;
    case NVS_DELEGATE_KEY_ALREADY_EXISTS:
        Log_Error(_logger, "Key already exists");
        return DATABASE_KEY_ALREADY_EXISTS;
    default:
        break;
    }

    Log_Error(_logger, "Unknown error");
    return DATABASE_ERROR;
}

bool DatabaseAPI::isKeyValid(char const *const key) const
{
    return key && strlen(key) > 0 && strlen(key) < NVS_DELEGATE_MAX_KEY_LENGTH;
}
