#include <Arduino.h>

#include "DatabaseAPI.hpp"
#include "NVSDelegate.hpp"

#include <esp_system.h>

void setup()
{
    Serial.begin(115200);
    Serial.printf("Free heap: %d\n", esp_get_free_heap_size());
    // Serial.printf("Minimum free heap: %d\n", esp_get_minimum_free_heap_size());

    NVSDelegate *nvs_delegate = new NVSDelegate();
    DatabaseAPI *database_api = new DatabaseAPI(nvs_delegate, "test");

    char value[50] = "lll";
    char key[10] = "bbbbbbb";

    database_api->eraseAll();
    database_api->set(key, value);

    // Serial.printf("Free heap: %d\n", esp_get_free_heap_size());
    // Serial.printf("Minimum free heap: %d\n", esp_get_minimum_free_heap_size());

    delete database_api;
    delete nvs_delegate;

    Serial.printf("Free heap: %d\n", esp_get_free_heap_size());
    // Serial.printf("Minimum free heap: %d\n", esp_get_minimum_free_heap_size());
}

void loop()
{
    delay(1000);
}
