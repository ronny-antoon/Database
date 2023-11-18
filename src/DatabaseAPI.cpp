#include <string.h> // for strcpy and strlen

#include "DatabaseAPI.hpp"

DatabaseAPI::DatabaseAPI(DatabaseDelegateInterface *delegate, MultiPrinterLoggerInterface *logger) : _delegate(delegate), _logger(logger)
{
}

DatabaseError_t DatabaseAPI::get(const char *key, char *value, size_t *maxLength)
{
    // Check if the delegate is valid.
    if (_delegate == nullptr)
    {
        Log_Error(_logger, "Delegate is nullptr.");
        return DATABASE_ERROR;
    }
    // Check if the key is valid.
    if (key == nullptr)
    {
        Log_Error(_logger, "Key is nullptr.");
        return DATABASE_KEY_INVALID;
    }

    // Check if the value and maximum length is valid.
    if (value == nullptr || maxLength == nullptr)
    {
        Log_Error(_logger, "Value or maxLength is nullptr.");
        return DATABASE_VALUE_INVALID;
    }

    // Get the value.
    return _delegate->get(key, value, maxLength);
}

DatabaseError_t DatabaseAPI::set(const char *key, const char *value)
{
    // Check if the _delegate is valid.
    if (_delegate == nullptr)
    {
        Log_Error(_logger, "Delegate is nullptr.");
        return DATABASE_ERROR;
    }
    // Check if the key is valid.
    if (key == nullptr)
    {
        Log_Error(_logger, "Key is nullptr.");
        return DATABASE_KEY_INVALID;
    }

    // Check if the value is valid.
    if (value == nullptr)
    {
        Log_Error(_logger, "Value is nullptr.");
        return DATABASE_VALUE_INVALID;
    }

    // Set the value.
    return _delegate->insert(key, value);
}

DatabaseError_t DatabaseAPI::remove(const char *key)
{
    // Check if the _delegate is valid.
    if (_delegate == nullptr)
    {
        Log_Error(_logger, "Delegate is nullptr.");
        return DATABASE_ERROR;
    }
    // Check if the key is valid.
    if (key == nullptr)
    {
        Log_Error(_logger, "Key is nullptr.");
        return DATABASE_KEY_INVALID;
    }

    // Remove the key.
    return _delegate->remove(key);
}

DatabaseError_t DatabaseAPI::isExist(const char *key)
{
    // Check if the _delegate is valid.
    if (_delegate == nullptr)
    {
        Log_Error(_logger, "Delegate is nullptr.");
        return DATABASE_ERROR;
    }
    // Check if the key is valid.
    if (key == nullptr)
    {
        Log_Error(_logger, "Key is nullptr.");
        return DATABASE_KEY_INVALID;
    }

    size_t maxLength = 0;
    // Check if the key exists.
    DatabaseError_t err = _delegate->getLength(key, &maxLength);
    return err;
}

DatabaseError_t DatabaseAPI::getLength(const char *key, size_t *requiredLength)
{
    // Check if the _delegate is valid.
    if (_delegate == nullptr)
    {
        Log_Error(_logger, "Delegate is nullptr.");
        return DATABASE_ERROR;
    }
    // Check if the key is valid.
    if (key == nullptr)
    {
        Log_Error(_logger, "Key is nullptr.");
        return DATABASE_KEY_INVALID;
    }

    // Check if the required length is valid.
    if (requiredLength == nullptr)
    {
        Log_Error(_logger, "Required length is nullptr.");
        return DATABASE_VALUE_INVALID;
    }

    // Get the length.
    return _delegate->getLength(key, requiredLength);
}

DatabaseError_t DatabaseAPI::eraseAll()
{
    // Check if the _delegate is valid.
    if (_delegate == nullptr)
    {
        Log_Error(_logger, "Delegate is nullptr.");
        return DATABASE_ERROR;
    }

    // Erase all key-value pairs.
    return _delegate->eraseAll();
}

char *DatabaseAPI::errorToString(DatabaseError_t error, char *errorString, size_t *maxLength)
{
    // Check if the error string and maximum length is valid.
    if (errorString == nullptr || maxLength == nullptr)
    {
        return nullptr;
    }

    // Check if the error is valid.
    if (error < DATABASE_OK || error > DATABASE_ERROR)
    {
        return nullptr;
    }

    if (*maxLength < 50)
    {
        return nullptr;
    }
    // Get the error string.
    switch (error)
    {
    case DATABASE_OK:
        char errorStringTemp[] = "No Error.";
        strncpy(errorString, errorStringTemp, strlen(errorStringTemp));
        break;
    case DATABASE_KEY_INVALID:
        char errorStringTemp[] = "The key is invalid.";
        strncpy(errorString, errorStringTemp, strlen(errorStringTemp));
        break;
    case DATABASE_VALUE_INVALID:
        char errorStringTemp[] = "The value is invalid.";
        strncpy(errorString, errorStringTemp, strlen(errorStringTemp));
        break;
    case DATABASE_KEY_NOT_FOUND:
        char errorStringTemp[] = "The key was not found.";
        strncpy(errorString, errorStringTemp, strlen(errorStringTemp));
        break;
    case DATABASE_ERROR:
        char errorStringTemp[] = "Database error.";
        strncpy(errorString, errorStringTemp, strlen(errorStringTemp));
        break;
    default:
        return nullptr;
        break;
    }

    return errorString;
}