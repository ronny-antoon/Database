# DatabaseAPI Library

[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/donate/?hosted_button_id=BACPRJTAU4G4E)
[![Latest](https://img.shields.io/github/v/tag/ronny-antoon/DatabaseAPI?color=red&label=last+release)](https://github.com/ronny-antoon/DatabaseAPI/releases)
[![Latest](https://badges.registry.platformio.org/packages/ronny-antoon/library/DatabaseAPI.svg)](https://registry.platformio.org/libraries/ronny-antoon/DatabaseAPI)
[![UnitTests](https://github.com/ronny-antoon/DatabaseAPI/actions/workflows/build-and-test-embeded.yaml/badge.svg)](https://github.com/ronny-antoon/DatabaseAPI/actions/workflows/build-and-test-embeded.yaml)

The DatabaseAPI Library is a C++ library that provides an interface and implementation for a simple key-value database. It is designed to work on the ESP32/Arduino platform, making use of the Non-Volatile Storage (NVS) capabilities for data storage.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Dependencies](#dependencies)
- [Installation](#installation)
- [Usage](#usage)
- [API](#API)
- [Example](#example)
- [License](#license)
- [Contributions](#contributions)
- [Platformio Registry](#platformio-registry)

## Introduction

The DatabaseAPI Library is designed to simplify the management of key-value pairs in a non-volatile storage environment. It provides a flexible and easy-to-use interface for interacting with a database.

## Features

- `Key-Value Storage`: Store and retrieve key-value pairs.
- `Error Handling`: Comprehensive error handling with error codes.
- `Database Delegation`: Easily switch between different database implementations.
- `ESP32/Arduino NVS Support`: Includes an implementation for the ESP32/Arduino NVS database.

## Dependencies

The DatabaseAPI Library has the following dependencies:
- C++ Standard Library
- ESP32/Arduino NVS library (for the NVSDelegate implementation)
- [MultiPrinterLogger](https://github.com/ronny-antoon/MultiPrinterLogger#readme)

## Installation

**Method 1**:
To use the **DatabaseAPI** library in your PlatformIO project, follow these steps:

1. Open "platformio.ini", a project configuration file located in the root of PlatformIO project.

2. Add the following line to the `lib_deps` option of `[env:]` section:
`ronny-antoon/DatabaseAPI@^4.1.0`

3. Build a project, PlatformIO will automatically install dependencies.

**Method 2**:
To use the **DatabaseAPI** library in your Arduino project, follow these steps:

1. Download the latest release from the [GitHub repository](https://github.com/ronny-antoon/DatabaseAPI).

2. In the Arduino IDE, click "Sketch" -> "Include Library" -> "Add .ZIP Library" and select the downloaded `.zip` file.

3. Make sure to link your project with the necessary ESP-IDF libraries for NVS support.

## Usage

1. Initialize the database delegate, such as `NVSDelegate` for ESP32/Arduino NVS.
```cpp
NVSDelegate nvsDelegate("MyPartition");
```

2. Create a `DatabaseAPI` instance and pass the delegate to it.
```cpp
DatabaseAPI database(&nvsDelegate);
```

3. Use the API to perform database operations.
```cpp
char key[] = "my_key";
char value[] = "my_value";

DatabaseError_t result = database.set(key, value);
if (result == DATABASE_OK) {
    // Key-Value pair successfully set.
} else {
    // Handle the error.
}
```

## API

The DatabaseAPI Library provides the following classes and interfaces:
- `DatabaseDelegateInterface`: Interface for database delegate classes.
- `NVSDelegate`: Implementation of the DatabaseDelegateInterface for ESP32/Arduino NVS.
- `DatabaseAPIInterface`: Interface for the Database API class.
- `DatabaseAPI`: Implementation of the DatabaseAPIInterface for interacting with the database.

Detailed documentation and usage examples can be found in the library source code.

## Example

Here's a simple example of how to use the DatabaseAPI library to store and retrieve data from the NVS database:

```cpp
#include <DatabaseAPI.hpp>
#include <NVSDelegate.hpp>
#include <DatabaseError.hpp>

// Initialize the database delegate
NVSDelegate nvsDelegate("MyPartition");

// Initialize the database API class
DatabaseAPI database(&nvsDelegate);

// Set a new key-value pair in database.
DatabaseError_t result = database.set("key", "value");

if (result == DATABASE_OK) {
    // Key-Value pair successfully set.
} else {
    // Handle the error.
}
```

For a complete list of methods and error codes, refer to the class documentation in the source code.

## License

This library is provided under the MIT License. You are free to use, modify, and distribute it as needed.

## Contributions

If you'd like to contribute to the Database Library, please follow these guidelines:
1. Fork the repository.
2. Make your changes and document them.
3. Submit a pull request.

## Author

This library was created by Ronny Antoon. You can contact me at [ronny.antoon@gmail.com] for any questions or feedback.

## Platformio Registry

https://registry.platformio.org/libraries/ronny-antoon/DatabaseAPI

For more information on the ESP-IDF's NVS API, refer to the [official documentationy](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/storage/nvs_flash.html)

