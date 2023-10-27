# NVSDatabase Library

[![Latest](https://img.shields.io/github/v/tag/ronny-antoon/NVSDatabase?color=red&label=last+release)](https://github.com/ronny-antoon/NVSDatabase/releases)
[![Latest](https://badges.registry.platformio.org/packages/ronny-antoon/library/NVSDatabase.svg)](https://registry.platformio.org/libraries/ronny-antoon/NVSDatabase)
[![Unit Test](https://github.com/ronny-antoon/NVSDatabase/actions/workflows/Unit_Tests.yml/badge.svg)](https://github.com/ronny-antoon/NVSDatabase/actions/workflows/Unit_Tests.yml)

The NVSDatabase library is a C++ library that provides an interface for working with the ESP-IDF's Non-volatile Storage (NVS) API. NVS is used for storing configuration data and other non-volatile information on ESP32 and ESP8266 microcontrollers. This library is designed to facilitate the storage and retrieval of key-value pairs in the NVS database.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Class Overview](#class-overview)
- [Example](#example)
- [Thread Safety](#thread-safety)
- [License](#license)
- [Contributions](#contributions)
- [Platformio Registry](#platformio-registry)

## Introduction

The NVSDatabase library is a useful tool for applications running on ESP32 and ESP8266 microcontrollers that require non-volatile storage capabilities. It abstracts the NVS API, making it easier to interact with the database and manipulate data. The library provides functions for storing, retrieving, and managing key-value pairs in the NVS database.

## Features
- Store key-value pairs in the NVS database.
- Retrieve values associated with keys.
- Check if a key exists in the database.
- Remove key-value pairs from the database.
- Erase all key-value pairs within a specific namespace.
- Error handling for common database operations.

## Installation

**Method 1**:
To use the **NVSDatabase** library in your PlatformIO project, follow these steps:

1. Open "platformio.ini", a project configuration file located in the root of PlatformIO project.

2. Add the following line to the `lib_deps` option of `[env:]` section:
`ronny-antoon/NVSDatabase@^1.0.0`

3. Build a project, PlatformIO will automatically install dependencies.

**Method 2**:
To use the **NVSDatabase** library in your Arduino project, follow these steps:

1. Download the latest release from the [GitHub repository](https://github.com/ronny-antoon/NVSDatabase).

2. In the Arduino IDE, click "Sketch" -> "Include Library" -> "Add .ZIP Library" and select the downloaded `.zip` file.

3. Make sure to link your project with the necessary ESP-IDF libraries for NVS support.

## Usage

The NVSDatabase library provides an interface for creating and interacting with an NVS database. You can use it to store and retrieve key-value pairs in the database.

### Class Overview

The library includes the following classes:
- `DatabaseStorage`: An abstract class for database storage that defines the common methods for interacting with the database.
- `NVSDatabase`: A concrete class that extends `DatabaseStorage` and provides an implementation of the NVS database storage.

### Example

Here's a simple example of how to use the NVSDatabase library to store and retrieve data from the NVS database:

```cpp
#include "NVSDatabase.hpp"

// Create an NVSDatabase instance with a specific namespace.
NVSDatabase nvsDB("my_namespace");

// Store a key-value pair in the database.
DatabaseError_t result = nvsDB.putPair("my_key", "my_value");

if (result == DATABASE_OK) {
    // Key-value pair was successfully stored.
} else {
    // Handle the error.
}

// Retrieve the value associated with a key.
char value[100];
size_t maxSize = sizeof(value);
result = nvsDB.getValueOf("my_key", value, &maxSize);

if (result == DATABASE_OK) {
    // Value was successfully retrieved.
} else {
    // Handle the error.
}
```

For a complete list of methods and error codes, refer to the class documentation in the source code.

### Thread Safety

Please note that the NVSDatabase class is not thread-safe by default. If you plan to use the NVSDatabase instance from multiple threads, consider implementing external synchronization mechanisms to ensure safe access to the database.

### License

This library is provided under the MIT License. You are free to use, modify, and distribute it as needed.

## Contributions

Contributions to the NVSDatabase library are welcome. If you find any issues, have suggestions for improvements, or want to contribute code, please create a pull request on the GitHub repository.

## Author

This library was created by Ronny Antoon. You can contact me at [ronny.antoon@gmail.com] for any questions or feedback.

## Platformio Registry

https://registry.platformio.org/libraries/ronny-antoon/NVSDatabase

For more information on the ESP-IDF's NVS API, refer to the [official documentationy](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_flash.html)