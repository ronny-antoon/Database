#include <string.h>

#include "DatabaseAPI.hpp"
#include "NVSDelegateInterface.hpp"

DatabaseAPI::DatabaseAPI(NVSDelegateInterface *nvsDelegate, const char *nvsNamespace) : _nvsDelegate(nvsDelegate)
{
    if (nvsNamespace == nullptr || strlen(nvsNamespace) >= NVS_DELEGATE_MAX_NAMESPACE_LENGTH || strlen(nvsNamespace) == 0)
        strcpy(_nvsNamespace, "DEFAULT_NVS");
    else
        strcpy(_nvsNamespace, nvsNamespace);
}

DatabaseError_t DatabaseAPI::get(const char *key, char *value, size_t maxValueLength)
{
    if (_nvsDelegate == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
        return DatabaseError_t::DATABASE_KEY_INVALID;
    if (value == nullptr || maxValueLength == 0)
        return DatabaseError_t::DATABASE_VALUE_INVALID;

    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READONLY, &handle);
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
    {
        if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
            return DatabaseError_t::DATABASE_KEY_NOT_FOUND;
        else
            return DatabaseError_t::DATABASE_ERROR;
    }

    err = _nvsDelegate->get_str(handle, key, nullptr, &maxValueLength);

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

    err = _nvsDelegate->get_str(handle, key, value, &maxValueLength);

    _nvsDelegate->close(handle);

    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    return DatabaseError_t::DATABASE_OK;
}

DatabaseError_t DatabaseAPI::set(const char *key, const char *value)
{
    if (_nvsDelegate == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
        return DatabaseError_t::DATABASE_KEY_INVALID;
    if (value == nullptr || strlen(value) >= NVS_DELEGATE_MAX_VALUE_LENGTH || strlen(value) == 0)
        return DatabaseError_t::DATABASE_VALUE_INVALID;

    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READWRITE, &handle);
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    err = _nvsDelegate->set_str(handle, key, value);

    _nvsDelegate->close(handle);

    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    return DatabaseError_t::DATABASE_OK;
}

DatabaseError_t DatabaseAPI::remove(const char *key)
{
    if (_nvsDelegate == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
        return DatabaseError_t::DATABASE_KEY_INVALID;

    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READWRITE, &handle);
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
    {
        if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
            return DatabaseError_t::DATABASE_KEY_NOT_FOUND;
        else
            return DatabaseError_t::DATABASE_ERROR;
    }

    err = _nvsDelegate->erase_key(handle, key);

    _nvsDelegate->close(handle);

    if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
        return DatabaseError_t::DATABASE_KEY_NOT_FOUND;

    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    return DatabaseError_t::DATABASE_OK;
}

DatabaseError_t DatabaseAPI::isExist(const char *key)
{
    if (_nvsDelegate == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
        return DatabaseError_t::DATABASE_KEY_INVALID;

    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READONLY, &handle);
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
    {
        if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
            return DatabaseError_t::DATABASE_KEY_NOT_FOUND;
        else
            return DatabaseError_t::DATABASE_ERROR;
    }

    size_t length = 0;

    err = _nvsDelegate->get_str(handle, key, nullptr, &length);

    _nvsDelegate->close(handle);

    if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
        return DatabaseError_t::DATABASE_KEY_NOT_FOUND;

    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    if (length == 0)
        return DatabaseError_t::DATABASE_KEY_NOT_FOUND;

    return DatabaseError_t::DATABASE_OK;
}

DatabaseError_t DatabaseAPI::getValueLength(const char *key, size_t *length)
{
    if (_nvsDelegate == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    if (key == nullptr || strlen(key) >= NVS_DELEGATE_MAX_KEY_LENGTH || strlen(key) == 0)
        return DatabaseError_t::DATABASE_KEY_INVALID;
    if (length == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READONLY, &handle);
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    err = _nvsDelegate->get_str(handle, key, nullptr, length);

    _nvsDelegate->close(handle);

    if (err == NVSDelegateError_t::NVS_DELEGATE_KEY_NOT_FOUND)
        return DatabaseError_t::DATABASE_KEY_NOT_FOUND;

    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    return DatabaseError_t::DATABASE_OK;
}

DatabaseError_t DatabaseAPI::eraseAll()
{
    if (_nvsDelegate == nullptr)
        return DatabaseError_t::DATABASE_ERROR;

    NVSDelegateHandle_t handle;
    NVSDelegateError_t err = _nvsDelegate->open(_nvsNamespace, NVSDelegateOpenMode_t::NVSDelegate_READWRITE, &handle);
    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    err = _nvsDelegate->erase_all(handle);

    _nvsDelegate->close(handle);

    if (err != NVSDelegateError_t::NVS_DELEGATE_OK)
        return DatabaseError_t::DATABASE_ERROR;

    return DatabaseError_t::DATABASE_OK;
}

void DatabaseAPI::errorToString(DatabaseError_t error, char *errorString, uint8_t maxLength)
{
    if (errorString == nullptr || maxLength < 50)
        return;

    switch (error)
    {
    case DatabaseError_t::DATABASE_OK:
        strncpy(errorString, "No Error.", maxLength);
        break;
    case DatabaseError_t::DATABASE_ERROR:
        strncpy(errorString, "Internal Error.", maxLength);
        break;
    case DatabaseError_t::DATABASE_KEY_INVALID:
        strncpy(errorString, "Key is invalid.", maxLength);
        break;
    case DatabaseError_t::DATABASE_VALUE_INVALID:
        strncpy(errorString, "Value is invalid.", maxLength);
        break;
    case DatabaseError_t::DATABASE_KEY_NOT_FOUND:
        strncpy(errorString, "Key not found.", maxLength);
        break;
    default:
        strncpy(errorString, "Unknown Error.", maxLength);
        break;
    }
}