
/*
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;   // SD card CS pin
File dataFile;

long readEncoder() {
    // <-- Replace with your actual encoder read
    return 0;  
}

void setup() {
    SD.begin(chipSelect);

    dataFile = SD.open("log.csv", FILE_WRITE);
    if (dataFile) {
        dataFile.println("timestamp_ms,value"); // header
        dataFile.close();
    }
}

unsigned long lastLog = 0;
const unsigned long LOG_INTERVAL_MS = 10; // 100 Hz

void loop() {
    unsigned long now = millis();

    if (now - lastLog >= LOG_INTERVAL_MS) {
        lastLog = now;

        long enc = readEncoder();

        dataFile = SD.open("log.csv", FILE_WRITE);
        if (dataFile) {
            dataFile.print(now);
            dataFile.print(",");
            dataFile.println(enc);
            dataFile.close();
        }
    }
}
*/