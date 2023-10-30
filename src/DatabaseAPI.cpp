#include <string.h> // for strcpy and strlen
#include <stddef.h> // size_t

#include "DatabaseAPI.hpp"
#include "DatabaseDelegateInterface.hpp"
#include "DatabaseError.hpp"

DatabaseAPI::DatabaseAPI(DatabaseDelegateInterface *delegate)
{
    // Set the delegate.
    this->delegate = delegate;
}

DatabaseError_t DatabaseAPI::get(const char *key, char *value, size_t *maxLength)
{
    // Check if the delegate is valid.
    if (delegate == nullptr)
    {
        return DATABASE_ERROR;
    }
    // Check if the key is valid.
    if (key == nullptr)
    {
        return DATABASE_KEY_INVALID;
    }

    // Check if the value and maximum length is valid.
    if (value == nullptr || maxLength == nullptr)
    {
        return DATABASE_VALUE_INVALID;
    }

    // Get the value.
    return delegate->get(key, value, maxLength);
}

DatabaseError_t DatabaseAPI::set(const char *key, const char *value)
{
    // Check if the delegate is valid.
    if (delegate == nullptr)
    {
        return DATABASE_ERROR;
    }
    // Check if the key is valid.
    if (key == nullptr)
    {
        return DATABASE_KEY_INVALID;
    }

    // Check if the value is valid.
    if (value == nullptr)
    {
        return DATABASE_VALUE_INVALID;
    }

    // Set the value.
    return delegate->insert(key, value);
}

DatabaseError_t DatabaseAPI::remove(const char *key)
{
    // Check if the delegate is valid.
    if (delegate == nullptr)
    {
        return DATABASE_ERROR;
    }
    // Check if the key is valid.
    if (key == nullptr)
    {
        return DATABASE_KEY_INVALID;
    }

    // Remove the key.
    return delegate->remove(key);
}

DatabaseError_t DatabaseAPI::isExist(const char *key)
{
    // Check if the delegate is valid.
    if (delegate == nullptr)
    {
        return DATABASE_ERROR;
    }
    // Check if the key is valid.
    if (key == nullptr)
    {
        return DATABASE_KEY_INVALID;
    }

    size_t maxLength = 0;
    // Check if the key exists.
    DatabaseError_t err = delegate->getLength(key, &maxLength);
    return err;
}

DatabaseError_t DatabaseAPI::getLength(const char *key, size_t *requiredLength)
{
    // Check if the delegate is valid.
    if (delegate == nullptr)
    {
        return DATABASE_ERROR;
    }
    // Check if the key is valid.
    if (key == nullptr)
    {
        return DATABASE_KEY_INVALID;
    }

    // Check if the required length is valid.
    if (requiredLength == nullptr)
    {
        return DATABASE_VALUE_INVALID;
    }

    // Get the length.
    return delegate->getLength(key, requiredLength);
}

DatabaseError_t DatabaseAPI::eraseAll()
{
    // Check if the delegate is valid.
    if (delegate == nullptr)
    {
        return DATABASE_ERROR;
    }

    // Erase all key-value pairs.
    return delegate->eraseAll();
}
