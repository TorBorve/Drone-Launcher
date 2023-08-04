#pragma once
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

// extern IMU imu;