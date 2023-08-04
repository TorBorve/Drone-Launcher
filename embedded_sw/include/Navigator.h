#pragma once
#include "IMU.h"
#include "GPS.h"

#include <stdint.h>
#include <Adafruit_BNO08x.h>
// Define data structures for each sensor type
typedef struct euler_t {
  float yaw;
  float pitch;
  float roll;
} euler_t;

typedef struct quaternion_t {
    float qr;
    float qi;
    float qj;
    float qk;
} quaternion_t;

class IMU {
    private:
        uint32_t _prevUpdate;
        sh2_RotationVectorWAcc_t* _quaternion;
        void enableAll();
        void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false);
        void quaternionToEulerRV(sh2_RotationVectorWAcc_t* rotational_vector, euler_t* ypr, bool degrees = false);
    public:
        void update(uint32_t now);
        void init();
        euler_t getEuler(bool degrees);
        quaternion_t getQuat();
        bool status(uint32_t now);
        IMU();

};

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

class GuidanceSystem {
    private:
        GPS _gps;
        IMU _imu;
    
    public:
        void update(uint32_t now);
        void init();
        bool status(uint32_t now);
        PositionArray getPos() {return _gps.getPos();}
        euler_t getOrientation(bool degrees) {return _imu.getEuler(degrees);}
        quaternion_t getOrientationQuat() {return _imu.getQuat();}
        TinyGPSTime getTime() {return _gps.getTime();}
        TinyGPSDate getDate() {return _gps.getDate();}
};

extern GuidanceSystem guidanceSystem;