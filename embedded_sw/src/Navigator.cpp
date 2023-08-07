#include "Navigator.h"

#include "Log.h"

namespace DroneLauncher {

#define IMU_BNO08X_RESET -1
#define IMU_UPDATE_INTERVAL 10

#define GPS_UPDATE_INTERVAL 500
#define GPS_BAUD 38400
#define GPS_SERIAL Serial5
#define GPS_MAX_TIMEOUT 10000

Navigator navigator{};

IMU::IMU() : _imuSensor{IMU_BNO08X_RESET}, _prevUpdate{0} {
}

void IMU::init() {
    if (!_imuSensor.begin_I2C()) {
        LOG_ERROR("Failed to find BNO08x chip");
        while (1)
            ;
    }
    enableReports();
}

void IMU::enableReports() {
    if (!_imuSensor.enableReport(SH2_ARVR_STABILIZED_RV, IMU_UPDATE_INTERVAL * 1000)) {
        Serial.println("Could not enable rotation vector");
    }
}

void IMU::update(uint32_t now) {
    if (now - _prevUpdate < IMU_UPDATE_INTERVAL) {
        return;
    }
    _prevUpdate = now;

    if (_imuSensor.wasReset()) {
        enableReports();
    }

    if (!_imuSensor.getSensorEvent(&_sensorValue)) {
        return;
    }
}

void IMU::quaternionToEuler(float qr, float qi, float qj, float qk, euler_t* ypr, bool degrees = false) {
    float sqr = sq(qr);
    float sqi = sq(qi);
    float sqj = sq(qj);
    float sqk = sq(qk);

    ypr->yaw = atan2(2.0 * (qi * qj + qk * qr), (sqi - sqj - sqk + sqr));
    ypr->pitch = asin(-2.0 * (qi * qk - qj * qr) / (sqi + sqj + sqk + sqr));
    ypr->roll = atan2(2.0 * (qj * qk + qi * qr), (-sqi - sqj + sqk + sqr));

    if (degrees) {
        ypr->yaw *= RAD_TO_DEG;
        ypr->pitch *= RAD_TO_DEG;
        ypr->roll *= RAD_TO_DEG;
    }
}

void IMU::quaternionToEulerRV(sh2_RotationVectorWAcc_t* rotational_vector, euler_t* ypr, bool degrees = false) {
    quaternionToEuler(rotational_vector->real, rotational_vector->i, rotational_vector->j, rotational_vector->k, ypr, degrees);
}

euler_t IMU::getEuler(bool degrees) {
    euler_t ypr;
    quaternionToEulerRV(&_sensorValue.un.arvrStabilizedRV, &ypr, degrees);
    return ypr;
}

quaternion_t IMU::getQuat() { return quaternion_t{_sensorValue.un.arvrStabilizedRV.real,
                                                  _sensorValue.un.arvrStabilizedRV.i,
                                                  _sensorValue.un.arvrStabilizedRV.j,
                                                  _sensorValue.un.arvrStabilizedRV.k}; }

GPS::GPS() : _prevUpdate{0} {}

void GPS::init() {
    GPS_SERIAL.begin(GPS_BAUD);
}

void GPS::update(uint32_t now) {
    for (int i = 0; i < 64 && GPS_SERIAL.available(); i++) {
        _tinyGPS.encode(GPS_SERIAL.read());
    }
    if (now - _prevUpdate < GPS_UPDATE_INTERVAL) {
        return;
    }
    _prevUpdate = now;
}

PositionArray GPS::getPos() {
    PositionArray pos{0, 0, 0};
    if (_tinyGPS.location.isValid()) {
        pos.lat = _tinyGPS.location.lat();
        pos.lng = _tinyGPS.location.lng();
        pos.alt = _tinyGPS.altitude.meters();
    }
    return pos;
}

bool GPS::getGPSFix() { 
   return _tinyGPS.location.age() > GPS_MAX_TIMEOUT ? false : true; }

Navigator::Navigator() :  _gps{}, _imu{} {}

void Navigator::init() {
    _imu.init();
    _gps.init();
}

void Navigator::update(uint32_t now) {
    _imu.update(now);
    _gps.update(now);
}

PositionArray Navigator::getPos() {
    return _gps.getPos();
}

bool Navigator::getGPSFix() {
    return _gps.getGPSFix();
}

euler_t Navigator::getOrientationEul(bool degrees) {
    return _imu.getEuler(degrees);
}

quaternion_t Navigator::getOrientationQuat() {
    return _imu.getQuat();
}

}  // namespace DroneLauncher