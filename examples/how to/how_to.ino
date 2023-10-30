#include <Arduino.h>

#include <DatabaseAPI.hpp>
#include <NVSDelegate.hpp>
#include <DatabaseError.hpp>

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(10); // Wait for serial port to connect. Needed for native USB port only

    // Initialize the database
    DatabaseAPI *myDataBase = new DatabaseAPI(new NVSDelegate("myNamespace"));

    const char *myKey = "myKey";
    const char *myValue = "myValue";

    myDataBase->set(myKey, myValue);
}

void loop()
{
    delay(1000);
}