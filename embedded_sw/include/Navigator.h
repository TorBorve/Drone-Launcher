#pragma once
#include <Adafruit_BNO08x.h>
#include <TinyGPSPlus.h>
#include <stdint.h>

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
   public:
    IMU();
    void update(uint32_t now);
    void init();
    euler_t getEuler(bool degrees);
    quaternion_t getQuat();

   private:
    void enableReports();
    void quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false);
    void quaternionToEulerRV(sh2_RotationVectorWAcc_t* rotational_vector, euler_t* ypr, bool degrees = false);
    Adafruit_BNO08x _imuSensor;
    sh2_SensorValue_t _sensorValue;
    uint32_t _prevUpdate;
};

typedef struct PositionArray {
    double lat;
    double lng;
    double alt;  // meters
} PositionArray;

class GPS {
   public:
    GPS();
    void init();
    void update(uint32_t now);
    PositionArray getPos();
    bool getGPSFix();
   private:
    TinyGPSPlus _tinyGPS;
    bool _gpsFix;
    uint32_t _prevUpdate;

};

class Navigator {
   public:
    Navigator();
    void init();
    void update(uint32_t now);
    PositionArray getPos();
    bool getGPSFix();
    euler_t getOrientationEul(bool degrees);
    quaternion_t getOrientationQuat();
   private:
    GPS _gps;
    IMU _imu;

};

extern Navigator navigator;