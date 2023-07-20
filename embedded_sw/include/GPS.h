#pragma once
#include <stdint.h>
typedef struct PositionArray {
    double lat;
    double lng;
    double alt; // meters
} PositionArray;

class GPS {
    private:
        void pingGPS();
        uint32_t _prevUpdate;
        uint8_t _rx;
        uint8_t _tx;
        double _lat;
        double _long;
        uint32_t _last_loc_update;
        double _alt; // meters
        uint32_t _last_alt_update;
        TinyGPSPlus tiny_gps;
        TinyGPSDate _date;
        TinyGPSTime _time;

    public:
        void update(uint32_t now);
        void init();
        PositionArray getPos() {return PositionArray{_lat, _long, _alt};}
        TinyGPSTime getTime() {return _time;}
        TinyGPSDate getDate() {return _date;}
        GPS();
};

extern GPS gps;