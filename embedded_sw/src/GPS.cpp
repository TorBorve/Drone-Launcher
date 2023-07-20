#include <TinyGPSPlus.h>

#include "GPS.h"
#include "Pins.h"
#include <SoftwareSerial.h>
#include <Arduino.h>

#define UPDATE_INTERVAL 1000
#define GPS_RX 21 // We RECIEVE - READ HERE
#define GPS_TX 20 // GPS RECIEVES - WRITE HERE
#define GPSBaud 38400
#define GPS_SERIAL Serial4
#define MAX_GPS_TIMEOUT 10000

GPS gps;

GPS::GPS() : _prevUpdate(0),_rx(GPS_RX), _tx(GPS_TX), _lat(0), _long(0), _last_loc_update(0),
            _alt(0), _last_alt_update(0), tiny_gps(), _date(), _last_date_update(0), _time(), _last_time_update(0),
            _speed(), _last_speed_update(0) {
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
                _last_date_update = now;
            }

            if (tiny_gps.time.isValid()) {
                _time = tiny_gps.time;
                _last_time_update = now;
            }

            if (tiny_gps.speed.isValid()) {
                _speed = tiny_gps.speed;
                _last_speed_update = now;
            }
        }
    }
}

bool GPS::status(uint32_t now) {
    LastUpdateArray lastUpdates = getUpdateTimes(now);
    uint32_t* ptr = &lastUpdates.time_since_last_loc_update;
    int numMembers = sizeof(LastUpdateArray) / sizeof(uint32_t);

    for (int i = 0; i < numMembers; i++) {
        if (*ptr > MAX_GPS_TIMEOUT) {
            return false;
        }
        ptr++;
    }
    return true;
}