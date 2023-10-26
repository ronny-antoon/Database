#include <Arduino.h>
#include <NVSDatabase.hpp>

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        delay(10); // Wait for serial port to connect. Needed for native USB port only

    NVSDatabase *nvsDb = new NVSDatabase("myNamespace");
    DatabaseError_t result;
    const char *myKey = "myKey";
    const char *myValue = "myValue";
    result = nvsDb->putPair(myKey, myValue);
    Serial.println(result);
    size_t maxSize = 32;
    char retrievedValue[maxSize];
    result = nvsDb->getValueOf(myKey, retrievedValue, &maxSize);
    Serial.println(result);
    Serial.println(retrievedValue);
    delete nvsDb;
}

void loop()
{
    delay(1000);
}