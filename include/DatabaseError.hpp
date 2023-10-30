#ifndef DATABASE_ERROR_H
#define DATABASE_ERROR_H

#include <stdint.h> // uint8_t

// Error codes for database
enum DatabaseError_t : uint8_t
{
    DATABASE_OK,                 // No error
    DATABASE_KEY_INVALID,        // Key is invalid
    DATABASE_VALUE_INVALID,      // Invalid value
    DATABASE_KEY_NOT_FOUND,      // Key not found
    DATABASE_KEY_ALREADY_EXISTS, // Key already exists
    DATABASE_ERROR,              // General database error
};

#endif // DATABASE_ERROR_H