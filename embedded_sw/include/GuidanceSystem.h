#pragma once
#include "IMU.h"
#include "GPS.h"

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