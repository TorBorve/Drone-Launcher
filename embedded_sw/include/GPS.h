#pragma once
#include <stdint.h>
typedef struct PositionArray {
    double lat;
    double lng;
    double alt; // meters
} PositionArray;

typedef struct LastUpdateArray {
    uint32_t time_since_last_loc_update;
    uint32_t time_since_last_alt_update;
    uint32_t time_since_last_date_update;
    uint32_t time_since_last_time_update;
    uint32_t time_since_last_speed_update;
} LastUpdateArray;

class GPS {
    private:
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
        uint32_t _last_date_update;
        TinyGPSTime _time;
        uint32_t _last_time_update;
        TinyGPSSpeed _speed;
        uint32_t _last_speed_update;

    public:
        void update(uint32_t now);
        void init();
        bool status(uint32_t now);
        PositionArray getPos() {return PositionArray{_lat,
                                                     _long, 
                                                     _alt};}
        LastUpdateArray getUpdateTimes(uint32_t now) {return LastUpdateArray{now - _last_loc_update,
                                                                             now - _last_alt_update,
                                                                             now - _last_date_update,
                                                                             now - _last_time_update,
                                                                             now - _last_speed_update};}
        TinyGPSTime getTime() {return _time;}
        TinyGPSDate getDate() {return _date;}
        GPS();
};

extern GPS gps;