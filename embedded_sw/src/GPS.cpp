#include <TinyGPSPlus.h>

#include "GPS.h"
#include "Pins.h"
#include <SoftwareSerial.h>
#include <Arduino.h>

#define UPDATE_INTERVAL 1000
#define GPS_RX 21 // We RECIEVE - READ HERE
#define GPS_TX 20 // GPS RECIEVES - WRITE HERE
#define GPSBaud 9600
#define GPS_SERIAL Serial4

GPS gps;


GPS::GPS() : _rx{GPS_RX}, _tx{GPS_TX}, _prevUpdate{0}, tiny_gps{} {
    _lat = 0;
    _long = 0;
    _last_loc_update = 0;
    _alt = 0;
    _last_alt_update = 0;
    return;
}

void GPS::init() {
    GPS_SERIAL.begin(GPSBaud);
}

void GPS::update(uint32_t now) {
    if (now - _prevUpdate < UPDATE_INTERVAL) {
        return;
    }
    _prevUpdate = now;
    if (GPS_SERIAL.available() > 0) {
        if (tiny_gps.encode(GPS_SERIAL.read())) {
            if (tiny_gps.location.isValid()) {
                _lat = tiny_gps.location.lat();
                _long = tiny_gps.location.lng();
                _last_loc_update = now;
            }

            if (tiny_gps.altitude.isValid()) {
                _alt = tiny_gps.altitude.meters();
                _last_alt_update = now;
            }

            if (tiny_gps.date.isValid()) {
                _date = tiny_gps.date;
            }

            if (tiny_gps.time.isValid()) {
                _time = tiny_gps.time;
            }
        }
    }
}

void GPS::pingGPS() {
}